package com.pica.ui.tools;

import java.util.Map;

import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;
import android.widget.CheckedTextView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;


public class LabelItem extends LinearLayout{
	int idx;
	boolean bSplashMessage = false;
	public boolean bPressSplash = false;
	SessionImage img;
	MSNApplication app;
	TextView text;
	public LabelItem(Context context, boolean bText, int index) {
		super(context);
		// TODO Auto-generated constructor stub
		idx = index;
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
		init();
	}
	private void init()
	{
		setOrientation(LinearLayout.HORIZONTAL);
		setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));

        setGravity(Gravity.CENTER);

        Resources r = this.getContext().getResources();
        
        LayoutParams param = new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT);      
  
        if(idx == 0)
        {
	        img = new SessionImage(getContext(), 0);
	        img.setImageDrawable(r.getDrawable(R.drawable.roster_flash_1));
	        img.setLayoutParams(param);
	        addView(img);
	        
	        text = new TextView(getContext());
	        text.setText(R.string.roster);
	        text.setTextColor(0xff000000);
	        text.setLayoutParams(param);
	        addView(text);  
        }
        else if(idx == 1)
        {
	        img = new SessionImage(getContext(), 1);
	        img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
	        img.setLayoutParams(param);
	        addView(img);
	        
	        text = new TextView(getContext());
	        text.setText(R.string.chat);
	        text.setTextColor(0xffffffff);
	        text.setLayoutParams(param);
	        addView(text);  
        }
        else if(idx == 2)
        {
        	img = new SessionImage(getContext(), 2);
	        img.setImageDrawable(r.getDrawable(R.drawable.zwp_flash_2));
	        img.setLayoutParams(param);
	        addView(img);
	        
	        text = new TextView(getContext());
	        text.setText(R.string.zwap);
	        text.setTextColor(0xffffffff);
	        text.setLayoutParams(param);
	        addView(text);  
        }
        
	}
	
	public void setRoster(boolean bFlash)
	{
		Resources r = this.getContext().getResources();
		if(bFlash)
		{
			if(idx == 0)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 1)
			{
				text.setTextColor(0xffffffff);
			}
			else if(idx == 2)
			{
				text.setTextColor(0xffffffff);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.roster_flash_1));
		}
		else
		{
			if(idx == 0)
			{
				text.setTextColor(0xffffffff);
			}
			else if(idx == 1)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 2)
			{
				text.setTextColor(0xff000000);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.roster_flash_2));
		}
	}
	
	public void setSession(boolean bFlash)
	{
		Resources r = this.getContext().getResources();
        int session_num = 0;
        for(int i=0; i<app.sessionData.size(); i++)
        {
                Map<String, Item> map=app.sessionData.get(i);
                Item session=map.get("SESSION");
                session_num = session_num + Integer.parseInt(session.SESSION_numOfNewMsg);             
        }
		if(bFlash)
		{
			if(idx == 0)
			{
				text.setTextColor(0xffffffff);
			}			
			else if(idx == 1)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 2)
			{
				text.setTextColor(0xffffffff);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
		}
		else
		{
			if(idx == 0)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 1)
			{
				text.setTextColor(0xffffffff);
			}
			else if(idx == 2)
			{
				text.setTextColor(0xff000000);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_2));
		}
        if(session_num == 0)
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
        }
        else if(session_num > 0 && session_num <= 9)
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_2));
        }
        else
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_3));
        }
        img.setNum(session_num);
        img.invalidate();
	}
	
	public void setZwap(boolean bFlash)
	{
		Resources r = this.getContext().getResources();
		if(bFlash)
		{
			if(idx == 0)
			{
				text.setTextColor(0xffffffff);
			}			
			else if(idx == 1)
			{
				text.setTextColor(0xffffffff);				
			}
			else if(idx == 2)
			{
				text.setTextColor(0xff000000);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.zwp_flash_1));
		}
		else
		{
			if(idx == 0)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 1)
			{
				text.setTextColor(0xff000000);
			}
			else if(idx == 2)
			{
				text.setTextColor(0xffffffff);
			}
			img.setImageDrawable(r.getDrawable(R.drawable.zwp_flash_2));
		}
	}

	public void updateMessage()
	{
        Resources r = this.getContext().getResources();
        int session_num = 0;
        for(int i=0; i<app.sessionData.size(); i++)
        {
            Map<String, Item> map=app.sessionData.get(i);
            Item session=map.get("SESSION");
            session_num = session_num + Integer.parseInt(session.SESSION_numOfNewMsg);             
        }
        
        if(session_num == 0)
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
        }
        else if(session_num > 0 && session_num <= 9)
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_2));
        }
        else
        {
            img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_3));
        }
        img.setNum(session_num);
        img.invalidate();
	}	
    		
//    private Handler MessageHandler = new Handler() 
//    { 
//         public void handleMessage(Message msg) 
//         { 
//              switch (msg.what) 
//              { 
//	              case 1: 
//	                  super.handleMessage(msg); 
//	                  if(bPressSplash)
//	                  {
//	                	  splashMessage(true);
//	                  }
//	                  else
//	                  {
//	                	  
//	                  }
//	                  break; 
//              }          
//         } 
//    }; 
//    
//    public void splashMessage(boolean bFlash)
//    {
//    	bPressSplash = bFlash;
//    	if(idx == 1)
//    	{
//    		Resources r = this.getContext().getResources();
//    		boolean hasMessage = false;
//    		for(int i=0; i<app.sessionData.size(); i++)
//    		{
//    			Map<String, Item> map=app.sessionData.get(i);
//    	        Item session=map.get("SESSION");
//    	        if(Integer.parseInt(session.SESSION_numOfNewMsg) > 0)
//    	        {
//    	        	hasMessage = true;
//    	        	break;
//    	        }    	        
//    		}
// //   		if(!hasMessage)
// //  		{
// //   			bPressSplash = false;
// //   		}
//    		
//    		if(bPressSplash)
//    		{		    	
//		    	if(bSplashMessage)
//		    	{
//		    		img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_4));
//		    		bSplashMessage = false;
//		    	}
//		    	else
//		    	{
//		    		img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
//		    		bSplashMessage = true;
//		    	}
//		        Message msg = new Message(); 
//		        msg.what = 1; 
//		        MessageHandler.sendMessageDelayed(msg, 800); 
//    		}
//    		else
//    		{
//    		    if(hasMessage)
//    		    {
//    		        img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_4));
//    		    }
//    		    else
//    		    {
//    		        img.setImageDrawable(r.getDrawable(R.drawable.seesion_flash_1));
//    		    }
//    		}
//    	}
//    }
}






















