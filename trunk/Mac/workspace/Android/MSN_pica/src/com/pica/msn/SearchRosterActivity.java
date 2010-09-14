package com.pica.msn;

import java.util.Vector;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

import com.pica.ui.tools.DetailInfoView;
import com.pica.ui.tools.RosterMiniItem;



public class SearchRosterActivity extends Activity implements AdapterView.OnItemClickListener
{
	private MSNApplication app;
	private SearchListAdapter mAdapter;
	public Vector<Contact> rosterList;	
	ListView roster;
	EditText rosterName;
	Contact mCurItem;
	DetailInfoView div;
	private String errorText;

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
		setContentView(R.layout.search_roster);
		
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
		app.setSearchRosterHander(searchRosterHander);
		rosterList = new Vector();
		Group gp;
		
		Vector<Contact> onlineV = new Vector<Contact>();
		Vector<Contact> offlineV = new Vector<Contact>();
		offlineV.removeAllElements();
		onlineV.removeAllElements();
        for(int m = 0; m < app.groupData.size(); m++)
		{
            gp = app.groupData.get(m);
			for(int n = 0; n < gp.contactVector.size(); n++)
			{			
                Contact ct = gp.contactVector.get(n);

            	if(ct.SUBITEM_STATE == app.STATE_OFFLINE)
            	{
            		offlineV.addElement(ct);
            	}
            	else
            	{
            		onlineV.addElement(ct);
            	}
			}
		}	
		
        for(int p = 0; p < onlineV.size(); p++)
        {
        	Contact ct = onlineV.get(p);
        	rosterList.addElement(ct);
        }
        
        for(int p = 0; p < offlineV.size(); p++)
        {
        	Contact ct = offlineV.get(p);
        	rosterList.addElement(ct);
        }
		roster = (ListView) findViewById(R.id.allRoster);
		mAdapter = new SearchListAdapter(this);
		roster.setAdapter(mAdapter);
		roster.setOnItemClickListener(this);
		
		rosterName = (EditText) findViewById(R.id.name);
		TextWatcher watcher = new TextWatcher()
		{
            public void beforeTextChanged(CharSequence s, int start, int before, int after)
            {
            	
            }

            public void onTextChanged(CharSequence s, int start,int before, int count)
            {
            	MatchAccount();
            }

            public void afterTextChanged(android.text.Editable s)
            {
            	
            }
        };
        rosterName.addTextChangedListener(watcher);
	}
	
	public void onDestroy()
    {
	    app.screenManager.popActivity(this);
		super.onDestroy();
		app.setSearchRosterHander(null);
		searchRosterHander = null;
    }
	
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub
		mCurItem = rosterList.get(arg2);
		if(mCurItem.CONTACT_contactFlag == app.CONTACT_NORMAL_FLAG)
    	{
			showDialog(app.CONTACT_NORMAL_FLAG);
    	}    		
    	else if(mCurItem.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
    	{
    		showDialog(app.CONTACT_BLOCK_FLAG);
    	}
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
        	Message msg = new Message();
        	msg.what = EventListener.OPEN_CHAT_FROM_SEARCH_ROSTER;
        	msg.obj = item;
        	app.MainHandler.sendMessage(msg);
        	finish();
        	
			//openChatroomUIFromContactList(item);
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
	
	
	protected Dialog onCreateDialog(int id) {

    	if(id == app.CONTACT_NORMAL_FLAG)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.opear_normal, null);
            view.setBackgroundColor(Color.WHITE);//15
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
            return new AlertDialog.Builder(SearchRosterActivity.this)
            .setView(view)
            .create();    		
    	}    		
    	else if(id == app.CONTACT_BLOCK_FLAG)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View view = factory.inflate(R.layout.opear_block, null);
            view.setBackgroundColor(Color.WHITE);//15
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
            return new AlertDialog.Builder(SearchRosterActivity.this)
            .setView(view)
            .create();     		
    	}
    	
    	
    	
    	else if(id == UiEventListener.UI_EVENT_DELETE_ROSTER)
    	{
            LayoutInflater factory = LayoutInflater.from(this);
            final View DeleteRosterView = factory.inflate(R.layout.deleteroster, null);
            final CheckBox check = (CheckBox)DeleteRosterView.findViewById(R.id.check);
            return new AlertDialog.Builder(SearchRosterActivity.this)
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
            return new AlertDialog.Builder(SearchRosterActivity.this)
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
            return new AlertDialog.Builder(SearchRosterActivity.this)
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
    	
    	else if(id == UiEventListener.UI_EVENT_DETAIL_INFO)
    	{
    		div = new DetailInfoView(SearchRosterActivity.this,mCurItem);
    		int icon = ChangeIcon();
    		return new AlertDialog.Builder(SearchRosterActivity.this)
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
    	
    	else if(id == UiEventListener.UI_EVENT_ERROR_USERNAME)
    	{
            return new AlertDialog.Builder(SearchRosterActivity.this)
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
    	
    	
        return null;
    }
	
	
	private void MatchAccount()
	{
		rosterList.removeAllElements();
		Vector<Contact> onlineV = new Vector<Contact>();
		Vector<Contact> offlineV = new Vector<Contact>();
		offlineV.removeAllElements();
		onlineV.removeAllElements();
		for(int m = 0; m < app.groupData.size(); m++)
		{
			Group gp = app.groupData.get(m);
			for(int n = 0; n < gp.contactVector.size(); n++)
			{
				Contact ct = gp.contactVector.get(n);				
				if((ct.CONTACT_nickname.toLowerCase().indexOf(rosterName.getText().toString().toLowerCase()) != -1)
						|| (ct.CONTACT_imid.toLowerCase().indexOf(rosterName.getText().toString().toLowerCase()) != -1))
				{
	            	if(ct.SUBITEM_STATE == app.STATE_OFFLINE)
	            	{
	            		offlineV.addElement(ct);
	            	}
	            	else
	            	{
	            		onlineV.addElement(ct);
	            	}
				}
			}
		}
        for(int p = 0; p < onlineV.size(); p++)
        {
        	Contact ct = onlineV.get(p);
        	rosterList.addElement(ct);
        }
        
        for(int p = 0; p < offlineV.size(); p++)
        {
        	Contact ct = offlineV.get(p);
        	rosterList.addElement(ct);
        }
		mAdapter.notifyDataSetChanged();
	}
	
	 private Handler searchRosterHander = new Handler() 
	    { 
	         public void handleMessage(Message msg) 
	         { 
	              switch (msg.what) 
	              { 
		              case EventListener.EVENT_UPDATE_CONTACT_UI: 
		                  super.handleMessage(msg); 
		                  MatchAccount();
		                  break; 
	              }          
	         } 
	    }; 
	
	public class SearchListAdapter extends BaseAdapter implements Filterable
	{
		private Context mContext;
        public SearchListAdapter(Context context) {
            super();
            mContext = context;
        }
        
		
		public int getCount() {
			// TODO Auto-generated method stub			
			return rosterList.size();
		}

		
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
			RosterMiniItem rosterItem = new RosterMiniItem(mContext, (Contact)rosterList.get(position), false);
			return rosterItem;
			/*final LayoutInflater inflater = LayoutInflater.from(mContext);
            AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
                    ViewGroup.LayoutParams.FILL_PARENT, 30); 
			TextView gItem = (TextView) inflater.inflate(
                    android.R.layout.simple_dropdown_item_1line, parent, false);
			gItem.setLayoutParams(lp);
			gItem.setGravity(Gravity.CENTER_VERTICAL);
			gItem.setText( ((Contact)rosterList.get(position)).CONTACT_nickname);
			gItem.setTextColor(Color.BLACK);
			return gItem;*/
		}
        

		
		public Filter getFilter()
		{
			// TODO Auto-generated method stub
			return null;
		}

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
	
}
