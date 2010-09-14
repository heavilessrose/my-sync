package com.pica.ui.tools;

import java.util.List;
import java.util.Map;
import java.util.Vector;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;

import com.pica.msn.Contact;
import com.pica.msn.MSNApplication;

public class HistoryListView extends ListView implements OnItemClickListener,OnItemSelectedListener{
	public List<Map<String,Contact>> CheckInviteData;
	public HistoryListAdapter mAdapter;
	Vector<String> mList = new Vector();
	MSNApplication app;
	GeneralItem selectItem;
	public String historyText = null;
	
	public HistoryListView(Context context, Vector<String> list) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) 
    	{
    		return;
    	}
    	mList = list;
        mAdapter = new HistoryListAdapter();
        setAdapter((ListAdapter) mAdapter);
        setOnItemClickListener(this);
        setOnItemSelectedListener(this);
	}

	public class HistoryListAdapter extends BaseAdapter
	{
		
		public int getCount() 
		{
			return mList.size();
		}

		
		public Object getItem(int position)
		{
			return mList.elementAt(position);
		}

		
		public long getItemId(int position)
		{
			return position;
		}

		
		public View getView(int position, View convertView, ViewGroup parent)
		{
			AbsListView.LayoutParams lp = new AbsListView.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, 40);
			GeneralItem gItem = new GeneralItem(getContext(), mList.elementAt(position));			
			gItem.setBackgroundColor(Color.WHITE);
			gItem.setLayoutParams(lp);
			return gItem;
		}
    }

	
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3)
	{
		historyText = (String) getItemAtPosition(arg2);
		if(selectItem != null)
		{
			selectItem.setBackgroundColor(Color.WHITE);
		}
		arg1.setBackgroundColor(Color.rgb(185, 235, 246));
		selectItem = (GeneralItem)arg1;
	}

	
	public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3)
	{
		historyText = (String) getItemAtPosition(arg2);
		if(selectItem != null)
		{
			selectItem.setBackgroundColor(Color.WHITE);
		}
		arg1.setBackgroundColor(Color.rgb(185, 235, 246));
		selectItem = (GeneralItem)arg1;
	}

	
	public void onNothingSelected(AdapterView<?> arg0)
	{
		
	}
}
