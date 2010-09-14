package com.pica.ui.tools;

import java.util.Map;

import com.pica.msn.LoginActivity;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class UserSelectView extends ListView{
		private UserSelectAdapter mAdapter;
		public LoginActivity activity;
		public UserSelectView(Context context, Activity ac) {
			super(context);
			activity = (LoginActivity)ac;
	        mAdapter = new UserSelectAdapter();
	        setAdapter((ListAdapter) mAdapter);		
		}
		
		public class UserSelectAdapter extends BaseAdapter{

			
			public int getCount() {
				// TODO Auto-generated method stub
				return activity.UserData.size();
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
	            AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
	                    ViewGroup.LayoutParams.FILL_PARENT, 40); 
				TextView gItem = new TextView(getContext());
				gItem.bringToFront();
				gItem.setLayoutParams(lp);
				gItem.setBackgroundColor(Color.WHITE);
				gItem.setGravity(Gravity.CENTER_VERTICAL);
				Map<String, String> map=activity.UserData.get(position);
				gItem.setText(map.get("strLiveID"));
				return gItem;
			}
	        // Sample data set.  children[i] contains the children (String[]) for groups[i].


	    }
	}

