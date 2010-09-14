package com.pica.msn;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

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
import android.os.Process;
import android.preference.Preference;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.text.InputFilter;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

import com.pica.ui.tools.UserSelectView;


public class LoginActivity extends Activity implements AdapterView.OnItemSelectedListener{
	private Menu mMenu;
	private static final int MENU_1 = Menu.FIRST;
	private static final int MENU_2 = Menu.FIRST + 1;
	private static final int MENU_3 = Menu.FIRST + 2;
	private MSNApplication app;
	private String domain;
	private String status = "0";
	private EditText liveid;
	private EditText password;
	private Spinner s1, s2;
	private CheckBox save;
	private CheckBox auto;
	private ArrayAdapter<CharSequence> adapter1, adapter2;
	private static Context context;
	private String errorText;
	private ImageView logintitle;
	public List<Map<String, String>> UserData;
	int version_Spinner = 5;
	
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        context=this;

        app = MSNApplication.getApplication(this);
        if(app.isStartLogin)
        {
            return;
        }
        else
        {
            app.isStartLogin = true;
        }
        if(app.GetEditName() == null)
        {
        	reLoginSet();
        }
        UserData = new ArrayList<Map<String, String>>();
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.login);
        app.screenManager.pushActivity(this);
        if(app.GetMSNHandler().hasMessages(EventListener.EVENT_SEND_HAERT))
        app.GetMSNHandler().removeMessages(EventListener.EVENT_SEND_HAERT);
//        app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
//        app.DB.initUserData();
        app.setLoginHandler(loginHandler);

        
        liveid = (EditText)findViewById(R.id.usernameE);
        TextWatcher watcher = new TextWatcher() {
            public void beforeTextChanged(CharSequence s, int start,
                                          int before, int after) { }

            public void onTextChanged(CharSequence s, int start,
                                          int before, int count) {
            	MatchAccount();
            }

            public void afterTextChanged(android.text.Editable s) { }
        };
        liveid.addTextChangedListener(watcher);
        password = (EditText)findViewById(R.id.passwordE);
        save = (CheckBox)findViewById(R.id.save);
        save.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(save.isChecked())
				{
					showDialog(UiEventListener.UI_EVENT_SAVE_PASSWROD);
				}
				else
				{
					auto.setChecked(false);
				}
			}
        }); 
        final CheckBox auto1 = (CheckBox)findViewById(R.id.auto1);
        final CheckBox auto2 = (CheckBox)findViewById(R.id.auto2);  
        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
        	auto1.setVisibility(View.GONE);
        	auto2.setVisibility(View.VISIBLE);
        	auto = auto2;
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
        	auto2.setVisibility(View.GONE);
        	auto1.setVisibility(View.VISIBLE);
        	auto = auto1;
        } 
        auto1.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(auto1.isChecked())
				{
					if(save.isChecked())
					{
						
					}
					else
					{
						save.setChecked(true);
						showDialog(UiEventListener.UI_EVENT_SAVE_PASSWROD);
					}
					showDialog(UiEventListener.UI_EVENT_AUTO_LOGIN);
				}
				else
				{
				}
			}
        }); 
        
        auto2.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(auto2.isChecked())
				{
					if(save.isChecked())
					{
						
					}
					else
					{
						save.setChecked(true);
						showDialog(UiEventListener.UI_EVENT_SAVE_PASSWROD);
					}
					showDialog(UiEventListener.UI_EVENT_AUTO_LOGIN);
				}
				else
				{
				}
			}
        }); 
        
    	logintitle = (ImageView)findViewById(R.id.logintitle);
        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
        	logintitle.setVisibility(View.GONE);
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
        	logintitle.setVisibility(View.VISIBLE);
        } 
        
        
        Button ac_btn = (Button)findViewById(R.id.account_btn);
		ac_btn.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0){		
					GetUserData();
					return;
			}
        });  
        
        Button bt = (Button)findViewById(R.id.login);
        bt.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0){
				// TODO Auto-generated method stub
		    	if((liveid.getText().toString().trim().length() == 0))
		    	{
		    		showDialog(UiEventListener.UI_EVENT_USERNAME_NULL_ERROR);
		    		return;
		    	}
		    	if(password.getText().toString().trim().length() == 0)
		    	{
		    		showDialog(UiEventListener.UI_EVENT_PASSWORD_NULL_ERROR);
		    		return;
		    	} else if(password.getText().toString().trim().length() < 6){
		    		showDialog(UiEventListener.UI_EVENT_LOGIN_PASSWORD_ERROR);
		    		return;
		    	}
		    	if(domain.equals(getText(R.string.domain_other)))
		    	{
                	String tmp = MsnUtil.getUsernameIsValid(liveid.getText().toString().trim(), true);
			    	if(tmp == null)
			    	{
			    		if(MsnUtil.isNormalPasswordFormat(password.getText().toString().trim()))
			    		{
							if(CheckFirstTimeLogin())
							{
								showDialog(UiEventListener.UI_EVENT_FIRST_TIME);
							}
							else
							{
								login(false);
							}
			    		}
			    		else
			    		{
			    			showDialog(UiEventListener.UI_EVENT_ERROR_PASSWORD);
			    		}
			    	}
			    	else
			    	{
			    		errorText = tmp;
			    		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
			    	}
		    	}
		    	else
		    	{
		    		if(liveid.getText().toString().indexOf("@", 0) != -1)
		    		{
		    			showDialog(UiEventListener.UI_EVENT_LOGIN_EMAIL_ERROR);
		    			return ;
		    		}
		    		String tmp = MsnUtil.getUsernameIsValid(liveid.getText().toString().trim()+ domain, true);
			    	if(tmp == null)
			    	{
			    		if(MsnUtil.isNormalPasswordFormat(password.getText().toString().trim()))
			    		{
							if(CheckFirstTimeLogin())
							{
								showDialog(UiEventListener.UI_EVENT_FIRST_TIME);
							}
							else
							{
								login(false);
							}
			    		}
			    		else
			    		{
			    			showDialog(UiEventListener.UI_EVENT_ERROR_PASSWORD);
			    		}
			    	}
			    	else
			    	{
		    			errorText = tmp;
			    		showDialog(UiEventListener.UI_EVENT_ERROR_USERNAME);
			    	}
		    	}
			}
        });      
        s1 = (Spinner) findViewById(R.id.spinner1);
        adapter1 = ArrayAdapter.createFromResource(
                this, R.array.domains, android.R.layout.simple_spinner_item);
        adapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        s1.setAdapter(adapter1);
		s1.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView arg0, View arg1, int arg2, long arg3) {
            	domain = s1.getItemAtPosition(arg2).toString();
            }

            public void onNothingSelected(AdapterView<?> arg0) {
            }
        });
        
        s2 = (Spinner) findViewById(R.id.spinner2);
        adapter2 = ArrayAdapter.createFromResource(
                this, R.array.status_items, android.R.layout.simple_spinner_item);
        adapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        s2.setAdapter(adapter2);
		s2.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView arg0, View arg1, int arg2, long arg3) {
            	status = Integer.toString(arg2);
            }

            public void onNothingSelected(AdapterView<?> arg0) {
            }
        }); 
        /////////0519
		liveid.setText(app.GetEditName());
		password.setText(app.GetPassWord());
		save.setChecked(app.isSavePassword);
		auto.setChecked(app.isAutoLogin);
		

		for(int i = 0; i < adapter1.getCount(); i++)
		{
			if(adapter1.getItem(i).equals(app.GetDomain()))
			{
				s1.setSelection(i);
				domain = s1.getItemAtPosition(i).toString();
				break;
			}
		}
		
		for(int i = 0; i < adapter2.getCount(); i++)
		{
			if(Integer.toString(i).equals(app.iLoginStatus))
			{
				s2.setSelection(i);
				status = app.iLoginStatus;
				break;
			}
		}
        /////////
        
        
    	Bundle extras=getIntent().getExtras();
    	if(extras!=null)
    	{
    		byte currentType=extras.getByte("type");
    		errorText=extras.getString("errorText");
    		if(currentType==MSNApplication.LOGOUT_SUCCESS_FLAG)
    		{
              	 if(app.isSynchronize)
                 {
    	             //自动重连       	
              		login(true);
                 }
                 else
                 {
                    
                     if(app.LMHandler!=null)
                     {
                    	 Message m = new Message(); 
    				     m.what = EventListener.EVENT_FINISH_ACTIVITY; 
    				     app.LMHandler.sendMessage(m);    
                     }
                   //给段提示
                  	String text=(String) getText(R.string.text_signoff_success);
                 	Toast.makeText(context, text, Toast.LENGTH_SHORT).show();  
                 }
    		}
    		else if(currentType==MSNApplication.LOGOUT_UNAVALIABLE_FLAG)
    		{
                if(app.MainHandler!=null)
                {

               	 	Message m = new Message(); 
				     m.what = EventListener.EVENT_FINISH_ACTIVITY; 
				     app.MainHandler.sendMessage(m);    
                }
                showDialog(UiEventListener.UI_EVENT_LOGIN_LOSE);
    		}
    		else if(currentType==MSNApplication.LOGOUT_ABALIABLE_FLAG)
    		{
    			if(app.MainHandler!=null)
    			{
    				Message m = new Message();
    				m.what = EventListener.EVENT_FINISH_ACTIVITY;
    				app.MainHandler.sendMessage(m);
    			}
    			showDialog(UiEventListener.UI_EVENT_LOGOUT_AVALIABLE);
    		}
    	} 
//    	app.getVersion(app.GetIMSI(), app);
    }
    
    public Handler  loginHandler = new Handler() 
    { 
         
        public void handleMessage(Message msg) 
        { 
             switch (msg.what) 
             {
             	case EventListener.EVENT_FINISH_ACTIVITY:

             		finish();
             		break;   		

             }
        }
        
    };
    public void onConfigurationChanged(Configuration newConfig) 
    {
    	super.onConfigurationChanged(newConfig);
        CheckBox auto1 = (CheckBox)findViewById(R.id.auto1);
        CheckBox auto2 = (CheckBox)findViewById(R.id.auto2); 
        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
        	logintitle.setVisibility(View.GONE);
        	auto1.setVisibility(View.GONE);
        	auto2.setVisibility(View.VISIBLE);
        	if(auto1.isChecked())
        	{
        		auto2.setChecked(true);
        	}
        	else
        	{
        		auto2.setChecked(false);
        	}
        	auto = auto2;
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
        	logintitle.setVisibility(View.VISIBLE);
        	auto2.setVisibility(View.GONE);
        	auto1.setVisibility(View.VISIBLE);
        	if(auto2.isChecked())
        	{
        		auto1.setChecked(true);
        	}
        	else
        	{
        		auto1.setChecked(false);
        	}
        	auto = auto1;
        }  
    	return;
    }
    
    public void onDestroy()
    {
        app.screenManager.popActivity(this);
		super.onDestroy();
		app.isStartLogin = false;
 		app.setLoginHandler(null);
 		loginHandler=null;
    }

    public void onPause()
    {
        super.onPause();
        //Log.e("MainActivity", "onPause");
    }
    
    public void onResume() 
    {
       super.onResume();
  	   app.mCurActivityIdx = app.Login_Activity;
 	   app.SetCurActivity(this);
    }
    
    private void reLoginSet()
    {
        app.DB = new DatabaseHelper(this, MSNApplication.DB_NAME, null, MSNApplication.DB_VERSION);
        app.DB.initUserData();   
        Cursor cur = app.DB.getLastUserData();

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
		        		app.getVcardHead(MsnUtil.getUnvalidFormRs(cur.getString(0)));
		        	
		        	////////////
		        	app.isReceiveVcardChange=cur.getString(18).equals("true");
		        	app.isNudge=cur.getString(19).equals("true");
		        	//////////////////增加按状态排序功能
		        	if(app.isShowOnline)
			        {
				    	if(1 == app.getGroupOrder() || 3 == app.getGroupOrder())
				    	{
				    		app.setGroupOrder((byte)(app.getGroupOrder() + 1));
				    	}
			        }				
					else
					{
						if(2 == app.getGroupOrder() || 4 == app.getGroupOrder())
				    	{
				    		app.setGroupOrder((byte)(app.getGroupOrder() - 1));
				    	}
					}
					////////////////////

	        	}
	        	catch(Exception e)
	        	{

	        	}
	        }
	        else
	        {

	        }
        }
        else
        {

        }
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
		        	//app.SetLiveId(MsnUtil.getUnvalidFormRs(cur.getString(0)), MsnUtil.getUnvalidFormRs(cur.getString(1)));
		        	app.isSavePassword=cur.getString(5).equals("true");
		        	app.isAutoLogin=cur.getString(6).equals("true");
		        	app.isOpenOnlineSound =cur.getString(7).equals("true");
		        	app.isOpenNewMessageSound=cur.getString(8).equals("true");
		        	app.SetLoginStatus(cur.getString(15));
		        	app.isShowOnline =cur.getString(16).equals("true");
		        	app.isReceiveHeadChange=cur.getString(17).equals("true");
		        	/////////////取出自己的头像
		        	if(app.isReceiveHeadChange)
		        		app.getVcardHead(liveId);
		        	
		        	////////////
		        	app.isReceiveVcardChange=cur.getString(18).equals("true");
		        	app.isNudge=cur.getString(19).equals("true");
		        	//////////////////增加按状态排序功能
		        	if(app.isShowOnline)
			        {
				    	if(1 == app.getGroupOrder() || 3 == app.getGroupOrder())
				    	{
				    		app.setGroupOrder((byte)(app.getGroupOrder() + 1));
				    	}
			        }				
					else
					{
						if(2 == app.getGroupOrder() || 4 == app.getGroupOrder())
				    	{
				    		app.setGroupOrder((byte)(app.getGroupOrder() - 1));
				    	}
					}
					////////////////////

	        	}
	        	catch(Exception e)
	        	{
 
	        	}
	        }
	        else
	        {

	        }
        }
        else
        {

        }
    }
    
    //static final int RG_REQUEST = 0;
/**登录 
 * @param isReconnect 是否是重新登录
 * */
	private void login(boolean isReconnect)

    {
        finish();
    	app.reset();

        if(domain.equals(getText(R.string.domain_other)))
        {
        	app.SetLiveId(liveid.getText().toString(), password.getText().toString());        	
        }
        else
        {
        	app.SetLiveId(liveid.getText().toString() + domain, password.getText().toString());
        }

    	reLoginSet(app.GetLiveID());
    	app.SetEditAccount(liveid.getText().toString(), domain); 	  	
    	app.isSavePassword = save.isChecked();
    	app.isAutoLogin = auto.isChecked();
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
//    	app.setGroupOrder(app.ROSTER_NORMAL_NO_OFFLINE);
//    	app.isShowOnline=true;
//    	app.isReceiveHeadChange=true;
    	////////
        app.startJabber();
    }
    
    private boolean CheckFirstTimeLogin()
    {
    	
        if(domain.equals(getText(R.string.domain_other)))
        {
        	app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
        	Cursor cur = app.DB.checkFirstTime(liveid.getText().toString());
        	if(cur == null)
        	{
        		return true;
        	}
        	
        	if(cur.getCount() == 0)
        	{
        		return true;
        	}
        }
        else
        {
        	app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
        	Cursor cur = app.DB.checkFirstTime(liveid.getText().toString()+ domain);  
        	if(cur == null)
        	{
        		return true;
        	}
        	
        	if(cur.getCount() == 0)
        	{
        		return true;
        	}
        }
        return false;
    }
    
    private void MatchAccount()
    {
    	if(UserData.size() == 0)
    	{
    		app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
    		Cursor cur = app.DB.getAllUserData();
    		if(cur != null)
    		{
	    		if(cur.getCount() == 0)
	    		{
    	    		if(liveid.getText().toString().indexOf("@", 0) != -1)
    	    		{
    	    			s1.setSelection(version_Spinner);
    	    			domain = s1.getItemAtPosition(version_Spinner).toString();
    	    		}
	    			return;
	    		}
	    		Map<String, String> Curdata;
	    		Map<String, String> map;
	    		for(int m = 0; m < cur.getCount(); m++)
	    		{
	    			Curdata = null;
					Curdata = new HashMap<String, String>();      
					
					UserData.add(Curdata);
					Curdata.put("strLiveID", MsnUtil.getUnvalidFormRs(cur.getString(0)));
					Curdata.put("strPassWord", MsnUtil.getUnvalidFormRs(cur.getString(1)));
					Curdata.put("strPicaID", MsnUtil.getUnvalidFormRs(cur.getString(2)));
					Curdata.put("strEditName", MsnUtil.getUnvalidFormRs(cur.getString(3)));
					Curdata.put("strDomain", MsnUtil.getUnvalidFormRs(cur.getString(4)));
					Curdata.put("bSavePassword", MsnUtil.getUnvalidFormRs(cur.getString(5)));
					Curdata.put("bAutoLogin", MsnUtil.getUnvalidFormRs(cur.getString(6)));
					Curdata.put("bOpenOnlineSound", MsnUtil.getUnvalidFormRs(cur.getString(7)));
					Curdata.put("bOpenNewMessageSound", MsnUtil.getUnvalidFormRs(cur.getString(8)));
					Curdata.put("iLoginStatus", MsnUtil.getUnvalidFormRs(cur.getString(9)));
					Curdata.put("bShowOnline", MsnUtil.getUnvalidFormRs(cur.getString(10)));
					Curdata.put("bGetRosterIcon", MsnUtil.getUnvalidFormRs(cur.getString(11)));
					Curdata.put("isReceiveContactImpresaChange", MsnUtil.getUnvalidFormRs(cur.getString(12)));
					Curdata.put("bNudge", MsnUtil.getUnvalidFormRs(cur.getString(13)));
					cur.moveToNext();
	
	    		}
	    		
	    		for(int n = 0; n < UserData.size(); n++)
	    		{
	    			map = null;
					map = UserData.get(n);
	    			if(liveid.getText().equals(map.get("strEditName")))
	    			{
	    				
	    	        	/////////////取出自己的头像
	    	        	app.getVcardHead((liveid.getText()).toString());	        	
	    	        	////////////
	    				
	    	    		if(map.get("bSavePassword").equals("true"))
	    	    		{
	    	    			password.setText(map.get("strPassWord"));
	    	    			save.setChecked(true);
	    	    			app.isSavePassword = true;
	    	    		}
	    	    		else
	    	    		{
	    	    			app.isSavePassword = false;
	    	    		}
	    	    		
	    	    		if(map.get("bAutoLogin").equals("true"))
	    	    		{
	    	    			app.isAutoLogin = true;
	    	    		}
	    	    		else
	    	    		{
	    	    			app.isAutoLogin = false;
	    	    		} 	    		
	    	    		for(int i = 0; i < adapter1.getCount(); i++)
	    	    		{
	    	    			if(adapter1.getItem(i).equals(map.get("strDomain")))
	    	    			{
	    	    				s1.setSelection(i);
	    	    				domain = s1.getItemAtPosition(i).toString();
	    	    				break;
	    	    			}
	    	    		}
	    	    		
	    	    		for(int i = 0; i < adapter2.getCount(); i++)
	    	    		{
	    	    			if(Integer.toString(i).equals(map.get("iLoginStatus")))
	    	    			{
	    	    				s2.setSelection(i);
	    	    				status = Integer.toString(i);
	    	    				break;
	    	    			}
	    	    		}
	    	    		return;
	    			}
	    			else
	    			{
	    	    		password.setText(map.get(""));
	    	    		if(liveid.getText().toString().indexOf("@", 0) != -1)
	    	    		{
    	    				s1.setSelection(version_Spinner);
    	    				domain = s1.getItemAtPosition(version_Spinner).toString();
	    	    		}
	    			}
	    		}
    		}
    		else
    		{
	    		if(liveid.getText().toString().indexOf("@", 0) != -1)
	    		{
    				s1.setSelection(version_Spinner);
    				domain = s1.getItemAtPosition(version_Spinner).toString();
	    		}
    		}
    	}
    	else
    	{
    		Map<String, String> map;
    		for(int n = 0; n < UserData.size(); n++)
    		{
    			map = null;
				map= UserData.get(n);
    			if(liveid.getText().toString().equals(map.get("strEditName")))
    			{   	    
    				
    	        	/////////////取出自己的头像
    	        	app.getVcardHead((liveid.getText()).toString());	        	
    	        	////////////
    				
    	    		if(map.get("bSavePassword").equals("true"))
    	    		{
    	    			password.setText(map.get("strPassWord"));
    	    			save.setChecked(true);
    	    			app.isSavePassword = true;
    	    		}
    	    		else
    	    		{
    	    			app.isSavePassword = false;
    	    		}
    	    		
    	    		if(map.get("bAutoLogin").equals("true"))
    	    		{
    	    			app.isAutoLogin = true;
    	    		}
    	    		else
    	    		{
    	    			app.isAutoLogin = false;
    	    		} 	    		
    	    		for(int i = 0; i < adapter1.getCount(); i++)
    	    		{
    	    			if(adapter1.getItem(i).equals(map.get("strDomain")))
    	    			{
    	    				s1.setSelection(i);
    	    				domain = s1.getItemAtPosition(i).toString();
    	    				break;
    	    			}
    	    		}
    	    		
    	    		for(int i = 0; i < adapter2.getCount(); i++)
    	    		{
    	    			if(Integer.toString(i).equals(map.get("iLoginStatus")))
    	    			{
    	    				s2.setSelection(i);
    	    				status = Integer.toString(i);
    	    				break;
    	    			}
    	    		}
    	    		return;
    			}
    			else
    			{
    	    		password.setText(map.get(""));   
    	    		if(liveid.getText().toString().indexOf("@", 0) != -1)
    	    		{
	    				s1.setSelection(version_Spinner);
	    				domain = s1.getItemAtPosition(version_Spinner).toString();
    	    		}
    			}
    		}
    	}
    }
    
	
	public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,
			long arg3) {
		// TODO Auto-generated method stub
		domain = arg0.getItemAtPosition(arg2).toString();
	}

	
	public void onNothingSelected(AdapterView<?> arg0) {
		// TODO Auto-generated method stub
		
	}
	
    public boolean onKeyUp(int keyCode, KeyEvent event) {
/*    	if(keyCode == event.KEYCODE_DPAD_CENTER)
    	{
    		EditText liveid = (EditText)findViewById(R.id.usernameE);
    		if(liveid.isFocused())
    		{
    			GetUserData();
    			return true;
    		}
    	}
    	else */if(keyCode == event.KEYCODE_BACK)
    	{
    		return true;
    	}
    	
        return super.onKeyUp(keyCode, event);
    }
    
    public boolean dispatchKeyEvent(KeyEvent event) 
    {
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK) 
        {
            return true;
        }
		return super.dispatchKeyEvent(event);
    }
    
    private void GetUserData()
    {
    	if(UserData.size() == 0)
    	{
    		app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
    		Cursor cur = app.DB.getAllUserData();
    		if(cur != null)
    		{
//	    		int n = cur.getCount();
	    		if(cur.getCount() == 0)
	    		{
	    			return;
	    		}
	    		
	    		Map<String, String> Curdata;
	    		for(int i = 0; i < cur.getCount(); i++)
	    		{
	    			Curdata = null;
					Curdata = new HashMap<String, String>();      
					UserData.add(Curdata);
					Curdata.put("strLiveID", MsnUtil.getUnvalidFormRs(cur.getString(0)));
					Curdata.put("strPassWord", MsnUtil.getUnvalidFormRs(cur.getString(1)));
					Curdata.put("strPicaID", MsnUtil.getUnvalidFormRs(cur.getString(2)));
					Curdata.put("strEditName", MsnUtil.getUnvalidFormRs(cur.getString(3)));
					Curdata.put("strDomain", MsnUtil.getUnvalidFormRs(cur.getString(4)));
					Curdata.put("bSavePassword", MsnUtil.getUnvalidFormRs(cur.getString(5)));
					Curdata.put("bAutoLogin", MsnUtil.getUnvalidFormRs(cur.getString(6)));
					Curdata.put("bOpenOnlineSound", MsnUtil.getUnvalidFormRs(cur.getString(7)));
					Curdata.put("bOpenNewMessageSound", MsnUtil.getUnvalidFormRs(cur.getString(8)));
					Curdata.put("iLoginStatus", MsnUtil.getUnvalidFormRs(cur.getString(9)));
					Curdata.put("bShowOnline", MsnUtil.getUnvalidFormRs(cur.getString(10)));
					Curdata.put("bGetRosterIcon", MsnUtil.getUnvalidFormRs(cur.getString(11)));
					Curdata.put("isReceiveContactImpresaChange", MsnUtil.getUnvalidFormRs(cur.getString(12)));
					Curdata.put("bNudge", MsnUtil.getUnvalidFormRs(cur.getString(13)));
					cur.moveToNext();
	    		}
	    		showDialog(UiEventListener.UI_EVENT_USER_LIST);
    		}
    	}
    	else
    	{
    		showDialog(UiEventListener.UI_EVENT_USER_LIST);
    	}
    }
    
    
    protected Dialog onCreateDialog(int id) 
    {

    	if(id == UiEventListener.UI_EVENT_USER_LIST)
    	{
        	final UserSelectView list = new UserSelectView(LoginActivity.this, this);
        	list.setOnItemClickListener(new OnItemClickListener() { 
				
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) {
					// TODO Auto-generated method stub
					reSetUserdata(arg2);
					dismissDialog(UiEventListener.UI_EVENT_USER_LIST);
				} 
            });
            return new AlertDialog.Builder(LoginActivity.this)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.text_login_account)
            .setView(list)
            .create();     		
    	}
    	else if(id == UiEventListener.UI_EVENT_USERNAME_NULL_ERROR)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.text_input_username_cannot_null)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_LOGIN_EMAIL_ERROR){
    		return new AlertDialog.Builder(LoginActivity.this)
    		.setCancelable(false)
    		.setIcon(R.drawable.msn)
    		.setTitle(R.string.error_prompt)
    		.setMessage(R.string.login_email_error)
    		.setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
				
				}
			})
    		.create();
    	}
    	else if(id==UiEventListener.UI_EVENT_PASSWORD_NULL_ERROR)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.text_input_password_cannot_null)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_LOGIN_PASSWORD_ERROR)
    	{
    		return new AlertDialog.Builder(LoginActivity.this)
    		.setCancelable(false)
    		.setIcon(R.drawable.msn)
    		.setTitle(R.string.error_prompt)
    		.setMessage(R.string.login_password_error)
    		.setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
					
				}
			})
    		.create();
    	}
    	else if(id == UiEventListener.UI_EVENT_ERROR_USERNAME)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.alert_dialog_icon)
            .setTitle(R.string.error_prompt)
            .setMessage(errorText)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_ERROR_PASSWORD)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.edit_password_error)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_SAVE_PASSWROD)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.safe)
            .setMessage(R.string.save_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_AUTO_LOGIN)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.safe)
            .setMessage(R.string.auto_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_FIRST_TIME)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.permission)
            .setMessage(R.string.permission_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                   login(false);
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_ABOUT)
    	{
    		String aboutMsg="Windows Live Messenger.\nPowered by MMIM Interactive Co,Ltd.\n"+this.getText(R.string.about_version)+
    		MSNApplication.VERSION + "\n"+MSNApplication.EMAIL_ABOUT;
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.about)
            .setMessage(aboutMsg)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	CoverImage();
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_LOGIN_LOSE)
    	{
            return new AlertDialog.Builder(LoginActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.log_lose)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {

                    /* User clicked OK so do some stuff */
                }
            })
            .create();
    	}
    	else if(id == UiEventListener.UI_EVENT_LOGOUT_AVALIABLE)
    	{
    		return new AlertDialog.Builder(LoginActivity.this)
    		.setCancelable(false)
    		.setIcon(R.drawable.msn)
    		.setTitle(R.string.error_prompt)
    		.setMessage(errorText)
    		.setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
					
				}
			})
    		.create();
    	}
		return null;  
    }
    
    private void reSetUserdata(int idx)
    {
		Map<String, String> map=UserData.get(idx);
	
		app.SetLiveId(map.get("strLiveID"), map.get("strPassWord"));
		app.SetPicaId(map.get("strPicaID"));
		liveid.setText(map.get("strEditName"));
		password.setText(map.get("strPassWord"));
		if(map.get("bOpenOnlineSound").equals("1"))
		{
			app.isOpenOnlineSound = true;
		}
		else
		{
			app.isOpenOnlineSound = false;
		}
		
		if(map.get("bOpenNewMessageSound").equals("1"))
		{
			app.isOpenNewMessageSound = true;
		}
		else
		{
			app.isOpenNewMessageSound = false;
		} 
		
		if(map.get("bNudge").equals("1"))
		{
		    app.isNudge = true;
		}
		else
		{
		    app.isNudge = false;
		}
		
		if(map.get("bShowOnline").equals("1"))
		{
			app.isShowOnline = true;
		}
		else
		{
			app.isShowOnline = false;
		}
		
		//////////////////增加按状态排序功能
		if(app.isShowOnline)
        {
	    	if(1 == app.getGroupOrder() || 3 == app.getGroupOrder())
	    	{
	    		app.setGroupOrder((byte)(app.getGroupOrder() + 1));
	    	}
        }				
		else
		{
			if(2 == app.getGroupOrder() || 4 == app.getGroupOrder())
	    	{
	    		app.setGroupOrder((byte)(app.getGroupOrder() - 1));
	    	}
		}
		////////////////
		if(map.get("bGetRosterIcon").equals("1"))
		{
			app.isReceiveHeadChange = true;
		}
		else
		{
			app.isReceiveHeadChange = false;
		} 
		if(map.get("isReceiveContactImpresaChange").equals("1"))
		{
			app.isReceiveVcardChange=true;
		}
		else
		{
			app.isReceiveVcardChange=false;
		}
		
		for(int i = 0; i < adapter1.getCount(); i++)
		{
			if(adapter1.getItem(i).equals(map.get("strDomain")))
			{
				s1.setSelection(i);
				domain = s1.getItemAtPosition(i).toString();
				break;
			}
		}
    }

	
    public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	mMenu = menu;
    	menu.add(Menu.NONE, MENU_3, Menu.NONE, R.string.register_account).setIcon(R.drawable.menu_register);
    	menu.add(Menu.NONE, MENU_1, Menu.NONE, R.string.about).setIcon(R.drawable.about);
    	menu.add(Menu.NONE, MENU_2, Menu.NONE, R.string.exit).setIcon(R.drawable.exit);
    	return true;
        }
    
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case MENU_1:
			showDialog(UiEventListener.UI_EVENT_ABOUT);
			return true;
		case MENU_2:
			finish();
			Process.killProcess(Process.myPid());
			return true;	
		case MENU_3:
			Intent intent= new Intent();
            intent.setClass(this, RegisterActivity.class);
            startActivityForResult(intent, 0);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	private void CoverImage()
    {
        /*String url = "tel:13522773464";
        if (url == null) {
            return;
        }
        Intent intent = new Intent(Intent.ACTION_CALL, Uri.parse(url));
        startActivity(intent);*/
		
//		ZwpView zwp = new ZwpView(this);
//		setContentView(zwp);
	
		//OutPutPicture();
    }
	
	public void OutPutPicture()
    {  
		FileOutputStream fOut = null;
		File f = null;
		try {

				f =  new File("/data/data/com.pica.msn/temp.txt");
				fOut = new FileOutputStream(f); 
		} catch (FileNotFoundException e) {
			e.printStackTrace();

		}

		try {
			fOut.write(1111);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		try {
        	fOut.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
        try {
        	fOut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}	
		
		FileInputStream fIn = null;
		try {
			fIn = new FileInputStream(f);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			int i = fIn.read();
			int tt = 0;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return;
    }
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch(resultCode){
		case RESULT_OK:
			Bundle bundle = data.getExtras();
			this.liveid.setText(bundle.getString("registerUsername"));
			this.password.setText(bundle.getString("registerPassword"));
			s1.setSelection(4);
			domain = s1.getItemAtPosition(4).toString();
			save.setChecked(true);
			s2.setSelection(0);
			showDialog(UiEventListener.UI_EVENT_FIRST_TIME);
			break;
		}
	}
}
