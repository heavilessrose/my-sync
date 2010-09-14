package com.pica.ui.tools;

import com.pica.msn.Contact;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class RosterMiniItem extends LinearLayout
{
	ImageView starImage;
	ImageView statusImage;
	EmotionTextView name;
	Context mContext;
	MSNApplication app;
	String imid;
    int textsize = 18;
    int textframe = 28;
	
	public RosterMiniItem(Context context,Contact contact, boolean isSearchRoster)
	{
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null)
    	{
    		return;
    	}
		mContext = context;
		imid = contact.CONTACT_imid;
        textsize = (int)(18*app.screenScale);
        textframe = (int)(28*app.screenScale);
		init(contact, isSearchRoster);		
	}
		
	private void init(Contact contact, boolean isSearchRoster)
	{
		setOrientation(LinearLayout.HORIZONTAL);
		setGravity(Gravity.CENTER_VERTICAL);
		setBackgroundColor(Color.rgb(255, 255, 255));
		//空间更新图标
		starImage = new ImageView(mContext);
    	starImage.setImageResource(R.drawable.space_middle);
    	addView(starImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
    	starImage.setVisibility(View.INVISIBLE);
        if(contact.CONTACT_gleam != null && contact.CONTACT_gleam.equals("1"))
        {
        	starImage.setVisibility(View.VISIBLE);
        }            	
        	
        //状态图标
        statusImage = new ImageView(mContext);
        if((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG) 
        		&& (contact.SUBITEM_STATE == app.STATE_OFFLINE))
        {
        	statusImage.setImageDrawable(app.offline_block);
        }
        else if ((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
        		&& (contact.SUBITEM_STATE != app.STATE_OFFLINE))
        {
        	statusImage.setImageDrawable(app.online_block);
        }
        else
        {
        	 if (contact.SUBITEM_STATE == app.STATE_ONLINE)
             {
             	statusImage.setImageDrawable(app.online);
             }
             else if (contact.SUBITEM_STATE == app.STATE_BUSY)
             {
             	statusImage.setImageDrawable(app.busy);
             }
             else if (contact.SUBITEM_STATE == app.STATE_AWAY)
             {
             		statusImage.setImageDrawable(app.away);
             }
             else if (contact.SUBITEM_STATE == app.STATE_BeRightBack)
             {
             	statusImage.setImageDrawable(app.away);
             }
             else if (contact.SUBITEM_STATE == app.STATE_InACall)
             {
             	statusImage.setImageDrawable(app.busy);
             }
             else if (contact.SUBITEM_STATE == app.STATE_OutToLunch)
             {
             	statusImage.setImageDrawable(app.away);
             }
             else if (contact.SUBITEM_STATE == app.STATE_OFFLINE)
             {
             	statusImage.setImageDrawable(app.offline);
             }
             else
             {
             	statusImage.setImageDrawable(app.offline);
             }
        }
        
        addView(statusImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
                
        if(isSearchRoster)
        {
        	//个人昵称
        	name = new EmotionTextView(getContext(), null,textsize,Color.WHITE,EmotionTextView.Emotion_Yes_OneLine_Yes);
        	if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
                int h_1 = (int)(390 * app.screenScale);
                int h_2 = (int)(420 * app.screenScale);
                int startY = (int)(20*app.screenScale);
	        	if(contact.CONTACT_nickname != null)
	            {
	            	name.setShowText(contact.CONTACT_nickname, 0, startY, h_1);
	            }
	            else
	            {
	            	name.setShowText(contact.CONTACT_imid, 0, startY, h_1);
	            }
		        addView(name, new LinearLayout.LayoutParams(h_2,textframe));
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
                int h_1 = (int)(240 * app.screenScale);
                int h_2 = (int)(270 * app.screenScale);
                int startY = (int)(20*app.screenScale);
	        	if(contact.CONTACT_nickname != null)
	            {
	            	name.setShowText(contact.CONTACT_nickname, 0, startY, h_1);
	            }
	            else
	            {
	            	name.setShowText(contact.CONTACT_imid, 0, startY, h_1);
	            }
		        addView(name, new LinearLayout.LayoutParams(h_2,textframe));
	        }
        }
        else
        {
        	//个人昵称
        	name = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);
        	if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
                int h_1 = (int)(410 * app.screenScale);
                int h_2 = (int)(440 * app.screenScale);
                int startY = (int)(20*app.screenScale);
	        	if(contact.CONTACT_nickname != null)
	            {
	            	name.setShowText(contact.CONTACT_nickname, 0, startY, h_1);
	            }
	            else
	            {
	            	name.setShowText(contact.CONTACT_imid, 0, startY, h_1);
	            }
		        addView(name, new LinearLayout.LayoutParams(h_2,textframe));
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
                int h_1 = (int)(260 * app.screenScale);
                int h_2 = (int)(290 * app.screenScale);
                int startY = (int)(20*app.screenScale);
	        	if(contact.CONTACT_nickname != null)
	            {
	            	name.setShowText(contact.CONTACT_nickname, 0, startY, h_1);
	            }
	            else
	            {
	            	name.setShowText(contact.CONTACT_imid, 0, startY, h_1);
	            }
		        addView(name, new LinearLayout.LayoutParams(h_2,textframe));
	        }
        }
	}
}
