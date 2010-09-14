package com.pica.ui.tools;

import java.util.Vector;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class SessionItem extends LinearLayout{
	private ImageView icon;
	private MSNApplication app;
    int textsize = 18;
	public SessionItem(Context context, Item session) 
	{
		super(context);
		app = MSNApplication.getApplication();
        textsize = (int)(18*app.screenScale);
		init(session);
	}

	private void init(Item session)
	{
		setBackgroundColor(Color.WHITE);
		setOrientation(LinearLayout.HORIZONTAL);
		setGravity(Gravity.CENTER_VERTICAL);
		
        Resources r = this.getContext().getResources();
        
        icon = new ImageView(getContext());      
        icon.setPadding(5, 0, 5, 0);
        
        byte type = session.SESSION_sessionType;
    	//#ifdef support_groupmessage
		if (type == MSNApplication.SESSION_TYPE_GROUP_CHAT)
		{
			Vector chaters = (Vector) session.SESSION_chaters;
			StringBuffer sb = new StringBuffer(MSNApplication.mActivity.getText(R.string.text_multiple_dialog));
			for (int i = 0; i < chaters.size(); i++)
			{
				Contact roster=(Contact) chaters.elementAt(i);
				if (roster != null)
					sb.append(roster.CONTACT_nickname + ",");
			}
			if(Integer.parseInt(session.SESSION_numOfNewMsg)>0)
			{
				icon.setImageDrawable(r.getDrawable(R.drawable.groupconvhavemsg));
			} 
			else
			{
				icon.setImageDrawable(r.getDrawable(R.drawable.groupconv));
			}
			
	        addView(icon, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));       
		  	
	        EmotionTextView etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);

	        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
	            int h_1 = (int)(420 * app.screenScale);
	            int startY = (int)(25*app.screenScale);
                if(sb == null)
                {
                    etv1.setShowText("",0,startY,h_1);
                }
                else
                {
                    etv1.setShowText(sb.toString(),0,startY,h_1);
                }
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
	            int h_1 = (int)(280 * app.screenScale);
	            int startY = (int)(25*app.screenScale);
                if(sb == null)
                {
                    etv1.setShowText("",0,startY,h_1);
                }
                else
                {
                    etv1.setShowText(sb.toString(),0,startY,h_1);
                }
	        }

	        addView(etv1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));

			
		}
		// 单人对话
		else 
		//#endif		
			if (type == MSNApplication.SESSION_TYPE_CHAT)
		{
			Vector chaters = (Vector) session.SESSION_chaters;
			Contact chater=(Contact)chaters.firstElement();
	        if(session.SESSION_numOfNewMsg == null)
	        {
	        	icon.setImageDrawable(r.getDrawable(R.drawable.normal_message_status));
	        }
	        else
	        {
		        if(session.SESSION_numOfNewMsg.equals("0"))
		        {
		        	icon.setImageDrawable(r.getDrawable(R.drawable.normal_message_status));    
		        }
		        else
		        {
		        	icon.setImageDrawable(r.getDrawable(R.drawable.new_message_status)); 
		        }
	        }
	        
	        addView(icon, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));       
	  	
	        EmotionTextView etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);
	        
	        String show1=chater.CONTACT_nickname;
	        
           if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
            {
                int h_1 = (int)(420 * app.screenScale);
                int startY = (int)(25*app.screenScale);
                if(show1 == null)
                {
                    etv1.setShowText(chater.CONTACT_imid,0,startY,h_1);
                }
                else
                {
                    etv1.setShowText(show1,0,startY,h_1);
                }
            }
            else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
            {
                int h_1 = (int)(280 * app.screenScale);
                int startY = (int)(25*app.screenScale);
                if(show1 == null)
                {
                    etv1.setShowText(chater.CONTACT_imid,0,startY,h_1);
                }
                else
                {
                    etv1.setShowText(show1,0,startY,h_1);
                }
            }

	        addView(etv1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
		} 
	}
}
