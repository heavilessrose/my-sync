package com.pica.ui.tools;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.media.MediaRecorder;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.ProgressBar;

public class RecordSendView extends LinearLayout{	
	MSNApplication app;
	MediaRecorder Mrecorder;
	ProgressBar recrodP;
	int recordS = 0;

	
	Item message;
	Contact contact;
	
	public RecordSendView(Context context,Item msg,Contact contact) {
		super(context);

		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	this.message=msg;
    	this.contact=contact;
    	init();
	}
	
	public void init()
	{
		  LayoutInflater inflate = (LayoutInflater)getContext().getSystemService(getContext().LAYOUT_INFLATER_SERVICE);	        
		  LinearLayout recItem = (LinearLayout)inflate.inflate(R.layout.record_send, null);
		  addView(recItem);
		  
		  recrodP = (ProgressBar)findViewById(R.id.progress);
		  recrodP.setProgress(0);
	}
	
	/**如果需要关闭该对话条 则返回true*/
	public boolean  resetState()
	{
		if(this.message!=null)
		{
			if(this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_COMPLETE
					||this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_COMPLETE
					||this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION
					||this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION
					||this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL
					||this.message.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)
			{
				//结束
				return true;
			}
			else
				return false;
		}
		else//联系人头像 一律关闭
		{
			return true;
		}
	}
	
	public void setProgress(int size)
	{
		recrodP.setProgress(size);
	}
	

}
