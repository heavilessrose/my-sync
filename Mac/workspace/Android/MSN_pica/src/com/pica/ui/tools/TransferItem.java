package com.pica.ui.tools;

import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class TransferItem extends LinearLayout{	
	Item mMsg;
	MSNApplication app;
	boolean mSend;
	ProgressBar tranProgress;
	Button tranAccept;
	Button tranReject;
	Button tranCancel;
	TextView tranStatus;
	public Context context;
	public TransferItem(Context context,Item msg, boolean bSend) {
		super(context);
		this.context=context;
		mSend = bSend;
		mMsg = msg;
		// TODO Auto-generated constructor stub
		
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	init();
	}
	
	public void init()
	{
		  LayoutInflater inflate = (LayoutInflater)getContext().getSystemService(getContext().LAYOUT_INFLATER_SERVICE);	        
		  LinearLayout tranItem = (LinearLayout)inflate.inflate(R.layout.transfers, null);
		  
		  TextView tranText = (TextView)tranItem.findViewById(R.id.transfersfile);
		  int size = Integer.parseInt(mMsg.MESSAGE_FILE_SIZE);
		  if(size <= 1024)
		  {
			  size = 1;
		  }
		  else
		  {
			  if(size%1024 > 0)
			  {
				  size = size/1024;
				  size = size + 1;
			  }
			  else
			  {
				  size = size/1024;
			  }

		  }
		  String temp = Integer.toString(size);
		  
		  tranText.setText(mMsg.MESSAGE_FILE_VOICE_NAME + "(" + temp + "K)");
		  addView(tranItem);
		  
		  tranProgress = (ProgressBar)tranItem.findViewById(R.id.progress);
//		  tranProgress.setBackgroundResource(R.drawable.middle);//索爱X10 mini bug
		  tranProgress.setLayoutParams(new LayoutParams((int) (300 * app.screenScale), LayoutParams.WRAP_CONTENT));
		  
		  tranAccept = (Button)tranItem.findViewById(R.id.accept);
		  tranAccept.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) 
				{
					if(app.currentSession.SESSION_sessionType==MSNApplication.SESSION_TYPE_GROUP_CHAT)
					{
						clickReject();
					}
					else
					{
						String str = android.os.Environment.getExternalStorageState();
						if((android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_BAD_REMOVAL))
								||(android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_REMOVED)))
						{
							Toast.makeText(getContext(), (String)getContext().getText(R.string.error_sdcardState), Toast.LENGTH_LONG).show();
							clickReject();
							return;
						}
						
						//if(app.isTransfering)	
						if(app.isTransferingMessageVector.size()!=0)
						{
							Toast.makeText(context, (String) context.getText(R.string.text_transfer_only_one), Toast.LENGTH_SHORT).show();
						}
						else
						{
							//app.isTransfering=true;//
							if(!app.isTransferingMessageVector.contains(mMsg))
								app.isTransferingMessageVector.addElement(mMsg);
							
							  tranAccept.setVisibility(View.GONE);
							  tranReject.setVisibility(View.GONE);
							  tranCancel.setVisibility(View.VISIBLE);
							  mMsg.MESSAGE_FILE_DATA=new byte[Integer.parseInt(mMsg.MESSAGE_FILE_SIZE)];//开辟附件大小的空间
							  app.jabber.sendTransferInviteResponse(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID, "accept");
							  mMsg.MESSAGE_GAUGE_INDEX="0";
							  
							  if(!mSend)
								  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_SENDING;
						}		

						 reset();
					}

				}
	      });
		  
		  tranReject = (Button)tranItem.findViewById(R.id.reject);
		  tranReject.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) 
				{
//				  tranAccept.setVisibility(View.GONE);
//				  tranReject.setVisibility(View.GONE);
//				  tranCancel.setVisibility(View.GONE);
//				  tranStatus.setVisibility(View.VISIBLE);
//				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
//				  app.jabber.sendTransferInviteResponse(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID, "reject");
//				  mMsg.MESSAGE_FILE_DATA=null;//拒绝后 可以立即置空
//				  if(!mSend)
//					  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING_REFUSE;
//				  reset();
					clickReject();
				}
	      });
		  
		  tranCancel = (Button)tranItem.findViewById(R.id.cancel);
		  tranCancel.setOnClickListener(new OnClickListener() {
				
				public void onClick(View arg0) 
				{
//				  tranAccept.setVisibility(View.GONE);
//				  tranReject.setVisibility(View.GONE);
//				  tranCancel.setVisibility(View.GONE);
//				  tranStatus.setVisibility(View.VISIBLE);
//				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
//				  
//				  
////				  if(mMsg.MESSAGE_FILE_STATUS_TYPE == MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING)
////				  {
////					  
////				  }
////				  else
////				  {
////
////					  app.jabber.sendTransferCancel(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID);
////				  }
//				  app.jabber.sendTransferCancel(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID);
//				  
//				  
//				  if(!mSend)
//					  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL;
//				  else
//					  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL;
//				  if(!app.isInnerSendFile)//带外 文件
//				  {
//					  if(mMsg.MESSAGE_FILEOUTTRANSFER!=null)
//						  mMsg.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(mMsg.MESSAGE_FILE_STATUS_TYPE);
//				  }
//				  reset();
					
					clickCancel();
				}
	      });
		  	  
		  tranStatus = (TextView)tranItem.findViewById(R.id.tranStatus);
		  reset();
	}
	
	/**点击取消*/
	public void clickCancel()
	{
		  tranAccept.setVisibility(View.GONE);
		  tranReject.setVisibility(View.GONE);
		  tranCancel.setVisibility(View.GONE);
		  tranStatus.setVisibility(View.VISIBLE);
		  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
		  app.jabber.sendTransferCancel(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID);  
		  if(!mSend)
			  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL;
		  else
			  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL;
		  if(!app.isInnerSendFile)//带外 文件
		  {
			  if(mMsg.MESSAGE_FILEOUTTRANSFER!=null)
				  mMsg.MESSAGE_FILEOUTTRANSFER.setOuterCancelTransfer(mMsg.MESSAGE_FILE_STATUS_TYPE);
		  }
		  reset();
	}
	
	/**点击拒绝*/
	private void clickReject()
	{
		  tranAccept.setVisibility(View.GONE);
		  tranReject.setVisibility(View.GONE);
		  tranCancel.setVisibility(View.GONE);
		  tranStatus.setVisibility(View.VISIBLE);
		  //tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
		  tranStatus.setText(getContext().getString(R.string.text_trans_rcvexec));
		  app.jabber.sendTransferInviteResponse(mMsg.MESSAGE_FILE_transferID, mMsg.MESSAGE_sid, mMsg.MESSAGE_FILE_ftID, "reject");
		  if(!mSend)
			  mMsg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING_REFUSE;
		  reset();
	}
	
	public void reset()
	{
		if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.VISIBLE);
			  tranProgress.setVisibility(View.VISIBLE);
			  tranStatus.setVisibility(View.VISIBLE);	
			  tranStatus.setText(getContext().getString(R.string.text_trans_wait));	
//			  if(mMsg.MESSAGE_nickname == null)
//			  {
//				  tranStatus.setText(getContext().getString(R.string.text_trans_wait) + " " +mMsg.MESSAGE_imid + " " +getContext().getString(R.string.text_trans_accept));	
//			  }
//			  else
//			  {
//				  tranStatus.setText(getContext().getString(R.string.text_trans_wait) + " " +mMsg.MESSAGE_nickname + " " +getContext().getString(R.string.text_trans_accept));
//			  }
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING_REFUSED)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
//			  if(mMsg.MESSAGE_nickname == null)
//			  {
//				  tranStatus.setText(mMsg.MESSAGE_imid + " " +getContext().getString(R.string.text_trans_alreadycancel));
//			  }
//			  else
//			  {
//				  tranStatus.setText(mMsg.MESSAGE_nickname + " " +getContext().getString(R.string.text_trans_alreadycancel));
//			  }
			  tranStatus.setText(getContext().getString(R.string.text_trans_sendexec));	
			  
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
			
			  mMsg.MESSAGE_FILE_DATA=null;
			  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_SENDING)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.VISIBLE);
			  tranProgress.setVisibility(View.VISIBLE);
			  tranStatus.setVisibility(View.GONE);
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_COMPLETE)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  tranStatus.setText(getContext().getString(R.string.text_trans_send_complete));
			  
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
			
			mMsg.MESSAGE_FILE_DATA=null;
			System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  if(mSend)
			  {
				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));				  
			  }
			  else
			  {
//				  if(mMsg.MESSAGE_nickname == null)
//				  {
//					  tranStatus.setText(mMsg.MESSAGE_imid + " " +getContext().getString(R.string.text_trans_alreadycancel));					  
//				  }
//				  else
//				  {
//					  tranStatus.setText(mMsg.MESSAGE_nickname + " " +getContext().getString(R.string.text_trans_alreadycancel));
//				  }
				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
			  }
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_EXCEPTION)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  tranStatus.setText(getContext().getString(R.string.text_trans_sendexec));
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING)
		{
			  tranAccept.setVisibility(View.VISIBLE);
			  tranReject.setVisibility(View.VISIBLE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.VISIBLE);
			  tranStatus.setVisibility(View.GONE);
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_WAITING_REFUSE)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  //tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));
			  tranStatus.setText(getContext().getString(R.string.text_trans_rcvexec));
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_SENDING)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.VISIBLE);
			  tranProgress.setVisibility(View.VISIBLE);
			  tranStatus.setVisibility(View.VISIBLE);
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_COMPLETE)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  tranStatus.setText(getContext().getString(R.string.text_trans_receive_complete)+"/sdcard/"+mMsg.MESSAGE_FILE_VOICE_NAME);
			  
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  if(!mSend)
			  {
				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));				  
			  }
			  else
			  {
//				  if(mMsg.MESSAGE_nickname == null)
//				  {
//					  tranStatus.setText(mMsg.MESSAGE_imid + " " +getContext().getString(R.string.text_trans_alreadycancel));					  
//				  }
//				  else
//				  {
//					  tranStatus.setText(mMsg.MESSAGE_nickname + " " +getContext().getString(R.string.text_trans_alreadycancel));
//				  }
				  tranStatus.setText(getContext().getString(R.string.text_trans_alreadycancel));				  
			  }
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
		else if(mMsg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_EXCEPTION)
		{
			  tranAccept.setVisibility(View.GONE);
			  tranReject.setVisibility(View.GONE);
			  tranCancel.setVisibility(View.GONE);
			  tranProgress.setVisibility(View.GONE);
			  tranStatus.setVisibility(View.VISIBLE);
			  tranStatus.setText(getContext().getString(R.string.text_trans_rcvexec));
			  //app.isTransfering=false;//
				if(app.isTransferingMessageVector.contains(mMsg))
					app.isTransferingMessageVector.removeElement(mMsg);
				  mMsg.MESSAGE_FILE_DATA=null;
				  System.gc();	
		}
	}
	
	public void setProgress(int size)
	{
		tranProgress.setProgress(size);
	}
	


}
