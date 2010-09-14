package com.pica.msn;

public class FileOuterTransfer implements Runnable{

	public static final byte command_select_uploadFile=(byte)1;
	public static final byte command_continue_uploadFile=(byte)2;
	public static final byte command_continue_skip_uploadFile=(byte)3;
	
	public static final byte command_downloadFile=(byte)4;
	public static final byte command_continue_downloadFile=(byte)5;
	public static final byte command_continue_skip_downloadFile=(byte)6;
	
	int startPos, endPos, totalSize, totalPackage;// ���ݰ���ʼĩλ�ú��ļ��ܴ�С
	Item msg;//��Ϣ�е��ļ�����������
	Contact contact;//��ϵ��ͷ��
	byte commandType;
	private MSNApplication msnApp;
	String uploadResult;
	public FileConnection fileConnect=null;

	String downloadURL;
	
	public FileOuterTransfer(MSNApplication msnApp,byte type,Item msg,Contact contact,String url)
	{
		commandType = type;
		if(type==command_downloadFile)
		{
			if(msg!=null)
			{
				if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
					downloadURL=msg.MESSAGE_FILE_URL;
				else if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_VOICECLIP)
					downloadURL=msg.MESSAGE_VOICE_URL;
			}
			if(contact!=null)
			{
				downloadURL=url;
			}
		}
		this.msg=msg;
		this.contact=contact;
		fileConnect = new FileConnection(msg==null?contact:msg);
		this.msnApp = msnApp;
		isLive=true;
		
		new Thread(this).start();
	}
	
	/**���⴫������е��ȡ��*/
	public void setOuterCancelTransfer(byte type)
	{
		if(type==MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL||type==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL)
		{
			//msnApp.isTransfering=false;//
			if(msnApp.isTransferingMessageVector.contains(msg))
				msnApp.isTransferingMessageVector.removeElement(msg);
			msg.MESSAGE_FILE_STATUS_TYPE=type;
		}
		else if(type==MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL||type==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)
		{
			//msnApp.isTransfering=false;//
			if(msnApp.isTransferingMessageVector.contains(msg))
				msnApp.isTransferingMessageVector.removeElement(msg);
			msg.MESSAGE_VOICE_STATUS_TYPE=type;
			msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);//֪ͨ �����Ͻ�dialogȥ��
		}
		else if(type==MSNApplication.MESSAGE_HEAD_TYPE_SEND_CANCEL)
		{
			msnApp.isTransferingOuterSendHead=false;//
			contact.CONTACT_OUTER_HEAD_STATUS=type;
			msnApp.eventNotify(EventListener.EVENT_UPDATE_OUTER_HEAD_STATUS, contact);
		}
		isLive=false;
	}
	
	public void setFileOuterTransfer(byte type, int sp, int ep, int ts)
	{
		commandType = type;
		startPos = sp;
		endPos = ep;
		totalSize = ts;
	}
	
	boolean isLive=false;
	public void run() 
	{
		while(isLive)
		{
			switch(commandType)
			{
				case command_select_uploadFile:
					if(msg!=null)
					{
						if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
						{
							if (msg.MESSAGE_FILE_DATA == null)
							{
								//"�Բ�����ѡ����ļ������д���ѡ�����ļ�"
								return;
							} 
							startPos = 0;
							totalSize = msg.MESSAGE_FILE_DATA.length;
						}
						else
						{
							if (msg.MESSAGE_VOICE_DATA == null)
							{
								//"�Բ�����ѡ����ļ������д���ѡ�����ļ�"
								return;
							} 
							startPos = 0;
							totalSize = msg.MESSAGE_VOICE_DATA.length;
						}

						if (totalSize <= msnApp.per_file_outer_upload_size)
							endPos = totalSize - 1;
						else
							endPos = (int) (msnApp.per_file_outer_upload_size) - 1;
						msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT = "0";
					}
					else if(contact!=null)//�ϴ�ͷ��
					{
						if (contact.CONTACT_FILE_DATA == null)
						{
							//"�Բ�����ѡ����ļ������д���ѡ�����ļ�"
							return;
						} 
						startPos = 0;
						totalSize = contact.CONTACT_FILE_DATA.length;
						if (totalSize <= msnApp.per_file_outer_upload_size)
							endPos = totalSize - 1;
						else
							endPos = (int) (msnApp.per_file_outer_upload_size) - 1;
						contact.CONTACT_FILE_VOICE_TRANSFER_COUNT = "0";
					}

					setFileOuterTransfer(command_continue_uploadFile, startPos, endPos, totalSize);	
				break;
				
				case command_continue_uploadFile:
					uploadFile(startPos, endPos, totalSize);
					break;
					
				case command_continue_skip_uploadFile:
					setFileOuterTransfer(command_continue_uploadFile, startPos, endPos, totalSize);
					break;
					
				case command_downloadFile:
					if(msg!=null)
					{
						if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
						{
							if(msg.MESSAGE_FILE_SIZE!=null&&msg.MESSAGE_FILE_SIZE.length()!=0)
							{
								msg.MESSAGE_FILE_DATA=new byte[Integer.parseInt(msg.MESSAGE_FILE_SIZE)];
								totalSize=msg.MESSAGE_FILE_DATA.length;
							}
							msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT = "0";
						}
						else
						{
							if(msg.MESSAGE_VOICE_SIZE!=null&&msg.MESSAGE_VOICE_SIZE.length()!=0)
							{
								msg.MESSAGE_VOICE_DATA=new byte[Integer.parseInt(msg.MESSAGE_VOICE_SIZE)];
								totalSize=msg.MESSAGE_VOICE_DATA.length;
							}
							msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT = "0";
						}
					}
					else//����ͷ�� �����Լ��ͺ��ѵ�
					{
						contact.CONTACT_FILE_VOICE_TRANSFER_COUNT = "0";
					}

					startPos=0;
					endPos = msnApp.per_file_outer_download_size - 1;
					
					setFileOuterTransfer(command_continue_downloadFile,startPos,endPos,totalSize);
					break;
					
				case command_continue_downloadFile:
					downloadFile(startPos,endPos,totalSize);
					break;
				case command_continue_skip_downloadFile:
					setFileOuterTransfer(command_continue_downloadFile, startPos, endPos, totalSize);
					break;
			}
		}

	}

	/**����*/
	public void downloadFile(int sp, int ep, int ts)
	{
		byte[] file=null;
		try 
		{
			String[][] head = null;
			head= new String[][]{{"Pica-X-Range","bytes="+sp+"-"+ep+""},
					{"Accept","*/*"},
					{"Content-Type","*/*"},
					{"Pica-Clientver",MSNApplication.VERSION}};
			file=fileConnect.getUrl(downloadURL,head, false);
			if(file==null)
			{
				isLive=false;
				return;
			}
			if (file.length == 3)
			{
				if ((new String(file)).equals("400"))
				{
					//"��Ҫ���ص��ļ������� ***K��Ϊ��ȫ��������ر�ȡ����"
					isLive=false;
					return;
				} 
				else if ((new String(file)).equals("401"))
				{
					//"ϵͳʣ���ڴ治�㣬�˴����ر�ȡ����"
					isLive=false;
					return;
				} 
				else if ((new String(file)).equals("402"))
				{
					
					isLive=false;
					return;
				} 	
				else if ((new String(file)).equals("403"))
				{				
					return;
				}
			}
			/////////////
			if (fileConnect.end == fileConnect.total - 1)
			{
				if(msg!=null)
				{
					if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
					{
						System.arraycopy(file, 0, msg.MESSAGE_FILE_DATA, fileConnect.start, fileConnect.length);
					}
					else
					{
						if(msg.MESSAGE_VOICE_DATA==null)
						{
							msg.MESSAGE_VOICE_DATA=new byte[fileConnect.total];
						}
						System.arraycopy(file, 0, msg.MESSAGE_VOICE_DATA, fileConnect.start, fileConnect.length);
					}			 	
					msg.MESSAGE_GAUGE_INDEX="100";				
					msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
					if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)		
					{
						msg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_COMPLETE;
						boolean isSaveSuccess=MsnUtil.saveDataToFile("/sdcard/", msg.MESSAGE_FILE_VOICE_NAME, msg.MESSAGE_FILE_DATA);
						if(isSaveSuccess)
						{
							msg.MESSAGE_FILE_DATA=null;
							System.gc();
						}
					}
					else
						msg.MESSAGE_VOICE_STATUS_TYPE=MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_COMPLETE;
					
					msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);
				}
				else//��ϵ�˵�ͷ��
				{
					if(contact.CONTACT_imageData==null)
					{
						contact.CONTACT_imageData=new byte[fileConnect.total];
					}
					System.arraycopy(file, 0, contact.CONTACT_imageData, fileConnect.start, fileConnect.length);
					msnApp.isTransferingOuterSendHead=false;//
					
					if(contact.CONTACT_OUTER_HEAD_ONESELF_IMID!=null&&contact.CONTACT_OUTER_HEAD_ONESELF_IMID.length()!=0
							&&contact.CONTACT_OUTER_HEAD_ONESELF_IMID.equals(msnApp.GetLiveID()))
					{
						//���Լ���ͷ��
						msnApp.setHeadImage(contact.CONTACT_imageData);
						msnApp.deleteVcardHead();
						msnApp.addVcardHeadToRMS();
						msnApp.eventNotify(EventListener.EVENT_MY_VCARD_RECEIVED, null);
					}
					else
					{
						//����ϵ�˵�ͷ��
						 //#ifdef support_save_rms
						//msnApp.addContactToRMS(contact);//����hash
						msnApp.addContactHeadToRMS(contact);
						//#endif
						msnApp.eventNotify(EventListener.EVENT_RECEIVE_CONTACT_PORTRAIT, contact);
					}


				}
				isLive=false;
			} 
			else  
			{
				if(msg!=null)
				{

					if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)
					{
						System.arraycopy(file, 0, msg.MESSAGE_FILE_DATA, fileConnect.start, fileConnect.length);
					}
					else
					{
						if(msg.MESSAGE_VOICE_DATA==null)
						{
							msg.MESSAGE_VOICE_DATA=new byte[fileConnect.total];
						}
						System.arraycopy(file, 0, msg.MESSAGE_VOICE_DATA, fileConnect.start, fileConnect.length);
					}
					startPos = fileConnect.end + 1;
					endPos = startPos+ (int) msnApp.per_file_outer_download_size - 1;
					int gauge=((startPos*100)/fileConnect.total);		
					msg.MESSAGE_GAUGE_INDEX=Integer.toString(gauge);	
					msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
				}
				else//��ϵ�˵�ͷ��
				{
					if(contact.CONTACT_imageData==null)
					{
						contact.CONTACT_imageData=new byte[fileConnect.total];
					}
					System.arraycopy(file, 0, contact.CONTACT_imageData, fileConnect.start, fileConnect.length);
					startPos = fileConnect.end + 1;
					endPos = startPos+ (int) msnApp.per_file_outer_download_size - 1;
				}
			}
			////////////			
			return;
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			if(msg!=null)//���� ���ļ�
			{
				int count=Integer.parseInt(msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT);
				msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT=String.valueOf(++count);
				if (count < MSNApplication.MAX_TRY_COUNT)
				{
					setFileOuterTransfer(command_continue_skip_downloadFile, sp, ep, ts);
				} 
				else
				{
					//ʧ��
					isLive=false;
					if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)			
					{
						msg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_EXCEPTION;
					}
					else
					{
						msg.MESSAGE_VOICE_STATUS_TYPE=MSNApplication.MESSAGE_VOICE_TYPE_OUTER_RECEIVE_EXCEPTION;
					}
					//msnApp.isTransfering=false;//
					if(msnApp.isTransferingMessageVector.contains(msg))
						msnApp.isTransferingMessageVector.removeElement(msg);
					msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);
				}
			}
			else//ͷ��
			{
				int count=Integer.parseInt(contact.CONTACT_FILE_VOICE_TRANSFER_COUNT);
				contact.CONTACT_FILE_VOICE_TRANSFER_COUNT=String.valueOf(++count);
				if (count < MSNApplication.MAX_TRY_COUNT)
				{
					setFileOuterTransfer(command_continue_skip_downloadFile, sp, ep, ts);
				} 
				else
				{
					//ʧ��
					isLive=false;
					msnApp.isTransferingOuterSendHead=false;//
				}
			}

		}
	}
	
	/**�ϴ�*/
	public void uploadFile(int sp, int ep, int ts)
	{ 
		try
		{
			if(msg!=null)
			{
				if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)//�ļ�	
				{
					if (msg.MESSAGE_FILE_DATA != null)
					{
						String[][] head = null;
						if (uploadResult == null)
							head = new String[][]
							{
							{ "Upload-Range", "bytes " + sp + "-" + ep + "/" + ts },
							{ "Content-Length", String.valueOf(ep - sp + 1) } };
						else
							head = new String[][]
							{
									{ "Upload-Range",
											"bytes " + sp + "-" + ep + "/" + ts },
									{ "Content-Length", String.valueOf(ep - sp + 1) },
									{"Upload-ETag",uploadResult.substring(uploadResult
													.lastIndexOf('/') + 1, uploadResult
													.length()) } 
							};

						byte[] result= fileConnect.postUrl(ep,sp,
								msg.MESSAGE_FILE_DATA,msnApp.data_url+"/"+ msg.MESSAGE_FILE_VOICE_NAME, head);
						if (result.length==3)
						{
							if (new String(result).equals("402"))
							{
								//"�˴��ϴ������Ѿ���ȡ��"
								isLive=false;	
								return;
							} 
							else if (new String(result).equals("403"))
							{
								return;
							}
						}
						if (uploadResult == null)
							uploadResult=new String(result);
						msg.MESSAGE_FILE_URL=uploadResult;
						if (ep == ts - 1)
						{
							System.gc();
							isLive=false;						
							msnApp.jabber.sendTransferFileOuterClose(msg.MESSAGE_FILE_transferID,msg.MESSAGE_sid,msg.MESSAGE_FILE_ftID,uploadResult);		
							msg.MESSAGE_GAUGE_INDEX="100";				
							msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
							msg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_SEND_COMPLETE;
							msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);
						} 
						else  
						{
							startPos = endPos + 1;
							/* �ж���һ�������� */
							if (startPos +  msnApp.per_file_outer_upload_size > totalSize - 1)
								endPos = totalSize - 1;
							else
								endPos = startPos+ (int) msnApp.per_file_outer_upload_size - 1;
							int gauge=((startPos*100)/totalSize);		
							msg.MESSAGE_GAUGE_INDEX=Integer.toString(gauge);	
							msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
						}
						return;
					}
				}
				else//����
				{
					if (msg.MESSAGE_VOICE_DATA != null)
					{
						String[][] head = null;
						if (uploadResult == null)
							head = new String[][]
							{
							{ "Upload-Range", "bytes " + sp + "-" + ep + "/" + ts },
							{ "Content-Length", String.valueOf(ep - sp + 1) } };
						else
							head = new String[][]
							{
									{ "Upload-Range",
											"bytes " + sp + "-" + ep + "/" + ts },
									{ "Content-Length", String.valueOf(ep - sp + 1) },
									{"Upload-ETag",uploadResult.substring(uploadResult
													.lastIndexOf('/') + 1, uploadResult
													.length()) } 
							};

						byte[] result= fileConnect.postUrl(ep,sp,
								msg.MESSAGE_VOICE_DATA,msnApp.data_url+"/"+ msg.MESSAGE_FILE_VOICE_NAME, head);
						
						if (result.length==3)
						{
							if (new String(result).equals("402"))
							{
								//"�˴��ϴ������Ѿ���ȡ��"
								isLive=false;	
								return;
							} 
							else if (new String(result).equals("403"))
							{
								return;
							}
						}
						if (uploadResult == null)
							uploadResult=new String(result);
						msg.MESSAGE_VOICE_URL=uploadResult;
						if (ep == ts - 1)
						{
							System.gc();
							isLive=false;
							msnApp.jabber.sendVoiceClip(msg.MESSAGE_jid,msg.MESSAGE_sid,"audio/amr",null,null,null,msg.MESSAGE_VOICE_URL,false);
							msg.MESSAGE_GAUGE_INDEX="100";				
							msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
							msg.MESSAGE_VOICE_STATUS_TYPE=MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_COMPLETE;
							msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);	
						} 
						else  
						{
							startPos = endPos + 1;
							/* �ж���һ�������� */
							if (startPos +  msnApp.per_file_outer_upload_size > totalSize - 1)
								endPos = totalSize - 1;
							else
								endPos = startPos+ (int) msnApp.per_file_outer_upload_size - 1;
							int gauge=((startPos*100)/totalSize);		
							msg.MESSAGE_GAUGE_INDEX=Integer.toString(gauge);	
							msnApp.eventNotify(EventListener.EVENT_UPDATE_GAUGE_MESSAGE, msg);
						}
						return;
					}
				}
			}
			else//��ϵ��ͷ��
			{
				if (contact.CONTACT_FILE_DATA != null)
				{
					String[][] head = null;
					if (uploadResult == null)
						head = new String[][]
						{
						{ "Upload-Range", "bytes " + sp + "-" + ep + "/" + ts },
						{ "Content-Length", String.valueOf(ep - sp + 1) } };
					else
						head = new String[][]
						{
								{ "Upload-Range",
										"bytes " + sp + "-" + ep + "/" + ts },
								{ "Content-Length", String.valueOf(ep - sp + 1) },
								{"Upload-ETag",uploadResult.substring(uploadResult
												.lastIndexOf('/') + 1, uploadResult
												.length()) } };

					byte[] result= fileConnect.postUrl(ep,sp,
							contact.CONTACT_FILE_DATA,msnApp.data_url+"/"+ contact.CONTACT_FILE_VOICE_NAME, head);
					if (result.length==3)
					{
						if (new String(result).equals("402"))
						{
							//"�˴��ϴ������Ѿ���ȡ��"
							isLive=false;	
							return;
						} 
						else if (new String(result).equals("403"))
						{
							return;
						}
					}
					if (uploadResult == null)
						uploadResult=new String(result);
					contact.CONTACT_FILE_URL=uploadResult;
					if (ep == ts - 1)//�ϴ�ͷ��ɹ�
					{
						System.gc();
						isLive=false;			
						contact.CONTACT_GAUGE_INDEX="100";				
						msnApp.eventNotify(EventListener.EVENT_UPDATE_OUTER_HEAD_GAUGE, contact);
						contact.CONTACT_OUTER_HEAD_STATUS=MSNApplication.MESSAGE_HEAD_TYPE_OUTER_SEND_COMPLETE;
						msnApp.eventNotify(EventListener.EVENT_UPDATE_OUTER_HEAD_STATUS, contact);
						msnApp.jabber.sendModifyMyHead("image/jpeg", null, null, contact.CONTACT_FILE_URL);
					} 
					else  
					{
						startPos = endPos + 1;
						/* �ж���һ�������� */
						if (startPos +  msnApp.per_file_outer_upload_size > totalSize - 1)
							endPos = totalSize - 1;
						else
							endPos = startPos+ (int) msnApp.per_file_outer_upload_size - 1;
						int gauge=((startPos*100)/totalSize);		
						contact.CONTACT_GAUGE_INDEX=Integer.toString(gauge);	
						msnApp.eventNotify(EventListener.EVENT_UPDATE_OUTER_HEAD_GAUGE, contact);
					}
					return;
				}
			}

		} 
		catch (Exception e)
		{
			e.printStackTrace();
			if(msg!=null)
			{
				
				int count=Integer.parseInt(msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT);
				msg.MESSAGE_FILE_VOICE_TRANSFER_COUNT=String.valueOf(++count);
				if (count < MSNApplication.MAX_TRY_COUNT)
				{
					setFileOuterTransfer(command_continue_skip_uploadFile, sp, ep, ts);
				} 
				else
				{
					//ʧ��
					isLive=false;
					if(msg.MESSAGE_type==MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)			
					{
						msg.MESSAGE_FILE_STATUS_TYPE=MSNApplication.MESSAGE_FILE_TYPE_SEND_EXCEPTION;
					}
					else
					{
						msg.MESSAGE_VOICE_STATUS_TYPE=MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION;
						System.gc();
					}
					//msnApp.isTransfering=false;//
					if(msnApp.isTransferingMessageVector.contains(msg))
						msnApp.isTransferingMessageVector.removeElement(msg);
					msnApp.eventNotify(EventListener.EVENT_UPDATE_MESSAGE, msg);
				}

			}
			else//��ϵ��ͷ��
			{
				int count=Integer.parseInt(contact.CONTACT_FILE_VOICE_TRANSFER_COUNT);
				contact.CONTACT_FILE_VOICE_TRANSFER_COUNT=String.valueOf(++count);
				if (count < MSNApplication.MAX_TRY_COUNT)
				{
					setFileOuterTransfer(command_continue_skip_uploadFile, sp, ep, ts);
				} 
				else
				{
					//ʧ��
					isLive=false;
					contact.CONTACT_OUTER_HEAD_STATUS=MSNApplication.MESSAGE_HEAD_TYPE_OUTER_SEND_EXCEPTION;
					msnApp.isTransferingOuterSendHead=false;//
					msnApp.eventNotify(EventListener.EVENT_UPDATE_OUTER_HEAD_STATUS, contact);
				}
			}
		}
	}
}
