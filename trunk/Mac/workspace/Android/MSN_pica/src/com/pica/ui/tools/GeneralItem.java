package com.pica.ui.tools;



import java.util.Vector;

import com.pica.msn.Item;
import com.pica.msn.MSNApplication;

import android.R;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;

import android.graphics.Color;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
public class GeneralItem extends LinearLayout{
	private ImageView icon;
	static MSNApplication app;
	public GeneralItem(Context context,String nickname)
	{
		super(context);
		app = MSNApplication.getApplication();
		init(nickname);
	}
	
	private void init(String str)
	{
		setOrientation(LinearLayout.HORIZONTAL);
		setGravity(Gravity.CENTER_VERTICAL);	
		int textsize = (int)(18*app.screenScale);
        EmotionTextView etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);
        
        if(str == null)
        {
        	etv1.setShowText("",0,25,280);
        }
        else
        {
        	etv1.setShowText(str,0,25,280);
        }
        
    	if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
        {
            int h_1 = (int)(25 * app.screenScale);
            int h_2 = (int)(360 * app.screenScale);
            if(str == null)
            {
            	etv1.setShowText("",0,h_1,h_2);
            }
            else
            {
            	etv1.setShowText(str,0,h_1,h_2);
            }
        }
        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
        {
            int h_1 = (int)(25 * app.screenScale);
            int h_2 = (int)(260 * app.screenScale);
            if(str == null)
            {
            	etv1.setShowText("",0,h_1,h_2);
            }
            else
            {
            	etv1.setShowText(str,0,h_1,h_2);
            }
        }
        
        addView(etv1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	}
}