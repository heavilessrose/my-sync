package com.pica.msn;


import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.Preference;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;

public class SplashActivity extends Activity
{
    private static final int STOPSPLASH = 0; 
    private MSNApplication app;
    private static final long SPLASHTIME = 3000; 

    private Handler splashHandler = new Handler() 
    { 
         public void handleMessage(Message msg) 
         { 
              switch (msg.what) 
              { 
	              case STOPSPLASH: 
	                  super.handleMessage(msg); 
	                  Login();
	                  break; 
              }          
         } 
    }; 
    
    private void Login()
    {
        app.DB = new DatabaseHelper(this, MSNApplication.DB_NAME, null, MSNApplication.DB_VERSION);
        app.DB.initUserData();   
        Cursor cur = app.DB.getLastUserData();
        
        long freeMemory=Runtime.getRuntime().freeMemory();
        if(freeMemory<600000)
        {
        	try 
        	{
        		System.gc();
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
        }
        
        if(cur != null)
        {
	        if(cur.getCount() != 0)
	        {
	        	try
	        	{
		        	app.SetPicaId(MsnUtil.getUnvalidFormRs(cur.getString(2)));
		        	app.SetEditAccount(MsnUtil.getUnvalidFormRs(cur.getString(3)), cur.getString(4));
		        	app.SetLiveId(MsnUtil.getUnvalidFormRs(cur.getString(0)), MsnUtil.getUnvalidFormRs(cur.getString(1)));
		        	app.isSavePassword=cur.getString(5).equals("true");
		        	app.isAutoLogin=cur.getString(6).equals("true");
		        	app.isOpenOnlineSound =cur.getString(7).equals("true");
		        	app.isOpenNewMessageSound=cur.getString(8).equals("true");
		        	app.SetLoginStatus(cur.getString(15));
		        	app.isShowOnline =cur.getString(16).equals("true");
		        	app.isReceiveHeadChange=cur.getString(17).equals("true");
		        	/////////////取出自己的头像
		        	if(app.isReceiveHeadChange)
		        		app.getVcardHead(app.GetLiveID());
		        	
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
	        		/////////////////
	        	
		        	if(app.isAutoLogin)
		        	{
		        		autoLogin(cur);
		        	}
		    		else
		    		{
						Intent intent;
				        intent = new Intent();
				        intent.setClass(this, LoginActivity.class);
				        startActivity(intent);
				        finish();	    			
		    		}
	        	}
	        	catch(Exception e)
	        	{
					Intent intent;
			        intent = new Intent();
			        intent.setClass(this, LoginActivity.class);
			        startActivity(intent);
			        finish();	 
	        	}
	        }
	        else
	        {
				Intent intent;
		        intent = new Intent();
		        intent.setClass(this, LoginActivity.class);
		        startActivity(intent);
		        finish();
	        }
        }
        else
        {
			Intent intent;
	        intent = new Intent();
	        intent.setClass(this, LoginActivity.class);
	        startActivity(intent);
	        finish();
        }
    }

   private void autoLogin(Cursor cur)
   {
		app.reset();
		//app.SetLiveId(app.DB.getUnvalidFormRs(cur.getString(0)), app.DB.getUnvalidFormRs(cur.getString(1)));
		//app.SetPicaId(app.DB.getUnvalidFormRs(cur.getString(2)));
		
//		if(cur.getString(5).equals("true"))
//		{
//			app.isSavePassword = true;
//		}
//		else
//		{
//			app.isSavePassword = false;
//		}
		
//		if(cur.getString(6).equals("true"))
//		{
//			app.isAutoLogin = true;
//		}
//		else
//		{
//			app.isAutoLogin = false;
//		}
		
//		if(cur.getString(7).equals("true"))
//		{
//			app.isOpenOnlineSound = true;
//		}
//		else
//		{
//			app.isOpenOnlineSound = false;
//		}
//		
//		if(cur.getString(8).equals("true"))
//		{
//			app.isOpenNewMessageSound = true;
//		}
//		else
//		{
//			app.isOpenNewMessageSound = false;
//		} 	
//		app.SetEditAccount(cur.getString(3), cur.getString(4));
		
        if(app.isReceiveHeadChange)
            app.getVcardHead(app.GetLiveID());
		
		Intent intent= new Intent();
		intent.setClass(this, LoginMonitorActivity.class);
		intent.putExtra("type",MSNApplication.MONITOR_LOGIN_FLAG);
		startActivity(intent);
		
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
		
		app.startJabber();
		finish();	
   }
     
   /** Called when the activity is first created. */ 
   @Override 
   public void onCreate(Bundle icicle) 
   { 
       super.onCreate(icicle);
       //byte[] ss=new byte[10240000];
       requestWindowFeature(Window.FEATURE_NO_TITLE);
       //final Window win = getWindow(); 
       //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);  

       app = MSNApplication.getApplication(this);
       if(app.isStart)
       {
    	   finish();
           return;
       }
       else
       {
    	   app.isStart =true;
       }
//       if(app.isPause)
//       {
//           Intent intent;
//           intent = new Intent();
//           intent.setClass(this, MainActivity.class);
//           startActivity(intent);
//           finish();
//       }
       app.screenManager.pushActivity(this);
       app.getConnectType(this);
//       Preference pre = new Preference(this);
//       try {
//            app.Screen_Off_Timeout = Settings.System.getInt(getContentResolver(), Settings.System.SCREEN_OFF_TIMEOUT);
//       } catch (SettingNotFoundException e) {
//            // TODO Auto-generated catch block
//            e.printStackTrace();
//       }
       
       setContentView(R.layout.main); 
       int height = getWindowManager().getDefaultDisplay().getHeight();
       int width = getWindowManager().getDefaultDisplay().getWidth();
       app.InitWapAgent(height, width, height - 25 - 40, width);
       TelephonyManager mTelephonyMgr = (TelephonyManager)getSystemService(TELEPHONY_SERVICE); 
       String strIMSI = mTelephonyMgr.getSubscriberId();
       if(strIMSI != null)
       {
    	   app.SetIMSI(strIMSI);
       }
       app.getVersion(app.GetIMSI(), app);
       
       //splash = (ImageView) findViewById(R.id.flash); 
       Message msg = new Message(); 
       msg.what = STOPSPLASH; 
       splashHandler.sendMessageDelayed(msg, SPLASHTIME); 
       
		//删除所有对话的临时音频文件
		MsnUtil.deleteAllTempAudio(SplashActivity.this);
		//删除包目录下的"temprecord"目录的所有文件
		MsnUtil.deleteTempDir(SplashActivity.this, "app_temprecord");
   }
   
   public void onDestroy()
	{
       app.screenManager.popActivity(this);
       super.onDestroy();
       app.DB.close();
   }
		
//	public boolean onTouchEvent(MotionEvent event)
//	{
//	   Login();
//	   return super.onTouchEvent(event);
//		
//	}
	
	//屏蔽后退键
	public boolean dispatchKeyEvent(KeyEvent event)
	{
		return true;
	}
}