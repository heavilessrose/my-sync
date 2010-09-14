package com.pica.msn;


import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Locale;
import java.util.Timer;
import java.util.Vector;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Movie;
import android.graphics.Paint;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.preference.Preference;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.pica.ui.tools.Animation;
import com.pica.ui.tools.AnimationListener;


public class LoginMonitorActivity extends Activity implements OnClickListener, AnimationListener
{
	
	private MSNApplication app; 
	RelativeLayout layout;
    private final int WC = ViewGroup.LayoutParams.WRAP_CONTENT;
    RelativeLayout.LayoutParams param;
    static int windowHeight,windowWidth;
    Button button_cancel;
    Bitmap login_ad=null;
    private boolean isError = false;
    //////////////////////解决进程被杀死的bug
    boolean isKilled = false;
    
    protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		/**解决进程被杀死的bug*/
		outState.putBoolean("isKilled", true);
	}
    /////////////////////////////////
    
    public void onCreate(Bundle savedInstanceState)
    {
		super.onCreate(savedInstanceState);
		
		//////////////////解决进程被杀死的bug
		if(savedInstanceState != null && savedInstanceState.getBoolean("isKilled") && app == null)
        {

        	isKilled = true;
        	Intent toLogin = new Intent(this, LoginActivity.class);
        	startActivity(toLogin);
        	finish();
        	return;
        }
		////////////////////////////////
		isError = false;
		context=this;
		app = MSNApplication.getApplication(this);
		app.screenManager.pushActivity(this);
		app.SetLMHandler(sampleHandler);
		Bundle extras=getIntent().getExtras();
		currentType=extras.getByte("type");
		
	    requestWindowFeature(Window.FEATURE_NO_TITLE);
	    //final Window win = getWindow(); 
	    //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);  
	    windowHeight = getWindowManager().getDefaultDisplay().getHeight();
	    windowWidth = getWindowManager().getDefaultDisplay().getWidth();  
	    app.setWindowHeight(windowHeight);
	    app.setWindowWidth(windowWidth);
        layout = new RelativeLayout(this);
	    setContentView(layout); 
	    param = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT);
	    param.leftMargin =0;
	    viewGif = new SampleView(this,currentType);
		layout.addView(viewGif,param);
	    
		
		reset();
		DrawAd();
		//app.getVersion(app.GetIMSI(), app);
    }
   
    
    private void DrawAd()
    {
        FileInputStream fileinput = null;
        try {
        	fileinput = new FileInputStream(app.RMS_AD_DATA + "login_ad.png");
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}
		
		File file = new File(app.RMS_AD_DATA + "login_ad.png");
		if(file == null)
		{
			return;
		}
		
		DataInputStream dis = new DataInputStream(fileinput);
		if(dis == null)
		{
			return;
		}
		String temp = Long.toString(file.length());
		int size=Integer.parseInt(temp);
		byte[] imageData = null;
		imageData = new byte[(int)size];
		try
		{
			dis.read(imageData);
		} catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}

        login_ad=BitmapFactory.decodeByteArray(imageData, 0, imageData.length);
    }
    /**登录超时时间 120秒*/
    private static final int TIMEOUT_LOGIN = 120000; 
    /**注销超时时间 40秒*/
    private static final int TIMEOUT_LOGOUT = 40000; 
    /**退出超时时间 20秒*/
    private static final int TIMEOUT_EXIT = 20000; 
    
    public void reset()
    {
		if(currentType==MSNApplication.MONITOR_LOGIN_FLAG)
		{
			button_cancel = new Button(this);
			button_cancel.setText(R.string.exit_logout);
			button_cancel.setOnClickListener(this);
		    param = new RelativeLayout.LayoutParams(WC, WC);
		    param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		    param.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		    layout.addView(button_cancel, param);
		    Message msg = new Message(); 
		    msg.what = EventListener.EVENT_STOP_MONITOR; 
		    sampleHandler.sendMessageDelayed(msg, TIMEOUT_LOGIN); 
		}
		else if(currentType==MSNApplication.MONITOR_LOGOUT_FLAG)
		{
		    Message msg = new Message(); 
		    msg.what = EventListener.EVENT_STOP_MONITOR; 
		    sampleHandler.sendMessageDelayed(msg, TIMEOUT_LOGOUT); 
		}
		else if(currentType==MSNApplication.MONITOR_EXIT_FLAG)
		{
		    Message msg = new Message(); 
		    msg.what = EventListener.EVENT_STOP_MONITOR; 
		    app.isStart = false;
		    sampleHandler.sendMessageDelayed(msg, TIMEOUT_EXIT); 
		}
		((SampleView) viewGif).isDone=false;
		

    }
    
    /***
     * 提示信息N秒后 回到登录页面或者退出程序
     * @param time 时间 毫秒
     * @param isExit 是否退出程序
     */
    private void sleepTimeAndLogoutorExit(final long time,final boolean isExit)
    {
		 new Thread()
		 {
			 public void run() 
	            { 

	              try 
	              {                   
	                 Thread.sleep(time); 
	             	///退出包
	                 if(app.jabber.isSessionOpen())
	                	 app.jabber.sendSocketLogout();
	             	///
	                 if(isExit)
	                 {
	    	            finish();
	    	            Process.killProcess(Process.myPid());
	                 }
	                 else
	                 {
	                     if(isError == false)
	                     {
    	                     isError = true;
	                	 changeToLoginPage();
	                     }
	                 }

	              } catch (InterruptedException e) 
	              { 
	                  e.printStackTrace(); 
	              } 
	            }
		 }.start();
    }
    
    
    public void onDestroy()
    {
        app.screenManager.popActivity(this);
		super.onDestroy();
		app.DB.close();

		app.SetLMHandler(null);
		try {
			stopProgress();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(sampleHandler.hasMessages(EventListener.EVENT_STOP_MONITOR))
		sampleHandler.removeMessages(EventListener.EVENT_STOP_MONITOR);
		sampleHandler=null;
    }
    
    static final int RG_REQUEST = 0;
    
    
    public Handler  sampleHandler = new Handler() 
    { 
         
        public void handleMessage(Message msg) 
        { 
             switch (msg.what) 
             {
               case EventListener.EVENT_RECEIVE_LOGIN_ERROR_LOGINMONITOR:              	 
            	 if(msg.obj!=null)
            	 {
            		 super.handleMessage(msg); 
            		 Object[] temp=(Object[])msg.obj;
            		 String errorText=(String)temp[0];   
            		 String errorCode=(String)temp[1];
            		 
            		 if(errorCode==null)
            		 {
 						app.jabber.forceStop=true;//强制退出发包线程循环
	            		 //((SampleView) viewGif).setShowText((String)getText(R.string.error_prompt)+ (String)getText(R.string.error_content)+errorText);
 						((SampleView) viewGif).setShowText(errorText);
 						sleepTimeAndLogoutorExit(3000,false);//返回的登录页面
            		 }
            		 else
            		 {
     					//#ifdef support_save_rms	

     						app.jabber.forceStop=true;//强制退出发包线程循环
     	            		 //((SampleView) viewGif).setShowText((String)getText(R.string.error_code)+errorCode+(String)getText(R.string.error_content)+errorText);
     						((SampleView) viewGif).setShowText(errorText);
     						sleepTimeAndLogoutorExit(3000,false);//返回的登录页面
            		 }

            	 }
            	 break;
            	 
               case EventListener.EVENT_RECEIVE_OTHER_ERROR:              	 
              	 if(msg.obj!=null)
              	 {     	 
              		super.handleMessage(msg); 
              		 String errorText=(String)msg.obj;
              		 //((SampleView) viewGif).setShowText((String)getText(R.string.error_content)+errorText);
              		((SampleView) viewGif).setShowText(errorText);
              		 sleepTimeAndLogoutorExit(3000,false);
              	 }
              	 break;	             	 
             case EventListener.EVENT_NETWORK_FAILURE:
            	 super.handleMessage(msg); 
            	 ((SampleView) viewGif).setShowText((String)getText(R.string.error_link));
            	 sleepTimeAndLogoutorExit(3000,false);
            	 break;
             case EventListener.EVENT_DISPATCH_NOTICE: 
             {
            	 super.handleMessage(msg); 
            	 if(sampleHandler.hasMessages(EventListener.EVENT_STOP_MONITOR))
            	 sampleHandler.removeMessages(EventListener.EVENT_STOP_MONITOR);
            	 String notice = (String)app.jabber.noticeVector.elementAt(0);
            	 new AlertDialog.Builder(context)
            	 .setCancelable(false)
            	 .setTitle(R.string.text_show_info_title)
            	 .setMessage(notice)
            	 .setPositiveButton((String)getText(R.string.alert_dialog_ok),
    					new DialogInterface.OnClickListener() 
            	 {

					public void onClick(DialogInterface dialog,
							int whichButton) 
					{
						setResult(RESULT_OK);
						app.jabber.noticeVector.remove(0);
						if(app.jabber.noticeVector.size()!=0)
						{
			            	 Message m = new Message(); 
						     m.what = EventListener.EVENT_DISPATCH_NOTICE; 
						     sampleHandler.sendMessage(m);
						}
						else
						{
							synchronized (app.jabber)
							{
								app.jabber.notify();
							}
						    Message msg = new Message(); 
						    msg.what = EventListener.EVENT_STOP_MONITOR; 
						    sampleHandler.sendMessageDelayed(msg, TIMEOUT_LOGIN); 
						}
					}

				}).show();        	 
             }
            	 break;
             case EventListener.EVENT_DISPATCH_URL:
            	 super.handleMessage(msg); 
            	 if(sampleHandler.hasMessages(EventListener.EVENT_STOP_MONITOR))
            	 sampleHandler.removeMessages(EventListener.EVENT_STOP_MONITOR);
	              // String content=(String)getText(R.string.text_upgrade_edition)+app.jabber.updateVersion+(String)getText(R.string.text_upgrade_address)+app.jabber.updateUrl;
            	 String content=(String)getText(R.string.text_upgrade_edition)+app.jabber.updateVersion+"\n"+(String)getText(R.string.text_is_upgrade_now);
            	 new AlertDialog.Builder(context)
            	 	.setCancelable(false)
	     		    .setTitle(R.string.text_upgrade_title)
	     		    .setMessage(content).
	     		    setPositiveButton((String)getText(R.string.alert_dialog_ok),
	     					new DialogInterface.OnClickListener() 
	     		    {
	
	 					public void onClick(DialogInterface dialog,
	 							int whichButton) 
	 					{
	 						setResult(RESULT_OK);
	 						try
	 						{
	 				            Uri uri = Uri.parse(app.jabber.updateUrl);
	 				            if(uri == null)
	 				            {
	 				                return;
	 				            }
	 				             Intent returnIt = new Intent(Intent.ACTION_VIEW, uri);
	 				             startActivity(returnIt);
	 				            Process.killProcess(Process.myPid());
	 		    	             //Intent intent = new Intent(context, NewWebPage.class);
	 		    	             //startActivityForResult(intent,RG_REQUEST);
	 						}
	 						catch(Exception e)
	 						{
	 							e.printStackTrace();
	 							sleepTimeAndLogoutorExit(3000,true);
	 						}
	 					}
	
	 				}).
	 				setNegativeButton((String)getText(R.string.alert_dialog_cancel), 
	 						new DialogInterface.OnClickListener() 
	 				{
	
	 				public void onClick(DialogInterface dialog, int whichButton) 
	 				{
							synchronized (app.jabber)
								{
									app.jabber.notify();
								}
						    Message msg = new Message(); 
						    msg.what = EventListener.EVENT_STOP_MONITOR; 
						    sampleHandler.sendMessageDelayed(msg, TIMEOUT_LOGIN); 
	 				}
	
	 			     }).show();
	               	
	               	 break;
	               	 
	               	 
             case EventListener.EVENT_DISPATCH_REFUSE_LOGIN:
                 super.handleMessage(msg); 
                 sleepTimeAndLogoutorExit(1000,false);
                 break;
                 
             case EventListener.EVENT_DISPATCH_MUST_UPGRADE:
            	 super.handleMessage(msg); 
            	 sleepTimeAndLogoutorExit(0,true);
            	 break;
         		
             case EventListener.EVENT_LOGIN_SUCCESS: 
            	 super.handleMessage(msg); 
            	 //((SampleView) viewGif).setShowText((String)getText(R.string.login_success));
            	 Login_success();
                  break; 
             case EventListener.EVENT_RECEIVE_DISPATCH_OK:
            	 super.handleMessage(msg); 
            	 //((SampleView) viewGif).setShowText((String)getText(R.string.link_server));
            	 break;
             case EventListener.EVENT_RECEIVE_LOGIN_PICA_OK:
            	 super.handleMessage(msg); 
            	 //((SampleView) viewGif).setShowText((String)getText(R.string.load_roster_info));
            	 break;	 
            	 
	         case EventListener.EVENT_FINISH_ACTIVITY:      
	        	 super.handleMessage(msg); 

	        	finish();
	        	app.SetLMHandler(null);
	        	//Process.killProcess(Process.myPid());
	        	break;
	        	
	         case EventListener.EVENT_RECEIVE_SYNCHRONIZE_ERROR:
	        	 super.handleMessage(msg); 
				//需要同步联系人列表 因此要删掉所有的保存记录
				app.deleteSynchronizedAllRecordStore();
				((SampleView) viewGif).setShowText((String)getText(R.string.error_prompt)+(String)getText(R.string.synchronize_fail));
				sleepTimeAndLogoutorExit(3000,false);//返回的登录页面					         
	        	break;
	         case EventListener.EVENT_LOGIN_MONITOR_SETTEXT:
	        	 super.handleMessage(msg); 
	        	 if(msg.obj!=null)
	        	 {
		        	 String text=(String)msg.obj;
		        	 ((SampleView) viewGif).setShowText(text);
	        	 }
	     		currentType=MSNApplication.MONITOR_LOGIN_FLAG;
	     		if(sampleHandler.hasMessages(EventListener.EVENT_STOP_MONITOR))
	     		sampleHandler.removeMessages(EventListener.EVENT_STOP_MONITOR);
	     		reset();
            	 break;	
            	 
             case EventListener.EVENT_STOP_MONITOR: 
            	 super.handleMessage(msg);                 

            	 if(currentType==MSNApplication.MONITOR_LOGOUT_FLAG)
            	 {
            		 Toast.makeText(context, (String)getText(R.string.logout_timeout), Toast.LENGTH_SHORT).show();  
            	 }
            	 else
            	 {
            		 Toast.makeText(context, (String)getText(R.string.login_timeout), Toast.LENGTH_SHORT).show();  
            	 }
                
        		 sleepTimeAndLogoutorExit(0,false);//返回的登录页面
                 break; 
                 
         	case EventListener.EVENT_UPDATE_MONITOR_UI:
        	{
        		 super.handleMessage(msg);  
        		((SampleView) viewGif).invalidate();
        		break;
        	}    
                 
//=======
//             case 1000:            
//            	 super.handleMessage(msg); 
//            	 ((SampleView) viewGif).resetIcon();
//            	 break;
//>>>>>>> 1.29
             default:
            	 break;
             }
        }
        
    };
    
    private void logoutSuccess()
    {
    	app.jabber.sendEnd();
    	if(sampleHandler.hasMessages(EventListener.EVENT_STOP_MONITOR))
    	sampleHandler.removeMessages(EventListener.EVENT_STOP_MONITOR);
        Intent intent;
        intent = new Intent();
        intent.setClass(context, LoginActivity.class);
        intent.putExtra("type",MSNApplication.LOGOUT_SUCCESS_FLAG);
        startActivity(intent);
    }
    
    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
    	if (requestCode == RG_REQUEST) 
    	{
	    	if (resultCode == RESULT_CANCELED)
	    	{
				synchronized (app.jabber)
				{
					app.jabber.notify();
				}
			    Message msg = new Message(); 
			    msg.what = EventListener.EVENT_STOP_MONITOR; 
			    sampleHandler.sendMessageDelayed(msg, TIMEOUT_LOGIN); 
	    	}

    	}
    }
    
    
    View viewGif=null;
    static Context context=null;
    /**页面标志索引 正在登录、正在注销、正在退出*/
    private byte currentType;
    


    
    
    public void onResume() 
    {
       super.onResume();
       /////////////解决进程被杀死的bug
		if(isKilled && app == null)
		{
			finish();
			return;
		}
      ///////////////////////////////
  	   app.mCurActivityIdx = app.LoginM_Activity;
 	   app.SetCurActivity(this);
    }
    
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	if(keyCode == event.KEYCODE_BACK)
    	{
    		return true;
    	}
        return super.onKeyDown(keyCode, event);
    }
    
    /**登录成功 跳到联系人页面*/
    private void Login_success()
    {
        try
        {
		Intent intent;
        intent = new Intent();
        intent.setClass(this, MainActivity.class);
        startActivity(intent);
        app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
        app.DB.initUserLastLogin();
        app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
        if(app.isSavePassword)
        {
        app.DB.insertUserData(app.GetLiveID(), app.GetPassWord(), app.GetPicaID(), app.GetEditName(), app.GetDomain(), 
        		Boolean.toString(app.isSavePassword), Boolean.toString(app.isAutoLogin), Boolean.toString(app.isOpenOnlineSound), Boolean.toString(app.isOpenNewMessageSound), "true", app.myVCARD_myNickname, app.myVCARD_myImpresa, app.myVCARD_resourceId,
        		app.myContactGroupVersion, app.devicedId, app.iLoginStatus, Boolean.toString(app.isShowOnline), Boolean.toString(app.isReceiveHeadChange),Boolean.toString(app.isReceiveVcardChange), Boolean.toString(app.isNudge));
        }
        else
        {
            app.DB.insertUserData(app.GetLiveID(), "", app.GetPicaID(), app.GetEditName(), app.GetDomain(), 
            		Boolean.toString(app.isSavePassword), Boolean.toString(app.isAutoLogin), Boolean.toString(app.isOpenOnlineSound), Boolean.toString(app.isOpenNewMessageSound), "true", app.myVCARD_myNickname, app.myVCARD_myImpresa, app.myVCARD_resourceId,
            		app.myContactGroupVersion, app.devicedId, app.iLoginStatus, Boolean.toString(app.isShowOnline), Boolean.toString(app.isReceiveHeadChange),Boolean.toString(app.isReceiveVcardChange), Boolean.toString(app.isNudge));
        }       
        //////////取广告
        if (app.jabber.adServer != null)
        	app.jabber.getAD(false, app.windowWidth, app.windowHeight);
        ///
        }
        catch (Exception e) {
          e.printStackTrace(); 
          MsnUtil.DebugOutPutFile("LoginMo 557" + e.getMessage());
        }
        finish();
    }
    
    /**退出登录过程 跳回到登录页面*/
    private void changeToLoginPage()
    {
		Intent intent;
        intent = new Intent(this, LoginActivity.class);
        startActivity(intent);
        finish();
    }
    
	
	public void onClick(View v) 
	{
	    try {
		if(v==button_cancel)
		{
		    if(app.GetMSNHandler().hasMessages(EventListener.EVENT_SEND_HAERT))
	        app.GetMSNHandler().removeMessages(EventListener.EVENT_SEND_HAERT);
			app.jabber.cancelLogin=true;
			app.jabber.sendCancelLogout();
            if(isError == false)
            {
                isError = true;
			changeToLoginPage();
		}
		}
	    }
	       catch(Exception e)
	        {
	            e.printStackTrace();
	            MsnUtil.DebugOutPutFile("LOGINM 589" + e.getMessage());
	        }

	}
	
	public void onConfigurationChanged(Configuration newConfig) 
    {
    	super.onConfigurationChanged(newConfig);
    	windowHeight = getWindowManager().getDefaultDisplay().getHeight();
 	    windowWidth = getWindowManager().getDefaultDisplay().getWidth();
	    app.setWindowHeight(windowHeight);
	    app.setWindowWidth(windowWidth);
	    ((SampleView)viewGif).initView();
    }
	
	private class SampleView extends View
	{   
		
//<<<<<<< LoginMonitorActivity.java
////        private Movie mMovie;
////        private long mMovieStart;
//=======
        private Movie mMovie;
//    	private Bitmap bitIcon;
    	private int idxIcon = 0;
        private long mMovieStart;

        private int startX,startY;
        private int startImageX,startImageY;
        Paint paint;
        public String showText=null;//"查找服务器...";
//        private Context context;
        private byte type;
        /**是否进行过处理*/
        public boolean isDone=false;
    	public Bitmap[] bitmapSignIn;
    	
        public SampleView(Context context,byte currentType) 
        {
            super(context);
//            this.context=context;
            this.type=currentType;
            setFocusable(true);            
            paint = new Paint();
            int size = (int)(20 * app.screenScale);
            paint.setTextSize(size);
            paint.setAntiAlias(true);
//<<<<<<< LoginMonitorActivity.java
//            java.io.InputStream is;
//            is = context.getResources().openRawResource(R.drawable.animation);
            //mMovie = Movie.decodeStream(is);     
            
        	Resources r = getResources();

        	bitmapSignIn=new Bitmap[]{BitmapFactory.decodeResource(r,R.drawable.signin_1),
        			BitmapFactory.decodeResource(r,R.drawable.signin_2),
        			BitmapFactory.decodeResource(r,R.drawable.signin_3),
        			BitmapFactory.decodeResource(r,R.drawable.signin_4),
        			BitmapFactory.decodeResource(r,R.drawable.signin_5),
        			BitmapFactory.decodeResource(r,R.drawable.signin_6),
        			BitmapFactory.decodeResource(r,R.drawable.signin_7),
        			BitmapFactory.decodeResource(r,R.drawable.signin_8)};
//        	
//=======
//            java.io.InputStream is;
//            is = context.getResources().openRawResource(R.drawable.animation);
            //mMovie = Movie.decodeStream(is); 
//            bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_1);
//>>>>>>> 1.29
            initView();
            initProgress();
        }
        
        public void initView()
        {
////<<<<<<< LoginMonitorActivity.java
            startImageX=(LoginMonitorActivity.windowWidth-bitmapSignIn[0].getWidth())/2;
//=======
            //startImageX=(LoginMonitorActivity.windowWidth-mMovie.width())/2;
//            startImageX=(LoginMonitorActivity.windowWidth-bitIcon.getWidth())/2;
//>>>>>>> 1.29
	        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
	            if(app.screenScale == 0.75)
	            {
	                startImageY = 30;
	            }
	            else if(app.screenScale == 1)
	            {
	                startImageY=40;
	            }
	            else
	            {
	                startImageY=60;
	            }
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
	            if(app.screenScale == 0.75)
	            {
	                startImageY = 60;
	            }
	            else if(app.screenScale == 1)
	            {
	                startImageY=80;
	            }
	            else
	            {
	                startImageY=120;
	            }
	        }
            if(currentType==MSNApplication.MONITOR_LOGIN_FLAG)
            {
            	showText=(String)getText(R.string.text_signing_in);
            }
            else if(currentType==MSNApplication.MONITOR_LOGOUT_FLAG)
            {
            	showText=(String)getText(R.string.log_on);
            }
            else if(currentType==MSNApplication.MONITOR_EXIT_FLAG)
            {
            	showText=(String)getText(R.string.log_exit);
            }
            setShowText(showText);
        }

//        public void resetIcon()
//        {
//    	     Message message = new Message(); 
//     	     message.what = 1000;
//     	     if(sampleHandler == null)
//     	     {
//     	    	 return;
//     	     }
//    	     sampleHandler.sendMessageDelayed(message, 300); 
//    	     
//    	     if(idxIcon > 8)
//    	     {
//    	    	 idxIcon = 1;
//    	     }
//    	     
//    	     if(idxIcon == 1)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_1);
//    	     }
//    	     else if(idxIcon == 2)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_2);
//    	     }
//    	     else if(idxIcon == 3)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_3);
//    	     }
//    	     else if(idxIcon == 4)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_4);
//    	     }
//    	     else if(idxIcon == 5)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_5);
//    	     }
//    	     else if(idxIcon == 6)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_6);
//    	     }
//    	     else if(idxIcon == 7)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_7);
//    	     }
//    	     else if(idxIcon == 8)
//    	     {
//    	    	 bitIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.l_8);
//    	     }
//    	     idxIcon++;
//        }
        
        boolean isMoreOneLine=false;
        Vector msg;
        int distance;
        public void setShowText(String showText)
        {
        	if(showText==null)
        		showText="";
            Rect rect=new Rect();
            paint.getTextBounds(showText, 0, showText.length(), rect);
            int textWidth=(int)paint.measureText(showText, 0, showText.length());
            this.showText=showText;
            if(textWidth<LoginMonitorActivity.windowWidth)
            {
            	isMoreOneLine=false;
                startX=(LoginMonitorActivity.windowWidth-textWidth)/2;
//<<<<<<< LoginMonitorActivity.java
                startY=startImageY+bitmapSignIn[0].getHeight()+40;     
//=======
                //startY=startImageY;//+mMovie.height()+20;
//                startY=startImageY+bitIcon.getHeight()+20; 
//>>>>>>> 1.29
            }
            else//超过一行的情况 需要分行绘制
            {
            	isMoreOneLine=true;
            	char[] text=showText.toCharArray();
    	    	startX=5;	    	
//<<<<<<< LoginMonitorActivity.java
    	    	int yTop=startImageY+bitmapSignIn[0].getHeight()+40;
//=======
//    	    	int yTop=startImageY+bitIcon.getHeight()+20;/*startImageY+mMovie.height()+20;*/
//>>>>>>> 1.29
    	    	distance=rect.height()+ 5;
    	    	msg=new Vector(); //存储这段文字每行内容的向量
    	    	int firstX = startX; //第一行的起始x坐标
    	    	msg.addElement(new StringBuffer(""));
    	    	
    	    	int charW;
    	    	StringBuffer strbuff;
    	    	int length;
    	    	int tempWidth;
    	    	boolean find;
    	    	for(int j=0;j<showText.length();j++)
    	    	{
    		          if (text[j] == '\n') 
    		          { //遇到换行符则换行
    		        	 msg.addElement(new StringBuffer(""));
    		          } 
    		          else 
    		          {
    		        	  if(msg.size()!=1)//如果不止一行，则以后每行的起始坐标为行首
    			        	startX = 5;

    		        	  charW=(int)paint.measureText(showText, j, j+1);
    		        	  
    			          strbuff = ((StringBuffer)(msg.lastElement()));
    			          length = strbuff.length();//最后一行现有字符串的长度
    			          
    			          tempWidth=(int)paint.measureText(strbuff.toString(), 0, strbuff.toString().length());
    			          if (startX+tempWidth + charW > LoginMonitorActivity.windowWidth-5) 
    			            {
    			            	if(MsnUtil.isLetter(text[j]))//后面是英文字母 要回溯遍历
    			            	{
    			            		find = false;//表示回朔完该行是否找到可以换行的字符
    			            		for(int n=length-1;n>=0;n--)
    			            		{
    			            			if(!MsnUtil.isLetter(strbuff.charAt(n)))
    			            			{//在该字符处换行
    			            				msg.addElement(new StringBuffer(""));
    			            				((StringBuffer)(msg.lastElement())).append(strbuff.toString().substring(n+1));
    			            				((StringBuffer)(msg.lastElement())).append(text[j]);
    			            				strbuff.delete(n,length);
    			            				find = true;
    			            				break;
    			            			}
    			            		}
    			            		if(!find)//回朔完该行后没有找到可以换行的字符
    			            		{
    				            		if(msg.size()==1&&firstX != 5)//如果该行是第一行且起始位置不在行首，则需要另起一行
    				            		{
    				            			yTop = yTop+distance;
    				            			firstX = 5;
    				            			startX = 5;
    				            			((StringBuffer)(msg.lastElement())).append(text[j]);
    				            		}
    				            		else
    				            		{			
    				            			msg.addElement(new StringBuffer(""));
    				            			((StringBuffer)(msg.lastElement())).append(text[j]);
    				            		}
    			            		}
    			            		
    			            	}
    			            	else //不是英文字母不需要回朔
    			            	{           		
    			            		msg.addElement(new StringBuffer(""));
    			            		((StringBuffer)(msg.lastElement())).append(text[j]);
    			            	}	            	

    			            }
    			            else //不用换行
    			            	((StringBuffer)(msg.lastElement())).append(text[j]);
    		          }         	     	
                }
  
            }
      
        }      
        

        
        protected void onDraw(Canvas canvas)
        {
        	if(!isDone)
        	{
    			if (app.isExit)// 退出
    			{
    				if (app.jabber.requestQueue.isEmpty()
    						&& app.jabber.isSendLogoutRequest)
    				{
    					
    					app.eventAction(EventListener.EVENT_RECEIVE_EXIT, null);
//    					try {
//							stopProgress();
//						} catch (Exception e) {
//							// TODO Auto-generated catch block
//							e.printStackTrace();
//						}
    					isDone=true;
    				}
    			} 
    			else if (app.isLogout)// 注销
    			{

    				if (app.jabber.receive_logout_unavailable)// 收到注销成功包
    				{
    					// 回到登录页面
    					logoutSuccess();
//    					try {
//							stopProgress();
//						} catch (Exception e) {
//							// TODO Auto-generated catch block
//							e.printStackTrace();
//						}
    					isDone=true;
    				}
    			}
        	}
        	
            canvas.drawColor(0xffffffff);
            if(isMoreOneLine)
            {
//<<<<<<< LoginMonitorActivity.java
            	startY=startImageY+bitmapSignIn[0].getHeight()+20;  
//=======
//            	startY=startImageY+bitIcon.getHeight()+20;/*startImageY+mMovie.height()+20;*/  
//>>>>>>> 1.29
    	    	for(int k=0;k<msg.size();k++)
    	    	{
    	    		canvas.drawText(((StringBuffer)msg.elementAt(k)).toString(), startX, startY, paint); 
    	    		startY+=distance;
    	    	}  
            }
            else
            	canvas.drawText(showText, startX, startY, paint);    
           
            canvas.drawBitmap(bitmapSignIn[progressFrameIdx], startImageX, startImageY, paint);
            if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
            {
                if(login_ad != null)
                {
                	int height = (int)(110*app.screenScale);
                    canvas.drawBitmap(login_ad, (LoginMonitorActivity.windowWidth-login_ad.getWidth())/2, startImageY + height, paint);
                }
            }
            else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
            {
                if(login_ad != null)
                {
                	int height = (int)(150*app.screenScale);
                    canvas.drawBitmap(login_ad, (LoginMonitorActivity.windowWidth-login_ad.getWidth())/2, startImageY + height, paint);
                }
            }

            
        }        
	}

//<<<<<<< LoginMonitorActivity.java
	int count=0;//
	private Timer progressTimer;
	int tipIndex=0;
	private byte progressFrameIdx;
	
	private void initProgress()
	{
		count=0;//置初始值
		progressTimer = new Timer();
		tipIndex=0;
		progressTimer.schedule(new Animation(this, "progressTimer"), 0, 200);
	}
	
	
	public void TimerNotify(Object tag) {
		
		try
		{		
			if (tag.equals("progressTimer"))
			{
				count++;// 0.2秒运行一次 可以计时
				progressFrameIdx++;
				if (progressFrameIdx > 7)
				{
					progressFrameIdx = 0;
				}
				//Log.v("progressFrameIdx", "progressFrameIdx="+progressFrameIdx);
				app.eventNotify(EventListener.EVENT_UPDATE_MONITOR_UI, null);	
			}
		}catch(Exception e)
		{
			e.printStackTrace();
			
		}
//=======
//            long now = android.os.SystemClock.uptimeMillis();
//            if (mMovieStart == 0) 
//            {   
//                mMovieStart = now;
//            }
//            int relTime = (int)((now - mMovieStart) % 3);//mMovie.duration());
//            //mMovie.setTime(relTime);
//            //mMovie.draw(canvas, startImageX, startImageY);
//            canvas.drawBitmap(bitIcon, startImageX, startImageY, null);
//            invalidate();
//        }        
//>>>>>>> 1.29
	}
	
	/**将这个定时器停掉*/
    private void stopProgress()throws Exception
    {
        if(progressTimer != null)
        {
        	count=0;
        	progressTimer.cancel();
        	progressTimer = null;
        }
    }
}
