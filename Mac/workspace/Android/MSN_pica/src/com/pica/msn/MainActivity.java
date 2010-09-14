package com.pica.msn;


import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Locale;
import java.util.Vector;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.telephony.gsm.SmsManager;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RadioGroup;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.LinearLayout.LayoutParams;

import com.pica.ui.tools.DetailInfoView;
import com.pica.ui.tools.EmotionTextView;
import com.pica.ui.tools.HistoryListView;
import com.pica.ui.tools.LabelView;
import com.pica.ui.tools.RecordSendView;
import com.pica.ui.tools.RosterItem;
import com.pica.ui.tools.RosterView;
import com.pica.ui.tools.SessionView;
import com.pica.ui.tools.ZwpView;


public class MainActivity extends Activity{
//	private Menu mMenu;
//	private Dialog detail;
//	private Dialog vcard;
//	private Dialog system;
//    private PanelSwitcher mPanelSwitcher;
    private DetailInfoView div = null;
	private static final int MENU_STATUS = Menu.FIRST;
	private static final int MENU_VCARD = Menu.FIRST + 1;
	
	
	private static final int MENU_SYSTEM = Menu.FIRST + 2;
	private static final int MENU_FLOW_SETTING=Menu.FIRST+3;
	private static final int MENU_ADD_ROSTER = Menu.FIRST + 4;
//	private static final int MENU_SEND_MESSAGE = Menu.FIRST + 5;
//	private static final int MENU_TAXI = Menu.FIRST + 6;
	private static final int MENU_CLOSE_CHAT=  Menu.FIRST + 7;
	private static final int MENU_UPDATE = Menu.FIRST + 8;
	private static final int MENU_SEARCH_ROSTER=Menu.FIRST+9;
	private static final int MENU_ROSTER_HELP = Menu.FIRST + 10;
	private static final int MENU_LOGOUT=  Menu.FIRST + 11;
	private static final int MENU_EXIT=Menu.FIRST+12;
	private static final int MENU_SESSION_HELP=Menu.FIRST+13;
	private static final int MENU_CHECK_HISTORY=Menu.FIRST+14;
//	private static final int MENU_TAXI_GROUP = Menu.FIRST + 15;
//	private static final int MENU_TAXI_STATUS = Menu.FIRST + 16;
	private static final int MENU_HEAD=Menu.FIRST+17;
	private static final int MENU_HEAD_FROM_FILE=Menu.FIRST+18;
	private static final int MENU_HEAD_FROM_CAMERA=Menu.FIRST+19;
	//////////////////增加按状态排序功能
	private static final int MENU_SORT_ROSTER = Menu.FIRST + 20;
	private static final int MENU_SORT_ROSTER_BY_STATUS = Menu.FIRST + 21;
	private static final int MENU_SORT_ROSTER_BY_GROUP = Menu.FIRST + 22;
	/////////////	
	public int mCurView = 0;
	public LabelView lab;
	private String errorText;
	//private MediaPlayer mMediaPlayer;
	private RadioGroup mRadio;
	private boolean bPause = false;
	MSNApplication app;
	RosterView mRoster;
	SessionView mSession;
	ZwpView mZwp;
	
	/**联系人列表 普通分组 可以显示离线状态的联系人列表*/
	//private RosterList roster;

	//#ifdef support_exchange_contact_group
	
	/**联系人列表 普通分组 仅显示在线联系人*/
	//public RosterList normalOnlyOnlineRoster;
	
	
	private Contact mCurItem;
	private int mSingleChoiceIdx;
	private String historyText = null;
	private static Context context;
	RecordSendView recS;
	private HistoryListView list;

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
		    Log.e("charRoomActivity", "to LoginMonitorActivity");            
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
	
	
    
    public void onCreate(Bundle savedInstanceState) 
{
        super.onCreate(savedInstanceState);
        
         ConnectivityManager connMgr = (ConnectivityManager)MainActivity.this.getSystemService(Context.CONNECTIVITY_SERVICE);
        
        //////////////////////解决进程被杀死的bug
        if(savedInstanceState != null && savedInstanceState.getBoolean("isKilled") && app == null)
        {
        	isKilled = true;
        	finish();
        	app = MSNApplication.getApplication(this);
        	app.screenManager.pushActivity(this);
        	app.pushMain(this);
        	if(!app.killedToRelogin)//没有在别的Activity中重新登录
        	{
        		reLogin(false, savedInstanceState);
        		//Log.e("MainActivity", "iskilled then relogin");
        	}        	
        	
        	return;
        }
        /////////////////////////////////
        app = MSNApplication.getApplication(this);
        //Log.e("MainActivity", "onCreate");
        context=this;
        //final Window win = getWindow();
        //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);     
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        app.screenManager.pushActivity(this);
        app.pushMain(this);
        //////////////////////解决进程被杀死的bug，因为当Activity的状态是visible时，
        //////////////////////在被杀死之前不会进入onSaveInstanceState，所以做特殊处理
        if(app.myVCARD_myNickname == null)
        {
        	//Log.e("app.myVCARD", "null");
        	Intent toSplashActivity = new Intent(this, SplashActivity.class);
        	startActivity(toSplashActivity);
        	finish();
        	return;
        }

        /////////////////////
        app.setConMgr(connMgr);
        app.initBitmap();
        app.SetMainHandler(MainHandler);
        mRoster = new RosterView(this);
        mSession = new SessionView(this);
        mZwp = new ZwpView(this);
        mCurView=0;

        final LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));
        ///////////////
		Bitmap bmp = BitmapFactory.decodeResource(getResources(),EmotionTextView.emotion_index[0]);
		BitmapDrawable bmD = new BitmapDrawable(bmp);
		
		EmotionTextView.emotionWidth = (int)(bmD.getIntrinsicHeight()*app.screenScale);
		EmotionTextView.emotionHeight = (int)(bmD.getIntrinsicWidth()*app.screenScale);
		
	      ///////////////////
        LayoutParams Rparams = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT, 1);
//		mPanelSwitcher = new PanelSwitcher(this, this, null);
//		mPanelSwitcher.setLayoutParams(new ViewGroup.LayoutParams(
//                ViewGroup.LayoutParams.FILL_PARENT,
//                ViewGroup.LayoutParams.FILL_PARENT));
//		mPanelSwitcher.setLayoutParams(Rparams);
//		mPanelSwitcher.addView(mRoster);
//		mPanelSwitcher.addView(mSession);
//		mPanelSwitcher.setViewInLayout(mRoster, mSession);
//      layout.addView(mPanelSwitcher);
        
        mRoster.setLayoutParams(Rparams);
        layout.addView(mRoster);
        mRoster.setVisibility(View.VISIBLE);
        
        mSession.setLayoutParams(Rparams);
        layout.addView(mSession);
        mSession.setVisibility(View.GONE);
        
        mZwp.setLayoutParams(Rparams);
        layout.addView(mZwp);
        mZwp.setVisibility(View.GONE);
		
        LayoutParams Labparams = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        lab = new LabelView(this, 0);
        lab.setLayoutParams(Labparams);
        lab.setVisibility(View.VISIBLE);
        layout.addView(lab);
//        app.getVersion(app.GetIMSI(), app);
        setContentView(layout);
        
        if (app.zwp_homePageAddress != null && app.jabber.zipWapServer != null)
        {
	        mZwp.zParser.tempBaseUrl = app.getBaseUrl(app.zwp_homePageAddress);
	        app.jabber.sendZipwapRequest(app.zwp_homePageAddress, null, (byte)1, app.myVCARD_myNickname, app.agent);
        }
        
//        mRoster.rlist.adImage.adImage.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View arg0) {
//				//发送zwap请求
//				makeAdLink(0);
//			}
//	    });
    }
    
    private void makeAdLink(int type)
    {
    	Item item = new Item();
    	if(type == 0)
    	{
    		item = app.adRosterItem;
    	}
    	else
    	{
    		item = (Item)app.ad_chat_Vector.get(app.ad_chat_idx);
    	}
    	
		if(item.AD_flag == MSNApplication.AD_WAP_FLAG)
		{	
			app.jabber.advistClick(item.AD_dbid, item.AD_sale);
			item.AD_target = app.getNewStringReplaceUSERID(item.AD_target);
	    	Uri uri = Uri.parse(item.AD_target);
	    	if(uri == null)
			{
				return;
			}
	    	Intent returnIt = new Intent(Intent.ACTION_VIEW, uri);
	    	this.startActivity(returnIt);
	    	return;
		}
		else if(item.AD_flag == MSNApplication.AD_ZWP_FLAG)
		{
			app.jabber.advistClick(item.AD_dbid, item.AD_sale);
			mCurView = 2;
			lab.rosterItem.setRoster(false);
			lab.sessionItem.setSession(false);
			lab.zwpItem.setZwap(true);
			lab.rosterItem.setBackgroundResource(R.drawable.lab_unselect);
			lab.sessionItem.setBackgroundResource(R.drawable.lab_unselect);
			lab.zwpItem.setBackgroundResource(R.drawable.lab_select);
			mRoster.setVisibility(View.GONE);
			mSession.setVisibility(View.GONE);
			mZwp.setVisibility(View.VISIBLE);
			
			item.AD_target = app.getNewStringReplaceUSERID(item.AD_target);
			app.tempUrl = item.AD_target;
	        mZwp.isNeedSave = true;
	        mZwp.zParser.tempBaseUrl = app.getBaseUrl(app.tempUrl);
	        mZwp.LoadZwp();
	        app.bZwpDialog = true;
	        app.bZwpLoad = true;
	        app.jabber.sendZipwapRequest(item.AD_target, null, (byte)0, app.myVCARD_myNickname, app.agent);
	        return;
		}
		else if(item.AD_flag == MSNApplication.AD_IVR_FLAG)
		{
			app.jabber.advistClick(item.AD_dbid, item.AD_sale);
			Uri callUri = Uri.parse("tel:" + item.AD_target);
			if(callUri == null)
			{
				return;
			}
			Intent returnIt = new Intent(Intent.ACTION_CALL, callUri);
			startActivity(returnIt);
			return;
		}
		else if(item.AD_flag == MSNApplication.AD_SMS_FLAG)
		{
			app.jabber.advistClick(item.AD_dbid, item.AD_sale);
/*			item.AD_param0 = app.getNewStringReplaceUSERID(item.AD_param0);
			Uri uri = Uri.parse("smsto:" + item.AD_target); 
			if(uri == null)
			{
				return;
			}
			Intent it = new Intent(Intent.ACTION_SENDTO, uri);   
			it.putExtra("sms_body", item.AD_param0);   
			startActivity(it);*/
			SmsManager manager = SmsManager.getDefault();
			manager.sendTextMessage(item.AD_target,null,item.AD_param0,null,null);

			return;
		}		
    }
    
    
    public void onResume() 
    {
       super.onResume();
       app.mNotificationManager.cancel(R.layout.main);
       /////////////////解决进程被杀死的bug
       if(isKilled && app == null)
       {
    	   finish();
    	   return;
       }
       /////////////////////////////
       app.isPause = false;
       app.mCurActivityIdx = app.Main_Activity;
  	   mSession.slist.UpdateView();
  	   mRoster.rlist.UpdateView();
  	   if(mCurView == 0)
  	   {
           lab.rosterItem.setRoster(true);
           lab.sessionItem.setSession(false);
           lab.zwpItem.setZwap(false);
  	   }
  	   else if(mCurView == 1)
  	   {         
           lab.rosterItem.setRoster(false);
           lab.sessionItem.setSession(true);
           lab.zwpItem.setZwap(false);
  	   }
  	   else if(mCurView == 2)
  	   {         
           lab.rosterItem.setRoster(false);
           lab.sessionItem.setSession(false);
           lab.zwpItem.setZwap(true);
  	   }
 	   app.SetCurActivity(this);
	   if(app.bChatAdClick == true)
	   {
		   	app.bChatAdClick = false;
		   	makeAdLink(1);
	   }
    }
    
    public void onPause()
    {
    	super.onPause();
    	//Log.e("MainActivity", "onPause");
    	app.isPause = true;
    }
    
    public void onDestroy()
    {
        app.popMain();
        app.screenManager.popActivity(this);
		super.onDestroy();
		if(app != null)
		{
		app.SetMainHandler(null);
		}		
		MainHandler=null;
    }
    
    public Handler MainHandler = new Handler() { 
         
        public void handleMessage(Message msg) 
        {
            switch (msg.what) 
            { 
            	case EventListener.EVENT_MAIN_AD_CLICK:
            		makeAdLink(0);
            		break;
            	case EventListener.EVENT_ROSTER:
            	{
            		//mPanelSwitcher.moveRight();
            		mCurView = 0;
                    lab.rosterItem.setRoster(true);
                    lab.sessionItem.setSession(false);
                    lab.zwpItem.setZwap(false);
                    lab.rosterItem.setBackgroundResource(R.drawable.lab_select);
                    lab.sessionItem.setBackgroundResource(R.drawable.lab_unselect);
                    lab.zwpItem.setBackgroundResource(R.drawable.lab_unselect);
                    mRoster.setVisibility(View.VISIBLE);
                    mSession.setVisibility(View.GONE);
                    mZwp.setVisibility(View.GONE);
            		break;
            	}
            	case EventListener.EVENT_SESSION:
            	{
            		//mPanelSwitcher.moveLeft();
//    				mSession.currentIndex = -1;为了解决
//    				mSession.select_id = -1;   某个bug
            		mCurView = 1;
                    lab.rosterItem.setRoster(false);
                    lab.sessionItem.setSession(true);
                    lab.zwpItem.setZwap(false);
                    lab.rosterItem.setBackgroundResource(R.drawable.lab_unselect);
                    lab.sessionItem.setBackgroundResource(R.drawable.lab_select);
                    lab.zwpItem.setBackgroundResource(R.drawable.lab_unselect);
                    mRoster.setVisibility(View.GONE);
                    mSession.setVisibility(View.VISIBLE);
                    mZwp.setVisibility(View.GONE);
            		break;
            	}
            	case EventListener.EVENT_ZWP:
            	{
            		mCurView = 2;
                    lab.rosterItem.setRoster(false);
                    lab.sessionItem.setSession(false);
                    lab.zwpItem.setZwap(true);
                    lab.rosterItem.setBackgroundResource(R.drawable.lab_unselect);
                    lab.sessionItem.setBackgroundResource(R.drawable.lab_unselect);
                    lab.zwpItem.setBackgroundResource(R.drawable.lab_select);
                    mRoster.setVisibility(View.GONE);
                    mSession.setVisibility(View.GONE);
                    mZwp.setVisibility(View.VISIBLE);
            		break;
            	}
            	case EventListener.EVENT_ZWP_LOAD:
            	{
            		showDialog(UiEventListener.UI_EVENT_ZWAP_LOAD);
            		break;
            	}
            	case EventListener.EVENT_ZWP_CANCEL:
            	{
            		removeDialog(UiEventListener.UI_EVENT_ZWAP_LOAD);
            		break;
            	}
            	case EventListener.EVENT_MY_VCARD_RECEIVED:
            	case EventListener.EVENT_CHANGE_MY_STATE:
            	{
            		mRoster.resetMyItem();
            		break;
            	}
            	case EventListener.EVENT_UPDATE_CONTACT_UI:
            	{
            		if(mCurView == 0)
            		{
            			mRoster.rlist.UpdateView();
            		}
            		
            		if(mCurView == 1)
            		{
            			mSession.slist.UpdateView();
            		}
            		break;
            	}     		
            	case EventListener.EVENT_RECEIVED_MESSAGE:
            	{
            		if(app.isPause == false)
            		{
            			//lab.item2.splashMessage(true);
            		    lab.sessionItem.updateMessage();
            		}
            		else
            		{
            		    setNotify(R.drawable.roster_flash_1,msg.obj);
            		}
            		mSession.slist.UpdateView();
            		break;
            	}       	
            	case EventListener.EVENT_ADD_NEW_SESSION:
            	{
            		mSession.slist.UpdateView();
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
            	case EventListener.EVENT_SOUND_NEW_MESSENGE:
            	{
            		if(app.isOpenNewMessageSound)
            		{
            			app.playAudio(1);
            		}
            		break;
            	}
            	case EventListener.EVENT_SOUND_NEW_NUDGE:
            	{
            	    if(app.isOpenNewMessageSound)
            	    {
            		app.playAudio(2);
            	    }
            	    if(app.isNudge)
            	    {           		
                    Vibrator vibrator = (Vibrator)getSystemService(context.VIBRATOR_SERVICE);
                    vibrator.vibrate(1000);
            	    }
            		break;
            	}
            	case EventListener.EVENT_SOUND_NEW_ONLINE:
            	{
            		if(System.currentTimeMillis()-Jabber.timeLast>2500)
            		{
                		if(app.isOpenOnlineSound)
                		{
                			app.playAudio(0);
                			MSNApplication.hasContactOnline=false;//叫了后 不用再叫
                			Jabber.timeLast = System.currentTimeMillis();
                			//Log.v("sound", "sound");
                		}
            		}
            		break;
            	}
            	case EventListener.EVENT_RECEIVE_OTHER_ERROR:
            	{
            		errorText=(String)msg.obj;
            		showDialog(UiEventListener.UI_EVENT_ERROR_MESSAGE);
            		break;
            	}
            	case EventListener.EVENT_UPDATE_INPRESEA_UI:
            	{
            		EmotionTextView etv=(EmotionTextView)msg.obj;
            		etv.invalidate();
            		break;
            	}
            	case EventListener.EVENT_FINISH_ACTIVITY:
            	{         		
            		app.mNotificationManager.cancelAll();
            		finish();
            		break;
            	}
            	case EventListener.EVENT_CLOSE_TO_LOGIN:
            	{
                    //给段提示
            		app.mNotificationManager.cancelAll();
            		app.isStart = false;
                 	String text=(String)getText(R.string.log_lose);
                	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
                    Intent intent;
                    intent = new Intent();
                    intent.setClass(context, LoginActivity.class);
                    intent.putExtra("type",MSNApplication.LOGOUT_UNAVALIABLE_FLAG);
                    startActivity(intent);
                    finish();
                    break;
            	}
            	case EventListener.EVENT_RECEIVE_LOGIN_ERROR_MAIN:
            	{
               	 if(msg.obj!=null)
            	 {     	 
            		 Object[] temp=(Object[])msg.obj;
            		 String errorText=(String)temp[0];   
            		 String errorCode=(String)temp[1];
            		 
            		 if(errorCode==null)
            		 {
 						app.jabber.forceStop=true;//强制退出发包线程循环
 	                  	String text=(String)getText(R.string.error_prompt)+ (String)getText(R.string.error_content)+errorText;
 	                 	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
 	                    Intent intent;
 	                    intent = new Intent();
 	                    intent.setClass(context, LoginActivity.class);
 	                    intent.putExtra("type",MSNApplication.LOGOUT_UNAVALIABLE_FLAG);
 	                    startActivity(intent);

            		 }
            		 else
            		 {
     					//#ifdef support_save_rms	
     					if(errorCode.equals("1402"))
     					{
     						String text=(String)getText(R.string.error_code)+errorCode+(String)getText(R.string.error_content)+errorText;
     	                 	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
     						showDialog(UiEventListener.UI_EVENT_UPDATE);
     					
     					}
     					else  if(errorCode.equals("1403"))
     					{
     						app.deleteSynchronizedAllRecordStore();
     						app.jabber.forceStop=true;//强制退出发包线程循环
      	                  	String text=(String)getText(R.string.error_code)+errorCode+(String)getText(R.string.error_content)+errorText;
     	                 	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
     	                    Intent intent;
     	                    intent = new Intent();
     	                    intent.setClass(context, LoginActivity.class);
     	                    intent.putExtra("type",MSNApplication.LOGOUT_UNAVALIABLE_FLAG);
     	                    startActivity(intent);
     					}
     					else if(errorCode.equals("1600"))
     					{
     						app.jabber.forceStop=true;//强制退出发包线程循环
     						Intent intent = new Intent();
     						intent.setClass(context, LoginActivity.class);
     						intent.putExtra("type", MSNApplication.LOGOUT_ABALIABLE_FLAG);
     						intent.putExtra("errorText", errorText);
     						startActivity(intent);
     						
     					}
     					else
     					//#endif	
    					{
     						app.jabber.forceStop=true;//强制退出发包线程循环
      	                  	String text=(String)getText(R.string.error_code)+errorCode+(String)getText(R.string.error_content)+errorText;
     	                 	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
     	                    Intent intent;
     	                    intent = new Intent();
     	                    intent.setClass(context, LoginActivity.class);
     	                    intent.putExtra("type",MSNApplication.LOGOUT_UNAVALIABLE_FLAG);
     	                    startActivity(intent);
     					}
            		 }

            	 }
               	 break;
            	}
	   	         case EventListener.EVENT_RECEIVE_SYNCHRONIZE_ERROR:
						app.deleteSynchronizedAllRecordStore();
						app.jabber.forceStop=true;//强制退出发包线程循环
		                 String text=(String)getText(R.string.error_content) + (String)getText(R.string.synchronize_fail);//"错误提示:\n个人资料或者联系人同步失败，现在必须注销并重新登录";
		                 Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
		                  Intent intent;
		                  intent = new Intent();
		                  intent.setClass(context, LoginActivity.class);
		                  intent.putExtra("type",MSNApplication.LOGOUT_UNAVALIABLE_FLAG);
		                    startActivity(intent);
		                   break; 
		        
	   	         case EventListener.EVENT_RECEIVE_CONTACT_PORTRAIT:
	   	        	Contact c=(Contact)msg.obj;
					////刷新页面 无需另外处理 如果当前焦点正在这个联系人上 就即时显示出来头像
					if(c.SUBITEM_STATE==app.STATE_OFFLINE||c.CONTACT_contactFlag ==app.CONTACT_BLOCK_FLAG)
					{
						//黑名单和脱机的 都不显示自定义头像
						c.CONTACT_imageData=null;
						return;
					}
					else
						mRoster.rlist.UpdateView();  
					break;
	   	         case EventListener.EVENT_SEND_VCARDHEAD_I:
	   	         {
	   	        	Intent data =(Intent)msg.obj; 
	 				try {
						sendVcardHead(data);
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
	   	        	 break;
	   	         }
	   	         case EventListener.EVENT_SEND_VCARDHEAD_P:
	   	         {
	   	        	sendVcardHeadFromPhoto();
	   	        	break;
	   	         }
	   	         case EventListener.EVENT_UPDATE_OUTER_HEAD_STATUS:
     				if(recS!=null)//带外  
    				{       	
     					Contact contact=(Contact)msg.obj;
     					contact.CONTACT_FILE_DATA=null;
    					removeDialog(UiEventListener.UI_EVENT_OUTER_VCARDHEAD);
                    	app.currentOuterContact=null;
                    	app.isTransferingOuterSendHead=false;
                    	recS=null;
                    	System.gc();
    				}
	   	        	 break;
	   	         case EventListener.EVENT_UPDATE_OUTER_HEAD_GAUGE:
	   	        	Contact contact=(Contact)msg.obj;
     				if(recS!=null)//带外
    				{       	
    					recS.setProgress(Integer.parseInt(contact.CONTACT_GAUGE_INDEX));
    				}
	   	        	 break;
	   	         case EventListener.EVENT_ZIPWAP_OK:
	   	         {
	   	        	if(app.bZwpLoad == false)
	   	        	{
	   	        		app.bZwpLoad = true;
	   	        		break;
	   	        	}
	   	        	Object[] obj=(Object[])msg.obj;
	   	        	mZwp.splash((DataInputStream)obj[0], (Integer)obj[1]);
	   	         }
	   	        	 break;
	   	         case EventListener.EVENT_AD_UPDATA:
	   	         {
	   	        	mRoster.rlist.updateADImage();
	   	         }
	   	         break;
	   	      case EventListener.OPEN_CHAT_FROM_SEARCH_ROSTER:
	   	        	openChatroomUIFromContactList((Contact)msg.obj);
				////////////////////	
	   	        	break;
	   	      case EventListener.EVENT_MY_ITEM_LOGO:
	   	              selectFilePhoto();
	   	            break;
	           case EventListener.EVENT_MY_ITEM_INFO:             
	                   showDialog(UiEventListener.UI_EVENT_SET_VCARD);
                   break;
	           case EventListener.EVENT_UPDATE_PORTRAIT_HASH:
	               //mRoster.rlist.getPortaitHash((Contact)msg.obj);
	               break;
            	default:
            		break;
            }
        }
    };   

    public boolean dispatchKeyEvent(KeyEvent event) 
    {
    	
    	
        if ( (event.getKeyCode() == KeyEvent.KEYCODE_BACK) )
        {
	        if(event.getAction() == KeyEvent.ACTION_UP) 
	    	{
	    		return true;
	    	}
	        
    		removeDialog(UiEventListener.UI_EVENT_SET_VCARD);
    		removeDialog(UiEventListener.UI_EVENT_SET_SYSTEM);
    		removeDialog(UiEventListener.UI_EVENT_DETAIL_INFO);
    		removeDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
    		removeDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
    		removeDialog(UiEventListener.UI_EVENT_ADD_CONFRIM);
    		removeDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
    		removeDialog(UiEventListener.UI_EVENT_DELETE_HISTORY);
    		removeDialog(UiEventListener.UI_EVENT_FLOW_SETTING);
    		removeDialog(UiEventListener.UI_EVENT_ZWAP_LOAD);
    		if(div != null)
    		{
    			div = null;
    		}
    		
    		if(mCurView == 2)
    		{
    			mZwp.goBack();
    		}
            return true;
        }
        else if (event.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT)
        {        	
        	if(mCurView == 1)//由对话列表页面切换到联系人页面
        	{
        		if(app.MainHandler!=null)
                {
               	 	Message m = new Message(); 
				     m.what = EventListener.EVENT_ROSTER; 
				     app.MainHandler.sendMessage(m); 					     
                }
        	}
        	if(mCurView == 2)//由zwap页面切换到对话列表页面
        	{
        		if(app.MainHandler!=null)
                {
               	 	Message m = new Message(); 
				    m.what = EventListener.EVENT_SESSION; 
				    app.MainHandler.sendMessage(m); 
				    lab.sessionItem.updateMessage();
				    RosterItem.hideHead();
                }
        	}
        }
        else if (event.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT)
        {
        	if(mCurView == 0)//由联系人页面切换到对话列表页面
        	{
        		if(app.MainHandler!=null)
                {
               	 	Message m = new Message(); 
				    m.what = EventListener.EVENT_SESSION; 
				    app.MainHandler.sendMessage(m); 
//        			 lab.item2.splashMessage(false);
				    RosterItem.hideHead();
                }
        	}
        	if(mCurView == 1)//由对话列表页面切换到zwap页面
        	{
        		if(app.MainHandler!=null)
                {
               	 	Message m = new Message(); 
				    m.what = EventListener.EVENT_ZWP; 
				    app.MainHandler.sendMessage(m); 
				    lab.zwpItem.updateMessage();
				    RosterItem.hideHead();
                }
        	}
        	
        }
		return super.dispatchKeyEvent(event);
    }

    public void setCurWnd(int idx)
    {
    	mCurView = idx;
    }
    
    public void onConfigurationChanged(Configuration newConfig) 
    {
    	super.onConfigurationChanged(newConfig);
    	app.bRefoot = true;
//    	Contact me = new Contact();
//        me.SUBITEM_STATE = app.getMyState();
//        me.CONTACT_contactFlag = app.CONTACT_NORMAL_FLAG;
//        if(app.myVCARD_myNickname.length() == 0)
//        {
//        	me.CONTACT_nickname = app.GetLiveID();
//        }
//        else
//        {
//        	me.CONTACT_nickname = app.myVCARD_myNickname;
//        }
//        me.CONTACT_impresa = app.myVCARD_myImpresa;
    	mRoster.resetMyItem();
        mSession.slist.UpdateView();
        mRoster.rlist.UpdateView();
		if(div != null)
		{
			//removeDialog(UiEventListener.UI_EVENT_DETAIL_INFO);
			//showDialog(UiEventListener.UI_EVENT_DETAIL_INFO); 
		}
    	return;
    }
    
    
    public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0, MENU_STATUS, 0, R.string.mystatus).setIcon(R.drawable.mystause);
		menu.add(0, MENU_VCARD, 0, R.string.vcard_set).setIcon(R.drawable.accountinfo);
		//////////
		SubMenu subHead = menu.addSubMenu(0, MENU_HEAD, 0, R.string.text_modify_head).setIcon(R.drawable.changicon);
		subHead.add(0, MENU_HEAD_FROM_FILE, 0, R.string.text_head_from_file);
		subHead.add(0, MENU_HEAD_FROM_CAMERA, 0, R.string.text_take_camera);
		//////////
		menu.add(0, MENU_SYSTEM, 0, R.string.set_system).setIcon(R.drawable.systemset);
		menu.add(0,MENU_FLOW_SETTING,0,R.string.text_flow_setting).setIcon(R.drawable.flowset);
		menu.add(0, MENU_ADD_ROSTER, 0, R.string.add_roster).setIcon(R.drawable.addroster);
		
		//////////////////增加按状态排序功能
		SubMenu sortRoster = menu.addSubMenu(0, MENU_SORT_ROSTER, 0, R.string.text_taxi).setIcon(R.drawable.taxis);
		sortRoster.add(0, MENU_SORT_ROSTER_BY_GROUP, 0, R.string.text_taxi_group);
		sortRoster.add(0, MENU_SORT_ROSTER_BY_STATUS, 0, R.string.text_taxi_status);
		///////////

//		SubMenu subTaxi = menu.addSubMenu(0, MENU_TAXI, 0, R.string.text_taxi);
//		subTaxi.add(0, MENU_TAXI_GROUP, 0, R.string.text_taxi_group);
//		subTaxi.add(0, MENU_TAXI_STATUS, 0, R.string.text_taxi_status);
		menu.add(0, MENU_CLOSE_CHAT, 0, R.string.delete_chat).setIcon(R.drawable.closechat);
		menu.add(0, MENU_UPDATE, 0, R.string.text_upgrade_contact_list).setIcon(R.drawable.updatelist);
		menu.add(0,MENU_SEARCH_ROSTER,0,R.string.search_roster).setIcon(R.drawable.findroster);
		menu.add(0, MENU_ROSTER_HELP, 0, R.string.help).setIcon(R.drawable.appinfo);
		menu.add(0, MENU_LOGOUT, 0, R.string.logout).setIcon(R.drawable.signout);
		menu.add(0,MENU_EXIT,0,R.string.exit).setIcon(R.drawable.exit);
		menu.add(0,MENU_SESSION_HELP,0,R.string.help).setIcon(R.drawable.appinfo);
		menu.add(0,MENU_CHECK_HISTORY,0,R.string.text_check_history).setIcon(R.drawable.checkhistory);	
        return true;
    }
    
    public boolean onPrepareOptionsMenu(Menu menu)
    {
    	super.onPrepareOptionsMenu(menu);
		if(this.mCurView==0)
		{
			menu.findItem(MENU_STATUS).setVisible(true);
			menu.findItem(MENU_VCARD).setVisible(true);
			menu.findItem(MENU_SYSTEM).setVisible(true);
			menu.findItem(MENU_FLOW_SETTING).setVisible(true);
			menu.findItem(MENU_ADD_ROSTER).setVisible(true);
//			menu.findItem(MENU_SEND_MESSAGE).setVisible(true);
			menu.findItem(MENU_HEAD).setVisible(true);
			menu.findItem(MENU_SORT_ROSTER).setVisible(true);
			//menu.findItem(MENU_TAXI).setVisible(true);
			menu.findItem(MENU_UPDATE).setVisible(true);
			menu.findItem(MENU_ROSTER_HELP).setVisible(true);
			menu.findItem(MENU_LOGOUT).setVisible(true);
			menu.findItem(MENU_EXIT).setVisible(true);
			menu.findItem(MENU_CLOSE_CHAT).setVisible(false);
			menu.findItem(MENU_SESSION_HELP).setVisible(false);
			menu.findItem(MENU_CHECK_HISTORY).setVisible(false);
			menu.findItem(MENU_SEARCH_ROSTER).setVisible(true);
		}
		else if(this.mCurView==1)
		{
			menu.findItem(MENU_STATUS).setVisible(false);
			menu.findItem(MENU_VCARD).setVisible(false);
			menu.findItem(MENU_SYSTEM).setVisible(false);
			menu.findItem(MENU_FLOW_SETTING).setVisible(false);
			menu.findItem(MENU_ADD_ROSTER).setVisible(false);
//			menu.findItem(MENU_SEND_MESSAGE).setVisible(false);
			menu.findItem(MENU_HEAD).setVisible(false);
			menu.findItem(MENU_SORT_ROSTER).setVisible(false);
			//menu.findItem(MENU_TAXI).setVisible(false);
			menu.findItem(MENU_UPDATE).setVisible(false);
			menu.findItem(MENU_ROSTER_HELP).setVisible(false);
			menu.findItem(MENU_LOGOUT).setVisible(false);
			menu.findItem(MENU_EXIT).setVisible(false);
			if((app.sessionData.size() > 0) && (mSession.currentIndex != -1))
			{
				menu.findItem(MENU_CLOSE_CHAT).setVisible(true);
			}
			else
			{
				menu.findItem(MENU_CLOSE_CHAT).setVisible(false);
			}
			menu.findItem(MENU_SESSION_HELP).setVisible(true);
			menu.findItem(MENU_SEARCH_ROSTER).setVisible(false);
			if(app.DB.checkHistory(MD5.toMD5(app.GetLiveID())) != null)
			{
				menu.findItem(MENU_CHECK_HISTORY).setVisible(true);			
			}
			else
			{
				menu.findItem(MENU_CHECK_HISTORY).setVisible(false);
			}
			menu.findItem(MENU_SEARCH_ROSTER).setVisible(false);
		}		
		else if(this.mCurView == 2)
		{
			menu.findItem(MENU_STATUS).setVisible(false);
			menu.findItem(MENU_VCARD).setVisible(false);
			menu.findItem(MENU_SYSTEM).setVisible(false);
			menu.findItem(MENU_FLOW_SETTING).setVisible(false);
			menu.findItem(MENU_ADD_ROSTER).setVisible(false);
//			menu.findItem(MENU_SEND_MESSAGE).setVisible(false);
			menu.findItem(MENU_HEAD).setVisible(false);
			menu.findItem(MENU_SORT_ROSTER).setVisible(false);
			//menu.findItem(MENU_TAXI).setVisible(false);
			menu.findItem(MENU_UPDATE).setVisible(false);
			menu.findItem(MENU_ROSTER_HELP).setVisible(false);
			menu.findItem(MENU_LOGOUT).setVisible(false);
			menu.findItem(MENU_EXIT).setVisible(false);
			menu.findItem(MENU_CLOSE_CHAT).setVisible(false);
			menu.findItem(MENU_SESSION_HELP).setVisible(false);			
			menu.findItem(MENU_CHECK_HISTORY).setVisible(false);
			menu.findItem(MENU_SEARCH_ROSTER).setVisible(false);
		}
		return true;
    }
    
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case MENU_STATUS:
			showDialog(UiEventListener.UI_EVENT_SET_STATUS);
			return true;
		case MENU_VCARD:
			showDialog(UiEventListener.UI_EVENT_SET_VCARD);
			return true;
		case MENU_SYSTEM:
			//SetSystem();
			showDialog(UiEventListener.UI_EVENT_SET_SYSTEM);
			return true;			
		case MENU_ADD_ROSTER:
			showDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
			return true;
//		case MENU_SEND_MESSAGE:
//			showDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
//			return true;
		case MENU_CLOSE_CHAT:
			deleteOneSession();
			return true;
		case MENU_UPDATE:
			showDialog(UiEventListener.UI_EVENT_UPDATE);
			return true;			
		case MENU_ROSTER_HELP:
			showDialog(UiEventListener.UI_EVENT_HELP);
			return true;
		case MENU_LOGOUT:
		    app.mNotificationManager.cancelAll();
			showDialog(UiEventListener.UI_EVENT_LOGOUT);
			return true;
		case MENU_EXIT:
		    app.mNotificationManager.cancelAll();
			showDialog(UiEventListener.UI_EVENT_EXIT);
			return true;
		case MENU_SESSION_HELP:
			showDialog(UiEventListener.UI_EVENT_SESSION_HELP);
			return true;
		case MENU_CHECK_HISTORY:
			showDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
			return true;
//		case MENU_TAXI_GROUP:
//		case MENU_TAXI_STATUS:
//			return true;
		case MENU_FLOW_SETTING:
			showDialog(UiEventListener.UI_EVENT_FLOW_SETTING);
			return true;		
		case MENU_HEAD_FROM_FILE:
			selectFilePhoto();
			return true;
		case MENU_HEAD_FROM_CAMERA:
			selectTakePhoto();
			return true;
		case MENU_SEARCH_ROSTER:
			Intent intent = new Intent(MainActivity.this, SearchRosterActivity.class);
			startActivity(intent);
			return true;	
			
		//////////////////增加按状态排序功能	
		case MENU_SORT_ROSTER_BY_GROUP:
			
			if(app.getGroupOrder() > MSNApplication.ROSTER_NORMAL_NO_OFFLINE)//按组分组
			{
				app.setGroupOrder((byte)(app.getGroupOrder() - 2));
			}
			app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_NORMAL, Boolean.toString(app.isShowOnline));
			Message msg = new Message();
			msg.what = EventListener.EVENT_UPDATE_CONTACT_UI;
			MainHandler.sendMessage(msg);			
			return true;
			
		case MENU_SORT_ROSTER_BY_STATUS:
			if(app.getGroupOrder() <= MSNApplication.ROSTER_NORMAL_NO_OFFLINE)//按状态分组
			{
				app.setGroupOrder((byte)(app.getGroupOrder() + 2));
			}
			app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_STATE, Boolean.toString(app.isShowOnline));
			Message m = new Message();
			m.what = EventListener.EVENT_UPDATE_CONTACT_UI;
			MainHandler.sendMessage(m);
			return true;
		/////////////////////
	
		}
		return super.onOptionsItemSelected(item);
	}
	

	
//	private void selectFilePhoto()
//    {
//		Intent intent = new Intent(this,SearchFileActivity.class);
//        intent.putExtra("type",MSNApplication.SEARCHFILE_MAINROSTER_FLAG);
//		startActivityForResult(intent, MSNApplication.SELECT_FILE);
//    }
	
	private void selectFilePhoto()
    {
        Intent innerIntent = new Intent(Intent.ACTION_GET_CONTENT);
        innerIntent.setType("image/*");
        Intent wrapperIntent = Intent.createChooser(innerIntent,  null);
        startActivityForResult(wrapperIntent, MSNApplication.SELECT_FILE_FROM_IMAGE);
    }

	private void selectTakePhoto()
	{
		Intent intent;
    	intent = new Intent();
    	intent.setClass(this, CameraActivity.class);
		intent.putExtra("type",MSNApplication.CAMERA_MAINROSTER_FLAG);  
        startActivityForResult(intent, MSNApplication.SELECT_FILE_FROM_TAKE_PHONE);
	}
	
    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
    	switch(requestCode) 
    	{
    	case MSNApplication.SELECT_FILE_FROM_IMAGE:
	    		if(resultCode == RESULT_OK)
	    		{
    		if(data == null)
    		{
    			return;
    		}
    		try 
			{
    			Message m = new Message(); 
    			m.obj = (Object)data;
				m.what = EventListener.EVENT_SEND_VCARDHEAD_I; 
				MainHandler.sendMessageDelayed(m, 1000);
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
	
				if(!app.isInnerSendHead)//带外
				{
					if(app.isTransferingOuterSendHead)
					{
						Toast.makeText(this, (String) getText(R.string.text_transfer_only_one), Toast.LENGTH_SHORT).show();
						return;
					}
					Contact myItemContact=new Contact();     
    				app.currentOuterContact=myItemContact;
    				app.isTransferingOuterSendHead=true;
    				myItemContact.CONTACT_GAUGE_INDEX="0";
    				myItemContact.CONTACT_FILE_DATA=app.CameraByte;
    				app.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, myItemContact);
    				myItemContact.CONTACT_FILEOUTTRANSFER=new FileOuterTransfer(app,FileOuterTransfer.command_select_uploadFile,null,myItemContact,app.data_url);   				
    				showDialog(UiEventListener.UI_EVENT_OUTER_VCARDHEAD);
				}
				else//带内
				{
	    			Message m = new Message(); 
					m.what = EventListener.EVENT_SEND_VCARDHEAD_P; 
					MainHandler.sendMessageDelayed(m, 1000);				
				}			
    		}
    		break;
    	}
    }
    
    private void sendVcardHeadFromPhoto()
    {
    	//如果图片大于900K
		if(app.CameraByte.length > 1024*900)
		{
			Toast.makeText(MainActivity.this, getText(R.string.text_pixel_too_high), Toast.LENGTH_LONG).show();
			return;
		}
		//如果图片大于24K
		if(app.CameraByte.length > 1024*24)
		{			
			Bitmap b = null;
			try
			{
				BitmapFactory.Options option = new BitmapFactory.Options();
				option.inSampleSize = 4;
				b = BitmapFactory.decodeByteArray(app.CameraByte, 0, app.CameraByte.length, option);
			}
			catch (Exception e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(b != null)
			{
				Matrix m = new Matrix();
				float scale = (1024*24*100)/(app.CameraByte.length);
				float sx = (float) Math.sqrt(scale)/10;
				float sy = (float) Math.sqrt(scale)/10;
				//缩放图片
				m.setScale(sx, sy);
				b = Bitmap.createBitmap(b, 0, 0, b.getWidth(), b.getHeight(), m, true);			
				ByteArrayOutputStream os = new ByteArrayOutputStream();
				
				if(b.compress(Bitmap.CompressFormat.JPEG, 60, os))
				{
					app.CameraByte = os.toByteArray();
					b.recycle();
				}
			}
		}
    	
		String base64Data=MsnUtil.encode(app.CameraByte);
		if(base64Data == null)
		{
			return;
		}
		String base64Length=String.valueOf(base64Data.length());
		app.jabber.sendModifyMyHead("image/jpeg", base64Length, base64Data, null);
		Toast.makeText(MainActivity.this, getText(R.string.text_uploading_outer_head), Toast.LENGTH_SHORT).show();
    }
	
    /**发送个人头像*/
    private void sendVcardHead(Intent data)throws Exception
	{
    	Uri u = data.getData();
        Cursor cursor = getContentResolver().query(u, new String[]{MediaStore.Images.ImageColumns.DATA}, null, null, null); 
        cursor.moveToFirst();
        String filename = cursor.getString(0); 
        FileInputStream fileinput = null;
        try 
        {
        	fileinput = new FileInputStream(filename);
		} 
        catch (FileNotFoundException e) 
		{
			e.printStackTrace();
		}
        
		DataInputStream dis = new DataInputStream(fileinput);
		if(dis == null)
		{
			return;
		}
		File file = new File(filename);
		String trans_file = file.getName();
		Long trans_size = file.length();
		if(trans_size<=0)
		{
			return;
		}
			
		String temp = Long.toString(trans_size);
		int size=Integer.parseInt(temp);
		byte[] rcv_bytes = null;
		rcv_bytes = new byte[(int)size];
		try 
		{
			dis.readFully(rcv_bytes);
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
			return;
		}
		
		//如果图片大于900K
		if(rcv_bytes.length > 1024*900)
		{
			Toast.makeText(MainActivity.this, getText(R.string.text_out__of_memory), Toast.LENGTH_LONG).show();
			return;
		}
		//如果图片大于24K
		if(rcv_bytes.length > 1024*24)
		{			
			Bitmap b = null;
			try
			{
				BitmapFactory.Options option = new BitmapFactory.Options();
				option.inSampleSize = 4;
				b = BitmapFactory.decodeByteArray(rcv_bytes, 0, rcv_bytes.length, option);
			}
			catch (Exception e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(b != null)
			{
				Matrix m = new Matrix();
				float scale = (1024*24*100)/(rcv_bytes.length);
				float sx = (float) Math.sqrt(scale)/10;
				float sy = (float) Math.sqrt(scale)/10;
				//缩放图片
				m.setScale(sx, sy);
				b = Bitmap.createBitmap(b, 0, 0, b.getWidth(), b.getHeight(), m, true);			
				ByteArrayOutputStream os = new ByteArrayOutputStream();
				
				if(b.compress(Bitmap.CompressFormat.JPEG, 60, os))
				{
					rcv_bytes = os.toByteArray();
					b.recycle();
				}
			}
		}
		
		if(app.isInnerSendHead)
		{
			String base64Data=MsnUtil.encode(rcv_bytes);
    		String base64Length=String.valueOf(base64Data.length());
			app.jabber.sendModifyMyHead("image/jpeg", base64Length, base64Data, null);
			Toast.makeText(MainActivity.this, getText(R.string.text_uploading_outer_head), Toast.LENGTH_SHORT).show();
		}
		else
		{
			//界面 带外发送
			if(app.isTransferingOuterSendHead)
			{
				Toast.makeText(this, (String) getText(R.string.text_transfer_only_one), Toast.LENGTH_SHORT).show();
				return;
			}
			app.isTransferingOuterSendHead=true;
			Contact myItemContact=new Contact();        
			app.currentOuterContact=myItemContact;				
			myItemContact.CONTACT_GAUGE_INDEX="0";
			myItemContact.CONTACT_FILE_DATA=rcv_bytes;
			myItemContact.CONTACT_FILE_VOICE_NAME=trans_file;
			app.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, myItemContact);
			myItemContact.CONTACT_FILEOUTTRANSFER=new FileOuterTransfer(app,FileOuterTransfer.command_select_uploadFile,null,myItemContact,app.data_url);
			
			showDialog(UiEventListener.UI_EVENT_OUTER_VCARDHEAD);
		}

	}
    
	public void deleteOneSession()
	{
		int s=mSession.currentIndex;
		if(s==-1)
		{
			return;
		}
		else
		{
			Item item = app.sessionData.get(s).get("SESSION");
			if(item!=null)
			{
				showDialog(UiEventListener.UI_EVENT_DELETE_ONE_SESSION);
			}

		}
		
	}
	
	public void PopOperaMenu(final Contact item)
	{
		//Todo 如果是自己怎么处理
		mCurItem = item;
		showDialog(mCurItem.CONTACT_contactFlag);	
		return;		
	}
	
	public boolean isCurrentTransfering=false;
	public Vector isCurrentTransferMsgVector=new Vector();//
	
    
    protected Dialog onCreateDialog(int id) {

    	if(id == app.CONTACT_NORMAL_FLAG)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.opear_normal, null);
            view.setBackgroundColor(Color.WHITE);
            final ListView listView = (ListView)view.findViewById(R.id.normal);
            listView.setAdapter(new ArrayAdapter<String>(this,
                    android.R.layout.simple_list_item_1, 
                    new String[] {(String)getText(R.string.open_chat),
            		(String)getText(R.string.detail),
            		(String)getText(R.string.delete_roster_title),
            		(String)getText(R.string.block_title)}));
            
            listView.setOnItemClickListener(new OnItemClickListener() { 
				
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) {
					operaNormalItem(mCurItem, arg2);
					dismissDialog(app.CONTACT_NORMAL_FLAG);
				} 
            });
            return new AlertDialog.Builder(MainActivity.this)
            .setView(view)
            .create();    		
    	}    		
    	else if(id == app.CONTACT_BLOCK_FLAG)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.opear_block, null);
            view.setBackgroundColor(Color.WHITE);
            final ListView listView = (ListView)view.findViewById(R.id.block);
            listView.setAdapter(new ArrayAdapter<String>(this,
                    android.R.layout.simple_list_item_1, 
                    new String[] {(String)getText(R.string.detail),
            		(String)getText(R.string.delete_roster_title),
            		(String)getText(R.string.unblock_title)}));
            
            listView.setOnItemClickListener(new OnItemClickListener() { 
				
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) {
					// TODO Auto-generated method stub
					operaBlockItem(mCurItem, arg2);
					dismissDialog(app.CONTACT_BLOCK_FLAG);
				} 
            });
            return new AlertDialog.Builder(MainActivity.this)
            .setView(view)
            .create();     		
    	}
    	else if(id == UiEventListener.UI_EVENT_SET_STATUS)
    	{
    		int i = app.getMyState();
    		setSingleChoiceIdx(i);
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.set_status, null);
            final RadioGroup radio = (RadioGroup)view.findViewById(R.id.menu);
        	if(i == 0)
        	{
        		radio.check(R.id.set_online);
        	}
        	else if(i == 1)
        	{
        		radio.check(R.id.set_busy);
        	}
        	else if(i == 2)
        	{
        		radio.check(R.id.set_away);
        	}
        	else if(i == 3)
        	{
        		
        		radio.check(R.id.set_berightback);
        	}
        	else if(i == 4)
        	{
        		radio.check(R.id.set_onthephone);
        	}
        	else if(i == 5)
        	{
        		radio.check(R.id.set_outtolunch);
        	}
        	else if(i == 6)
        	{
        		radio.check(R.id.set_hide);
        	}

            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.mystatus)
            .setView(view)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	int idx = radio.getCheckedRadioButtonId();
                	if(idx == R.id.set_online)
                	{
                		setSingleChoiceIdx(0);
                	}
                	else if(idx == R.id.set_busy)
                	{
                		setSingleChoiceIdx(1);
                	}
                	if(idx == R.id.set_away)
                	{
                		setSingleChoiceIdx(2);
                	}
                	if(idx == R.id.set_berightback)
                	{
                		setSingleChoiceIdx(3);
                	}
                	if(idx == R.id.set_onthephone)
                	{
                		setSingleChoiceIdx(4);
                	}
                	if(idx == R.id.set_outtolunch)
                	{
                		setSingleChoiceIdx(5);
                	}
                	if(idx == R.id.set_hide)
                	{
                		setSingleChoiceIdx(6);
                	}
                	app.jabber.sendModifyMyState(app.STATE_FLAG_All[mSingleChoiceIdx]);
                	removeDialog(UiEventListener.UI_EVENT_SET_STATUS);
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_SET_STATUS);
                    /* User clicked Cancel so do some stuff */
                }
            })
            .create();       		
    	}
    	else if(id==UiEventListener.UI_EVENT_DELETE_ONE_SESSION)
    	{
    		isCurrentTransfering=false;
    		if(isCurrentTransferMsgVector==null)
    			isCurrentTransferMsgVector=new Vector();//
    		else
    			isCurrentTransferMsgVector.removeAllElements();
    		
    		Item session = app.sessionData.get(mSession.currentIndex).get("SESSION");
    		Item msg;
    		for(int i=0;i<session.SESSION_contents.size();i++)
    		{
    			msg = null;
    			msg=(Item)session.SESSION_contents.elementAt(i);
//    			if(msg.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
//    			{
//    				if(msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_SENDING
//    						||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.mess
//    						||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING
//    						||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_SENDING)
//    				{
//    					isCurrentTransfering=true;
//    					isTransferMsgVc.addElement(msg);
//    				}
//    				else
//    					continue;
//
//    			}
//    			else if(msg.MESSAGE_type ==MSNApplication.MESSAGE_TYPE_VOICECLIP)
//    			{
//    				if(msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING
//    						||msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING)
//    				{
//    					isCurrentTransfering=true;
//    					isTransferMsgVc.addElement(msg);
//    				}
//    				else
//    					continue;
//    			}
    			if(app.isTransferingMessageVector.contains(msg))
    			{
    				isCurrentTransfering=true;
    				isCurrentTransferMsgVector.addElement(msg);
    			}
    			else
    				continue;
    		}

        	return new AlertDialog.Builder(MainActivity.this)
        		.setCancelable(false)
                .setIcon(R.drawable.msn)
                .setTitle(R.string.delete_chat)//关闭对话 要把所有的传输部分都关闭掉 只需要关闭网络部分 不同再通知界面了 因为要整个删除掉
                .setMessage(isCurrentTransfering?R.string.text_close_transfering_session:R.string.confirm_delete_chat)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() 
                {
                    public void onClick(DialogInterface dialog, int whichButton) 
                    {
    					Item session = app.sessionData.get(mSession.currentIndex).get("SESSION");	
    					//#ifdef support_groupmessage				
    					//只有多人对话 才发包

    						if(isCurrentTransfering)
    						{
    							Item tempMsg;
    							for(int k=0;k<isCurrentTransferMsgVector.size();k++)
    							{
    								tempMsg = null;
    								tempMsg=(Item)isCurrentTransferMsgVector.elementAt(k);
    								if(tempMsg.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
    								{
        			    				if(tempMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING
        			    						||tempMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_SENDING)
        			    				{
        			  					  	app.jabber.sendTransferCancel(tempMsg.MESSAGE_FILE_transferID, tempMsg.MESSAGE_sid, tempMsg.MESSAGE_FILE_ftID);
        			  		    			if(app.isTransferingMessageVector.contains(tempMsg))
        			  		    			{
        			  		    				app.isTransferingMessageVector.removeElement(tempMsg);
        			  		    			}
        			    				}
        			    				else if(tempMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING)
        			    				{
        			    					app.jabber.sendTransferInviteResponse(tempMsg.MESSAGE_FILE_transferID, tempMsg.MESSAGE_sid, tempMsg.MESSAGE_FILE_ftID, "reject");
        			  		    			if(app.isTransferingMessageVector.contains(tempMsg))
        			  		    			{
        			  		    				app.isTransferingMessageVector.removeElement(tempMsg);
        			  		    			}
        			    				}
        			    				else if(tempMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_SENDING)
        			    				{
        			    					 app.jabber.sendTransferCancel(tempMsg.MESSAGE_FILE_transferID, tempMsg.MESSAGE_sid, tempMsg.MESSAGE_FILE_ftID);
         			  		    			if(app.isTransferingMessageVector.contains(tempMsg))
        			  		    			{
        			  		    				app.isTransferingMessageVector.removeElement(tempMsg);
        			  		    			}
        			    				}
        			    				
    								}
    								else if(tempMsg.MESSAGE_type ==MSNApplication.MESSAGE_TYPE_VOICECLIP)
    								{
        			    				if(tempMsg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING)
        			    				{
        			    					tempMsg.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL);			    				
        			    				}
        			    				else if(tempMsg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING)
        								{
        			    					tempMsg.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL);
        								}
    								}
    								
    							}
    						}
        					//if(session.SESSION_sessionType==MSNApplication.SESSION_TYPE_GROUP_CHAT)
        					{
        						app.jabber.sendCloseSession(session.TOPITEM_JID);
        					}
    					//#endif
    					
                    	               	
        				app.sessionData.remove(mSession.currentIndex);
        				mSession.slist.UpdateView();
        				mSession.invalidate();
        				mSession.currentIndex = -1;
        				mSession.select_id = -1;
        				lab.sessionItem.updateMessage();
        				removeDialog(UiEventListener.UI_EVENT_DELETE_ONE_SESSION);
                    }
                })
                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() 
                {
                    public void onClick(DialogInterface dialog, int whichButton) 
                    {
                    	removeDialog(UiEventListener.UI_EVENT_DELETE_ONE_SESSION);
                    }
                })
                .create();      
    	}
    	else if(id == UiEventListener.UI_EVENT_SET_VCARD)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            //final String OldNick;
            //final String OldInfo;
            final View textEntryView = factory.inflate(R.layout.vcardentry, null);
            final EditText nick = (EditText)textEntryView.findViewById(R.id.nick_edit);
            final EditText info = (EditText)textEntryView.findViewById(R.id.info_edit);
            info.setText(app.myVCARD_myImpresa);
            //OldNick = nick.getText().toString();
            //OldInfo = info.getText().toString();
            if(app.myVCARD_myNickname.length() == 0)
            {
            	nick.setText(app.GetLiveID());
            }
            else
            {
            	nick.setText(app.myVCARD_myNickname);
            }

            return new AlertDialog.Builder(MainActivity.this)
            	.setCancelable(false)
                .setIcon(R.drawable.msn)
                .setTitle(R.string.vcard_set)
                .setView(textEntryView)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	
                    	String oldImpresa=(app.myVCARD_myImpresa==null?"":app.myVCARD_myImpresa);
                    	String newNickname=nick.getText().toString();
        				String newImpresa=info.getText().toString();
        				if(newNickname==null||newNickname.trim().length()==0)
        					newNickname=app.GetLiveID();
        				if(newImpresa==null||newImpresa.trim().length()==0)
        					newImpresa="";
        				if(newNickname.equals(app.myVCARD_myNickname)&&newImpresa.equals(oldImpresa))
        				{
        					removeDialog(UiEventListener.UI_EVENT_SET_VCARD);// 返回到联系人页面
        					return; //没有改变个人资料
        				}
        				else
        				{
        					boolean isChangeNickname=false;
        					boolean isChangeImpresa=false;
        					if(!newNickname.equals(app.myVCARD_myNickname))
        						isChangeNickname=true;
        					if(!newImpresa.equals(oldImpresa))
        						isChangeImpresa=true;
                        	app.jabber.sendModifyMyVcard(nick.getText().toString(), info.getText().toString(), isChangeNickname, isChangeImpresa);
                        	removeDialog(UiEventListener.UI_EVENT_SET_VCARD);
        				}
                        /* User clicked OK so do some stuff */
                    }
                })
                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	removeDialog(UiEventListener.UI_EVENT_SET_VCARD);
                        /* User clicked cancel so do some stuff */
                    }
                })
                .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_DELETE_ROSTER)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View DeleteRosterView = factory.inflate(R.layout.deleteroster, null);
            final CheckBox check = (CheckBox)DeleteRosterView.findViewById(R.id.check);
            if(mCurItem.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
            {
            	check.setChecked(true);
            }
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.delete_roster_title)
            .setView(DeleteRosterView)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() 
            {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
                	
            		String tmp = MsnUtil.getUsernameIsValid(mCurItem.CONTACT_imid, false);
                	if(tmp != null)
                	{
                		errorText = tmp;
                		removeDialog(UiEventListener.UI_EVENT_DELETE_ROSTER);
                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
                		return;
                	}
                	
                	
                	if(check.isChecked())
                	{
                		app.jabber.sendOperateContact(mCurItem.SUBITEM_JID, "rmblk");
                	}
                	else
                	{
                		app.jabber.sendOperateContact(mCurItem.SUBITEM_JID, "delete");
                	}
                    /* User clicked OK so do some stuff */
                	removeDialog(UiEventListener.UI_EVENT_DELETE_ROSTER);
                }
                
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_DELETE_ROSTER);
                    /* User clicked Cancel so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_BLOCK)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.block_title)
            .setMessage(R.string.block_info)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
            		String tmp = MsnUtil.getUsernameIsValid(mCurItem.CONTACT_imid, false);
                	if(tmp != null)
                	{
                		errorText = tmp;
                		removeDialog(UiEventListener.UI_EVENT_BLOCK);
                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
                		return;
                	}
                	
                	
                	app.jabber.sendOperateContact(mCurItem.SUBITEM_JID, "block");
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
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.unblock_title)
            .setMessage(R.string.unblock_info)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() 
            {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
                	
            		String tmp = MsnUtil.getUsernameIsValid(mCurItem.CONTACT_imid, false);
                	if(tmp != null)
                	{
                		errorText = tmp;
                		removeDialog(UiEventListener.UI_EVENT_UNBLOCK);
                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
                		return;
                	}
                	
                	app.jabber.sendOperateContact(mCurItem.SUBITEM_JID, "unblock");
                	removeDialog(UiEventListener.UI_EVENT_UNBLOCK);
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                	removeDialog(UiEventListener.UI_EVENT_UNBLOCK);
                }
            })
           .create();   		
    	}
    	else if(id == UiEventListener.UI_EVENT_ADD_ROSTER)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View AddRosterView = factory.inflate(R.layout.addroster, null);
            final EditText edit = (EditText)AddRosterView.findViewById(R.id.edit);
            edit.setText(null);
            edit.addTextChangedListener(watcher);
            final RadioGroup radio = (RadioGroup)AddRosterView.findViewById(R.id.menu);
            SetAdapt(radio);
            radio.check(R.id.hot_com);
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.add_roster)
            .setView(AddRosterView)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                	String str = edit.getText().toString();

                	int idx = radio.getCheckedRadioButtonId();
                	if(idx == R.id.hot_com)
                	{
                		str += "@hotmail.com";
                	}
                	else if(idx == R.id.msn_com)
                	{
                		str += "@msn.com";
                	}
                	else if(idx == R.id.live_com)
                	{
                		str += "@live.com";
                	}
                	else if(idx == R.id.live_cn)
                	{
                		str += "@live.cn";
                	}
                	else if(idx == R.id.msn_cn)
                	{
                		str += "@msn.cn";
                	}
                	String tmp = MsnUtil.getUsernameIsValid(str, false);
                	if(tmp != null)
                	{
                		errorText = tmp;
                		removeDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
                		return;
                	}
                	
                	//添加联系人 不再加判断了 服务器会返回
//                	Item object=app.getContact(str,false);
//                	
//    	    		if (object == null) 
//    	            {            
//    	    			
//    	            } 
//    	            else  
//    	            {	
//                		removeDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
//                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
//                		return;
//    	            } 
    	    		
                	app.jabber.sendAddContact(str);
                	removeDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
                    /* User clicked OK so do some stuff */
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_ADD_ROSTER);
                    /* User clicked Cancel so do some stuff */
                }
            })
            .create();    		
    	}
    	else if(id == UiEventListener.UI_EVENT_ERROR_USERNAME)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.alert_dialog_icon)
            .setTitle(R.string.error_prompt)
            .setMessage(errorText)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                	removeDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
                }
            })
            .create();
    	}
//    	else if(id == UiEventListener.UI_EVENT_SEND_ROSTER)
//    	{
//            LayoutInflater factory = LayoutInflater.from(this);
//            final View AddRosterView = factory.inflate(R.layout.addroster, null);
//            final EditText edit = (EditText)AddRosterView.findViewById(R.id.edit);
//            edit.setText(null);
//            edit.addTextChangedListener(watcher);
//            final RadioGroup radio = (RadioGroup)AddRosterView.findViewById(R.id.menu);
//            radio.check(R.id.hot_com);
//            SetAdapt(radio);
//            return new AlertDialog.Builder(MainActivity.this)
//            .setCancelable(false)
//            .setIcon(R.drawable.msn)
//            .setTitle(R.string.send_roster)
//            .setView(AddRosterView)
//            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
//                public void onClick(DialogInterface dialog, int whichButton) {
//
//                	String str = edit.getText().toString();
//
//                	int idx = radio.getCheckedRadioButtonId();
//                	if(idx == R.id.hot_com)
//                	{
//                		str += "@hotmail.com";
//                	}
//                	else if(idx == R.id.msn_com)
//                	{
//                		str += "@msn.com";
//                	}
//                	else if(idx == R.id.live_com)
//                	{
//                		str += "@live.com";
//                	}
//                	else if(idx == R.id.live_cn)
//                	{
//                		str += "@live.cn";
//                	}
//                	else if(idx == R.id.msn_cn)
//                	{
//                		str += "@msn.cn";
//                	}
//                	String tmp = MsnUtil.getUsernameIsValid(str, false);
//                	if(tmp != null)
//                	{
//                		errorText = tmp;
//                		removeDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
//                		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
//                		return;
//                	}
//                	
//                	Contact stranger = null;
//                	Contact object=app.getContact(str,false);
//                	
//    	    		if (object == null) 
//    	            {            
//    	    			stranger = app.newStranger(str);
//    	            } 
//    	            else  
//    	            {	//在联系人列表         
//    	            	Contact contact=object;
//    	                if(contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
//    	                {  //该联系人不在您的允许名单里，您无法向其发送消息
//    	                	errorText=(context.getText(R.string.text_cannot_send_msg)).toString();
//                    		removeDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
//                    		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
//    	                    return;
//    	                }
//    	                else 
//    	                {      	
//    	                	stranger =object;
//    	                }			       
//    	            }                 	
//                	
//                	openChatroomUIFromContactList(stranger);
//                	removeDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
//                    /* User clicked OK so do some stuff */
//                }
//            })
//            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
//                public void onClick(DialogInterface dialog, int whichButton) {
//                	removeDialog(UiEventListener.UI_EVENT_SEND_ROSTER);
//                    /* User clicked Cancel so do some stuff */
//                }
//            })
//            .create();    		
//    	}
    	else if(id == UiEventListener.UI_EVENT_SET_SYSTEM)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.set_system, null);
            final CheckBox online_check = (CheckBox)view.findViewById(R.id.online_ring);
            if(app.isOpenOnlineSound)
            {
            	online_check.setChecked(true);
            }
            final CheckBox message_check = (CheckBox)view.findViewById(R.id.message_ring);
            if(app.isOpenNewMessageSound)
            {
            	message_check.setChecked(true);
            }
            final CheckBox nudge_check = (CheckBox)view.findViewById(R.id.nudge);
            if(app.isNudge)
            {
                nudge_check.setChecked(true);
            }
            final CheckBox show_check = (CheckBox)view.findViewById(R.id.show_online);
            if(app.isShowOnline)
            {
            	show_check.setChecked(true);
            }
//            final CheckBox getIcon_check = (CheckBox)view.findViewById(R.id.roster_icon);
//            if(app.isReceiveHeadChange)
//            {
//            	getIcon_check.setChecked(true);
//            }
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.set_system)
            .setView(view)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
                	boolean oldIsOpenOnlineSound=app.isOpenOnlineSound;
                	boolean oldIsOpenNewMessageSound=app.isOpenNewMessageSound;
                	boolean oldIsNudge=app.isNudge;
                	boolean oldIsOnlyShowOnline=app.isShowOnline;
                	
                	if(online_check.isChecked())
                	{
                		app.isOpenOnlineSound = true;
                	}
                	else
                	{
                		app.isOpenOnlineSound = false;
                	}
                	if(message_check.isChecked())
                	{
                		app.isOpenNewMessageSound = true;
                	}
                	else
                	{
                		app.isOpenNewMessageSound = false;
                	}
                	if(nudge_check.isChecked())
                	{
                	    app.isNudge = true;
                	}
                	else
                	{
                	    app.isNudge = false;
                	}
                	if(show_check.isChecked())
                	{
                		app.isShowOnline = true;
                	}
                	else
                	{
                		app.isShowOnline = false;
                	}
//                	if(getIcon_check.isChecked())
//                	{
//                		app.isReceiveHeadChange = true;
//                	}
//                	else
//                	{
//                		app.isReceiveHeadChange = false;
//                	}
                	removeDialog(UiEventListener.UI_EVENT_SET_SYSTEM);
                	if((oldIsOpenOnlineSound!=app.isOpenOnlineSound) || (oldIsOpenNewMessageSound!=app.isOpenNewMessageSound)
                			|| (oldIsNudge != app.isNudge) || (oldIsOnlyShowOnline!=app.isShowOnline))
                		{
                			SaveSystemSetting();
                		}
                	////////
                	if(oldIsOnlyShowOnline!=app.isShowOnline)
                	{
                		//////////////////增加按状态排序功能
                		app.eventNotify(EventListener.EVENT_SET_IS_SHOW_ONLINE, null);
//                		app.setGroupOrder(app.isShowOnline?app.ROSTER_NORMAL_ONLY_ONLINE:app.ROSTER_NORMAL_HAS_OFFLINE);
//                		rosterGroupByNormal(app.isShowOnline);
//                		app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);                		
                		//mRoster.rlist.UpdateView();
                	}
                	//////////
                	
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_SET_SYSTEM);
                    /* User clicked Cancel so do some stuff */
                }
            })
            .create();     		
    	}
    	else if(id == UiEventListener.UI_EVENT_DETAIL_INFO)
    	{
    		div = new DetailInfoView(MainActivity.this,mCurItem);
    		int icon = ChangeIcon();
    		return new AlertDialog.Builder(MainActivity.this)
    			.setCancelable(false)
                .setIcon(icon)
                .setTitle(R.string.detail)
                .setView(div)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	removeDialog(UiEventListener.UI_EVENT_DETAIL_INFO);
                    	div = null;
                         /* User clicked OK so do some stuff */
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
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.validate_title)
            .setMessage(str)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
            		app.acceptOrDenyVerifyFriend("accept", contact.VERIFY_imid);
            		removeDialog(UiEventListener.UI_EVENT_ADD_CONFRIM);
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	app.acceptOrDenyVerifyFriend("deny", contact.VERIFY_imid);
                	removeDialog(UiEventListener.UI_EVENT_ADD_CONFRIM);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_ERROR_MESSAGE)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(this.errorText)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_ERROR_MESSAGE);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_UPDATE)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.text_upgrade_contact_list)
            .setMessage(R.string.text_upgrade_contact_list_info)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
     	
					// 注销 清除RMS数据库 然后登录
					synchronizeContactList();
					
					Intent intent= new Intent();
			        intent.setClass(context, LoginMonitorActivity.class);
			        intent.putExtra("type",MSNApplication.MONITOR_LOGOUT_FLAG);
			        startActivity(intent);
			        app.isLogout=true;
			        app.isStart = false;
    	        	app.jabber.sendLogoutMsn(false);  
    	        	finish();
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_UPDATE);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_HELP)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.help)
            .setMessage(R.string.help_contact_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_HELP);
                }
            })
           .create();
    	}
    	else if(id==UiEventListener.UI_EVENT_SESSION_HELP)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.help)
            .setMessage(R.string.help_session_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_SESSION_HELP);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_LOGOUT)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.logout)
            .setMessage(R.string.logout_confirm)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
                	app.removeContactGroupVersionFromRMS();
            		app.saveAllContactListToFile(app.GetLiveID());
            		app.setContactGroupVerToRMS(app.myContactGroupVersion);            		
            		
					Intent intent= new Intent();
			        intent.setClass(context, LoginMonitorActivity.class);
			        intent.putExtra("type",MSNApplication.MONITOR_LOGOUT_FLAG);
			        startActivity(intent);
			        app.isLogout=true;
    	        	app.jabber.sendLogoutMsn(false); 
    	        	////////////
    	        	//app.saveAllGroupList();
    	        	//app.saveAllContactList();
    	        	////////////
    	        	
    	        	
    	        	//删除所有对话的临时音频文件
    	        	MsnUtil.deleteAllTempAudio(MainActivity.this);
    	        	//删除包目录下的"temprecord"目录的所有文件
    	        	MsnUtil.deleteTempDir(MainActivity.this, "app_temprecord");
    	        	
    	        	finish();
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_LOGOUT);
                }
            })
           .create();
    	}
    	else if(id==UiEventListener.UI_EVENT_EXIT)
    	{
            return new AlertDialog.Builder(MainActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.exit)
            .setMessage(R.string.exit_confirm)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) 
                {
                	app.removeContactGroupVersionFromRMS();
            		app.saveAllContactListToFile(app.GetLiveID());
            		app.setContactGroupVerToRMS(app.myContactGroupVersion);

					Intent intent= new Intent();
			        intent.setClass(context, LoginMonitorActivity.class);
			        intent.putExtra("type",MSNApplication.MONITOR_EXIT_FLAG);
			        startActivity(intent);
					app.isExit=true;
					app.jabber.sendLogoutMsn(true);	
    	        	////////////
    	        	//app.saveAllGroupList();
    	        	//app.saveAllContactList();
    	        	////////////
					//删除所有对话的临时音频文件
    	        	MsnUtil.deleteAllTempAudio(MainActivity.this);
    	        	//删除包目录下的"temprecord"目录的所有文件
    	        	MsnUtil.deleteTempDir(MainActivity.this, "app_temprecord");
					finish();
                }
            })
            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_EXIT);
                }
            })
           .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_CHECK_HISTORY)
    	{
			list = new HistoryListView(MainActivity.this, app.DB.checkHistory(MD5.toMD5(app.GetLiveID())));

//			list.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
//	            public void onItemSelected(AdapterView arg0, View arg1, int arg2, long arg3) {
//	            	historyText = (String) list.getItemAtPosition(arg2);
//	            }
//
//	            public void onNothingSelected(AdapterView<?> arg0) {
//	            }
//	        });
//			list.setOnItemClickListener(new OnItemClickListener() 
//			{
//				
//				public void onItemClick(AdapterView<?> arg0, View arg1,
//						int arg2, long arg3) 
//				{
//					historyText = (String) list.getItemAtPosition(arg2);
//					//removeDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
//				}
//			});
			return new AlertDialog.Builder(MainActivity.this)
					.setCancelable(false)
					.setIcon(R.drawable.msn)
					.setTitle(R.string.text_check_history)
					.setView(list)
		            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
		                public void onClick(DialogInterface dialog, int whichButton) {
		                View v = (View)list.findFocus();
		                //int i = list.indexOfChild(v);
                    	if(list.historyText == null)
                    	{
                    		
                    	}
                    	else
                    	{
                    		showHistory(MD5.toMD5(app.GetLiveID()), list.historyText);
                    		removeDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
                    	}
		            }
		            })
		            .setNeutralButton(R.string.text_delete_history, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        /* User clicked Something so do some stuff */
                    	if(list.historyText == null)
                    	{
                    		
                    	}
                    	else
                    	{
                    		showDialog(UiEventListener.UI_EVENT_DELETE_HISTORY);
                    		removeDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
                    	}
                    }
		            })
		            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
		                public void onClick(DialogInterface dialog, int whichButton) {
		                	removeDialog(UiEventListener.UI_EVENT_CHECK_HISTORY);
		                }
		            })
		            .create();  
    	}
    	else if(id == UiEventListener.UI_EVENT_DELETE_HISTORY)
    	{
    		return new AlertDialog.Builder(MainActivity.this)
    		.setCancelable(false)
    		.setIcon(R.drawable.msn)
    		.setTitle(R.string.text_show_info_title)
    		.setMessage(R.string.is_delete_history)
    		.setPositiveButton(R.string.text_delete_history, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
					app.DB.deleteHistoryData(MD5.toMD5(app.GetLiveID()), list.historyText);
					removeDialog(UiEventListener.UI_EVENT_DELETE_HISTORY);
				}
			})
			.setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
					removeDialog(UiEventListener.UI_EVENT_DELETE_HISTORY);
				}
			})
    		.create();
    		
    	}
    	////////////////流量设定
    	else if(id == UiEventListener.UI_EVENT_FLOW_SETTING)
    	{
                LayoutInflater factory = LayoutInflater.from(this);
                final View view = factory.inflate(R.layout.flow_rate_setting, null);
                final CheckBox getIcon_check = (CheckBox)view.findViewById(R.id.roster_icon);
                if(app.isReceiveHeadChange)
                {
                	getIcon_check.setChecked(true);
                }
                final CheckBox getImpresa_check = (CheckBox)view.findViewById(R.id.contact_impresa_change);
                if(app.isReceiveVcardChange)
                {
                	getImpresa_check.setChecked(true);
                }
                
                return new AlertDialog.Builder(MainActivity.this)
                .setCancelable(false)
                .setIcon(R.drawable.msn)
                .setTitle(R.string.text_flow_setting)
                .setView(view)
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) 
                    {
                    	boolean oldIsReceiveHead=app.isReceiveHeadChange;
                    	boolean odlIsReceiveVcard=app.isReceiveVcardChange;
                    	if(getIcon_check.isChecked())
                    	{
                    		app.isReceiveHeadChange = true;
                    	}
                    	else
                    	{
                    		app.isReceiveHeadChange = false;
                    	}
                    	if(getImpresa_check.isChecked())
                    	{
                    		app.isReceiveVcardChange = true;
                    	}
                    	else
                    	{
                    		app.isReceiveVcardChange = false;
                    	}
                    	if(oldIsReceiveHead!=app.isReceiveHeadChange||odlIsReceiveVcard!=app.isReceiveVcardChange)
                    	{
                    		saveFlowSetting();
                         	Toast.makeText(context, (String)getText(R.string.text_flow_setting_notice), Toast.LENGTH_SHORT).show(); 
                    	}
                    	////////
//                    	if(oldIsOnlyShowOnline!=app.isShowOnline)
//                    	{
//                    		app.setGroupOrder(app.isShowOnline?app.ROSTER_NORMAL_ONLY_ONLINE:app.ROSTER_NORMAL_HAS_OFFLINE);
//                    		mRoster.rlist.UpdateView();
//                    	}
                    	//////////
                    	removeDialog(UiEventListener.UI_EVENT_FLOW_SETTING);
                    }
                })
                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                    	removeDialog(UiEventListener.UI_EVENT_FLOW_SETTING);
                        /* User clicked Cancel so do some stuff */
                    }
                })
                .create();     		
    	}
    	else if(id == UiEventListener.UI_EVENT_OUTER_VCARDHEAD)//带外 头像上传
    	{
    		recS=null;
    		if(app.currentOuterContact!=null)
    		{
    			recS = new RecordSendView(MainActivity.this,null,app.currentOuterContact);
    			return new AlertDialog.Builder(MainActivity.this)
    					.setCancelable(false)
		    			.setIcon(R.drawable.msn)
		    			.setTitle(R.string.text_uploading_outer_head)
    					.setView(recS)
    		            .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() 
    		            {
    		                public void onClick(DialogInterface dialog, int whichButton) 
    		                {
    		                	removeDialog(UiEventListener.UI_EVENT_OUTER_VCARDHEAD);
    		                	app.currentOuterContact.CONTACT_FILEOUTTRANSFER.setOuterCancelTransfer(MSNApplication.MESSAGE_HEAD_TYPE_SEND_CANCEL);
    		                	app.currentOuterContact=null;
    		                	app.isTransferingOuterSendHead=false;
    		                	recS=null;
    		                }
    		            })
    		            .create();  
    		}
    	}
    	else if(id == UiEventListener.UI_EVENT_ZWAP_LOAD)
    	{
            ProgressDialog dialog = new ProgressDialog(this);
            dialog.setMessage(getText(R.string.zwap_loading));
            dialog.setIndeterminate(true);
            dialog.setCancelable(true);
            return dialog;
    	}
        return null;
    }
    
    
	/**根据用户分组来分组 
	 * @param onlyShowOnlineContact 是否仅显示在线联系人
	 * */
	public  void rosterGroupByNormal(boolean onlyShowOnlineContact)
	{		
		app.eventNotify(EventListener.EVENT_ROSTER_GROUP_BY_NORMAL, Boolean.toString(onlyShowOnlineContact));
		Message m = new Message();
		m.what = EventListener.EVENT_UPDATE_CONTACT_UI;
		MainHandler.sendMessage(m);
//		try
//		{
//			if(!onlyShowOnlineContact)//还显示脱机联系人
//			{
//
//			}
//			else//只显示在线的联系人 无脱机人
//			{
//				app.groupDataNormalOnlyOnline.clear();
//				//app.childDataNormalOnlyOnline.clear();
//				Group currentGroup;
//				//boolean isAddDefault=false;//是否需要添加新的缺省组
//				Vector addVc=new Vector();//vc用来装需要添加的所有联系人
//				for (int i = 0; i < app.groupData.size(); i++)
//				{	
////					Map<String, Group> CurGroupMap = app.groupData.get(i);
////		    		currentGroup=(Group)CurGroupMap.get("GROUP");	   
//					currentGroup=app.groupData.elementAt(i);
//		    		app.addGroupToList(app.newGroup(currentGroup.GROUP_id,currentGroup.GROUP_gbcode,currentGroup.GROUP_name,currentGroup.GROUP_flag,false),app.groupDataNormalOnlyOnline);
//					//List<Map<String, Contact>> childData  = app.childData.get(i);
//		    		Group group=app.groupData.elementAt(i);
//		    		Contact currentContact;
//		    		Contact addContact;
//					for(int k=0;k<group.contactVector.size();k++)
//					{
//						//Map<String, Contact> CurChild = new HashMap<String, Contact>(); 
//						//CurChild= childData.get(k);
//						//Contact currentContact=CurChild.get("CONTACT");
//						currentContact = null;
//						currentContact=group.contactVector.elementAt(k);
//						if (currentContact.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE)
//							addVc.addElement(currentContact);	
//					}
//					if(addVc!=null&&addVc.size()!=0)
//					{
//						 for(int m=0;m<addVc.size();m++)
//						 {
//							 addContact = null;
//							 addContact=(Contact)addVc.elementAt(m);
//						    
//						    app.addSubItem(addContact,false,app.groupDataNormalOnlyOnline);
//						 }
//					}
//					addVc.removeAllElements();
//				}
//			}
//		}
//		catch(Exception e)
//		{
//			e.printStackTrace();
//
//		}
	}
    
    
	/**同步联系人
	 * 注销 清除RMS记录 然后再重新登陆
	 * */
	private void synchronizeContactList()
	{
		app.deleteSynchronizedAllRecordStore();
		//界面跳转到注销等待界面后再登陆
	}
    
    private int ChangeIcon()
    {
    	Contact contact= mCurItem;
		
//        if((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG) 
//        		&& (contact.SUBITEM_STATE == app.STATE_OFFLINE))
//        {
//        	return R.drawable.offline_block;
//        }
//        else if ((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
//        		&& (contact.SUBITEM_STATE != app.STATE_OFFLINE))
//        {
//        	return R.drawable.online_block;
//        }
//        else
//        {
        	if (contact.SUBITEM_STATE == app.STATE_ONLINE)
        	{
        		return R.drawable.online;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_BUSY)
        	{
        		return R.drawable.busy;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_AWAY)
        	{
        		return R.drawable.away;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_BeRightBack)
        	{
        		return R.drawable.away;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_InACall)
        	{
        		return R.drawable.busy;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_OutToLunch)
        	{
        		return R.drawable.away;
        	}
        	else if (contact.SUBITEM_STATE == app.STATE_OFFLINE)
        	{
        		return R.drawable.offline;
        	}
//        }
        return R.drawable.offline_block;
    }
	
    private void setSingleChoiceIdx(int idx)
    {
    	mSingleChoiceIdx = idx;
    }
    
    private void SaveSystemSetting()
    {	
    	app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
    	app.DB.setSystemUserData(app.GetLiveID(), Boolean.toString(app.isOpenOnlineSound), Boolean.toString(app.isOpenNewMessageSound), Boolean.toString(app.isShowOnline), Boolean.toString(app.isNudge));//, Boolean.toString(app.isReceiveHeadChange));
    }
    
    /**保存流量设置*/
    private void saveFlowSetting()
    {
    	app.DB=new DatabaseHelper(this,app.DB_NAME,null,app.DB_VERSION);
    	app.DB.setFlowUserData(app.GetLiveID(), Boolean.toString(app.isReceiveHeadChange), Boolean.toString(app.isReceiveVcardChange));
    }
	
	private void operaNormalItem(Contact item, int idx)
	{
		switch(idx)
		{
		case 0:
    		String tmp = MsnUtil.getUsernameIsValid(mCurItem.CONTACT_imid, false);
        	if(tmp != null)
        	{
        		errorText = tmp;
        		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
        		return;
        	}
			openChatroomUIFromContactList(item);
			break;
		case 1:
			//DetailInfo();
			showDialog(UiEventListener.UI_EVENT_DETAIL_INFO);
			break;
		case 2:
			showDialog(UiEventListener.UI_EVENT_DELETE_ROSTER);
			break;
		case 3:
			showDialog(UiEventListener.UI_EVENT_BLOCK);
			break;
		default:
			break;
		}
	}
	
	private void operaBlockItem(Contact item, int idx)
	{
		switch(idx)
		{
		case 0:
			//DetailInfo();
			showDialog(UiEventListener.UI_EVENT_DETAIL_INFO);
			break;
		case 1:
			showDialog(UiEventListener.UI_EVENT_DELETE_ROSTER);
			break;
		case 2:
			showDialog(UiEventListener.UI_EVENT_UNBLOCK);
			break;
		default:
			break;
		}		
	}
	
	/**从联系人页面进入到正在对话页面<br>
	 * 将该会话赋值给 app.currentSession
	 * */
	private void openChatroomUIFromContactList(Contact contact)
	{
		Item session=app.openChatUI(contact,null);
        app.currentSession=session;
        session.SESSION_numOfNewMsg="0";
        app.currentSession.SESSION_numOfNewMsg="0";
        startChatRoomActivity();
	}
	
	/**从对话列表页面进入到正在对话页面<br>
	 * 将该会话赋值给 app.currentSession
	 * */
	public void openChatroomUIFromSessionList(Item session)
	{
        app.currentSession=session;
        session.SESSION_numOfNewMsg="0";
        app.currentSession.SESSION_numOfNewMsg="0";
        startChatRoomActivity();
	}
	
	/**进入正在对话页面
	 * */
    public void startChatRoomActivity()
    {
		Intent intent;
    	intent = new Intent();
        intent.setClass(this, ChatRoomActivity.class);
        startActivity(intent);
        mSession.slist.UpdateView();
    }
    

    
    
	public boolean PointInRect(Rect rect, int x, int y)
	{
		if(x>=rect.left	&&	x<=rect.right && y>=rect.top &&	y<=rect.bottom )
		{
			return true;
		}
		return false;
	}
	
	
    TextWatcher watcher = new TextWatcher() {
        public void beforeTextChanged(CharSequence s, int start,
                                      int before, int after) { }

        public void onTextChanged(CharSequence s, int start,
                                      int before, int count) {
        	MatchAccount(s.toString());
        }

        public void afterTextChanged(android.text.Editable s) { }
    };
    
    private void SetAdapt(RadioGroup radio)
    {
    	mRadio = radio;
    }
    
    private void MatchAccount(String str)
    {  				
		if(str.indexOf("@") != -1)
		{   	    		 	    			    		
    		mRadio.check(R.id.other);
    		for(int i = 0; i < mRadio.getChildCount(); i++)
    		{
    			mRadio.getChildAt(i).setEnabled(false);
    		}
    		return;
    	}
		else
		{
    		for(int i = 0; i < mRadio.getChildCount(); i++)
    		{
    			mRadio.getChildAt(i).setEnabled(true);
    		}
    		return;
		}
    }
    
    private void showHistory(String HistoryTab, String HistoryTitle)
    {
    	Cursor cur = app.DB.loadHistoryData(HistoryTab, HistoryTitle);
    	if(cur == null)
    	{
    		return;
    	}
    	if(cur.getCount() == 0)
    	{
    		return;
    	}
    	
		Intent intent;
    	intent = new Intent();
    	intent.putExtra("history", HistoryTitle);
        intent.setClass(this, ShowHistoryActivity.class);
        startActivity(intent);
    }
   private void setNotify(int id, Object obj) {
        app.screenManager.KillActivity();
        
        Item msg = (Item)obj;
        String str;
        if(msg.MESSAGE_nudge)
        {
            str =msg.MESSAGE_nickname + " " +getText(R.string.text_shake);
        }
        else
        {
            if(msg.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
            {
                str =msg.MESSAGE_nickname + " " +getText(R.string.text_send_file);
            }
            else if(msg.MESSAGE_type == MSNApplication.MESSAGE_TYPE_VOICECLIP)
            {
                str =msg.MESSAGE_nickname + " " +getText(R.string.text_record);              
            }
            else
            {
                str =msg.MESSAGE_nickname + " " +getText(R.string.text_chat_says) + ":" + msg.MESSAGE_body;
            }
        }
        Item session = app.getSessionBySid(msg.MESSAGE_sid);
        app.currentSession = session;
        Notification notification = new Notification(id, str,
                System.currentTimeMillis());
    
        notification.setLatestEventInfo(this, getString(R.string.app_name),str, makeNotifiyIntent(id));
    
        app.mNotificationManager.notify(R.layout.main, notification);
    }
    
    private PendingIntent makeNotifiyIntent(int id) {
    
        Intent intent;
        intent = new Intent();
        intent.setClass(this, ChatRoomActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        intent.putExtra("id", id);
        
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                        intent,
                        PendingIntent.FLAG_UPDATE_CURRENT);
        return contentIntent;
    }
}