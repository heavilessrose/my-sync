package com.pica.ui.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Date;
import java.util.Vector;

import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.pica.msn.ChatRoomActivity;
import com.pica.msn.EventListener;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;
import com.pica.msn.SaveFileActivity;

public class RecordItem extends LinearLayout{	
	ImageButton RecPlay;
	Button deleteButton;
	Button RecSave;
	Button RecAccept;
	ImageView img=null;
	TextView text=null;
	Item mMsg;	
	MSNApplication app;
	MediaPlayer mMediaPlayer;
	boolean isFirst = true;
	String mediaFilePath = "";
	ChatRoomActivity chatroom;
	int MIndex;
	public Context context;
	
	private OnCompletionListener complete = new OnCompletionListener()
	{
		
		public void onCompletion(MediaPlayer mp) 
		{
			// TODO Auto-generated method stub
			RecPlay.setImageResource(R.drawable.ic_media_play);
		}		
	};

	private OnErrorListener setDataError = new OnErrorListener()
	{

		
		public boolean onError(MediaPlayer mp, int what, int extra)
		{
			// TODO Auto-generated method stub			
			mp.reset();	
			try
			{
				mp.setDataSource(mediaFilePath);
				mp.prepare();
				if((what== -38)&&(extra == 0))
					mp.start();
			}		  
			catch (IOException e1)
			{
				// TODO Auto-generated catch block
				e1.printStackTrace();	
				Toast.makeText(getContext(), (String)getContext().getText(R.string.error_playAudio), Toast.LENGTH_LONG).show();
				RecPlay.setEnabled(false);						
			}
			RecPlay.setImageResource(R.drawable.ic_media_pause);			
			return false;
		}	
	};
	
	
	public RecordItem(Context context,Item msg)
	{
		super(context);
		this.context=context;
		mMsg = msg;		
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	if((msg.MESSAGE_VOICE_STATUS_TYPE == MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION)
    			||(msg.MESSAGE_VOICE_STATUS_TYPE == MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION))
    	{
    		init(context, -1); 
    	}
    	else if(msg.MESSAGE_VOICE_STATUS_TYPE == MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_WAITING)
    	{
    		init(context, 0);
    	}
    	else
    	{
    		init(context, 1);
    	}
    	chatroom = (ChatRoomActivity) context;    	
	}	
	
	
	public void reset()
	{
		if(mMsg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)
		{
			text.setVisibility(View.GONE);
			RecPlay.setVisibility(View.GONE);
			RecSave.setVisibility(View.GONE);
			RecAccept.setVisibility(View.VISIBLE);
		}
		else if(mMsg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION)
		{
			RecPlay.setVisibility(View.GONE);
			RecSave.setVisibility(View.GONE);
			RecAccept.setVisibility(View.VISIBLE);
			text.setVisibility(View.VISIBLE);
			text.setText(R.string.text_receiving_outer_exception);
		}
		else if(mMsg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_COMPLETE)
		{
			text.setVisibility(View.GONE);
			RecAccept.setVisibility(View.GONE);
			RecPlay.setVisibility(View.VISIBLE);
			RecSave.setVisibility(View.VISIBLE);
			if(mMsg.MESSAGE_VOICE_PATH!=null&&mMsg.MESSAGE_VOICE_PATH.length()!=0)
			{
				mediaFilePath=mMsg.MESSAGE_VOICE_PATH;
			}
			else
			{
				mediaFilePath =	createTempAudioFile(context, mMsg);		
				mMsg.MESSAGE_VOICE_PATH=mediaFilePath;
			}
			mMediaPlayer = new MediaPlayer();
			try
			{
				mMediaPlayer.setDataSource(mediaFilePath);
				mMediaPlayer.prepare();
			}		  
			catch (IOException e1)
			{
				e1.printStackTrace();				
				return;			
			}
		}
	}

	
	public void init(Context context, int type)
	{
		LayoutInflater inflate = (LayoutInflater)getContext().getSystemService(getContext().LAYOUT_INFLATER_SERVICE);	        
		LinearLayout recItem = (LinearLayout)inflate.inflate(R.layout.record_play, null);
		addView(recItem);	
		RecPlay = (ImageButton)recItem.findViewById(R.id.recordplay);
		RecPlay.setImageResource(R.drawable.ic_media_play);
		img = (ImageView)findViewById(R.id.icon);
		text = (TextView)findViewById(R.id.text);
		RecAccept = (Button)findViewById(R.id.recordaccept);
		if((type == 1) || (type == 0))
		{
			text.setVisibility(View.GONE);
			if(type == 1)
			{
				RecAccept.setVisibility(View.GONE);
			}
		}
		else
		{
			RecAccept.setVisibility(View.GONE);
			img.setVisibility(View.GONE);
			if(mMsg.MESSAGE_VOICE_STATUS_TYPE == MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION)
				text.setText(R.string.text_sending_outer_exception);
			else if(mMsg.MESSAGE_VOICE_STATUS_TYPE == MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION)
				text.setText(R.string.text_receiving_outer_exception);
				
		}
		if(type==1)
		{
			if(mMsg.MESSAGE_VOICE_PATH!=null&&mMsg.MESSAGE_VOICE_PATH.length()!=0)
			{
				mediaFilePath=mMsg.MESSAGE_VOICE_PATH;
			}
			else
			{
				mediaFilePath =	createTempAudioFile(context, mMsg);		
				mMsg.MESSAGE_VOICE_PATH=mediaFilePath;
			}
			mMediaPlayer = new MediaPlayer();
			try
			{
				mMediaPlayer.setDataSource(mediaFilePath);
				mMediaPlayer.prepare();				
			}		  
			catch (IOException e1)
			{
				// TODO Auto-generated catch block
				e1.printStackTrace();				
			}			
		}
		
		Log.v("mediaFilePath", "mediaFilePath="+mediaFilePath);
		//RecPlay = (Button)recItem.findViewById(R.id.recordplay);
		
		if((type == -1) || (type == 0))
		{
			RecPlay.setVisibility(View.GONE);
		}		

		RecAccept.setOnClickListener(new OnClickListener()
		{
			public void onClick(View v)
			{				
//				/////// 带外
				mMsg.MESSAGE_GAUGE_INDEX="0";
				mMsg.MESSAGE_VOICE_STATUS_TYPE=app.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_SENDING;
				app.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, mMsg);
			}			
		});

		//实现了和pc机上相同的功能，可以暂停/播放，一次只能一个recordItem播放，
		//点击时，如果有一个recordItem（如名为otherItem）在播放，则停止otherItem,并在下一次点击otherItem时重头播放	
		RecPlay.setOnClickListener(new OnClickListener()
		{
			
			public void onClick(View arg0)
			{
				ControlOtherItem(chatroom.playingItemIndex);
				MIndex = chatroom.history.indexOfChild(RecordItem.this);	
				chatroom.playingItemIndex = chatroom.history.indexOfChild(RecordItem.this);				
				if(mMediaPlayer.isPlaying())
				{					
					mMediaPlayer.pause();
					RecPlay.setImageResource(R.drawable.ic_media_play);					
				}
				else
				{
					mMediaPlayer.start();
					RecPlay.setImageResource(R.drawable.ic_media_pause);					
				}
				mMediaPlayer.setOnCompletionListener(complete);
				mMediaPlayer.setOnErrorListener(setDataError);
			}

		});
		  
		RecSave = (Button)recItem.findViewById(R.id.recordsave);
		if((type == -1) || (type == 0))
		{
			RecSave.setVisibility(View.GONE);
		}
		RecSave.setOnClickListener(new OnClickListener()
		{
			
			public void onClick(View arg0) 
			{
				if((android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_BAD_REMOVAL))						
						||(android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_REMOVED)))
				{
					Toast.makeText(getContext(), (String)getContext().getText(R.string.error_sdcardState), Toast.LENGTH_LONG).show();
				}
				else
				{					
					FileInputStream inStream;
					File tempfile = new File(mMsg.MESSAGE_VOICE_PATH);
					byte[] voiceData = new byte[(int) tempfile.length()];
					try
					{
						inStream = new FileInputStream(tempfile);
						inStream.read(voiceData);
					}
					catch (IOException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					Intent intent = new Intent();
					intent.setClass(getContext(), SaveFileActivity.class);
					intent.putExtra("MESSAGE_VOICE_DATA", voiceData);
					intent.putExtra("file_type", ".amr");
					getContext().startActivity(intent);					
				}					
			}
		});
		//删除按钮
		deleteButton = (Button)recItem.findViewById(R.id.deleteFile);
		if((type == -1) || (type == 0))
		{
			deleteButton.setVisibility(View.GONE);
		}
		deleteButton.setOnClickListener(new OnClickListener()
		{
			
			public void onClick(View arg0) 
			{
				(new File(mediaFilePath)).delete();
				
				Vector msgs=app.getSessionMessages(app.currentSession.TOPITEM_JID);
		        if (msgs != null)
				{
					for (int i = 0; i < msgs.size(); i++)
					{
						if(msgs.elementAt(i) == mMsg)
						{
							msgs.remove(i);
							break;
						}
					}
				}
		        int i = chatroom.history.indexOfChild(RecordItem.this);
		        chatroom.history.removeViewAt(i-1);
				chatroom.history.removeView(RecordItem.this);
			}
		});
	}	
	
	private void ControlOtherItem(int index)
	{
		if(index == -1)
			return;
		else
		{
			if(index != MIndex)
			{
				RecordItem tempItem = (RecordItem) chatroom.history.getChildAt(index);
				if(tempItem.mMediaPlayer!=null)
				{
					if(tempItem.mMediaPlayer.isPlaying())
					{
						tempItem.mMediaPlayer.stop();
						try 
						{
							tempItem.mMediaPlayer.prepare();
						}
						catch (Exception e)
						{	// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				}
				tempItem.RecPlay.setImageResource(R.drawable.ic_media_play);
			}
		}
	}
	
	/**
	 * 创建临时音频文件
	 * @param context
	 * @param mMsg
	 * @return
	 */
	public static String createTempAudioFile(Context context, Item mMsg)
	{
		String packageName = context.getPackageName();		
		
		String temp = "justforplay" + mMsg.MESSAGE_sid + MsnUtil.fileCreatedDate(new Date()) +".amr";
		File tempfile = new File(temp);
			  
		FileOutputStream outstream = null;
		try
		{			
			outstream = context.openFileOutput(tempfile.getName(), android.content.Context.MODE_WORLD_READABLE|android.content.Context.MODE_WORLD_WRITEABLE);
		}
		catch (FileNotFoundException e2)
		{
			// TODO Auto-generated catch block
			e2.printStackTrace();
			Toast.makeText(context, (String)context.getText(R.string.error_create_Audio), Toast.LENGTH_LONG).show();				
		}

		try 
		{
			outstream.write(mMsg.MESSAGE_VOICE_DATA);
			outstream.flush();
			outstream.close();
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			Toast.makeText(context, (String)context.getText(R.string.error_write_data), Toast.LENGTH_LONG).show();				
		}					  
		return "/data/data/" + packageName +"/files/" + tempfile.getName();
	}
}
