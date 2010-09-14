package com.pica.ui.tools;

import java.util.Map;
import java.util.Vector;

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

public class SessionList extends ListView{
	SessionListAdapter mAdapter;
	MSNApplication app;
	public SessionList(Context context) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
        mAdapter = new SessionListAdapter();
        setAdapter((ListAdapter) mAdapter);		
	}
	
	public void UpdateView()
	{
		mAdapter.notifyDataSetChanged();
	}

	public class SessionListAdapter extends BaseAdapter{

		
		public int getCount() {
			// TODO Auto-generated method stub
			//return 5;
			if(app.sessionData!=null)
			{

			}
			return app.sessionData.size();
		}

		
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			//return position;
			return app.sessionData.get(position);
		}

		
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		
		public View getView(int position, View convertView, ViewGroup parent) 
		{
			// TODO Auto-generated method stub
		    int frame = (int)(40*app.screenScale);
	        AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
	                ViewGroup.LayoutParams.FILL_PARENT, frame);       
	        
	        Map<String, Item> map=app.sessionData.get(position);
	        Item tempSession=map.get("SESSION");
	       				        	      
	        SessionItem gItem = new SessionItem(getContext(), tempSession);;
			gItem.setLayoutParams(lp);
			return gItem;
		}
    }
}
