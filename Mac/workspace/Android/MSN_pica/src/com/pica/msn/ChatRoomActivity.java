package com.pica.msn;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Vector;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.InputFilter;
import android.text.Selection;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

import com.pica.ui.tools.CheckInviteList;
import com.pica.ui.tools.EmotionTextView;
import com.pica.ui.tools.EmotionView;
import com.pica.ui.tools.HistoryTextView;
import com.pica.ui.tools.InviteList;
import com.pica.ui.tools.RecordItem;
import com.pica.ui.tools.RecordSendView;
import com.pica.ui.tools.RecordView;
import com.pica.ui.tools.ShortMessageView;
import com.pica.ui.tools.TitleView;
import com.pica.ui.tools.ToolBarView;
import com.pica.ui.tools.TransferItem;

public class ChatRoomActivity extends Activity{
	//public static final int SEND_FILE = 1;
	//private static final int SEND_CAMERA_PIC = 2;
	private Menu mMenu;
	private static final int MENU_1 = Menu.FIRST;
	private static final int MENU_2 = Menu.FIRST + 1;
	private static final int MENU_3 = Menu.FIRST + 2;
	private static final int MENU_4 = Menu.FIRST + 3;
	private static final int MENU_5 = Menu.FIRST + 4;
	private static final int MENU_INVITE = Menu.FIRST + 5;
	private static final int MENU_MEMBER_LIST = Menu.FIRST + 6;
	private static final int MENU_SAVE_HISTORY = Menu.FIRST + 7;
	private static final int MENU_SEND_AUDIO = Menu.FIRST + 8;
	private static final int MENU_SEND_VOICE_CLIP = Menu.FIRST + 9;
	private static final int MENU_SEND_AUDIO_FILE = Menu.FIRST + 10;
	private static final int MENU_SEND_FILE = Menu.FIRST + 11;
	private static final int MENU_ALL_FILE = Menu.FIRST + 12;
	private static final int MENU_SEND_PICTURE = Menu.FIRST + 13;

	int ad_idx = 0;
	TitleView title;
	TextView ad_text;
	public HistoryTextView  history;
	EditText edit;
	ToolBarView tool;
	ScrollView scroll;
	String chatEdit;
	public Dialog record;
	RecordView recV;
	public Button stopButton;
	RecordSendView recS;
//	private int sidType;
	static Context context;
	private MSNApplication app;
//	private List<Map<String, String>> UserList;
	public int playingItemIndex = -1;
	
//////////////////////解决进程被杀死的bug
	boolean isKilled = false;
    
    /**登录 
     * @param isReconnect 是否是重新登录
     * */
	private void reLogin(boolean isReconnect, Bundle Map)
	{
		app.reset();
		String domain = Map.getString("domain");
		String liveId = Map.getString("liveId");
		String password = Map.getString("password");
		boolean isAutoLogin = Map.getBoolean("isAutoLogin");
		boolean isSavePassword = Map.getBoolean("isSavePassword");
		String status = Map.getString("status");
	    		
		app.SetLiveId(liveId, password);
	            
		reLoginSet(app.GetLiveID());
	        		
		app.isSavePassword = isSavePassword;
		app.isAutoLogin = isAutoLogin;
		app.SetLoginStatus(status);
            
		if(!isReconnect)
		{
			Intent intent= new Intent();
			intent.setClass(this, LoginMonitorActivity.class);
			intent.putExtra("type",MSNApplication.MONITOR_LOGIN_FLAG);
			startActivity(intent);
		    //Log.e("charRoomActivity", "to LoginMonitorActivity");            
			app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
			app.DB.initGroupData();
			app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
			app.DB.initRosterData();
			/////////////////////
		
			//取数据库相关数据
			app.initVcardFromRMS(app.GetLiveID());
			app.getContactGroupVerFromRMS(app.GetLiveID());
			app.initGroupsContactsFromFile(app.GetLiveID());
		 	//app.initGroupsFromRMS(app.GetLiveID());
			//app.initContactsFromRMS(app.GetLiveID());
			app.initContactHeadFromRMS(app.GetLiveID());
			app.getDeviceIdFromRMS(app.GetLiveID());
		}
		else
		{
			if(app.LMHandler!=null)
			{
				Message m = new Message(); 
				m.what = EventListener.EVENT_LOGIN_MONITOR_SETTEXT; 
				//m.obj= getText(R.string.findserver);
				m.obj=getText(R.string.text_signing_in);
				app.LMHandler.sendMessage(m);    
			}
		}
	/////////暂时写死
//        	app.setGroupOrder(app.ROSTER_NORMAL_NO_OFFLINE);
//        	app.isShowOnline=true;
//        	app.isReceiveHeadChange=true;
        	////////
		app.startJabber();
	}
    	
	private void reLoginSet(String liveId)
	{
		app.DB = new DatabaseHelper(this, MSNApplication.DB_NAME, null, MSNApplication.DB_VERSION);
		app.DB.initUserData();   
		Cursor cur = app.DB.getLoginUserData(liveId);
	            
		if(cur != null)
		{
			if(cur.getCount() != 0)
			{
				try
				{
					app.SetPicaId(MsnUtil.getUnvalidFormRs(cur.getString(2)));
					app.SetEditAccount(MsnUtil.getUnvalidFormRs(cur.getString(3)), cur.getString(4));
					app.isSavePassword=cur.getString(5).equals("true");
					app.isAutoLogin=cur.getString(6).equals("true");
					app.isOpenOnlineSound =cur.getString(7).equals("true");
					app.isOpenNewMessageSound=cur.getString(8).equals("true");
					app.SetLoginStatus(cur.getString(15));
					app.isShowOnline =cur.getString(16).equals("true");
					app.isReceiveHeadChange=cur.getString(17).equals("true");
					/////////////取出自己的头像
					if(app.isReceiveHeadChange)
					{
						app.getVcardHead(liveId);
					}
				    		        		
					////////////
					app.isReceiveVcardChange=cur.getString(18).equals("true");
					app.isNudge=cur.getString(19).equals("true");
				    //////////////////增加按状态排序功能		        	
					if(app.isShowOnline)
			        {
			        	app.setGroupOrder((byte)(app.getGroupOrder() + 1));
			        }				
					else
					{
						app.setGroupOrder((byte)(app.getGroupOrder() - 1));
					}
					////////////////////
				}
				catch(Exception e)
				{
		     
				}
			}
			else// end if(cur.getCount() != 0)
			{

			}
		}
		else// end if(cur != null)
		{

		}
	}
    	
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		/**解决进程被杀死的bug*/
		outState.putBoolean("isKilled", true);
		outState.putString("domain", app.GetDomain());
		outState.putString("liveId", app.GetLiveID());
		outState.putString("password", app.GetPassWord());
		outState.putBoolean("isAutoLogin", app.isAutoLogin);
		outState.putBoolean("isSavePassword", app.isSavePassword);
		outState.putString("status", app.sStatus);
	}
    	

    //////////////////////////////
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        final Window win = getWindow(); 
        
        ///////////////////////解决进程被杀死的bug
        if(savedInstanceState != null && savedInstanceState.getBoolean("isKilled") && app == null)
        {
        	isKilled = true;
        	finish();
        	app = MSNApplication.getApplication(this);
        	app.screenManager.pushActivity(this);
        	app.pushChat(this);
        	if(!app.killedToRelogin)//没有在别的Activity中重新登录
        	{
        		reLogin(false, savedInstanceState);
        		//Log.e("MainActivity", "iskilled then relogin");
        	}
        	//在其他的Activity中就不要在重复处理
        	app.killedToRelogin = true;
        	//Log.e("chatRoomActivity", "be killed then relogin");
        	return;
        }
        ////////////////////////////////
        app = MSNApplication.getApplication(this);
        context = this;
        //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        app.screenManager.pushActivity(this);
        app.setChatroomHandler(chatroomHandler);
        app.pushChat(this);
//        UserList = new ArrayList<Map<String, String>>();
        
        final LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));
        
        title = new TitleView(this, null);
        
        int frame= (int)(40*app.screenScale);
        LinearLayout.LayoutParams paramT = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                frame);        
        title.setLayoutParams(paramT);     
        layout.addView(title);
        
        RelativeLayout.LayoutParams paramA = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT);  
        RelativeLayout.LayoutParams paramB = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT);  
        
        WebView wv = new WebView(this);
        WebViewClient mWebViewClient = new WebViewClient()
        {
			
			public boolean shouldOverrideUrlLoading(WebView view, String url)
			{
				app.ad_chat_idx = ad_idx;
				app.tempUrl = url;
				app.currentSession.SESSION_numOfNewMsg= "0";
	    		finish();
	    		app.bChatAdClick = true;
	    		return true;
			}
        	
        };
        wv.setPadding(5, 5, 0, 0);
        wv.setWebViewClient(mWebViewClient);
        wv.setVerticalScrollBarEnabled(false);
        wv.setHorizontalScrollBarEnabled(false);
        
        paramA.addRule(RelativeLayout.CENTER_HORIZONTAL);
        paramB.addRule(RelativeLayout.CENTER_VERTICAL);
        LinearLayout linkLayout = new LinearLayout(this);
        layout.addView(linkLayout, paramA);
        
        ImageView channelImage = new ImageView(this);
        channelImage.setPadding(5, 5, 0, 0);
        linkLayout.addView(channelImage, paramB);
        linkLayout.addView(wv);
        if(app.ad_chat_Vector.size() == 0)
        {
        	wv.setVisibility(View.GONE);
        }
        else
        {
        	app.ad_chat_idx++;
        	if(app.ad_chat_idx == app.ad_chat_Vector.size())
        	{
        		app.ad_chat_idx = 0;
        	}
        	Item adItem = (Item)app.ad_chat_Vector.get(app.ad_chat_idx);
        	ad_idx = app.ad_chat_idx;
        	if(adItem.AD_flag == MSNApplication.AD_IVR_FLAG) 
            {
        		channelImage.setImageResource(R.drawable.channel_ivr);
            }
            else if(adItem.AD_flag == MSNApplication.AD_WAP_FLAG) 
            { 	
            	channelImage.setImageResource(R.drawable.channel_wap);
            }
            else if(adItem.AD_flag == MSNApplication.AD_ZWP_FLAG) 
            {
            	channelImage.setImageResource(R.drawable.channel_wap);
            }
            else if(adItem.AD_flag == MSNApplication.AD_SMS_FLAG)
            {
            	channelImage.setImageResource(R.drawable.channel_sms);
            }
        	
        	StringBuffer sb = new StringBuffer();
			sb.delete(0, sb.length());
			sb.append("<a href=" + adItem.AD_target + ">" + adItem.AD_text + "</a>");
			wv.loadDataWithBaseURL(null, sb.toString(), "text/html", "utf-8", null);
        }
        
        LinearLayout.LayoutParams paramS = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT,1);      
        scroll = new ScrollView(this);
        scroll.setOnTouchListener(new OnTouchListener()
        {
			
			public boolean onTouch(View v, MotionEvent event)
			{				
				final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
    			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
				return false;
			}
        	
        });
        ViewGroup.LayoutParams paramSvLp = new ScrollView.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        scroll.setLayoutParams(paramS);
        layout.addView(scroll);

        history = new HistoryTextView(this);
        history.setLayoutParams(paramSvLp); 
        scroll.addView(history);

        LinearLayout.LayoutParams paramsE = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT);
        tool = new ToolBarView(this);
        tool.setLayoutParams(paramsE);       

        layout.addView(tool);
        
        tool.btn1.setOnClickListener(mEmotionListener);
        tool.btn2.setOnClickListener(mSendFileListener);
        tool.btn3.setOnClickListener(mCameraListener);
        tool.btn4.setOnClickListener(mRecordListener);
        tool.btn5.setOnClickListener(mInviteListener);
        tool.bSend.setOnClickListener(mSendListener);
        
        //568-------------------------------------
        LinearLayout.LayoutParams paramsEd = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        
        edit = new EditText(this);
        edit.setSingleLine();
        edit.setFilters(new InputFilter[]{new InputFilter.LengthFilter(500)});
        edit.setLayoutParams(paramsEd);
        //modify by liubing 20091123
        TextWatcher watcher = new TextWatcher() {
            public void beforeTextChanged(CharSequence s, int start,
                                          int before, int after) { }

            public void onTextChanged(CharSequence s, int start,
                                          int before, int count) {
                chatEdit = s.toString();
            }

            public void afterTextChanged(android.text.Editable s) { }
        };
        edit.addTextChangedListener(watcher);
        //end
        edit.setOnFocusChangeListener(new OnFocusChangeListener()
        {
			
			public void onFocusChange(View v, boolean hasFocus)
			{				
				// TODO Auto-generated method stub
				if(!hasFocus)
				{
					final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
        			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
				}
			}        	
        });
        edit.requestFocus();
        layout.addView(edit);
        
        //569------------------------------------------------
//        edit = new EditText(this);
//        edit.setSingleLine();
//        edit.setFilters(new InputFilter[]{new InputFilter.LengthFilter(500)});
//        edit.setLayoutParams(paramsE);
//        edit.setFocusable(true);
//        edit.setFocusableInTouchMode(true);
//			
////        edit.requestFocus();
//					final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
//        Vector msgs=app.getSessionMessages(app.currentSession.TOPITEM_JID);
//        			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
//        if(msgs.size() != 0)
//        {
//			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
//			edit.clearFocus();
//			edit.setFocusable(false);
//			edit.setFocusableInTouchMode(false);
//			edit.setFocusable(true);
//			edit.setFocusableInTouchMode(true);
//			scroll.setFocusable(true);
//			scroll.setFocusableInTouchMode(true);
//			scroll.requestFocus();
//				}
//        else
//        {
//        	imm.showSoftInputFromInputMethod(history.getWindowToken(), 0);
//        	edit.requestFocus();
//			}        	
////        edit.setOnFocusChangeListener(new OnFocusChangeListener()
////        {
////			@Override
////			public void onFocusChange(View v, boolean hasFocus)
////			{				
////				// TODO Auto-generated method stub
////				if(!hasFocus)
////				{
////					final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
////        			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
////				}
////			}        	
////        });
//        layout.addView(edit, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
        
        //------------------------------------------------
        setContentView(layout);
//        app.getVersion(app.GetIMSI(), app);
    }
 
    public void onResume() 
    {
        super.onResume();
        app.isPause = false;
        app.mNotificationManager.cancel(R.layout.main);
        //Log.e("ChatRoomActivity", "onResume");
        /////////////解决进程被杀死的bug
        if(isKilled && app == null)
        {
            finish();
            return;
        }
        app.currentSession.SESSION_numOfNewMsg = "0";
        Vector chaters = (Vector) app.currentSession.SESSION_chaters;
        Contact contact=(Contact)chaters.firstElement();
        
        if(contact.CONTACT_nickname != null)
        {
            title.setTitleText(contact.CONTACT_nickname);// = new TitleView(this, contact.CONTACT_nickname);
        }
        else
        {
            title.setTitleText(contact.CONTACT_imid);//title = new TitleView(this, contact.CONTACT_imid);
        }
        history.removeAllViews();
        history.init(app.currentSession);
        ChangeTitleIcon(contact);
    }
        
    public void onPause()
    {
        super.onPause();
        app.isPause = true;
    }  

    private OnClickListener mAdListener = new OnClickListener() {
        public void onClick(View v) {
    		app.currentSession.SESSION_numOfNewMsg= "0";
    		finish();
    		app.bChatAdClick = true;
        }
    };
    
    private OnClickListener mEmotionListener = new OnClickListener() {
        public void onClick(View v) {
        	showDialog(UiEventListener.UI_EVENT_EMOTION);
        }
    };
    
    private OnClickListener mSendFileListener = new OnClickListener() {
        public void onClick(View v) {
        	//showDialog(UiEventListener.UI_EVENT_SHORTMESSAGE);
        	SelectSendFile();
        }
    };
    
    private OnClickListener mCameraListener = new OnClickListener() {
        public void onClick(View v) {
    		Intent intent;
        	intent = new Intent();
            intent.setClass(ChatRoomActivity.this, CameraActivity.class);
            intent.putExtra("type",MSNApplication.CAMERA_CHATROOM_FLAG);  
            startActivityForResult(intent, MSNApplication.SELECT_FILE_FROM_TAKE_PHONE);            
        }
    };
    
    private OnClickListener mRecordListener = new OnClickListener() {
        public void onClick(View v) {
        	CreateRecord();
        }
    };
    
    private OnClickListener mSendListener = new OnClickListener() {
        public void onClick(View v) {
        	String content=edit.getText().toString();
        	if(content==null||content.length()==0)
        		return;
        	else
        	{
        		Item session = app.currentSession;
//				byte type = session.SESSION_sessionType;
				{
					String sid = session.TOPITEM_JID;//群消息没有sid
					Vector chaters = (Vector) session.SESSION_chaters;
					
					if(chaters.size()>1)
						app.sendGroupMessage(sid, content);
					else if(chaters.size()==1)
					{
						Contact contact = (Contact) chaters.firstElement();
						app.sendMessage(contact, sid, content);					
					}
				}
        	}
        	edit.setText(null);
        }
    };
    
    private void CreateRecord()
    {
		record = new Dialog(this);
		recV = new RecordView(this);
		record.setContentView(recV);
		record.setTitle(R.string.text_record);
		stopButton = (Button)recV.findViewById(R.id.stop);
		stopButton.setClickable(false);
		record.show();
		
		stopButton.setOnClickListener(new OnClickListener() {
			
			public void onClick(View arg0) {
				if(recV.stop())
				{
					String url=recV.saveLocationUrl;
					try 
					{
						SendFile(null,url);
						(new File(url)).delete();
					} 
					catch (Exception e) 
					{
						e.printStackTrace();
					}
				}								 
				//record.dismiss();
			}
	    });
    }
      
    private OnClickListener mInviteListener = new OnClickListener() {
        public void onClick(View v) {
        	showDialog(UiEventListener.UI_EVENT_INVITE);
        }
    };   

    
    public void onConfigurationChanged(Configuration newConfig) 
    {
    	super.onConfigurationChanged(newConfig);
        Vector chaters = (Vector) app.currentSession.SESSION_chaters;
        Contact contact=(Contact)chaters.firstElement();

        final LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));
        
        if(contact.CONTACT_nickname != null)
        {
        	title = new TitleView(this, contact.CONTACT_nickname);
        }
        else
        {
        	title = new TitleView(this, contact.CONTACT_imid);
        }
        int frame= (int)(40*app.screenScale);
        LinearLayout.LayoutParams paramT = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		frame);        
        title.setLayoutParams(paramT);     
        layout.addView(title);
        
        RelativeLayout.LayoutParams paramA = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT); 
        RelativeLayout.LayoutParams paramB = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT);  
        
        WebView wv = new WebView(this);
        WebViewClient mWebViewClient = new WebViewClient()
        {
			
			public boolean shouldOverrideUrlLoading(WebView view, String url)
			{
				app.tempUrl = url;
				app.currentSession.SESSION_numOfNewMsg= "0";
	    		finish();
	    		app.bChatAdClick = true;
	    		return true;
			}
        	
        };
        wv.setPadding(5, 5, 0, 0);
        wv.setWebViewClient(mWebViewClient);
        wv.setVerticalScrollBarEnabled(false);
        wv.setHorizontalScrollBarEnabled(false);
        
        paramA.addRule(RelativeLayout.CENTER_HORIZONTAL);
        paramB.addRule(RelativeLayout.CENTER_VERTICAL);
        LinearLayout linkLayout = new LinearLayout(this);
        layout.addView(linkLayout, paramA);
        
        ImageView channelImage = new ImageView(this);
        channelImage.setPadding(5, 5, 0, 0);
        linkLayout.addView(channelImage, paramB);
        linkLayout.addView(wv);
        if(app.ad_chat_Vector.size() == 0)
        {
        	wv.setVisibility(View.GONE);
        }
        else
        {
        	Item adItem = (Item)app.ad_chat_Vector.get(0);
        	if(adItem.AD_flag == MSNApplication.AD_IVR_FLAG) 
            {
        		channelImage.setImageResource(R.drawable.channel_ivr);
            }
            else if(adItem.AD_flag == MSNApplication.AD_WAP_FLAG) 
            { 	
            	channelImage.setImageResource(R.drawable.channel_wap);
            }
            else if(adItem.AD_flag == MSNApplication.AD_ZWP_FLAG) 
            {
            	channelImage.setImageResource(R.drawable.channel_wap);
            }
            else if(adItem.AD_flag == MSNApplication.AD_SMS_FLAG)
            {
            	channelImage.setImageResource(R.drawable.channel_sms);
            }
        	
        	StringBuffer sb = new StringBuffer();
			sb.delete(0, sb.length());
			sb.append("<a href=" + adItem.AD_target + ">" + adItem.AD_text + "</a>");
			wv.loadDataWithBaseURL(null, sb.toString(), "text/html", "utf-8", null);
        }
        
//        ad_text = new TextView(this);
//        paramA.addRule(RelativeLayout.CENTER_HORIZONTAL);
//        ad_text.setLayoutParams(paramA);     
//        layout.addView(ad_text);
//        if(app.ad_chat_Vector.size() == 0)
//        {
//        	ad_text.setVisibility(View.GONE);
//        }
//        else
//        {
//        	Item adItem = (Item)app.ad_chat_Vector.get(0);
//        	ad_text.setText(
//                    Html.fromHtml(
//                        "<a href=\"" + adItem.AD_target + "\">"+ adItem.AD_text +"</a> "));
//        	ad_text.setMovementMethod(LinkMovementMethod.getInstance());
//        }
        
        LinearLayout.LayoutParams paramS = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT,1);      
        scroll = new ScrollView(this);
        ViewGroup.LayoutParams paramSvLp = new ScrollView.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);        
        scroll.setLayoutParams(paramS);
        layout.addView(scroll);
        scroll.setOnTouchListener(new OnTouchListener()
        {
			@Override
			public boolean onTouch(View v, MotionEvent event)
			{				
				final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
    			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
				return false;
			}

        });
        history = new HistoryTextView(this);
        history.setLayoutParams(paramSvLp); 
        scroll.addView(history);
        history.init(app.currentSession);
        
        LinearLayout.LayoutParams paramsE = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		ViewGroup.LayoutParams.WRAP_CONTENT);
        tool = new ToolBarView(this);
        tool.setLayoutParams(paramsE);       
        layout.addView(tool);
        
        tool.btn1.setOnClickListener(mEmotionListener);
        tool.btn2.setOnClickListener(mSendFileListener);
        tool.btn3.setOnClickListener(mCameraListener);
        tool.btn4.setOnClickListener(mRecordListener);
        tool.btn5.setOnClickListener(mInviteListener);
        tool.bSend.setOnClickListener(mSendListener);
        
        //568---------------------------------------------
        LinearLayout.LayoutParams paramsEd = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);        
        edit = new EditText(this);
        edit.setSingleLine();
        //edit.setTextSize(500);
        edit.setFilters(new InputFilter[]{new InputFilter.LengthFilter(500)});
        edit.setLayoutParams(paramsEd);  
        //modify by liubing 20091123
        TextWatcher watcher = new TextWatcher() {
            public void beforeTextChanged(CharSequence s, int start,
                                          int before, int after) { }

            public void onTextChanged(CharSequence s, int start,
                                          int before, int count) {
                chatEdit = s.toString();
            }

            public void afterTextChanged(android.text.Editable s) { }
        };
        edit.addTextChangedListener(watcher);
        //end
        edit.setText(chatEdit);//modify by liubing 20091123
        layout.addView(edit);
        
        //569--------------------------------------------------
//        LinearLayout parent = (LinearLayout) edit.getParent();
//        parent.removeView(edit);
//        layout.addView(edit, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
//        edit.setFocusable(true);
//        edit.setFocusableInTouchMode(true);
//        edit.requestFocus();
        
        //------------------------------------
        ChangeTitleIcon(contact);
        setContentView(layout);
    	System.gc();
    	return;
    }
    
    
    
    protected Dialog onCreateDialog(int id) 
    {
		if (id == UiEventListener.UI_EVENT_EMOTION) 
		{
			final EmotionView emotion = new EmotionView(ChatRoomActivity.this);
			emotion.setOnItemClickListener(new OnItemClickListener() 
			{
				
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) 
				{
					Integer i = (Integer) emotion.getItemAtPosition(arg2);
					int start = Selection.getSelectionStart(edit.getText());
					Editable able = edit.getText();
					able.insert(start,EmotionTextView.emotion_face[i]);
					String newText = able.toString();
					edit.setText(newText);
					Selection.setSelection(edit.getText(), edit.getText().length());
					dismissDialog(UiEventListener.UI_EVENT_EMOTION);
				}
			});
			return new AlertDialog.Builder(ChatRoomActivity.this).setIcon(
					R.drawable.msn).setTitle(R.string.text_input_image)
					.setView(emotion).create();
		} 
		else if (id == UiEventListener.UI_EVENT_SHORTMESSAGE) 
		{
			final ShortMessageView shortmessage = new ShortMessageView(
					ChatRoomActivity.this);
			shortmessage.setOnItemClickListener(new OnItemClickListener() 
			{
				
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) 
				{
					int start = Selection.getSelectionStart(edit.getText());
					Editable able = edit.getText();
					String appendText = (String) shortmessage.getItemAtPosition(arg2);
					able.insert(start,appendText);
					String newText = able.toString();
					edit.setText(newText);
					Selection.setSelection(edit.getText(), edit.getText().length());
					dismissDialog(UiEventListener.UI_EVENT_SHORTMESSAGE);
				}
			});
			return new AlertDialog.Builder(ChatRoomActivity.this).setIcon(
					R.drawable.msn).setTitle(R.string.text_input_template)
					.setView(shortmessage).create();
		}
    	else if(id == UiEventListener.UI_EVENT_BLOCK)
    	{
            return new AlertDialog.Builder(ChatRoomActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.block_title)
            .setMessage(R.string.block_info)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                    Vector chaters = (Vector) app.currentSession.SESSION_chaters;
                    Contact contact=(Contact)chaters.firstElement();
                	app.jabber.sendOperateContact(contact.SUBITEM_JID, "block");
                	removeDialog(UiEventListener.UI_EVENT_BLOCK);
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_BLOCK);
               
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_UNBLOCK)
    	{
            return new AlertDialog.Builder(ChatRoomActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.unblock_title)
            .setMessage(R.string.unblock_info)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                    Vector chaters = (Vector) app.currentSession.SESSION_chaters;
                    Contact contact=(Contact)chaters.firstElement();
                	app.jabber.sendOperateContact(contact.SUBITEM_JID, "unblock");
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

               
                }
            })
           .create();   		
    	}
    	else if(id == UiEventListener.UI_EVENT_ADD_CONFRIM)
    	{
            Vector adder = (Vector) app.verifyFriendVector;
    		final Item contact=(Item)adder.firstElement();
    	
    		String tmp = getString(R.string.validate_message);
    		String str = contact.VERIFY_imid + "(" + contact.VERIFY_nickname + ")" + tmp;
            return new AlertDialog.Builder(ChatRoomActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.validate_title)
            .setMessage(str)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
            		app.acceptOrDenyVerifyFriend("accept", contact.VERIFY_imid);
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	app.acceptOrDenyVerifyFriend("deny", contact.VERIFY_nickname);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_INVITE)
    	{
    		final InviteList inv = new InviteList(ChatRoomActivity.this);
    		if(inv.InviteData.isEmpty())
    		{
    			return new AlertDialog.Builder(ChatRoomActivity.this)
    			.setCancelable(false)
                .setIcon(R.drawable.msn)
                    .setTitle(R.string.text_invite)
                    .setMessage(R.string.text_noOnlineBuddy)
                    .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) 
                        {
                        	removeDialog(UiEventListener.UI_EVENT_INVITE);                       	
                        }
                    })
    	            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
    	                public void onClick(DialogInterface dialog, int whichButton) {    	                	
    	                	removeDialog(UiEventListener.UI_EVENT_INVITE);
    	                }
    	            })                
                    .create();
    		}
    		else
    		{
    			return new AlertDialog.Builder(ChatRoomActivity.this)
    				.setCancelable(false)
    				.setIcon(R.drawable.msn)
                    .setTitle(R.string.text_invite)
                    .setView(inv)
                    .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) 
                        {
                        	Contact firstparticipant=null;
            				if (app.currentSession.SESSION_sessionType== MSNApplication.SESSION_TYPE_CHAT)
            				{
            					Vector chaters = (Vector)app.currentSession.SESSION_chaters;
            					firstparticipant=(Contact)chaters.firstElement();
            				}
            				//Log.v("app.jid=:", "size=:"+app.jids.size());
            				if(app.jids.size()==0)
            				{
            					removeDialog(UiEventListener.UI_EVENT_INVITE);
            					return;
            				}
            				String[] chaterJid = new String[app.jids.size()];
            				
            				app.jids.copyInto(chaterJid);
            				
                        	app.addSessionParticipants(firstparticipant,app.currentSession,chaterJid);
                        	removeDialog(UiEventListener.UI_EVENT_INVITE);
                        	app.jids.removeAllElements();
                        }
                    })
    	            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
    	                public void onClick(DialogInterface dialog, int whichButton) {
    	                	app.jids.removeAllElements();
    	                	removeDialog(UiEventListener.UI_EVENT_INVITE);
    	                }
    	            })                
                    .create(); 
    		}
             
    	}
    	else if(id == UiEventListener.UI_EVENT_CHECK_INVITE)
    	{
    		final CheckInviteList inv = new CheckInviteList(ChatRoomActivity.this);
            return new AlertDialog.Builder(ChatRoomActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
                .setTitle(R.string.text_member_list)
                .setView(inv)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	removeDialog(UiEventListener.UI_EVENT_CHECK_INVITE);
                    }
                })            
                .create(); 
    	}
    	else if(id == UiEventListener.UI_EVENT_SAVE_HISTORY)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View textEntryView = factory.inflate(R.layout.savehistory, null);
            final EditText text = (EditText)textEntryView.findViewById(R.id.info_edit);
            text.setText(title.titleText);
            return new AlertDialog.Builder(ChatRoomActivity.this)
            	.setCancelable(false)
                .setIcon(R.drawable.msn)
                .setTitle(R.string.text_save_history)
                .setView(textEntryView)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	if(text.getText().toString().trim().length() == 0)
                    	{
                    		Toast.makeText(context, getText(R.string.error_filename_empty), Toast.LENGTH_SHORT).show();
                    	}
                    	else
                    	{
                    		SaveHistory(text.getText().toString());
                    		removeDialog(UiEventListener.UI_EVENT_SAVE_HISTORY);
                    	}
                    	//app.jabber.sendModifyMyVcard(nick.getText().toString(), info.getText().toString(), true, true);                    	
                        /* User clicked OK so do some stuff */
                    }
                })
                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	removeDialog(UiEventListener.UI_EVENT_SAVE_HISTORY);
                        /* User clicked cancel so do some stuff */
                    }
                })
                .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_OUTER_VOICECLIP)
    	{
    		recS=null;
    		if(app.currentOuterMessage!=null)
    		{
    			recS = new RecordSendView(ChatRoomActivity.this,app.currentOuterMessage,null);
    			int titleID=R.string.text_sending_outer_voice;
    			if(app.currentOuterMessage.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING)
    			{
    				titleID=R.string.text_receiving_outer_voice;
    			}
    			return new AlertDialog.Builder(ChatRoomActivity.this)
		    			.setCancelable(false)		
		    			.setIcon(R.drawable.msn)
    					.setTitle(titleID)
    					.setView(recS)
    		            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() 
    		            {
    		                public void onClick(DialogInterface dialog, int whichButton) 
    		                {    		                	
    		                	removeDialog(UiEventListener.UI_EVENT_OUTER_VOICECLIP);
			    				if(app.currentOuterMessage.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING)
			    				{
			    					app.currentOuterMessage.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL);			    				
			    				}
			    				else if(app.currentOuterMessage.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING)
								{
			    					app.currentOuterMessage.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL);
								}
    		                	app.currentOuterMessage=null;
    		                	recS=null;
    		                }
    		            })
    		            .create();  
    		}

    	}
		return null;
	}
	
	public Handler chatroomHandler = new Handler() 
	{ 
        public void handleMessage(Message msg) 
        {
            switch (msg.what) 
            { 
            	case EventListener.EVENT_RECEIVED_MESSAGE:
            	{
            		try
            		{
                		Item message=(Item)msg.obj;
                		if ((app.currentSession.TOPITEM_JID).equals(message.MESSAGE_sid))
    					{
                			if(message.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
                			{
                				history.addTransferFile(message);          				
                			}
                			else if(message.MESSAGE_type==MSNApplication.MESSAGE_TYPE_VOICECLIP)
                			{
                				if(message.MESSAGE_VOICE_URL!=null&&message.MESSAGE_VOICE_URL.length()!=0)
                				{
                    				//带外
                    				//app.currentOuterMessage=message;                  
                    				//showDialog(UiEventListener.UI_EVENT_OUTER_VOICECLIP);
                					history.addRecordClip(message);//带内
                					
                				}
                				else//带内
                				{
                					history.addRecordClip(message);//带内
                				}
                					
                			}
                			else
                			{
                				history.addMessage(message, message.MESSAGE_IS_SEND);
                			}
                    		edit.requestFocus();
                    		history.invalidate();
    					}      		
            		}catch(Exception e)
            		{
            			e.printStackTrace();
            			//Log.v("exception:", e.getMessage());
            		}

            		break;
            	}       	
            	case EventListener.EVENT_UPDATE_MESSAGE:
            	{
            		try 
            		{
                		Item message=(Item)msg.obj;
                		if(message.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
            			{
                    		TransferItem tf=history.getTransferItemByMessage(message);
                    		if(tf!=null)
                    		{
                    			tf.reset();
                    		}       				
            			}
            			else if(message.MESSAGE_type==MSNApplication.MESSAGE_TYPE_VOICECLIP)
            			{
            				if(message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING)      					
            				{
                				app.currentOuterMessage=message;                  
                				showDialog(UiEventListener.UI_EVENT_OUTER_VOICECLIP);
                				message.MESSAGE_FILEOUTTRANSFER=new FileOuterTransfer(app,FileOuterTransfer.command_downloadFile,message,null,null);
                				if(!app.isTransferingMessageVector.contains(message))
                					app.isTransferingMessageVector.addElement(message);
            				}
            				else
            				{
                				if(recS!=null)//带外    语音剪辑 如果网络部分发生了异常 或者 取消 或者收发完毕
                				{       	
                					boolean isClose=recS.resetState();
                					if(isClose)
                					{
//                						if(message.MESSAGE_VOICE_STATUS_TYPE!=MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL
//                								&&message.MESSAGE_VOICE_STATUS_TYPE!=MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)	
//                						{
//                							history.addRecordClip(message);//
//                						}
                						if(message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL
                								||message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION
                								||message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_COMPLETE)
                						{
                							RecordItem recordItem=history.getRecordItemByMessage(message);
                                    		if(recordItem!=null)
                                    		{
                                    			recordItem.reset();
                                    		}     
                						}
                						else
                						{
                							history.addRecordClip(message);//                							
                							Item session = app.currentSession;                							
                							if (session != null)
                							{
                								session.SESSION_contents.addElement(message);
                								session.SESSION_numOfNewMsg=Integer.toString(Integer.parseInt(session.SESSION_numOfNewMsg)+1);
                							} 
                							
                						}
                						removeDialog(UiEventListener.UI_EVENT_OUTER_VOICECLIP);
                                		edit.requestFocus();
                                		history.invalidate();
                                		app.currentOuterMessage=null;
                                		recS=null;
                                		message.MESSAGE_VOICE_DATA=null;
                                		System.gc();
                                		//app.isTransfering=false;
                        				if(app.isTransferingMessageVector.contains(message))
                        					app.isTransferingMessageVector.removeElement(message);
                					}
                				}
            				}

            			}

  		
            		}catch(Exception e)
            		{
            			e.printStackTrace();
            			Log.v("exception:", e.getMessage());
            		}
            		break;
            	}
            	case EventListener.EVENT_UPDATE_GAUGE_MESSAGE:
            	{
            		try
            		{
                		Item message=(Item)msg.obj;
                		if(message.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
                		{
                    		TransferItem tf=history.getTransferItemByMessage(message);
                    		if(tf!=null)
                    		{
                    			tf.setProgress(Integer.parseInt(message.MESSAGE_GAUGE_INDEX));
                    		}
                		}
            			else if(message.MESSAGE_type==MSNApplication.MESSAGE_TYPE_VOICECLIP)
            			{
            				if(recS!=null)//带外
            				{       	
            					recS.setProgress(Integer.parseInt(message.MESSAGE_GAUGE_INDEX));
            				}
            			}

            		}catch(Exception e)
            		{
            			e.printStackTrace();
            			Log.v("exception:", e.getMessage());
            		}
            		break;
            	}
            	case EventListener.EVENT_SEND_MESSAGE:
            	{
            		try
            		{

                		Item message=(Item)msg.obj;
		
            			if(message.MESSAGE_type == app.MESSAGE_TYPE_FILE_TRANSFER)
            			{
            				history.addTransferFile(message);          				
            			}
            			else if(message.MESSAGE_type==app.MESSAGE_TYPE_VOICECLIP)
            			{
            				history.addRecordClip(message); 
            				
            			}
            			else
            			{
            				history.addMessage(message,message.MESSAGE_IS_SEND);
            			}
                		
                		edit.requestFocus();
                		final InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
            			imm.hideSoftInputFromWindow(history.getWindowToken(), 0);
                		history.invalidate();
            		}catch(Exception e)
            		{
            			e.printStackTrace();
            		}

            		break;
            	}
            	case EventListener.EVENT_UNBLOCK_CONTACT_OK:
            	case EventListener.EVENT_BLOCK_CONTACT_OK:
            	case EventListener.EVENT_ADD_CONTACT_OK:
            	case EventListener.EVENT_DELETE_CONTACT_OK:
            	case EventListener.EVENT_DELETE_BLOCK_CONTACT_OK:
            	{
            		Contact contact=(Contact)msg.obj;
            		ChangeTitleIcon(contact);
            		break;
            	}
            	case EventListener.EVENT_RECEIVE_VERIFY_FRIEND:
            	{
            		showDialog(UiEventListener.UI_EVENT_ADD_CONFRIM);
            		break;
            	}
            	case EventListener.EVENT_OVER_VERIFY_FRIEND:
            	{
            		break;
            	}
            	case EventListener.EVENT_UPDATE_CHAT_UI:
            	{
            		Contact contact=(Contact)msg.obj;
            		ChangeTitleIcon(contact);
            		break;
            	}
            	case EventListener.EVENT_FINISH_ACTIVITY:
            	{
            		finish();
            		break;
            	}
            	case EventListener.EVENT_UPDATE_SESSION_FLAG:
            	{
            		ChangeTitleIcon(null);
            		break;
            	}
            	case EventListener.EVENT_UPDATE_SESSION_TOP_NAME:
            	{
            		int childCount = history.getChildCount();
            		for(int i=0; i<childCount; i++)
            		{
            			if(history.getChildAt(i) instanceof TransferItem)
            			{
            				((TransferItem)history.getChildAt(i)).clickCancel();
            				break;
            			}
            		}
            		ChangeTitleIcon(null);
            		break;
            	}
            	case EventListener.EVENT_RECORD_COMPLETE:
            	{
            		if(recV!=null)
            		{
        				if(recV.stop())
        				{
        					String url=(String)msg.obj;

        					try 
        					{
    							SendFile(null,url);
    							(new File(url)).delete();
    						} 
        					catch (Exception e) 
    						{
    							
    							e.printStackTrace();
    						}
        				}    	
            		}
    				break;
            	}
            	default:
            		break;
            }
        }
    };   
	
	
	
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
    	if(keyCode == event.KEYCODE_BACK)
    	{
    		app.currentSession.SESSION_numOfNewMsg= "0";
    		this.finish();
    		return true;
    	}
        return super.onKeyDown(keyCode, event);
    }
    

    public void onDestroy()
    {
        app.popChat();
        app.screenManager.popActivity(this);
		super.onDestroy();
    	app.currentSession=null;
		app.setChatroomHandler(null);
		chatroomHandler=null;
    }
    

    public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	mMenu = menu;
    	menu.add(0, MENU_1, 0, R.string.text_send).setIcon(R.drawable.send);
    	menu.add(1, MENU_2, 0, R.string.text_input_image).setIcon(R.drawable.emoticons);
    	menu.add(2, MENU_3, 0, R.string.text_input_template).setIcon(R.drawable.inset);
    	menu.add(3, MENU_4, 0, R.string.block_title).setIcon(R.drawable.block);
    	menu.add(4, MENU_5, 0, R.string.unblock_title).setIcon(R.drawable.unblock);	
    	menu.add(5,MENU_INVITE,0,R.string.text_invite).setIcon(R.drawable.invite_menu);
    	menu.add(6,MENU_MEMBER_LIST,0,R.string.text_member_list).setIcon(R.drawable.taxis);
    	menu.add(7,MENU_SAVE_HISTORY,0,R.string.text_save_history).setIcon(R.drawable.savehistory);
    	SubMenu headMenu = menu.addSubMenu(0, MENU_SEND_AUDIO, 0, R.string.text_send_audio).setIcon(R.drawable.voicecilp);
		headMenu.add(0, MENU_SEND_VOICE_CLIP, 0, R.string.text_record_record);
		headMenu.add(0, MENU_SEND_AUDIO_FILE, 0, R.string.text_send_audio_file);
		SubMenu sendFileMenu = menu.addSubMenu(0, MENU_SEND_FILE, 0, R.string.text_send_file).setIcon(R.drawable.sendfile);
		sendFileMenu.add(0, MENU_SEND_PICTURE, 0, R.string.text_send_picture);
		sendFileMenu.add(0, MENU_ALL_FILE, 0, R.string.text_all_file);		
    	return true;
        }
    
    
    
    
    public boolean onPrepareOptionsMenu(Menu menu) {
        super.onPrepareOptionsMenu(menu);
        if(app.currentSession.SESSION_sessionType==MSNApplication.SESSION_TYPE_GROUP_CHAT)
        {
            tool.btn2.setVisibility(View.GONE);
            tool.btn3.setVisibility(View.GONE);
            tool.btn4.setVisibility(View.GONE);
			menu.findItem(MENU_1).setVisible(true);
			menu.findItem(MENU_2).setVisible(true);
			menu.findItem(MENU_3).setVisible(true);
			menu.findItem(MENU_4).setVisible(false);
			menu.findItem(MENU_5).setVisible(false);
			menu.findItem(MENU_INVITE).setVisible(true);
			menu.findItem(MENU_MEMBER_LIST).setVisible(true);
			menu.findItem(MENU_SEND_AUDIO).setVisible(false);
			menu.findItem(MENU_SEND_FILE).setVisible(false);
			if(getHistory() == true)
			{
				menu.findItem(MENU_SAVE_HISTORY).setVisible(true);
			}
			else
			{
				menu.findItem(MENU_SAVE_HISTORY).setVisible(false);
			}			
        }
        else
        {
            Vector chaters = (Vector) app.currentSession.SESSION_chaters;
            Contact contact=(Contact)chaters.firstElement();
    		
            Contact rosterContact=app.getContact(contact.CONTACT_imid,false);
    		if(rosterContact==null)//陌生人
    		{
    //////////////
    			if(contact!=null)
    			{
    				contact.CONTACT_contactFlag=MSNApplication.CONTACT_STRANGER_FLAG;
    			}
    			////////////
                tool.btn2.setVisibility(View.GONE);
                tool.btn3.setVisibility(View.GONE);
                tool.btn4.setVisibility(View.GONE);
                tool.btn5.setVisibility(View.GONE);
    			menu.findItem(MENU_1).setVisible(true);
    			menu.findItem(MENU_2).setVisible(true);
    			menu.findItem(MENU_3).setVisible(true);
    			menu.findItem(MENU_4).setVisible(false);
    			menu.findItem(MENU_5).setVisible(false);
    			menu.findItem(MENU_INVITE).setVisible(false);
    			menu.findItem(MENU_MEMBER_LIST).setVisible(false);
    			menu.findItem(MENU_SEND_AUDIO).setVisible(false);
    			menu.findItem(MENU_SEND_FILE).setVisible(false);
    			if(getHistory() == true)
    			{
    				menu.findItem(MENU_SAVE_HISTORY).setVisible(true);
    			}
    			else
    			{
    				menu.findItem(MENU_SAVE_HISTORY).setVisible(false);
    			}
    		}
    		else
    		{
    			//////////////
    			if(contact!=null)
    			{
    				app.currentSession.SESSION_chaters.removeElement(contact);
    				app.currentSession.SESSION_chaters.addElement(rosterContact);
    			}
    			////////////
    			if (rosterContact.CONTACT_contactFlag == MSNApplication.CONTACT_BLOCK_FLAG)//阻止
    			{
                    tool.btn2.setVisibility(View.GONE);
                    tool.btn3.setVisibility(View.GONE);
                    tool.btn4.setVisibility(View.GONE);
                    tool.btn5.setVisibility(View.GONE);
    				menu.findItem(MENU_1).setVisible(false);
    				menu.findItem(MENU_2).setVisible(false);
    				menu.findItem(MENU_3).setVisible(false);
    				menu.findItem(MENU_4).setVisible(false);
    				menu.findItem(MENU_5).setVisible(true);
        			menu.findItem(MENU_INVITE).setVisible(false);
        			menu.findItem(MENU_MEMBER_LIST).setVisible(false);
        			menu.findItem(MENU_SEND_AUDIO).setVisible(false);
        			menu.findItem(MENU_SEND_FILE).setVisible(false);
        			if(getHistory() == true)
        			{
        				menu.findItem(MENU_SAVE_HISTORY).setVisible(true);
        			}
        			else
        			{
        				menu.findItem(MENU_SAVE_HISTORY).setVisible(false);
        			}
    			}
    			else//非阻止状态
    			{
    				menu.findItem(MENU_1).setVisible(true);
    				menu.findItem(MENU_2).setVisible(true);
    				menu.findItem(MENU_3).setVisible(true);
    				menu.findItem(MENU_4).setVisible(true);
    				menu.findItem(MENU_5).setVisible(false);
    				
    				if(rosterContact.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE)
    				{
    					menu.findItem(MENU_INVITE).setVisible(true);
    					menu.findItem(MENU_SEND_AUDIO).setVisible(true);
    					menu.findItem(MENU_SEND_FILE).setVisible(true);
    				}
    				else
    				{
    					menu.findItem(MENU_SEND_AUDIO).setVisible(false);
    					menu.findItem(MENU_SEND_FILE).setVisible(false);
    					menu.findItem(MENU_INVITE).setVisible(false);
    				}    					
        			menu.findItem(MENU_MEMBER_LIST).setVisible(false);
        			if(getHistory() == true)
        			{
        				menu.findItem(MENU_SAVE_HISTORY).setVisible(true);
        			}
        			else
        			{
        				menu.findItem(MENU_SAVE_HISTORY).setVisible(false);
        			}
        			
    			}
    		}
        }
        return true;
    }
    
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case MENU_1:
        	String content=edit.getText().toString();
        	if(content==null||content.length()==0)
        		return true;
        	else
        	{
        		Item session = app.currentSession;
				//byte type = session.SESSION_sessionType;
				//{
					String sid = session.TOPITEM_JID;//群消息没有sid
					Vector chaters = (Vector) session.SESSION_chaters;
					if(chaters.size()>1)
						app.sendGroupMessage(sid, content);
					else if(chaters.size()==1)
					{
						Contact contact = (Contact) chaters.firstElement();
						app.sendMessage(contact, sid, content);					
					}
				//}
        	}
        	edit.setText(null);
			return true;
		case MENU_2:
			//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
			showDialog(UiEventListener.UI_EVENT_EMOTION);
			return true;
		case MENU_3:
			//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
			showDialog(UiEventListener.UI_EVENT_SHORTMESSAGE);
			return true;			
		case MENU_4:
			showDialog(UiEventListener.UI_EVENT_BLOCK);
			return true;
		case MENU_5:
			showDialog(UiEventListener.UI_EVENT_UNBLOCK);
			return true;
		case MENU_INVITE:
			showDialog(UiEventListener.UI_EVENT_INVITE);
			return true;
		case MENU_MEMBER_LIST:
			showDialog(UiEventListener.UI_EVENT_CHECK_INVITE);
			return true;
		case MENU_SAVE_HISTORY:
			showDialog(UiEventListener.UI_EVENT_SAVE_HISTORY);
			return true;
		case MENU_SEND_VOICE_CLIP:
			CreateRecord();
			return true;
		case MENU_SEND_AUDIO_FILE:
			Intent intent = new Intent(ChatRoomActivity.this,SearchFileActivity.class);		
		    intent.putExtra("type",MSNApplication.SEARCHFILE_CHATROOM_FLAG);
		    //只显示录制的AMR文件
		    intent.putExtra("isOnlyShowAMR", true);
			startActivityForResult(intent, MSNApplication.SELECT_FILE_FROM_EXPLORE);
			return true;
		case MENU_ALL_FILE:
			SelectSendFile();
			return true;
		case MENU_SEND_PICTURE:			
        	Intent cameraIntent = new Intent();
        	cameraIntent.setClass(ChatRoomActivity.this, CameraActivity.class);
        	cameraIntent.putExtra("type",MSNApplication.CAMERA_CHATROOM_FLAG);  
            startActivityForResult(cameraIntent, MSNApplication.SELECT_FILE_FROM_TAKE_PHONE);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
    
    private void ChangeTitleIcon(Contact currentContact)
    {
    	
    	byte sessionType = app.currentSession.SESSION_sessionType;
    	//#ifdef support_groupmessage
		if (sessionType == MSNApplication.SESSION_TYPE_GROUP_CHAT)
		{	
			//#ifdef support_enough_memory
			Vector chaters = app.currentSession.SESSION_chaters;
			StringBuffer sb = new StringBuffer(this.getText(R.string.text_multiple_dialog));
			boolean hasOldContact=false;
			Contact c;
			for (int i = 0; i < chaters.size(); i++)
			{
				c = null;
				c = (Contact) chaters.elementAt(i);
				if (c != null)
				{
					sb.append(c.CONTACT_nickname+ ",");
					if(currentContact == null)
					{
						
					}
					else
					{
						if(c.CONTACT_imid.equals(currentContact.CONTACT_imid))
							hasOldContact=true;
					}
				}
			}
			if(currentContact == null)
			{
				
			}
			else
			{
				if(!hasOldContact)
					return;
			}
			String text = sb.toString().substring(0, sb.length() - 1);
			title.setTitleText(text);
			//#else
			//把-去掉
			//# String text=Simplified.text_multiple_dialog.substring(0,Simplified.text_multiple_dialog.length()-1);
			//#endif
			Resources r = this.getResources();
			if (Integer.parseInt(app.currentSession.SESSION_numOfNewMsg)> 0)
			{
				tool.btn1.setVisibility(View.VISIBLE);
	        	tool.btn2.setVisibility(View.GONE);
	        	tool.btn3.setVisibility(View.GONE);
	        	tool.btn4.setVisibility(View.GONE);
	        	tool.btn5.setVisibility(View.VISIBLE);
				//多人对话的图片
				title.icon.setImageDrawable(r.getDrawable(R.drawable.groupconvhavemsg));
			} 
			else
			{
				tool.btn1.setVisibility(View.VISIBLE);
	        	tool.btn2.setVisibility(View.GONE);
	        	tool.btn3.setVisibility(View.GONE);
	        	tool.btn4.setVisibility(View.GONE);
	        	tool.btn5.setVisibility(View.VISIBLE);
				title.icon.setImageDrawable(r.getDrawable(R.drawable.groupconv));
			}				
		} 
		else 
		//#endif	
			if (sessionType == MSNApplication.SESSION_TYPE_CHAT)
		{
			Vector chaters = app.currentSession.SESSION_chaters;
			Resources r = this.getResources();
			Contact contact = (Contact) chaters.firstElement();
			Contact rosterContact=app.getContact(contact.CONTACT_imid,false);
			if(rosterContact==null)//陌生人
			{
				//////////////
				if(contact!=null)
				{
					contact.CONTACT_contactFlag=MSNApplication.CONTACT_STRANGER_FLAG;
				}
				////////////
			}
			else
			{
				//////////////
				if(contact!=null)
				{
					app.currentSession.SESSION_chaters.removeElement(contact);
					app.currentSession.SESSION_chaters.addElement(rosterContact);
				}
				////////////
			}
			//String text=null;
			if(contact!=null)
			{
				if(currentContact != null)	
				{
					if(contact.CONTACT_imid.equals(currentContact.CONTACT_imid))
					{
					}
					else
					{
						return;
					}
				}
				title.setTitleText(rosterContact==null?contact.CONTACT_nickname:rosterContact.CONTACT_nickname);
				if(rosterContact!=null&&(rosterContact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG) 
		        		&& (rosterContact.SUBITEM_STATE == app.STATE_OFFLINE))
		        {
		        	edit.setFocusableInTouchMode(false);
		        	edit.setFocusable(false);
		        	tool.btn1.setVisibility(View.GONE);
		        	tool.btn2.setVisibility(View.GONE);
		        	tool.btn3.setVisibility(View.GONE);
		        	tool.btn4.setVisibility(View.GONE);
		        	tool.btn5.setVisibility(View.GONE);
		        	title.icon.setImageDrawable(r.getDrawable(R.drawable.offline_block));
		        }
		        else if (rosterContact!=null&&(rosterContact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
		        		&& (rosterContact.SUBITEM_STATE != app.STATE_OFFLINE))
		        {
		        	edit.setFocusableInTouchMode(false);
		        	edit.setFocusable(false);
		        	tool.btn1.setVisibility(View.GONE);
		        	tool.btn2.setVisibility(View.GONE);
		        	tool.btn3.setVisibility(View.GONE);
		        	tool.btn4.setVisibility(View.GONE);
		        	tool.btn5.setVisibility(View.GONE);
		        	title.icon.setImageDrawable(r.getDrawable(R.drawable.online_block));
		        }
		        else
		        {
		        	//Contact rosterContact=app.getContact(contact.CONTACT_imid,false);
					if(rosterContact == null)
					{
			        	edit.setFocusableInTouchMode(true);
			        	edit.setFocusable(true);
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.offline));
		        		tool.btn1.setVisibility(View.VISIBLE);
			        	tool.btn2.setVisibility(View.GONE);
			        	tool.btn3.setVisibility(View.GONE);
			        	tool.btn4.setVisibility(View.GONE);
			        	tool.btn5.setVisibility(View.GONE);
			        	return;
					}
		        	edit.setFocusableInTouchMode(true);
		        	edit.setFocusable(true);
		        	if(rosterContact.SUBITEM_STATE == app.STATE_OFFLINE)
		        	{
		        		tool.btn1.setVisibility(View.VISIBLE);
			        	tool.btn2.setVisibility(View.GONE);
			        	tool.btn3.setVisibility(View.GONE);
			        	tool.btn4.setVisibility(View.GONE);
			        	tool.btn5.setVisibility(View.GONE);
		        	}
		        	else
		        	{
		        		tool.btn1.setVisibility(View.VISIBLE);
			        	tool.btn2.setVisibility(View.VISIBLE);
			        	tool.btn3.setVisibility(View.VISIBLE);
			        	tool.btn4.setVisibility(View.VISIBLE);
			        	tool.btn5.setVisibility(View.VISIBLE);
		        	}
		        	if (rosterContact.SUBITEM_STATE == app.STATE_ONLINE)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.online));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_BUSY)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.busy));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_AWAY)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.away));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_BeRightBack)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.away));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_InACall)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.busy));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_OutToLunch)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.away));
		        	}
		        	else if (rosterContact.SUBITEM_STATE == app.STATE_OFFLINE)
		        	{
		        		title.icon.setImageDrawable(r.getDrawable(R.drawable.offline));;
		        	}
		        }
			}
		} 
        title.invalidate();
    }


	private void SelectSendFile()
    {
		Intent intent = new Intent(ChatRoomActivity.this,SearchFileActivity.class);		
	    intent.putExtra("type",MSNApplication.SEARCHFILE_CHATROOM_FLAG);
	    intent.putExtra("isOnlyShowAMR", false);
		startActivityForResult(intent, MSNApplication.SELECT_FILE_FROM_EXPLORE);
    }
	
	private void SendFile(Intent data,String recordUrl)throws Exception
	{
		String filename=null;
		if(recordUrl==null)
			filename =data.getStringExtra("filename"); 
		else
			filename=recordUrl;
        FileInputStream fileinput = null;
        try {
        	fileinput = new FileInputStream(filename);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//To do
		DataInputStream dis = new DataInputStream(fileinput);
		if(dis == null)
		{
			return;
		}
		
		File file = new File(filename);
		String trans_file = file.getName();
		Long trans_size = file.length();
		if(trans_size<=0)
			return;
		String temp = Long.toString(trans_size);
		int size=Integer.parseInt(temp);
		byte[] rcv_bytes = null;
		rcv_bytes = new byte[(int)size];
		try {
			dis.readFully(rcv_bytes);
		} catch (IOException e) {
			// TODO Auto-generated catch block

			e.printStackTrace();
			return;
		}
		
		Item session = app.currentSession;
		String sid = session.TOPITEM_JID;//群消息没有sid
		Vector chaters = app.currentSession.SESSION_chaters;
		Contact contact = (Contact) chaters.firstElement();
		
		if(recordUrl==null)//文件
			app.sendTransferFileMessage(contact, sid, trans_file, temp,rcv_bytes);
		else//录音
		{
			
			if(app.isInnerSendVoice)//带内
			{

				app.sendTransferVoiceMessage(contact,sid,"audio/amr",String.valueOf(rcv_bytes.length),null,rcv_bytes,true);
			}
			else//带外
			{
				
				app.sendTransferVoiceMessage(contact,sid,"audio/amr",String.valueOf(rcv_bytes.length),null,rcv_bytes,false);
				showDialog(UiEventListener.UI_EVENT_OUTER_VOICECLIP);
			}
		}
	}
    
    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
    	switch(requestCode) 
    	{
    	case MSNApplication.SELECT_FILE_FROM_EXPLORE:
    		if(data == null)
    		{
    			return;
    		}
    		//else if(app.isTransfering)//
    		else if(app.isTransferingMessageVector.size()!=0)
    		{    	
    			Toast.makeText(this, (String) getText(R.string.text_transfer_only_one), Toast.LENGTH_SHORT).show();
    		}
    		else
    		{
    			try 
    			{
					SendFile(data,null);
				} 
    			catch (Exception e) 
    			{

					e.printStackTrace();
				}
    			
    		}
    		break;
    	case MSNApplication.SELECT_FILE_FROM_TAKE_PHONE:

    		if(resultCode == RESULT_OK)
    		{
    			if(app.CameraByte == null)
    			{
    				break;
    			}
    			//if(app.isTransfering)
    			if(app.isTransferingMessageVector.size()!=0)
    				Toast.makeText(this, (String) getText(R.string.text_transfer_only_one), Toast.LENGTH_SHORT).show();
    			else
    			{
    				//app.isTransfering=true;//
        			Item session = app.currentSession;
        			String sid = session.TOPITEM_JID;
        			Vector chaters = app.currentSession.SESSION_chaters;
        			Contact contact = (Contact) chaters.firstElement();
        			app.sendTransferFileMessage(contact, sid, System.currentTimeMillis()+".jpeg", String.valueOf(app.CameraByte.length),app.CameraByte);
    			}
    		}
    		break;
    	}
    }

    private void SaveHistory(String title)
    {
		if(app.currentSession==null)
			return;
		
		String tmp = title;
    	int idx = 0;
    	while(!app.DB.checkAlreadyTitle(MD5.toMD5(app.GetLiveID()), tmp))
		{
    		idx++;
    		tmp = title + "_" + Integer.toString(idx);
		} 
    	
    	title = tmp;
		
		Vector msgs=null;
		msgs=app.getSessionMessages(app.currentSession.TOPITEM_JID);
		if (msgs != null)
		{
            app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
            app.DB.initHistoryData(MD5.toMD5(app.GetLiveID()));
            Item msg;
            String str;
			for (int i = 0; i < msgs.size(); i++)
			{
				msg = null;
				msg = (Item) msgs.elementAt(i);
				if(msg.MESSAGE_type == app.MESSAGE_TYPE_FILE_TRANSFER)
				{
				}
				else if(msg.MESSAGE_type==app.MESSAGE_TYPE_VOICECLIP)
				{
				}
				else
				{
					str = addText(msg);
					app.DB.insertHistoryData(MD5.toMD5(app.GetLiveID()), title, str);
//					if(app.DB.checkNotAlready(MD5.toMD5(app.GetLiveID()), title, str))
//					{
//						app.DB.insertHistoryData(MD5.toMD5(app.GetLiveID()), title, str);
//					}
//					else
//					{
//						//To do pop error
//					}
				}
				
			}
		}	        
    }
    
    private boolean getHistory()
    {
		Vector msgs=null;
		msgs=app.getSessionMessages(app.currentSession.TOPITEM_JID);
		if (msgs != null)
		{
			if(msgs.size() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
    }

	public String addText(Item msg)
	{
		String show1=msg.MESSAGE_nickname + "(" + msg.MESSAGE_stamp+ "):";
		String text=msg.MESSAGE_body;
		if(msg.MESSAGE_status!=null&&msg.MESSAGE_status.equals("fail"))
		{
			if(msg.MESSAGE_body==null||msg.MESSAGE_body.length()==0)
				text=msg.MESSAGE_reason;
			else
				text=msg.MESSAGE_body+"\n"+msg.MESSAGE_reason;
		}
		
		if(msg.MESSAGE_type == app.MESSAGE_TYPE_FILE_TRANSFER||msg.MESSAGE_type ==app.MESSAGE_TYPE_VOICECLIP)
		{
			return show1+"\n";
		}
		else
		{
			if(!msg.MESSAGE_nudge)
			{
				return show1+"\n"+text + "\n";
			}
			else
			{
				return show1+"\n"+msg.MESSAGE_nickname +" "+ context.getText(R.string.text_shake) + "\n";
			}
		}
	}
}