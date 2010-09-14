package com.pica.ui.tools;

import java.util.Timer;
import java.util.Vector;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.pica.msn.Contact;
import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;
import com.pica.msn.UiEventListener;

public class RosterItem extends LinearLayout implements AnimationListener{
	static MSNApplication app;
	public ImageView logo;
	private ImageView icon;
	public ImageView line;
	//private TextView nickname;
	
	LinearLayout linearH;
	LinearLayout linearE;
	//private Item contact;
	/**是否是个人项*/
	private boolean isMyItem;
	private boolean isExpand;
	
	String imid;
	int groupIndex;
	LinearLayout infoLayout;
	ImageView starImage;
	ImageView statusImage;
	EmotionTextView name;
	int textsize = 18;
	int textsizeS = 15;
	int textframe = 28;
	public RosterItem(Context context, Contact contact,int index, boolean isMyItem, boolean bExpand) {
		super(context);
		// TODO Auto-generated constructor stub		
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	this.isMyItem=isMyItem;
    	this.isExpand=bExpand;
    	
    	imid = contact.CONTACT_imid;
    	groupIndex = index;
        textsize = (int)(18*app.screenScale);
        textsizeS = (int)(15*app.screenScale);
        textframe = (int)(28*app.screenScale);
		init(contact);
	}
	
	
	EmotionTextView etv1=null;
	public EmotionTextView etv2=null;
	LinearLayout linearV=null;
	String show1=null;
	String show2=null;
	
    private OnClickListener mLogoListener = new OnClickListener() {
        public void onClick(View v) {
            app.eventNotify(EventListener.EVENT_MY_ITEM_LOGO, null);
        }
    };
    
    private OnClickListener linearFListener = new OnClickListener() {
        public void onClick(View v) {
            app.eventNotify(EventListener.EVENT_MY_ITEM_INFO, null);
        }
    };
	private void init(Contact contact)
	{
		if(isMyItem || isExpand)
		{
			setOrientation(LinearLayout.VERTICAL);
			setBackgroundColor(Color.rgb(185, 235, 246));
	        LinearLayout linearF = new LinearLayout(getContext());
	        linearF.setOrientation(LinearLayout.HORIZONTAL);
	        linearF.setGravity(Gravity.CENTER_VERTICAL);
	        
			Resources r = this.getContext().getResources();
			RelativeLayout relate = new RelativeLayout(getContext());
	        logo = new ImageView(getContext());  
	        if(isMyItem)
	        {
	            logo.setEnabled(true);
	            logo.setOnClickListener(mLogoListener);
	            linearF.setEnabled(true);
	            linearF.setOnClickListener(linearFListener);
	        }
	        
	        Bitmap contactHead=null;
	        BitmapDrawable contactHeadDrawable=null;
	        if(isMyItem)
	        {
	        	if(contact.CONTACT_imageData!=null)
	            {
	            	contactHead=BitmapFactory.decodeByteArray(contact.CONTACT_imageData, 0, contact.CONTACT_imageData.length);
	            	contactHeadDrawable = new BitmapDrawable(contactHead);
	            }
	            else
	            	contactHeadDrawable = (BitmapDrawable)app.logo;
	        }
	        else
	        {
	            if(contact.CONTACT_contactFlag!=MSNApplication.CONTACT_BLOCK_FLAG&&contact.SUBITEM_STATE!=MSNApplication.STATE_OFFLINE&&contact.CONTACT_hasRMSImageData&&contact.CONTACT_imageData!=null)        
	            {
	            	contactHead=BitmapFactory.decodeByteArray(contact.CONTACT_imageData, 0, contact.CONTACT_imageData.length);
	            	contactHeadDrawable = new BitmapDrawable(contactHead);
	            }
	            else
	            	contactHeadDrawable = (BitmapDrawable)app.logo;
	        }
	        logo.setImageDrawable(contactHeadDrawable);
	        logo.setPadding(3, 4, 3, 4);
			
			ImageView frame= new ImageView(getContext());
	        if(this.isMyItem)
	        {
	        	frame.setImageResource(R.drawable.my_frame);
	        }
	        else
	        {
	        	if(isExpand)
	        	{
	        		logo.setVisibility(View.VISIBLE);
	        	}
	        	else
	        	{
	        		logo.setVisibility(View.GONE);
	        	}
	        }
	        
	        int size = (int)(60*app.screenScale);
	        relate.addView(frame, new RelativeLayout.LayoutParams(size,size));
	        relate.addView(logo, new RelativeLayout.LayoutParams(size,size));

	        linearF.addView(relate, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        
	        /**在线状态，    个人昵称*/
	        linearH = new LinearLayout(getContext());
	        linearH.setOrientation(LinearLayout.HORIZONTAL);
            if(contact.CONTACT_gleam != null && contact.CONTACT_gleam.equals("1") && !(this.isMyItem))
            {
                ImageView starImage = new ImageView(getContext());
                starImage.setImageResource(R.drawable.space);
                linearH.addView(starImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
            } 
	        
	        icon = new ImageView(getContext()); 
	        icon.setPadding(5, 5, 5, 5);
	        
	        if((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG) 
	        		&& (contact.SUBITEM_STATE == app.STATE_OFFLINE))
	        {
	        	icon.setImageDrawable(app.offline_block);
	        }
	        else if ((contact.CONTACT_contactFlag == app.CONTACT_BLOCK_FLAG)
	        		&& (contact.SUBITEM_STATE != app.STATE_OFFLINE))
	        {
	        	icon.setImageDrawable(app.online_block);
	        }
	        else
	        {
	        	if (contact.SUBITEM_STATE == app.STATE_ONLINE)
	        	{

	        		icon.setImageDrawable(app.online);
	        	}
	        	else if (contact.SUBITEM_STATE == app.STATE_BUSY)
	        	{
	        		icon.setImageDrawable(app.busy);
	        	}
	        	else if (contact.SUBITEM_STATE == app.STATE_AWAY)
	        	{
	        		icon.setImageDrawable(app.away);
	        	}
//	        	else if (contact.SUBITEM_STATE == app.STATE_BeRightBack)
//	        	{
//	        		icon.setImageDrawable(app.away);
//	        	}
//	        	else if (contact.SUBITEM_STATE == app.STATE_InACall)
//	        	{
//	        		icon.setImageDrawable(app.busy);
//	        	}
//	        	else if (contact.SUBITEM_STATE == app.STATE_OutToLunch)
//	        	{
//	        		icon.setImageDrawable(app.away);
//	        	}
	        	else if (contact.SUBITEM_STATE == app.STATE_OFFLINE)
	        	{
	        		icon.setImageDrawable(app.offline);
	        	}
	        	else
	        	{
	        		icon.setImageDrawable(app.offline);
	        	}
	        }
	        
	        linearH.addView(icon, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        
	        int color = Color.BLACK;
	        if(this.isMyItem)
	        {
	        	color = Color.WHITE;
	        }
	        else
	        {
	        	color = Color.BLACK;
	        }
	        etv1 = new EmotionTextView(getContext(), null,textsize,color,EmotionTextView.Emotion_Yes_OneLine_Yes);
	        show1=contact.CONTACT_nickname;
	        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
	            int h_1 = (int)(350 * app.screenScale);
	            int h_2 = (int)(380 * app.screenScale);
	            int startY = (int)(20*app.screenScale);
		        etv1.setShowText(show1,0,startY,h_1);//370
		        linearH.addView(etv1, new LinearLayout.LayoutParams(h_2,textframe));//400
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
	            int h_1 = (int)(200 * app.screenScale);
	            int h_2 = (int)(230 * app.screenScale);
	            int startY = (int)(20*app.screenScale);
		        etv1.setShowText(show1,0,startY,h_1);//220
		        linearH.addView(etv1, new LinearLayout.LayoutParams(h_2,textframe));//210
	        }

	        
	        /**在线状态，    个人昵称*/
	        /**空间更新图标，心情短语*/
	        linearV = new LinearLayout(getContext());
	        linearV.setOrientation(LinearLayout.VERTICAL);
	        linearV.addView(linearH, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        
	        /**空间更新图标，心情短语*/
	        linearE = new LinearLayout(getContext());
	        linearE.setOrientation(LinearLayout.HORIZONTAL);
	        
//	        if(contact.CONTACT_gleam != null && contact.CONTACT_gleam.equals("1") && !(this.isMyItem))
//	        {
//	        	ImageView starImage = new ImageView(getContext());
//	        	starImage.setImageResource(R.drawable.space);
//	        	linearE.addView(starImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
//	        }        
	        
	        //EmotionTextView 
	        if(this.isMyItem)
	        {
	        	color = Color.WHITE;
	        }
	        else
	        {
	        	color = Color.GRAY;
	        }
	        etv2 = new EmotionTextView(getContext(), null,textsizeS,color,EmotionTextView.Emotion_Yes_OneLine_Yes);
	        paint=etv2.getCurrentPaint();
	        //show2=contact.CONTACT_impresa;
	        //if(show2==null)
	        //	show2="";
	        if(contact.CONTACT_imid!=null&&contact.CONTACT_imid.equals(app.GetLiveID()))
	        {
	        	if(contact.CONTACT_impresa==null||contact.CONTACT_impresa.length()==0)
	        	{
	        		show2=app.myVCARD_myImpresa;
	        		if(show2==null||show2.length()==0)
	        			show2="";
	        	}
	        	else
	        		show2=contact.CONTACT_impresa;
	        }
	        else
	        	show2=contact.CONTACT_impresa==null?"":contact.CONTACT_impresa;
	        
	        if(this.isMyItem)
	        {
	        	etv2.setVisibility(View.VISIBLE);
	        }
	        else
	        {
	        	if(isExpand)
	        	{
	        		etv2.setVisibility(View.VISIBLE);
	        	}
	        	else
	        	{
	        		etv2.setVisibility(View.GONE);
	        	}
	        }
	        
	        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
                int h_1 = (int)(370 * app.screenScale);
                int h_2 = (int)(400 * app.screenScale);
                int startY = (int)(20*app.screenScale);
                etv2.setShowText(show2,offX,startY,h_1);//390
                linearE.addView(etv2, new LinearLayout.LayoutParams(h_2,textframe));//420
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
                int h_1 = (int)(220 * app.screenScale);
                int h_2 = (int)(250 * app.screenScale);
                int startY = (int)(20*app.screenScale);
	            etv2.setShowText(show2,offX,startY,h_1);//240
	            linearE.addView(etv2, new LinearLayout.LayoutParams(h_2,textframe));//250
	        }
	        
	        linearV.addView(linearE, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        linearF.addView(linearV, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));     
	        addView(linearF, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));  
	        
//	        LinearLayout linearL = new LinearLayout(getContext());
//	        linearL.setOrientation(LinearLayout.VERTICAL);
//	        setGravity(Gravity.BOTTOM);
//	        line = new ImageView(getContext()); 
//	        if(this.isMyItem)
//	        {
//	    		line.setBackgroundResource(R.drawable.my_list_line);
//	        }
//	        else
//	        {
//	        	//line.setImageDrawable(r.getDrawable(R.drawable.list_line));//modify by liubing 20091123
//	        }
//	        linearL.addView(line, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
//	        addView(linearL, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
		}
		else//isMini
		{
			setOrientation(LinearLayout.VERTICAL);
			setBackgroundColor(Color.WHITE);
			infoLayout = new LinearLayout(getContext());
			infoLayout.setOrientation(LinearLayout.HORIZONTAL);
			
			infoLayout.setGravity(Gravity.CENTER_VERTICAL);		
			//空间更新图标
			starImage = new ImageView(getContext());
			starImage.setPadding(10, 12, 5, 5);
	    	starImage.setImageResource(R.drawable.space_middle);
	    	infoLayout.addView(starImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	    	starImage.setVisibility(View.INVISIBLE);
	        if(contact.CONTACT_gleam != null && contact.CONTACT_gleam.equals("1"))
	        {
	        	starImage.setVisibility(View.VISIBLE);
	        }            	
	        	
	        //状态图标
	        statusImage = new ImageView(getContext());
	        statusImage.setPadding(5, 10, 5, 5);
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
	        
	        infoLayout.addView(statusImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	                
	      //个人昵称
        	name = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes);
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
	        	infoLayout.addView(name, new LinearLayout.LayoutParams(h_2,textframe));
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
	        	infoLayout.addView(name, new LinearLayout.LayoutParams(h_2,textframe));
	        }
	        
	        addView(infoLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        //modify by liubing 20091123
	        //ImageView lineView = new ImageView(getContext());
	        //lineView.setImageDrawable(getResources().getDrawable(R.drawable.list_line));
	        //setGravity(Gravity.BOTTOM);	        
	        //addView(lineView, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	        //end
		}
		
	}
	
	
//	public void resetItem()
//	{
//		//Resources r = this.getContext().getResources();
//		/////////////
//        Bitmap contactHead=null;
//        BitmapDrawable contactHeadDrawable=null;
//        
//        ////////
//
//        if(app.myVCARD_headImageData!=null)
//        {
//            contactHead=BitmapFactory.decodeByteArray(app.myVCARD_headImageData, 0, app.myVCARD_headImageData.length);
//            contactHeadDrawable = new BitmapDrawable(contactHead);
//        }
//        else
//           contactHeadDrawable = (BitmapDrawable)app.logo;
//
//
//        logo.setImageDrawable(contactHeadDrawable);
//        ////////////////
//
//        	if (app.getMyState()== MSNApplication.STATE_ONLINE)
//        	{
//        		icon.setImageDrawable(app.online);
//        	}
//        	else if (app.getMyState() == MSNApplication.STATE_BUSY)
//        	{
//        		icon.setImageDrawable(app.busy);
//        	}
//        	else if (app.getMyState() == MSNApplication.STATE_AWAY)
//        	{
//        		icon.setImageDrawable(app.away);
//        	}
//        	else if (app.getMyState()== MSNApplication.STATE_BeRightBack)
//        	{
//        		icon.setImageDrawable(app.away);
//        	}
//        	else if (app.getMyState() == MSNApplication.STATE_InACall)
//        	{
//        		icon.setImageDrawable(app.busy);
//        	}
//        	else if (app.getMyState() == MSNApplication.STATE_OutToLunch)
//        	{
//        		icon.setImageDrawable(app.away);
//        	}
//        	else if (app.getMyState() == MSNApplication.STATE_OFFLINE)
//        	{
//        		icon.setImageDrawable(app.offline);
//        	}
//        	else
//        	{
//        		icon.setImageDrawable(app.offline);
//        	}
////        }
//        
//       String nickname= app.myVCARD_myNickname;
//       if(nickname==null||nickname.length()==0)
//    	   nickname= app.GetLiveID();
//
//       if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
//       {
//           if(app.screenWidth >= 800)
//           {
//               linearH.removeView(etv1);
//               etv1.setShowText(nickname,0,20,650);//370
//               linearH.addView(etv1, new LinearLayout.LayoutParams(670,28));//400
//           }
//           else if(app.screenWidth <= 320)
//           {
//               linearH.removeView(etv1);
//               etv1.setShowText(nickname,0,20,190);//370
//               linearH.addView(etv1, new LinearLayout.LayoutParams(220,28));//400
//           }
//           else
//           {
//               linearH.removeView(etv1);
//               etv1.setShowText(nickname,0,20,350);//370
//               linearH.addView(etv1, new LinearLayout.LayoutParams(380,28));//400
//           }
//       }
//       else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
//       {
//          if(app.screenWidth >= 800)
//           {
//              linearH.removeView(etv1);
//              etv1.setShowText(nickname,0,20,350);//220
//              linearH.addView(etv1, new LinearLayout.LayoutParams(380,28));//250
//           }
//          else if(app.screenWidth <= 320)
//          {
//             linearH.removeView(etv1);
//             etv1.setShowText(nickname,0,20,120);//220
//             linearH.addView(etv1, new LinearLayout.LayoutParams(140,28));//250
//          }
//           else
//           {
//               linearH.removeView(etv1);
//               etv1.setShowText(nickname,0,20,200);//220
//               linearH.addView(etv1, new LinearLayout.LayoutParams(230,28));//250
//           }
//       }       
//        
//        String show2=app.myVCARD_myImpresa;
//        if(show2==null)
//        	show2="";
//
//        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
//        {
//           if(app.screenWidth >= 800)
//            {
//               linearE.removeView(etv2);
//               etv2.setShowText(show2,offX,20,670);//390
//               //linearE.addView(etv2, new LinearLayout.LayoutParams(700,28));//400
//            }
//           else if(app.screenWidth <= 320)
//           {
//               linearE.removeView(etv2);
//               etv2.setShowText(show2,offX,20,210);//390
//               linearE.addView(etv2, new LinearLayout.LayoutParams(240,28));//400
//           }
//            else
//            {
//                linearE.removeView(etv2);
//                etv2.setShowText(show2,offX,20,370);//390
//                linearE.addView(etv2, new LinearLayout.LayoutParams(400,28));//400
//            }
//        }
//        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
//        {
//           if(app.screenWidth >= 800)
//            {
//               linearE.removeView(etv2);
//               etv2.setShowText(show2,offX,20,370);//240
//               linearE.addView(etv2, new LinearLayout.LayoutParams(400,28));//270
//            }
//           else if(app.screenWidth <= 320)
//           {
//               linearE.removeView(etv2);
//               etv2.setShowText(show2,offX,20,140);//390
//               linearE.addView(etv2, new LinearLayout.LayoutParams(160,28));//400
//           }
//            else
//            {
//                linearE.removeView(etv2);
//                etv2.setShowText(show2,offX,20,220);//240
//                linearE.addView(etv2, new LinearLayout.LayoutParams(250,28));//270
//            }
//        }        
//	}
	
	public void TimerNotify(Object tag) 
	{
		try
		{
			if (tag.equals("impresaTimer"))
			{
				boolean isChangeHead=false;
				if (contactTimer != null)
				{
					offX -= 2;
					if (offX < -(originalWidth - drawWidth+10))// 只滚动一次 10个像素是多滚一会
					{
						offX = 0;
						resetRosterItem();
						if(contactTimer!=null)
						{
							//Log.v("contactTimer.cancel()1", "1");
							contactTimer.cancel();
							contactTimer = null;
						}
					}
					
					//#ifdef support_head_image
					
					// 支持联系人头像 并且不是脱机联系人 不是阻止联系人
					if(app.isReceiveHeadChange
							&& currentContact.SUBITEM_STATE != MSNApplication.STATE_OFFLINE
							&&currentContact.CONTACT_contactFlag !=MSNApplication.CONTACT_BLOCK_FLAG)
					{
						 //#ifdef support_save_rms
						// 判断 如果rms里面存有头像 则取出来交给内存让其显示，不再发包取头像。
						if (currentContact.CONTACT_hasRMSImageData)
						{
							if(currentContact.CONTACT_imageData==null)
							{
								//Item cfrms = Shell.model.getContactFromRMS(currentContact.SUBITEM_JID);
								//Item cfrms=Shell.model.getContactHeadFromRMS(currentContact,Shell.model.allUsername);
								currentContact.CONTACT_imageData = app.getContactHeadFromRMS(currentContact.CONTACT_imid);
								isChangeHead=true;
								
							}
							if(currentContact.CONTACT_imageData==null)
							{
								if(!currentContact.CONTACT_hasSendGetImage)
								{
									app.jabber.getContactPortrait(currentContact.SUBITEM_JID);
									currentContact.CONTACT_hasSendGetImage = true;
								}
							}
						} 
						else // rms里面没有保存头像 则发包获取头像
						//#endif
						{
							if(!currentContact.CONTACT_hasSendGetImage)
							{
								app.jabber.getContactPortrait(currentContact.SUBITEM_JID);
								currentContact.CONTACT_hasSendGetImage = true;
							}
						}
					} 
					else
						currentContact.CONTACT_imageData = null;
					//#endif
					
					
					if (originalWidth <= drawWidth)// 仅仅发包取头像作用
					{
						offX = 0;
						if(contactTimer!=null)
						{
							contactTimer.cancel();
							contactTimer = null;
						}
					}
				}
				else
				{
					offX=0;
				}
				etv2.item[EmotionTextView.ITEM_STARTX]=new Integer(offX);
				app.eventNotify(EventListener.EVENT_UPDATE_INPRESEA_UI, etv2);	
				if(isChangeHead)
				{
					app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
				}
			}
			else if(tag.equals("headTimer"))
			{
				//Log.v("headTimer","headTimer");
				boolean isChangeHead=false;
				//#ifdef support_head_image
				
				// 支持联系人头像 并且不是脱机联系人 不是阻止联系人
				if(app.isReceiveHeadChange
						&& currentHeadContact.SUBITEM_STATE != MSNApplication.STATE_OFFLINE
						&&currentHeadContact.CONTACT_contactFlag !=MSNApplication.CONTACT_BLOCK_FLAG)
				{
					 //#ifdef support_save_rms
					// 判断 如果rms里面存有头像 则取出来交给内存让其显示，不再发包取头像。
					if (currentHeadContact.CONTACT_hasRMSImageData)
					{
						if(currentHeadContact.CONTACT_imageData==null)
						{
							//Item cfrms = Shell.model.getContactFromRMS(currentContact.SUBITEM_JID);
							//Item cfrms=Shell.model.getContactHeadFromRMS(currentContact,Shell.model.allUsername);
							currentHeadContact.CONTACT_imageData = app.getContactHeadFromRMS(currentHeadContact.CONTACT_imid);
							isChangeHead=true;							
						}
						if(!currentHeadContact.CONTACT_hasSendGetImage)
						{
							app.jabber.getContactPortrait(currentHeadContact.SUBITEM_JID);
							currentHeadContact.CONTACT_hasSendGetImage = true;
						}
					} 
					else // rms里面没有保存头像 则发包获取头像
					//#endif
					{
						if(!currentHeadContact.CONTACT_hasSendGetImage)
						{
						app.jabber.getContactPortrait(currentHeadContact.SUBITEM_JID);
						currentHeadContact.CONTACT_hasSendGetImage = true;
					}
				} 
				} 
				else
					currentHeadContact.CONTACT_imageData = null;
				//#endif
				if(isChangeHead)
				{
					app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
				}
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}	
	
	public static Timer contactTimer;
	public static Contact currentContact; // 联系人数据引用
	public static RosterItem currentRosterItem;
	
    ////心情短语滚动条
    static int drawWidth;
    int originalWidth;
	public int offX;
	public int width;
	int viewWidth_PORTRAIT=240;
	int viewWidth_LANDSPACE=390;
	int startXNickname=0;
	//static Rect rect=new Rect();
	static int omitWidth=0;
	public static void resetRosterItem()
	{
		try {
		//Log.v("resetRosterItem", "resetRosterItem1");
		currentRosterItem.etv2.item[EmotionTextView.ITEM_STARTX]=new Integer(0);
		//Log.v("resetRosterItem", "resetRosterItem2");
		//currentRosterItem.etv2.item[EmotionTextView.ITEM_CONTENT]=MsnUtil.getNewVisibleStringAt(drawWidth-rect.width(),paint, currentRosterItem.text);
		currentRosterItem.etv2.item[EmotionTextView.ITEM_CONTENT]=MsnUtil.getNewVisibleStringAt(drawWidth-omitWidth,paint, currentRosterItem.text);
		//Log.v("currentRosterItem.text", "1"+currentRosterItem.text);
		//Log.v("resetRosterItem", "resetRosterItem3");
		app.eventNotify(EventListener.EVENT_UPDATE_INPRESEA_UI, currentRosterItem.etv2);	
		//Log.v("resetRosterItem", "resetRosterItem4");
		currentRosterItem=null;
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public static  void hideVcard()
	{
		currentContact=null;
		if (contactTimer != null)
		{
			
			contactTimer.cancel();
			contactTimer = null;
		}
		if(currentRosterItem!=null)
			resetRosterItem();
		//offX=0;
	}
	static Paint paint=null;
	String text="";
	public void showVcard(Contact selectContact, int millisecond,RosterItem rosterItem)
	{
		currentContact = null;
		currentContact = selectContact;

		currentRosterItem=null;
		currentRosterItem=rosterItem;
		
		if (currentContact != null)
		{
			if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	        {
				drawWidth=(viewWidth_LANDSPACE)-(startXNickname);
	        }
	        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	        {
	        	drawWidth=(viewWidth_PORTRAIT)-(startXNickname);
	        } 
			//心情短语的长度 有表情 所以需要特别计算
			//5000像素宽
			
	        if(selectContact.CONTACT_imid.equals(app.GetLiveID()))
	        {
	        	if(selectContact.CONTACT_impresa==null||selectContact.CONTACT_impresa.length()==0)
	        	{
	        		text=app.myVCARD_myImpresa;
	        		if(text==null||text.length()==0)
	        			text="";
	        	}
	        	else
	        		text=selectContact.CONTACT_impresa;
	        }
	        else
	        	text=selectContact.CONTACT_impresa==null?"":selectContact.CONTACT_impresa;
			Object[] ob=MsnUtil.splitNewEmotionText(text, 5000, paint,false);
			originalWidth=((Integer)ob[1]).intValue();

			if (contactTimer != null)
			{
				contactTimer.cancel();
				contactTimer = null;
			}
			
			
			if (originalWidth <= drawWidth)// 仅仅发包取头像作用
			{
				offX = 0;
				etv2.item[EmotionTextView.ITEM_STARTX]=new Integer(offX);
			}
			else
			{
				offX=0;
				String omit="...";
				omitWidth=(int)paint.measureText(omit, 0, omit.length());
				etv2.item[EmotionTextView.ITEM_CONTENT]=((Vector)ob[0]).elementAt(0);
			}
			contactTimer = new Timer();
			contactTimer.schedule(new Animation(this, "impresaTimer"), millisecond,200);

		}
	}
	
	public static Timer headTimer;
	public static Contact currentHeadContact; // 联系人数据引用

	
	public void getHead(Contact selectContact)
	{
		//Log.v("getHead","getHead");
		currentHeadContact=null;
		currentHeadContact=selectContact;
		headTimer = new Timer();
		headTimer.schedule(new Animation(this, "headTimer"), 3000);
	}
	
	public static void hideHead()
	{
		//Log.v("hideHead","hideHead");
		currentHeadContact=null;
		if (headTimer != null)
		{
			headTimer.cancel();
			headTimer = null;
		}
	}	
}