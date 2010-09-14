package com.pica.ui.tools;

import com.pica.msn.EventListener;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.MainActivity;
import com.pica.ui.tools.*;
import android.content.Context;
import android.graphics.Color;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ExpandableListView;
import android.widget.LinearLayout;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ExpandableListView.OnChildClickListener;

public class SessionView extends LinearLayout implements OnItemClickListener,OnItemSelectedListener, OnItemLongClickListener{
	MSNApplication app;
	public LabelView lab;
	public SessionList slist;
	public long lastTouchTime;
	public int select_id = -1;
	SessionItem selectItem;
	
	/**µ±Ç°Ë÷Òý*/
	public int currentIndex;
	public SessionView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	currentIndex=-1;
		setOrientation(LinearLayout.VERTICAL);
		setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));			
		init();
	}
	
	private void init()
	{           
        LayoutParams Rparams = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        slist = new SessionList(getContext());
        slist.setLayoutParams(Rparams);
        slist.setOnItemClickListener(this); 
        slist.setOnItemSelectedListener(this);
        slist.setOnItemLongClickListener(this);
        addView(slist);
	}

	
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) 
	{
		if(select_id != arg2)
		{
			if(selectItem != null)
			{
				selectItem.setBackgroundColor(Color.WHITE);
			}
			arg1.setBackgroundColor(Color.rgb(185, 235, 246));
			selectItem = (SessionItem)arg1;
			select_id = arg2;
			currentIndex=arg2;
		}
		else
		{
			currentIndex=-1;
			Item item = app.sessionData.get(arg2).get("SESSION");
			((MainActivity)getContext()).openChatroomUIFromSessionList(item);			
			select_id = -1;
		}
		
//		if(arg1.hasFocus())
//		{
//			Item item = app.sessionData.get(arg2).get("SESSION");
//			((MainActivity)getContext()).openChatroomUIFromSessionList(item);
//		}
//		else
//		{
//			arg1.isFocusable();
//			arg1.isFocusableInTouchMode();
//			arg1.requestFocus();
//		}
		
//		long thisTime = System.currentTimeMillis();
//        if (thisTime - lastTouchTime < 250) {
//            // Double tap
//            lastTouchTime = -1;
//            Item item = app.sessionData.get(arg2).get("SESSION");
//            ((MainActivity)getContext()).openChatroomUIFromSessionList(item);
//        } 
//        else 
//        {
//            lastTouchTime = thisTime;
//        }
	}

	
	public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,
			long arg3) {
		//Log.v("onItemSelected", arg2+"");
		if(select_id != arg2)
		{
			if(selectItem != null)
			{
				selectItem.setBackgroundColor(Color.WHITE);
			}
			arg1.setBackgroundColor(Color.rgb(185, 235, 246));
			selectItem = (SessionItem)arg1;
			select_id = arg2;
			currentIndex=arg2;
		}
		else
		{
			arg1.setBackgroundColor(Color.rgb(185, 235, 246));
			selectItem = (SessionItem)arg1;
			select_id = arg2;
			currentIndex=arg2;
		}
	}

	
	public void onNothingSelected(AdapterView<?> arg0) {
		// TODO Auto-generated method stub
		currentIndex=-1;
		select_id = -1;
		//Log.v("onNothingSelected","onNothingSelected");
	}

    /* (non-Javadoc)
     * @see android.widget.AdapterView.OnItemLongClickListener#onItemLongClick(android.widget.AdapterView, android.view.View, int, long)
     */
    
    public boolean onItemLongClick(AdapterView<?> arg0, View arg1, int arg2,
            long arg3) {
        if(select_id != arg2)
        {
            if(selectItem != null)
            {
                selectItem.setBackgroundColor(Color.WHITE);
            }
            arg1.setBackgroundColor(Color.rgb(185, 235, 246));
            selectItem = (SessionItem)arg1;
            select_id = arg2;
            currentIndex=arg2;
        }
        else
        {
            arg1.setBackgroundColor(Color.rgb(185, 235, 246));
            selectItem = (SessionItem)arg1;
            select_id = arg2;
            currentIndex=arg2;
        }

        ((MainActivity)app.mActivity).deleteOneSession();
        return true;
    }
}