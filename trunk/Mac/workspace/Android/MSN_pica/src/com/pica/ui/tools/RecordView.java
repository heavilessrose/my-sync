package com.pica.ui.tools;

import java.io.File;

import android.content.Context;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;

import com.pica.msn.ChatRoomActivity;
import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

public class RecordView extends LinearLayout{	
	MSNApplication app;
	MediaRecorder Mrecorder;
	ProgressBar recrodP;
	int recordS = 0;
	ChatRoomActivity chatroom;
	public String saveLocationUrl=null;
	
	public RecordView(Context context) {
		super(context);

		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	init();
    	chatroom = (ChatRoomActivity)context;
    	
	}
	
	public void init()
	{
		  LayoutInflater inflate = (LayoutInflater)getContext().getSystemService(getContext().LAYOUT_INFLATER_SERVICE);	        
		  LinearLayout recItem = (LinearLayout)inflate.inflate(R.layout.record, null);
		  addView(recItem);
		  
		  recrodP = (ProgressBar)findViewById(R.id.progress);
		  final Button btnR = (Button)findViewById(R.id.record);
		  btnR.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) {					
					record();
					btnR.setEnabled(false);
					chatroom.stopButton.setClickable(true);
				}
	      });
		  
		  final Button cancelButton = (Button)findViewById(R.id.cancel);
		  cancelButton.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) {
					cancel();
				}
	      });
		  
	}
	
	
	private void record()
	{            
	    Mrecorder = new MediaRecorder();	    
		Mrecorder.setAudioSource(MediaRecorder.AudioSource.MIC); 
        Mrecorder.setOutputFormat(MediaRecorder.OutputFormat.RAW_AMR); 
        Mrecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
        //创建临时文件夹，用于支持语音剪辑
        File temprecord = app.mActivity.getDir("temprecord",
        		android.content.Context.MODE_WORLD_READABLE|android.content.Context.MODE_WORLD_WRITEABLE);
        String packageName = app.mActivity.getPackageName();
		Long time = System.currentTimeMillis();
		String temp = Long.toString(time);
        saveLocationUrl= "/data/data/" + packageName +"/app_temprecord/" + temp + ".amr";
        Mrecorder.setOutputFile(saveLocationUrl);
        try {
			Mrecorder.prepare();
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
        Mrecorder.start();
        RecordMessage(true);
	}
	
	private void cancel()
	{
		if(Mrecorder != null)
		{
			RecordMessage(false);
			Mrecorder.stop();
	    	if(Mrecorder != null)
	    	{
	    		Mrecorder.release();
	    		Mrecorder = null;
	    	}
	        File recordFile = new File(saveLocationUrl);
	        recordFile.delete();	    	
		}
		chatroom.record.dismiss();
	}
	
	public boolean stop()
	{
		if(Mrecorder != null)
		{
			RecordMessage(false);
			Mrecorder.stop();
	    	if(Mrecorder != null)
	    	{
	    		Mrecorder.release();
	    		Mrecorder = null;
	    	}
	    	chatroom.record.dismiss();
	    	return true;
		}
		else
		{
			return false;
		}
	}
	
    private Handler MessageHandler = new Handler() 
    { 
         public void handleMessage(Message msg) 
         { 
              switch (msg.what) 
              { 
	              case 1: 
	                  super.handleMessage(msg); 
	                  RecordMessage(true);
	                  break; 
              }          
         } 
    }; 
    
    
    public void RecordMessage(boolean bRecord)
    {
    	if(bRecord)
    	{
    		if(recordS >= 15)
    		{
		    	if(app.chatroomHandler!=null)
				{
				    Message mm = new Message(); 
				    mm.what = EventListener.EVENT_RECORD_COMPLETE; 
				    mm.obj=saveLocationUrl;
				    app.chatroomHandler.sendMessage(mm);				   
				}
    			return;
    		}
    		Message msg = new Message(); 
    		
    		msg.what = 1; 
    		MessageHandler.sendMessageDelayed(msg, 1000); 
    		recrodP.setProgress(recordS* 6);
            recordS++;
    	}
    	else
    	{
    		recordS = 0;
    		//if(msg!=null)
    			MessageHandler.removeMessages(1);
    	}
    }
}
