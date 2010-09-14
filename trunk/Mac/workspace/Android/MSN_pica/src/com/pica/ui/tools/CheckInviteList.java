package com.pica.ui.tools;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;


import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;

public class CheckInviteList extends ListView{
	public List<Map<String,Contact>> CheckInviteData;
	public CheckInviteListAdapter mAdapter;
	MSNApplication app;
	public CheckInviteList(Context context) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	init();
        mAdapter = new CheckInviteListAdapter();
        setAdapter((ListAdapter) mAdapter);		
	}
	
	private void init()
	{
		CheckInviteData= new ArrayList<Map<String, Contact>>();
		
        Vector chaters = (Vector) app.currentSession.SESSION_chaters;
        Contact chater;
        Map<String, Contact>  currentMap;
		for(int m = 0; m < chaters.size(); m++)
		{
			chater = null;
			chater = (Contact)chaters.elementAt(m);
			currentMap = null;
			currentMap = new HashMap<String, Contact>();      
			CheckInviteData.add(0,currentMap);
            currentMap.put("CHECK_INVITE", chater);
		}
	}

	public class CheckInviteListAdapter extends BaseAdapter{

		
		public int getCount() {
			// TODO Auto-generated method stub
			//return 5;
			return CheckInviteData.size();
		}

		
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			//return position;
			
			return CheckInviteData.get(position);
		}

		
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
//            if (convertView != null) 
//            {
//                return convertView;
//            }
//            else
//            {
		        AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
		                ViewGroup.LayoutParams.FILL_PARENT, 40);       
		        
		        Map<String, Contact> map=CheckInviteData.get(position);
		        Contact temp=map.get("CHECK_INVITE");
		       				        	      
		        GeneralItem gItem = new GeneralItem(getContext(), temp.CONTACT_nickname);;
				gItem.setLayoutParams(lp);
				return gItem;
//            }
		}
        // Sample data set.  children[i] contains the children (String[]) for groups[i].


    }
}
