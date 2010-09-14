package com.pica.msn;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Message;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class SaveFileActivity extends Activity implements AdapterView.OnItemClickListener{
    private String currentDir = "/sdcard/";    
    private EfficientAdapter adapter; 
    
    public static final long FileLengthMax = 1024*500;   
    private ListView listview;
    private TextView tv;
    private ImageView returnButton;
    
    private String[] str;
    private MSNApplication app;
    
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
    	requestWindowFeature(Window.FEATURE_NO_TITLE); 
        super.onCreate(savedInstanceState);
        setContentView(R.layout.savefile);
        
        ///////////////////////解决进程被杀死的bug
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
        ////////////////////////////////
        
        app = MSNApplication.getApplication(this);
        app.screenManager.pushActivity(this);
        final EditText editName = (EditText)findViewById(R.id.editName);        
        
        tv = (TextView) findViewById(R.id.currentDirName);
        tv.setText("/sdcard/");        
        str = sortFileByType(new File("/sdcard/"), currentDir);
        listview = (ListView) findViewById(R.id.allFile);
		adapter = new EfficientAdapter(SaveFileActivity.this);
		listview.setAdapter(adapter);
        listview.setOnItemClickListener(this);
        
        //返回按钮        
        returnButton = (ImageButton)findViewById(R.id.returnButton);
        returnButton.setEnabled(false);
        Resources r = this.getResources();    
        returnButton.setBackgroundDrawable(r.getDrawable(R.drawable.returnbutton));
       
        returnButton.setOnClickListener(new OnClickListener()
        {
			
			public void onClick(View arg0)
			{
				// TODO Auto-generated method stub				
				if(currentDir.equals("/sdcard/"))		
				{					
					tv.setText("/sdcard/");
				}
				else
				{					
					String temp =null;
					temp= currentDir.substring(0, currentDir.length()-1);
					currentDir = currentDir.substring(0, temp.lastIndexOf('/')+1);
					if(currentDir.equals("/sdcard/"))
					{
						returnButton.setEnabled(false);
						tv.setText("/sdcard/");
					}
					else
					{
						tv.setText(currentDir);
					}					
				}
				
				File tempfile = new File(currentDir);
				str = sortFileByType(tempfile, currentDir);
				listview.setAdapter(adapter);
				adapter.notifyDataSetChanged();
			}
        });
        //保存按钮
        Button saveButton = (Button)findViewById(R.id.saveButton);        
        saveButton.setOnClickListener(new OnClickListener()
        {
			
			public void onClick(View arg0)
			{
				// TODO Auto-generated method stub
				if(editName.getText().toString().equals(""))
				{					
					Toast.makeText(SaveFileActivity.this, (String)getText(R.string.error_filename_empty), Toast.LENGTH_SHORT).show();
				}
				else if(isHaveFile(editName.getText().toString()))
				{
					Toast.makeText(SaveFileActivity.this, (String)getText(R.string.error_filename_exists), Toast.LENGTH_SHORT).show();
				}
				else
				{
					Bundle extras=getIntent().getExtras();
					String saveFileName = currentDir + editName.getText().toString() + extras.getString("file_type");
					File tempfile = new File(saveFileName);
					FileOutputStream outstream =null;
					try
					{
						outstream = new FileOutputStream(tempfile, false);					
					}
					catch (FileNotFoundException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
						Toast.makeText(SaveFileActivity.this, (String)getText(R.string.error_write_data), Toast.LENGTH_SHORT).show();
						return;
					}
					try 
					{						
						outstream.write(extras.getByteArray("MESSAGE_VOICE_DATA"));
						outstream.flush();
						outstream.close();
					}
					catch (Exception e)
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
						Toast.makeText(SaveFileActivity.this, (String)getText(R.string.error_write_data), Toast.LENGTH_SHORT).show();
						return;
					}
					finish();
				}				
			}
        });        
    }   

    public void onDestroy()
    {
        app.screenManager.popActivity(this);
        super.onDestroy();
    }

	
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3)
	{
		// TODO Auto-generated method stub
		String temp1 = str[arg2].toString();
		//String temp1 = arg0.getItemAtPosition(arg2).toString();
		currentDir = currentDir + temp1 +"/";
		returnButton.setEnabled(true);
		
		File tempfile = new File(currentDir);
		if(tempfile.isFile())
		{			
			String temp2 =null;
			temp2= currentDir.substring(0, currentDir.length()-1);
			currentDir = currentDir.substring(0, temp2.lastIndexOf('/')+1);
		}
		else
		{
			//有些目录调用list()也可能返回null
			if(tempfile.list() == null)
			{			
				ArrayAdapter<String> adapter;
				adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
				listview.setAdapter(adapter);								
			}
			else
			{
				str = sortFileByType(tempfile, currentDir);								
				adapter.notifyDataSetChanged();
			}			
		}
		if(currentDir.equals("/sdcard/"))
		{
			tv.setText("/sdcard/"); 
		}
		else
		{
			tv.setText(currentDir); 
		}
	}
	
	/**
	 * 按照类型（目录或文件）列出目录下的文件名
	 * @param file		目录
	 * @param currentDir  目录名
	 * @return  装载目录下所有文件名的数组
	 */
	public String[] sortFileByType(File file,String currentDir)
	{			
		String[] tempname = file.list();
		int i = tempname.length;
		int n = 0;//未隐藏文件的数量	
		File tempFile;
		for(int j=0; j<i; j++)
		{
			tempFile = null;
			tempFile = new File(currentDir +tempname[j]);
			if(!tempFile.isHidden())
			{				
				n++;
			}				
		}			
		String[] childname = new String[n];
		int k=0;
		int m=n-1;
		for(int j=0; j<i; j++)
		{
			tempFile = null;
			tempFile = new File(currentDir +tempname[j]);
			//文件不是隐藏文件，文件名才返回
			if(!tempFile.isHidden())
			{
				//两种类型分别在数组两端插入
				if(tempFile.isDirectory())
				{
					childname[k] = tempname[j];
					k++;
				}
				else
				{
					childname[m] = tempname[j];
					m--;
				}
			}				
		}			
		return childname;
	}
	
	private boolean isHaveFile(String fName)
	{
		for(int i=0; i < str.length; i++){
			if(str[i].indexOf(".") != -1)
			{
				String oldName = str[i].substring(0,str[i].lastIndexOf("."));
				if(fName.equals(oldName))
				{
					return true;
				}
			}
		}
		return false;
	}
	
	 private class EfficientAdapter extends BaseAdapter 
	 {
	        private LayoutInflater mInflater;
	        private Bitmap txtIcon;
	        private Bitmap musicIcon;
	        private Bitmap pictureIcon;
	        private Bitmap rarIcon;
	        private Bitmap unknownIcon;
	        private Bitmap folderIcon;

	        public EfficientAdapter(Context context)
	        {
	            mInflater = LayoutInflater.from(context);	            
	            txtIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.txtfile);
	            musicIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.musicfile);
	            pictureIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.picturefile);
	            rarIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.rarfile);
	            unknownIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.unknownfile);
	            folderIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.folder);
	        }

	        public int getCount() 
	        {
	            return str.length;
	        }
	        
	        public Object getItem(int position)
	        {
	            return position;
	        }
	        
	        public long getItemId(int position)
	        {
	            return position;
	        }

	        public View getView(int position, View convertView, ViewGroup parent)
	        {	            
	            ViewHolder holder;
	            
	            if (convertView == null)
	            {
	                convertView = mInflater.inflate(R.layout.list_item_icon_text, null);
	                holder = new ViewHolder();
	                holder.text = (TextView) convertView.findViewById(R.id.text);
	                holder.icon = (ImageView) convertView.findViewById(R.id.icon);
	                convertView.setTag(holder);
	            }
	            else 
	            {	                
	                holder = (ViewHolder) convertView.getTag();
	            }	            
	            holder.text.setText(str[position]);
	            File tempfile = new File(currentDir + str[position]);	            
	            String suffix;
	            if(str[position].contains("."))
	            {
	            	suffix =str[position].substring(str[position].lastIndexOf(".")+1,str[position].length());
	            }
	            else
	            {
	            	suffix = "";
	            }
	            if(tempfile.isFile())
	            {
	            	holder.icon.setImageBitmap(unknownIcon);
	            	if(suffix.equals("txt"))
	            	{
	            		holder.icon.setImageBitmap(txtIcon);
	            	}
	            	if(suffix.equals("3gp")||suffix.equals("wav")||suffix.equals("amr")||suffix.equals("mid")
	            			||suffix.equals("ogg")||suffix.equals("mp3")||suffix.equals("mp4"))
	            	{
	            		holder.icon.setImageBitmap(musicIcon);
	            	}
	            	else if(suffix.equals("jpg")||suffix.equals("png")||suffix.equals("gif")||suffix.equals("bmp"))
	            	{
	            		holder.icon.setImageBitmap(pictureIcon);
	            	}
	            	else if(suffix.equals("zip")||suffix.equals("rar"))
	            	{
	            		holder.icon.setImageBitmap(rarIcon);
	            	}           	            	
	            }
	            else
	            {
	            	holder.icon.setImageBitmap(folderIcon);
	            }
	            return convertView;
	        }

	        class ViewHolder 
	        {
	            TextView text;
	            ImageView icon;
	        }
	 }
}
