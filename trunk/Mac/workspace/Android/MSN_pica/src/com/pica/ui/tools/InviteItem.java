package com.pica.ui.tools;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;
import com.pica.msn.UiEventListener;

public class InviteItem extends LinearLayout{
	static MSNApplication app;
	public CheckBox box;
	public Contact mItem;
	public InviteItem(Context context, Contact item) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
		mItem = item;
		// TODO Auto-generated constructor stub	
		init(mItem.CONTACT_nickname);
	}

	private void init(String str)
	{
		setOrientation(LinearLayout.HORIZONTAL);
		setGravity(Gravity.CENTER_VERTICAL);	

        Resources r = this.getContext().getResources();
        box = new CheckBox(getContext());      
        box.setPadding(5, 0, 5, 0);  
        box.setOnClickListener(mBoxListener);
        
        String str1;
        for(int i = 0; i < app.jids.size(); i++)
        {
        	str1 = (String)app.jids.elementAt(i);
        	if(str1.equals(mItem.SUBITEM_JID))
        	{
        		box.setChecked(true);
        	}
        }
        addView(box, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));       
     
        EmotionTextView etv1 = new EmotionTextView(getContext(), null,18,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);
        if(str == null)
        {
        	etv1.setShowText("",0,25,260);
        }
        else
        {
        	etv1.setShowText(str,0,25,260);
        }
        addView(etv1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	}
	
    private OnClickListener mBoxListener = new OnClickListener() {
        public void onClick(View v) {
        	if(box.isChecked())
        	{
				app.jids.addElement(mItem.SUBITEM_JID);                			
        	}
        	else
        	{
        		app.jids.removeElement(mItem.SUBITEM_JID);
        	}
        }
    };
}