package com.pica.ui.tools;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class DetailInfoView extends LinearLayout{

	MSNApplication app;
	public TextView email;
	public TextView status;
	public Button button;
	public EmotionTextView etv1;
	public EmotionTextView etv2;
	private Contact currentItem;
	private TextView username;
	int textsize = 18;
	public DetailInfoView(Context context,Contact mCurItem) 
	{
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	textsize = (int)(18*app.screenScale);
    	this.currentItem=mCurItem;
		setOrientation(LinearLayout.VERTICAL);
		setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));		
		init();
	}
	
	private void init()
	{
		//ÕËºÅ£º¡£¡£¡£
		LinearLayout emailLayout = new LinearLayout(getContext());
		emailLayout.setOrientation(LinearLayout.HORIZONTAL);
		addView(emailLayout);
		LayoutParams params = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
		email = new TextView(getContext());
        email.setTextColor(Color.WHITE);
        email.setPadding(3, 0, 0, 0);
        email.setText(getContext().getText(R.string.account_id));  
        email.setTextSize(18);
        username = new TextView(getContext());
        username.setTextColor(Color.WHITE);
        username.setPadding(5, 0, 0, 0);        
        username.setText(currentItem.CONTACT_imid);
        username.setTextSize(18);
        emailLayout.addView(email);
        emailLayout.addView(username, params);
        
        //×´Ì¬:¡£¡£¡£
        LinearLayout statusLayout = new LinearLayout(getContext());
        statusLayout.setOrientation(LinearLayout.HORIZONTAL);
        statusLayout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.FILL_PARENT));
        status = new TextView(getContext());
        status.setLayoutParams(params);
        status.setTextColor(Color.WHITE);
        status.setTextSize(18);
        status.setPadding(5, 0, 0, 0);
        String contact_status= getContext().getText(R.string.contact_status) + "";
        status.setText(contact_status);
        
        ImageView statusImage = new ImageView(getContext());
        
    	if (currentItem.SUBITEM_STATE == app.STATE_ONLINE)
    	{
        	statusImage.setImageDrawable(app.online);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_BUSY)
    	{
    		statusImage.setImageDrawable(app.busy);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_AWAY)
    	{
    		statusImage.setImageDrawable(app.away);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_BeRightBack)
    	{
    		statusImage.setImageDrawable(app.away);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_InACall)
    	{
    		statusImage.setImageDrawable(app.busy);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_OutToLunch)
    	{
    		statusImage.setImageDrawable(app.away);
    	}
    	else if (currentItem.SUBITEM_STATE == app.STATE_OFFLINE)
    	{
    		statusImage.setImageDrawable(app.offline);
    	}
    	else
    	{
    		statusImage.setImageDrawable(app.offline);
    	}        
        
        ImageView spaceImage = new ImageView(getContext());
        if(currentItem.CONTACT_gleam != null && currentItem.CONTACT_gleam.equals("1"))
        {
            spaceImage.setImageResource(R.drawable.space);
        }
        	
        statusLayout.addView(status);        
        statusLayout.addView(statusImage);
        statusLayout.addView(spaceImage);
        addView(statusLayout);
    
        LinearLayout.LayoutParams paramS = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
        		ViewGroup.LayoutParams.FILL_PARENT);      
        ScrollView scroll = new ScrollView(getContext());
        ViewGroup.LayoutParams paramSvLp = new ScrollView.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);  
         scroll.setLayoutParams(paramS);
        addView(scroll);

        final LinearLayout layout = new LinearLayout(getContext());
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));
        
        layout.setLayoutParams(paramSvLp);
        scroll.addView(layout);
        
        etv1 = new EmotionTextView(getContext(), null,textsize,Color.WHITE,EmotionTextView.Emotion_Yes_OneLine_No);
        String show1=getContext().getText(R.string.nickname)+(currentItem.CONTACT_nickname==null?currentItem.CONTACT_imid:currentItem.CONTACT_nickname);
        layout.addView(etv1);
        
        int height1 = 280;
        int startX = (int)(5 * app.screenScale);
        int startY = (int)(20*app.screenScale);
        int width = (int)(280*app.screenScale);
        
        height1=etv1.setShowText(show1,startX,startY,width); 
        LayoutParams params1 = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,height1 + 4);
        etv1.setLayoutParams(params1);
        
        etv2 = new EmotionTextView(getContext(), null,textsize,Color.WHITE,EmotionTextView.Emotion_Yes_OneLine_No);
        String impresa_text="";
        if(currentItem.CONTACT_imid.equals(app.GetLiveID()))
        {
        	if(currentItem.CONTACT_impresa==null||currentItem.CONTACT_impresa.length()==0)
        	{
        		impresa_text=app.myVCARD_myImpresa;
        		if(impresa_text==null||impresa_text.length()==0)
        			impresa_text="";
        	}
        	else
        		impresa_text=currentItem.CONTACT_impresa;
        }
        else
        	impresa_text=currentItem.CONTACT_impresa==null?"":currentItem.CONTACT_impresa;
        String show2=getContext().getText(R.string.roster_info)+impresa_text;
        
        layout.addView(etv2);
        int height2 = 280;
        int startX2 = (int)(5 * app.screenScale);
        int startY2 = (int)(20*app.screenScale);
        int width2 = (int)(280*app.screenScale);
        height2=etv2.setShowText(show2,startX2,startY2,width2);
        LayoutParams params2 = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,height2 + 4);
        etv2.setLayoutParams(params2);
	}
}
