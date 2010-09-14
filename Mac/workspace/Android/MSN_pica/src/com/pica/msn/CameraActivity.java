package com.pica.msn;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.os.Bundle;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.OrientationEventListener;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class CameraActivity extends Activity 
{
	private Preview mPreview;
	private MSNApplication app;
	private static final int MENU_1 = Menu.FIRST;
	private static final int MENU_2 = Menu.FIRST + 1;
	private static final int MENU_3 = Menu.FIRST + 2;
	private static final int MENU_4 = Menu.FIRST + 3;
	private Menu mMenu;
	
    /**页面标志索引 联系人页面拍照 聊天页面拍照*/
    private byte currentType;
	private OrientationEventListener mOrientationListener;
	Thread loadServiceThread;
	
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
					//////////////////
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
    	
	public void onResume() 
	{
		super.onResume();
		app.isPause = false;
		/**解决进程被杀死的bug*/
		if(isKilled && app == null)
		{
			finish();
			return;
		}
 	}	
	
    public void onPause()
    {
        super.onPause();
        app.isPause = true;
    }
    //////////////////////////////
	
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);
    	
    	////////////////////解决进程被杀死的bug
        if(savedInstanceState != null && savedInstanceState.getBoolean("isKilled") && app == null)
        {
        	isKilled = true;
        	finish();
        	app = MSNApplication.getApplication(this);
        	app.screenManager.pushActivity(this);
        	if(!app.killedToRelogin)//没有在别的Activity中重新登录
        	{
        		reLogin(false, savedInstanceState);
        	}
        	//在其他的Activity中就不要在重复处理
        	app.killedToRelogin = true;
        	return;
        }
        app = MSNApplication.getApplication(this);
        /////////////////////////
        requestWindowFeature(Window.FEATURE_NO_TITLE);//add by liubing 20091124
        final Window win = getWindow();
        win.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
//      requestWindowFeature(Window.FEATURE_NO_TITLE);
        Bundle extras=getIntent().getExtras();
        currentType=extras.getByte("type");
        
        app.screenManager.pushActivity(this);
        RelativeLayout layout = new RelativeLayout(this);
        
        mPreview = new Preview(this);
        
        mPreview.shutterButton = new Button(this);
        mPreview.shutterButton.setBackgroundResource(R.drawable.ic_menu_camera);
        mPreview.shutterButton.setOnClickListener(new OnClickListener()
        {
			
			public void onClick(View v)
			{
				if(mPreview.isCanShut)
				{
					mPreview.app.CameraByte = null;
					mPreview.takePicture();
				}
			}
        	
        });
        
        //add by liubing 20091124
        DisplayMetrics dm = new DisplayMetrics(); 
        getWindowManager().getDefaultDisplay().getMetrics(dm); 
        int screenWidth = dm.widthPixels; 
        int screenHeight = dm.heightPixels; 
        int width = 0;
        int height = 0;
        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
            height = screenHeight;
            width = (3 * height)/2;
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
            width = screenWidth;
            height = (3 * width)/2;
        } 


        RelativeLayout.LayoutParams previewParam = new RelativeLayout.LayoutParams(width,
                height);
        //layout.setLayoutParams(previewParam);
        previewParam.addRule(RelativeLayout.CENTER_IN_PARENT);
        //end
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
        									ViewGroup.LayoutParams.WRAP_CONTENT);
        param.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	    param.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
	    layout.addView(mPreview,previewParam);//modify by liubing 20091124
	    layout.addView(mPreview.shutterButton, param);
	    setContentView(layout);
	    
	    /////////////////////////////开启手机旋转监听（用于判断是否要倒转图片），太耗内存
	    
//	    loadServiceThread = new Thread(new Runnable()
//	    {
//            public void run()
//            {
//            	mOrientationListener = new OrientationEventListener(CameraActivity.this)
//                {
//                    public void onOrientationChanged(int orientation) {
//                        // We keep the last known orientation. So if the user
//                        // first orient the camera then point the camera to
//                        // floor/sky, we still have the correct orientation.
//                        if (orientation != ORIENTATION_UNKNOWN)
//                            mPreview.mLastOrientation = orientation;
//                    }
//                };
//                mOrientationListener.enable();
//            }
//        });
//        loadServiceThread.start();
    }

//    
	protected void onDestroy()
	{
	    app.screenManager.popActivity(this);
		super.onDestroy();
	}
    
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {		
        switch (keyCode) 
        { 
	        case KeyEvent.KEYCODE_FOCUS:
	        {
	        	if(mPreview.isCanShut)
	        	{
	        		mPreview.app.CameraByte = null;
	        		mPreview.takePicture();
	        	}			
	        }
            break; 
        }
		return super.onKeyUp(keyCode, event);
	}
    
    public boolean onCreateOptionsMenu(Menu menu)
    {
    	super.onCreateOptionsMenu(menu);
    	mMenu = menu;
    	menu.add(0, MENU_1, 0, R.string.text_camera_send);
    	menu.add(1, MENU_2, 0, R.string.text_camera_preview);
    	menu.add(0, MENU_3, 0, R.string.text_camera_takepicture);
    	menu.add(1, MENU_4, 0, R.string.text_camera_back);
    	return true;
    }
    
    public boolean onPrepareOptionsMenu(Menu menu) {
        super.onPrepareOptionsMenu(menu);
        if(mPreview.app.CameraByte == null)
        {
        	menu.findItem(MENU_1).setVisible(false);
        	menu.findItem(MENU_2).setVisible(false);
        	menu.findItem(MENU_3).setVisible(false);
        	menu.findItem(MENU_4).setVisible(false);
        }
        else
        {
        	menu.findItem(MENU_1).setVisible(true);
        	menu.findItem(MENU_2).setVisible(true);
        	menu.findItem(MENU_3).setVisible(false);
        	menu.findItem(MENU_4).setVisible(true);
        }
        return true;
    }
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) 
		{
			case MENU_1:
				setResult(RESULT_OK);
				finish();
				
				return true;
			case MENU_2:
				mPreview.isCanShut = true;
				mPreview.app.CameraByte = null;
//				mPreview.mCamera.stopPreview();
		        Camera.Parameters parameters = mPreview.mCamera.getParameters();             
		        parameters.setPreviewSize(320, 240);
		        parameters.setPictureSize(320, 240);
		        // FIXME: This is a hack to turn the flash off on the Samsung Galaxy.
		        parameters.set("flash-value", 2);

		        // This is the standard setting to turn the flash off that all devices should honor.
		        parameters.set("flash-mode", "off");
		        parameters.set("jpeg-quality", 70);
		        mPreview.mCamera.setParameters(parameters);
				mPreview.mCamera.startPreview();
				mPreview.shutterButton.setVisibility(View.VISIBLE);
//				closeOptionsMenu();
//				openOptionsMenu(); 
				return true;
			case MENU_3:
				mPreview.app.CameraByte = null;
				mPreview.takePicture();
				return true;
			case MENU_4:
				mPreview.app.CameraByte = null;
				setResult(RESULT_OK);
				finish();
				return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
    	if(keyCode == KeyEvent.KEYCODE_BACK)
    	{
    		mPreview.app.CameraByte = null;
    		setResult(RESULT_OK);
    		this.finish();
    		return true;
    	}
    	else if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER)
        {
    		if(mPreview.isCanShut)
        	{
        		mPreview.app.CameraByte = null;
        		mPreview.takePicture();
        	}
        }
        return super.onKeyDown(keyCode, event);
    }
    
}

class Preview extends SurfaceView implements SurfaceHolder.Callback 
{
    public int mLastOrientation;
	SurfaceHolder mHolder;
    public Camera mCamera;
	public MSNApplication app;
	public Button shutterButton;
	boolean isCanShut = true;
    Preview(Context context) 
    {
        super(context);
        app = MSNApplication.getApplication();
        app.CameraByte = null;
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) 
    {
        mCamera = Camera.open();      
        try 
        {
			mCamera.setPreviewDisplay(holder);
		} 
        catch (Exception e)
        {
			e.printStackTrace();
		}
    }

    public void surfaceDestroyed(SurfaceHolder holder) 
    {
        mCamera.stopPreview();
        mCamera.release();
        mCamera = null;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    	try
    	{
			mCamera.setPreviewDisplay(holder);
		}
    	catch (Exception e) 
    	{
			e.printStackTrace();
		}
        Camera.Parameters parameters = mCamera.getParameters();             
        parameters.setPreviewSize(320, 240);
        parameters.setPictureSize(320, 240);
        // FIXME: This is a hack to turn the flash off on the Samsung Galaxy.
        parameters.set("flash-value", 2);

        // This is the standard setting to turn the flash off that all devices should honor.
        parameters.set("flash-mode", "off");
        parameters.set("jpeg-quality", 70);
        mCamera.setParameters(parameters);        
        mCamera.startPreview();
//        ((CameraActivity)getContext()).closeOptionsMenu();
//        ((CameraActivity)getContext()).openOptionsMenu(); 
    }
    
    public void takePicture()
    {
    	isCanShut = false;
        mCamera.takePicture(null,null,new Camera.PictureCallback()
        {
            public void onPictureTaken(byte[] data,Camera camera)
            {            	
            	//Window window = ((CameraActivity)getContext()).getWindow();
            	//Log.e("!!!!!!!!!!", "" + wm.getDefaultDisplay().getOrientation());
            	//Log.e("!!!!!!!!!!", "" + mLastOrientation);            	
            	if(data != null)
            	{
//            		//如果竖屏拍照，图片倒转90度
//            		if( ((mLastOrientation + 45) % 360) < 180 )
//            		{
//            			Bitmap b = BitmapFactory.decodeByteArray(data, 0, data.length);
//	            		Matrix m = new Matrix();
//	        			m.setRotate(90, (float) b.getWidth() / 2, (float) b.getHeight() / 2);			
//	        			b = Bitmap.createBitmap(b, 0, 0, b.getWidth(), b.getHeight(), m, true);
//	        			
//	        			ByteArrayOutputStream os = new ByteArrayOutputStream();
//	        			b.compress(Bitmap.CompressFormat.JPEG, 80, os);
//	        			byte[] newData = new byte[data.length];
//	        			try
//						{
//							os.write(newData);
//						}
//	        			catch (IOException e)
//						{
//							// TODO Auto-generated catch block
//							e.printStackTrace();
//							Log.e("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~","1!!!!!!!!!!!!!!!!!!");
//						}
//	        			app.CameraByte = null;
//	            		app.CameraByte = new byte[newData.length];
//	            		app.CameraByte = newData;
//	            		shutterButton.setVisibility(View.INVISIBLE);
//            		}
//            		else//如果横屏拍照，无需倒转图片
            		{
            			app.CameraByte = null;
	            		app.CameraByte = new byte[data.length];
	            		app.CameraByte = data;
	            		shutterButton.setVisibility(View.INVISIBLE);
            		}
            	}
            	else
            	{
            		app.CameraByte = null;
            	}
            	((CameraActivity)getContext()).closeOptionsMenu();
            	((CameraActivity)getContext()).openOptionsMenu(); 
            }
        });
    }
}