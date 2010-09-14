package com.pica.ui.tools;

import android.content.Context;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

public class LabelView extends LinearLayout{
	MSNApplication app;
	int type;
	public LabelItem rosterItem;
	public LabelItem sessionItem;
	public LabelItem zwpItem;
	public LabelView(Context context, int t) {
		super(context);
		type = t;
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
		init();
		// TODO Auto-generated constructor stub
	}
	
	private void init()
	{
		//if(type == 0)
		{
			setOrientation(LinearLayout.HORIZONTAL);
			setLayoutParams(new ViewGroup.LayoutParams(
	                ViewGroup.LayoutParams.FILL_PARENT,
	                ViewGroup.LayoutParams.FILL_PARENT));
	        
	        LayoutParams param = new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.FILL_PARENT, 1);        
	        rosterItem = new LabelItem(getContext(), true, 0);
	        rosterItem.setLayoutParams(param);
	        rosterItem.setBackgroundResource(R.drawable.lab_select);
	        rosterItem.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) {
					// TODO Auto-generated method stub
	                if(app.MainHandler!=null)
	                {
	               	 	Message m = new Message(); 
					     m.what = EventListener.EVENT_ROSTER; 
					     app.MainHandler.sendMessage(m); 					     
	                }
				}
	        });
	        addView(rosterItem);
	        
	        sessionItem = new LabelItem(getContext(), true, 1);
	        sessionItem.setLayoutParams(param);
	        sessionItem.setBackgroundResource(R.drawable.lab_unselect);
	        sessionItem.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) {
					// TODO Auto-generated method stub
	                if(app.MainHandler!=null)
	                {
	               	 	Message m = new Message(); 
					    m.what = EventListener.EVENT_SESSION; 
					    app.MainHandler.sendMessage(m); 
					    RosterItem.hideHead();
	                }
				}
	        });
	        addView(sessionItem);
	        
	        zwpItem = new LabelItem(getContext(), true, 2);
	        zwpItem.setLayoutParams(param);
	        zwpItem.setBackgroundResource(R.drawable.lab_unselect);
	        zwpItem.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) {
					// TODO Auto-generated method stub
	                if(app.MainHandler!=null)
	                {
	               	 	Message m = new Message(); 
					    m.what = EventListener.EVENT_ZWP; 
					    app.MainHandler.sendMessage(m); 
					    RosterItem.hideHead();
	                }
				}
	        });
	        if (app.jabber.zipWapServer != null)
	        	addView(zwpItem);
		}
	}
}
