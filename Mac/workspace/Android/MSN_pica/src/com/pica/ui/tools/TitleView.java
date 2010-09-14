package com.pica.ui.tools;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.pica.msn.MSNApplication;
import com.pica.msn.R;

public class TitleView extends LinearLayout{
	public ImageView icon;
	public TextView strText;
	public String titleText;
	private MSNApplication app;
	int startY = 25;
	public TitleView(Context context,String str) 
	{
		super(context);
		app = MSNApplication.getApplication();
		setBackgroundResource(R.drawable.title);
    	titleText = str;
    	startY=(int)(25*app.screenScale);
		init(str);
	}

	public void setTitleText(String text)
	{
		titleText = text;
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
		    int h_1 = (int)(370 * app.screenScale);
			if(text == null)
	        {
	        	etv1.setShowText("",0,startY,h_1);
	        }
	        else
	        {
	        	etv1.setShowText(text,0,startY,h_1);
	        }
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
            int h_1 = (int)(210 * app.screenScale);
        	if(text == null)
            {
            	etv1.setShowText("",0,startY,h_1);
            }
            else
            {
            	etv1.setShowText(text,0,startY,h_1);
            }
        }
//		msnImage.invalidate();
	}
	
	public void setIcon(int resid)
	{
		Resources r = this.getContext().getResources();
		icon.setImageDrawable(r.getDrawable(resid)); 
	}
	
	EmotionTextView etv1;
	ImageView msnImage;
	private void init(String str)
	{
		setOrientation(LinearLayout.HORIZONTAL);
		setGravity(Gravity.CENTER_VERTICAL);	
        //Resources r = this.getContext().getResources();

        Resources r = this.getContext().getResources();
        icon = new ImageView(getContext());      
        //icon.setImageDrawable(d);
        icon.setPadding(5, 0, 5, 0);
        icon.setImageDrawable(r.getDrawable(R.drawable.online));        
        addView(icon, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));       
        
        int textsize = (int)(18*app.screenScale);
        etv1 = new EmotionTextView(getContext(), null,textsize,Color.WHITE,EmotionTextView.Emotion_Yes_OneLine_Yes);

        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
            int h_1 = (int)(370 * app.screenScale);
            int frame = (int)(400*app.screenScale);
            if(str == null)
            {
            	etv1.setShowText("",0,startY,h_1);
            }
            else
            {
            	etv1.setShowText(str,0,startY,h_1);
            }
            
            addView(etv1, new LinearLayout.LayoutParams(frame,ViewGroup.LayoutParams.WRAP_CONTENT));
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
            int h_1 = (int)(210 * app.screenScale);
            if(str == null)
            {
            	etv1.setShowText("",0,startY,h_1);
            }
            else
            {
            	etv1.setShowText(str,0,startY,h_1);
            }
            int frame = (int)(240*app.screenScale);
            addView(etv1, new LinearLayout.LayoutParams(frame,ViewGroup.LayoutParams.WRAP_CONTENT));
            
            //msnImage = new ImageView(getContext());
            //msnImage.setImageResource(R.drawable.title_flag);
            //addView(msnImage, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);  
        }     
	}
}