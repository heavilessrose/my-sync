package com.pica.ui.tools;


import java.util.Vector;

import com.pica.msn.Item;
import com.pica.msn.MD5;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;


import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;


public class HistoryTextView extends LinearLayout{ 

	  private LinearLayout midLayout, botLayout; 
	  private TextView midText,botText; 
	  private Activity activity; 
	  private EmotionTextView topText;
	  private FrameLayout topLayout;
	  private MSNApplication app; 
	  int startX = 5;
	  int startY = 20;
	  int textsize = 18;
	  public HistoryTextView(Context context)
	  { 
	    super(context); 
	    app = MSNApplication.getApplication();
	    startX = (int)(5*app.screenScale);
	    startY = (int)(20*app.screenScale);
	    textsize = (int)(18*app.screenScale);
	    activity = (Activity) context; 
	    setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT)); 
	    setOrientation(LinearLayout.VERTICAL);
	    setGravity(Gravity.FILL);    
	    this.invalidate();
	  }
	  
	  /**初始化页面*/
	  public void init(Item currSession)
	  {
	        if(currSession==null)
	        	return;
	        Vector msgs=null;
	        msgs=app.getSessionMessages(currSession.TOPITEM_JID);
	        if (msgs != null)
			{
	        	Item msg;
				for (int i = 0; i < msgs.size(); i++)
				{
					msg = null;
					msg = (Item) msgs.elementAt(i);
					if(msg.MESSAGE_type == app.MESSAGE_TYPE_FILE_TRANSFER)
					{
						addTransferFile(msg);
					}
					else if(msg.MESSAGE_type==app.MESSAGE_TYPE_VOICECLIP)
					{
						addRecordClip(msg); 
					}
					else
					{
						addMessage(msg, msg.MESSAGE_IS_SEND);
					}
				}
			}
	        
	  }
	  
	  public void addMessage(Item msg, boolean bSend)
	  {
          String show1=msg.MESSAGE_nickname + " " +getContext().getText(R.string.text_chat_says) + ": "+"(" + msg.MESSAGE_stamp+ ")";
		  addItem(show1, bSend, true, false);
		  String text=msg.MESSAGE_body;
		  if(msg.MESSAGE_status!=null&&msg.MESSAGE_status.equals("fail"))
		  {
			  if(msg.MESSAGE_body==null||msg.MESSAGE_body.length()==0)
			  {
				  text=msg.MESSAGE_reason;
			  }				  
			  else
			  {
				  text=msg.MESSAGE_body+"\n"+msg.MESSAGE_reason;
			  }				  
		  }
			
		  if(msg.MESSAGE_type == app.MESSAGE_TYPE_FILE_TRANSFER||msg.MESSAGE_type ==app.MESSAGE_TYPE_VOICECLIP)
		  {
			  
		  }
		  else
		  {
			  if(!msg.MESSAGE_nudge)
			  {
				  addItem(text, bSend, false, false);
			  }
			  else
			  {
				  addItem(msg.MESSAGE_nickname +" "+ getContext().getText(R.string.text_shake), bSend, false, false);
			  }
		  }

	  }
	  
	  public void addItem(String str, boolean bSend, boolean isTitle, boolean isHistory)
	  {
		  EmotionTextView etv1;
	      if(isTitle)
	      {
	    	  if(isHistory)
	    	  {
	    		  etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes_TimeStamp_Yes_History_Yes);
	    	  }
	    	  else
	    	  {
	    		  etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLACK,EmotionTextView.Emotion_Yes_OneLine_Yes_TimeStamp_Yes);	    	  
	    	  }
	      }
	      else
	      {
	    	  etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLUE,EmotionTextView.Emotion_Yes_OneLine_No);	 
	      }
		  
	      int height=etv1.setShowText(str,startX,startY,(int)(305*app.screenScale));
	      if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
	      {
	          int width = (int)(420*app.screenScale);
	    	  height=etv1.setShowText(str,startX,startY,width);
	      }
	      else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
	      {
	          int width = (int)(305*app.screenScale);
	    	  height=etv1.setShowText(str,startX,startY,width);
	      } 
	      LayoutParams params1;
	      if(isTitle)
	      {
	    	  params1 = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,height);
	      }
	      else
	      {
	    	  params1 = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,height + 5);
	      }
	      etv1.setLayoutParams(params1);      
	      
	      if(bSend)
	      {
		      etv1.setFocusable(true);
		      etv1.setFocusableInTouchMode(true);
		      etv1.requestFocus();
		      etv1.setFocusable(false);
		      etv1.setFocusableInTouchMode(false);
	      }
	      addView(etv1);
	  }
	  
	  
	  public void addTransferFile(Item msg)
	  {
		  boolean isSend=MsnUtil.getIsSendMessage(msg);
		  addMessage(msg, isSend);
		  TransferItem tranItem = new TransferItem(getContext(), msg, isSend);
	      if(isSend)
	      {
	    	  tranItem.setFocusable(true);
	    	  tranItem.setFocusableInTouchMode(true);
	    	  tranItem.requestFocus();
	    	  tranItem.setFocusable(false);
	    	  tranItem.setFocusableInTouchMode(false);
	      }
		  addView(tranItem);
	  }
	  
	  public void addRecordClip(Item msg)
	  {
		  boolean isSend=MsnUtil.getIsSendMessage(msg);
		  addMessage(msg, isSend);
		  RecordItem recItem = new RecordItem(getContext(), msg);
		  addView(recItem);
	  }

	  
	  
	  public TransferItem getTransferItemByMessage(Item message)
	  {
		  View currentView;
		  for(int i=0;i<this.getChildCount();i++)
		  {
			  currentView = null;
			  currentView = this.getChildAt(i);
			  if(currentView instanceof TransferItem)
			  {
				 if( ((TransferItem)currentView).mMsg.MESSAGE_FILE_ftID.equals(message.MESSAGE_FILE_ftID))
					 return (TransferItem)currentView;
			  }
		  }
		  return null;
	  }
	  
	  /**带外 才需要*/
	  public RecordItem getRecordItemByMessage(Item message)
	  {
		  View currentView;
		  for(int i=0;i<this.getChildCount();i++)
		  {
			  currentView = null;
			  currentView=this.getChildAt(i);
			  if(currentView instanceof RecordItem)
			  {
				 if( ((RecordItem)currentView).mMsg.MESSAGE_VOICE_URL.equals(message.MESSAGE_VOICE_URL))
					 return (RecordItem)currentView;
			  }
		  }
		  return null;
	  }
	  
	  public void LoadHistory(String historyTitle)
	  {
		  Cursor cur = app.DB.loadHistoryData(MD5.toMD5(app.GetLiveID()), historyTitle);
		  if(cur == null)
		  {
			  return;
		  }
		  
		  String str;
		  EmotionTextView etv1;
		  LayoutParams params1;
		  for(int i = 0; i < cur.getCount(); i++)
		  {
			  	str = MsnUtil.getUnvalidFormRs(cur.getString(1));
			  	String title = str.substring(0,str.indexOf("\n")-1);
			  	String body = str.substring(str.indexOf("\n")+1);
			  	addItem(title, false, true, true);
			  	addItem(body, false, false, true);
//		      	etv1 = new EmotionTextView(getContext(), null,textsize,Color.BLUE,EmotionTextView.Emotion_Yes_OneLine_Yes_TimeStamp_Yes);
//		        int height=etv1.setShowText(title,startX,startY,(int)(305*app.screenScale));
//		        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE)
//		        {
//		            int width = (int)(420*app.screenScale);
//		        	height=etv1.setShowText(title,startX,startY,width);
//		        }
//		        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
//		        {
//		            int width = (int)(305*app.screenScale);
//		        	height=etv1.setShowText(title,startX,startY,width);
//		        } 
//		      params1 = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,height);
//		      etv1.setLayoutParams(params1);
//		      addView(etv1);
		      cur.moveToNext();
		  }
	  }
}
