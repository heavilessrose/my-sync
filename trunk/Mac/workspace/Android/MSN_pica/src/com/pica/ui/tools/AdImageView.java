package com.pica.ui.tools;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.os.Message;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;

public class AdImageView extends LinearLayout
{
	public int groupIndex;
	byte[] AD_ImageData;
	MSNApplication app;
	public ImageView adImage = null;
	public AdImageView(Context context)
	{
		super(context);
		app = MSNApplication.getApplication();
    	setBackgroundColor(Color.WHITE);
    	setGravity(Gravity.CENTER);
    	setOrientation(LinearLayout.VERTICAL);
    	adImage = new ImageView(getContext());
    	adImage.setEnabled(true);
    	adImage.setFocusable(true);
    	adImage.setFocusableInTouchMode(true);
		AD_ImageData = MsnUtil.getDataFromFile(MSNApplication.RMS_AD_DATA, "roster_ad.png");
    	if( (AD_ImageData != null) && (AD_ImageData.length != 0) )
    	{
    		Bitmap adBitmap = BitmapFactory.decodeByteArray(AD_ImageData, 0, AD_ImageData.length);
    		BitmapDrawable AD_Drawable = new BitmapDrawable(adBitmap);
    		adImage.setBackgroundDrawable(AD_Drawable);
        	addView(adImage, new LinearLayout.LayoutParams(adBitmap.getWidth(),adBitmap.getHeight()));   	
    	}
    			
 	}
	
	public boolean  onTouchEvent(MotionEvent event)
	{
		super.onTouchEvent(event);
		app.eventAction(EventListener.EVENT_MAIN_AD_CLICK, null);
		return true;
		
	}

}
