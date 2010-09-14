package com.pica.msn;


import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.Socket;
import java.net.URL;
import java.util.Vector;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;



public class Jabber implements Runnable
{
	/**�ͻ���dispatch�����������jabber��������ַ*/
	public String jabberServer;
	/**�ͻ���dispatch�����������jabber�������˿�*/
	public String jabberServerPort;
	
	public static boolean isUseSocket=true;
	public static Proxy proxy;//http������ʹ�ô���
	private MSNApplication msn;
	private String session_id;
	private long firstTime = 0;
	/**dispatch���ص�notice(system notice����ǰ��)��˳��������������*/
	public Vector noticeVector=new Vector();
	public String force=null;//froce=0/1 ��������/ǿ������
	public String result=null;//0/1/2 ����� /ǿ������ /�ɹ�
	public Vector requestQueue= new Vector();
	String updateVersion = "";
	String updateUrl = "";
	ParserNotifier parserNotifier;
	EventListener eventListener;
	/////////////
	private Socket socketConnection ;  
	XmlReader reader;
	OutputStream writer;
	public long lastActiveTime = 0;//���һ���յ�����ʱ��
	//////////////
	public boolean receive_logout_unavailable;// �Ƿ��յ�ע�����ذ�
	
	/** �Ƿ�ע���� */
	public boolean isSendLogoutRequest;
	private boolean cacheEmpty;
	/** ǿ���˳��շ������� */
	public boolean forceStop;

	public String zwp_addFriendUrl = null,// msn.cn�û������ϵ�˵ĵ�ַ
			msn_ua = null,// UA
			msn_carrier = null,// ��Ӫ��
			transParent = null;// dispatch���ص����ø�����
	
	/**��wap��������ַ*/
	public String zipWapServer=null;
	
	/**����������ַ*/
	public String adServer = null;
	/**���������˿�*/
	public String adServerPort=null;

	
	/**�Ƿ���ȡ����¼*/
	public boolean cancelLogin=false;
	
	private int interval = 10000;
	
	
	Jabber(ParserNotifier parserNotifier,EventListener el) 
	{
		this.parserNotifier=parserNotifier;
		eventListener=el;
		msn=MSNApplication.getApplication();
		msn.parser=new Parser(parserNotifier);
		new Thread(this).start();//���������߳�
	}
	
	/**
	 * �����¼MSN�����󣨵�¼msp�� ����email�����룬������ϵ���������汾�� jabber:iq:register
	 * @param username �û�email
	 * @param password ����
	 * @param show  ״̬
	 * @param uastring 
	 * @param version
	 * @param source
	 * @param xroster all
	 * @param carrier
	 * @param capability
	 * @param profile_portraitsize �Լ�ͷ����ʾ�Ĵ�С-����  ���֧��ͷ����Ҫ���˽ڵ�
	 * @param contact_portraitsize ��ϵ��ͷ����ʾ�Ĵ�С-���� ���֧��ͷ����Ҫ���˽ڵ�
	 * @param deviceid  ���������ɣ����ر��棻�´ε�¼ʱ���ϣ���ͬ���汾����
	 * @param sync_contacts  ��ϵ��ͬ���汾
	 * @param resourceId �������ϵ�resourceid �������û�д˲������򲻴�
	 * @param transparent ͸��dispatch���ص����ø�����
	 * @param image_accept  image/jpeg,image/png...
	 * @param audio_accept  audio/wav,audio/amr,audio/qcp...
	 * @param <data_method>
	 */
	public void sendLoginMSN(String username, String password,String show,String uastring,String version,
			String source,String xroster,String carrier,String capability,String profile_portraitsize,
			String contact_portraitsize,String deviceid,String sync_contacts,
			String resourceid,String transparent,String image_accept,String audio_accept,String data_method)
	{
		///////////////////////
		String sendClientCapability=msn.CLIENT_CAPABILITY;
		//android��֧�� :
		//0.���˻Ự����ʱȺ�飩      							֧��
		//1.֧��ͷ��                                                                                              	          ֧��
		//2.֧���ļ�����                     							  ֧��
		//3.֧��¼������									 ֧��
		//4.֧�ֽ���nudge                             	         ֧��
		//5.֧�ֱ��ر�����ϵ�˺ͷ���							֧��
		//6.֧�ֽ�����ϵ��ͷ��仯����ָpresence��                            ȡ��diaptch�ͱ���RMS����
		//7.֧�ֽ�����ϵ�˸�����Ϣ�仯����ָpresence��                 ȡ��diaptch�ͱ���RMS����
		//8.֧��������Ϣ										ȡ��diaptch
		//9.֧�ֱ༭����ʾ��ϵ���ǳ�							ȡ��diaptch
		
		char[] capabilityCharArray=new char[32];
		for(int i=capabilityCharArray.length-1;i>=0;i--)//��ʼֵ ��"0000003F" 
		{
			if(i==capabilityCharArray.length-1||i==capabilityCharArray.length-1-1||
					i==capabilityCharArray.length-1-2||i==capabilityCharArray.length-1-3||
					i==capabilityCharArray.length-1-4||i==capabilityCharArray.length-1-5)
				capabilityCharArray[i]='1';
			else
				capabilityCharArray[i]='0';
		}
		//#ifdef support_head_image
		if(msn.isSupportReceiveHeadImageChangeByCapability&&msn.isReceiveHeadChange)
		{
			capabilityCharArray[capabilityCharArray.length-1-6]='1';
		}
		//#endif
		if(msn.isSupportReceiveVcardChangeByCapability&&msn.isReceiveVcardChange)
		{
			capabilityCharArray[capabilityCharArray.length-1-7]='1';
		}
		if(msn.isSupportOfflineMessageByCapability)
		{
			capabilityCharArray[capabilityCharArray.length-1-8]='1';
		}
		if(msn.isSupportLocalnameByCapability)
		{
			capabilityCharArray[capabilityCharArray.length-1-9]='1';
		}
		sendClientCapability=new String(capabilityCharArray);
		sendClientCapability=Integer.toString(Integer.parseInt(sendClientCapability, 2), 16);
		
		////////////////////////
		
		
		
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:iq:register'><username>");
		buf.append(MsnUtil.escapeString(username));
		buf.append("</username><password>");
		buf.append(MsnUtil.escapeString(password));
		buf.append("</password><show>"+show+"</show><uastring>");
		buf.append(MsnUtil.escapeString(uastring));
		buf.append("</uastring><version>"+version+"</version><source>");
		buf.append(source);
		buf.append("</source><xroster>"+xroster+"</xroster>");

		if (carrier != null)// ��Ӫ��
		{
			buf.append("<carrier>"+carrier+"</carrier>");
		}
		buf.append("<capability>"+sendClientCapability+"</capability>");
		if(profile_portraitsize!=null)
		{
			buf.append("<profile_portraitsize>"+profile_portraitsize+"</profile_portraitsize>");
		}		
		if(contact_portraitsize!=null)
		{
			buf.append("<contact_portraitsize>"+contact_portraitsize+"</contact_portraitsize>");
		}	
		buf.append("<image_accept>image/png</image_accept>");
		if(deviceid!=null)
		{
			buf.append("<deviceid>"+deviceid+"</deviceid>");
		}
//		if(sync_profile!=null)
//		{
//			buf.append("<sync_profile>"+sync_profile+"</sync_profile>");
//		}
//		else
//		{
//			buf.append("<sync_profile/>");
//		}
		if(sync_contacts!=null)
		{
			buf.append("<sync_contacts>"+sync_contacts+"</sync_contacts>");
		}
		else
		{
			buf.append("<sync_contacts/>");
		}
		if(image_accept!=null)
		{
			buf.append("<image_accept>"+image_accept+"</image_accept>");
		}

		if(audio_accept!=null)
		{
			buf.append("<audio_accept>"+audio_accept+"</audio_accept>");
		}
		
		if(resourceid!=null)
		{
			buf.append("<profile_resourceid>"+resourceid+"</profile_resourceid>");

		}
		if(data_method!=null)
		{
			buf.append("<data_method>"+data_method+"</data_method>");
		}
		if(transparent!=null)
		{
			buf.append("<transparent>"+transparent+"</transparent>");
		}
		

		buf.append("</query></iq>");
		//addRequest("loginmsn", "/raw.xml", buf.toString());
		addRequest("", buf.toString());
	}
	

	public void run() 
	{
		try
		{
			try 
			{
				Thread.sleep(2000);
			} 
			catch (InterruptedException e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if (jabberServer == null || jabberServer.length() == 0)
			{
				jabberServer = MSNApplication.dispServer;
				jabberServerPort=MSNApplication.dispServerPort;
				dispatch();
			}
			if (jabberServer.equals("0.0.0.0"))
			{
				throw new Exception("0.0.0.0:0");
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
			if (e.getMessage() != null)
			{
				if(e.getMessage().equals("0.0.0.0:0")||e.getMessage().equals("error network"))
				{
					if(!cancelLogin)
					{
						eventListener.eventAction(EventListener.EVENT_NETWORK_FAILURE,null);
					}
				}
			}
			return;
		}
		try
		{
			//����ʾnotice

			if (noticeVector != null && noticeVector.size() > 0&&!cancelLogin)
			{
				eventListener.eventAction(EventListener.EVENT_DISPATCH_NOTICE,null);
				try
				{
					
					synchronized (this)
					{
						wait();
					}
					
				}catch(Exception e)
				{
					e.printStackTrace();
				}

			}

			if(result!=null&&result.equals("0"))//ϵͳ�������¼
			{
				eventListener.eventAction(EventListener.EVENT_DISPATCH_REFUSE_LOGIN,null);
				return;//ֱ���˻ص���¼ҳ��
			}
			else
			{	
				if (updateUrl != null && updateUrl.length() > 0&&!cancelLogin)// /�����汾
				{				
					eventListener.eventAction(EventListener.EVENT_DISPATCH_URL,null);

					try
					{
						
						synchronized (this)
						{
							wait();
						}
						
					}catch(Exception e)
					{
						e.printStackTrace();
					}
				}
				if(result!=null&&result.equals("1"))//ǿ������
				{
					eventListener.eventAction(EventListener.EVENT_DISPATCH_MUST_UPGRADE,null);						
					return;//ֱ���˻ص���¼ҳ��
				}
				
			}

		}
		catch (Exception e)
		{
			
			e.printStackTrace();
		}
		if(isUseSocket&&!cancelLogin)
		{
			eventListener.eventAction(EventListener.EVENT_RECEIVE_DISPATCH_OK,null);	
			startSocket();
		}
		else
		{
			try
			{
				if (!cancelLogin)
					connect();
			} 
			catch (Exception e)
			{
				e.printStackTrace();
				if (e.getMessage() != null)
				{
					if (e.getMessage().equals("0.0.0.0:0"))
					{
						if (!cancelLogin)
						{
							// �����������������жϣ������µ�¼
							eventListener.eventAction(
									EventListener.EVENT_NETWORK_FAILURE, null);
						}
					}
				}
				return;
			}
		}
		if(isUseSocket&&!cancelLogin)
		{
			
			try
			{
				
				new Thread()
				{
					public void run()
					{
						try
						{
							cycle();//��wap�͹��
						}
						catch(Exception e)
						{
							e.printStackTrace();
						}
					}
				}.start();
				
				if(!cancelLogin)
				{
			        Message msg = new Message(); 
			        msg.what=EventListener.EVENT_SEND_HAERT ; 
			        msn.GetMSNHandler().sendMessageDelayed(msg, 60000);		   
					parse();
					//msn.GetMSNHandler().removeMessages(EventListener.EVENT_SEND_HAERT);
					//msn.GetMSNHandler().removeMessages(EventListener.EVENT_SAVE_CONTACT_LIST);
				}
			} 
			catch (Exception e)
			{
				e.printStackTrace();
				Log.v("parse error=", "="+e.toString());
				eventListener.eventAction(EventListener.EVENT_RECEIVE_MSN_LOGOUT, null);
			}
		}
		else
		{
			try
			{
				//��ʼ���Ϳͻ��˵�¼��
				//sendIqAuth();
				sendIqAuth();
				cycle();
			} 
			catch (Exception e)
			{
				e.printStackTrace();
			}
			
		}
		try
		{
			if(isUseSocket)
			{

				if(socketConnection!=null)
				{
					try 
					{
						socketConnection.close();
					} 
					catch (IOException e1) 
					{
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					socketConnection=null;
				}
				if(reader!=null)
				{
					reader.close();
					reader=null;
				}
				if(writer!=null)
				{
					try 
					{
						writer.close();
					} 
					catch (IOException e1) 
					{
						e1.printStackTrace();
					}
					writer=null;
				}
			}
		} 
		catch (Exception e)
		{
			eventListener.eventAction(EventListener.EVENT_RECEIVE_MSN_LOGOUT, null);
			e.printStackTrace();
		}

	}
	
	/** ѭ���շ��� */
	public void cycle() throws Exception
	{
		long timerFetchCache = System.currentTimeMillis();
		boolean startGetAd = false;// �Ƿ�ɹ�ȡ��ҳ�Ź��
		long time_get_ad = 0;// ����ʱ��
		Request req;
		String[][] head;
		while (true)
		{
			req = null;
			/** ѭ��ȡ��� */
			if (adServer != null
					&& startGetAd
					&& (System.currentTimeMillis() - time_get_ad > Long
							.parseLong(msn.getAdInterval + "000")))
			{
				time_get_ad = System.currentTimeMillis();
				getAD(true, msn.windowWidth, msn.windowHeight);//ȡ���
			}
			try
			{
				if (requestQueue.size() > 0)
				{
					req = null;
					synchronized (requestQueue)
					{
						req = (Request) requestQueue.firstElement();
						requestQueue.removeElementAt(0);
					}

				} 
				else if (!isSendLogoutRequest && !isUseSocket)
				{
					//��������
					if (System.currentTimeMillis() - timerFetchCache > interval|| !cacheEmpty)
					{
						req = new Request("cache", "/cache.xml", null );
						timerFetchCache = System.currentTimeMillis();
					} else
					{
						Thread.sleep(200);
					}
				}
				if (req != null)
				{
					head = null;
					head= new String[][]{
							{"Accept","*/*"},
							{"Content-Type","*/*"},
							{"Pica-Clientver",MSNApplication.VERSION}};
					if (!this.isSendLogoutRequest ||( this.isSendLogoutRequest
							//&&req.type==Constant.JABBER_REQUEST&&req.command.equals("logoutmas")))
							&&req.type==MSNApplication.JABBER_REQUEST))
					{
						if (req.type==MSNApplication.JABBER_REQUEST)// ��ͨjabber��
						{
							doHttpConn(req.command, req.urlPage, req.content);
							if (!req.command.startsWith("cache"))
							{
								timerFetchCache = System.currentTimeMillis()- (interval - 4000);
							}
						} 
						else if (req.type==MSNApplication.AD_LOGIN_REQUEST)// ��¼ҳ����
						{
							try
							{
								FileConnection fileConnect = new FileConnection(null);

								
								byte[] file = fileConnect.getUrl(req.urlPage,head, true);
								if (file != null&&file.length!=3)
								{							
									MsnUtil.saveDataToFile(msn.RMS_AD_DATA,"login_ad.png",file);
								}
							} 
							catch (Exception e)
							{
								//msn.adLoginImage=null;
								e.printStackTrace();
							}
						} 
						else if(req.type==MSNApplication.HEAD_VCARD_REQUEST)//�Լ���ͷ��
						{
							try
							{
								FileConnection fileConnect = new FileConnection(null);
								
								byte[] headData = fileConnect.getUrl(req.urlPage,head, false);
								if (headData != null&&headData.length!=3)
								{							
									msn.myVCARD_headImageData=headData;
								}
								
							}
							catch(Exception e)
							{
								e.printStackTrace();
							}
						}
						else if(req.type==MSNApplication.HEAD_CONTACT_REQUEST)//��ϵ��ͷ��
						{
							try
							{
								FileConnection fileConnect = new FileConnection(null);
								byte[] headData = fileConnect.getUrl(req.urlPage,head, false);
								Contact contact=msn.getContact(req.imid, false);
								if(contact!=null)
								{
									if (headData != null&&headData.length!=3)
									{							
										contact.CONTACT_imageData=headData;
									}
								}

							}
							catch(Exception e)
							{
								e.printStackTrace();
							}
						}
						else if (req.type==MSNApplication.AD_ROSTER_REQUEST   // ��ϵ��ҳ�Ź��
								||req.type==MSNApplication.AD_MAIN_REQUEST    //��ҳ��ҳ��     
								||req.type==MSNApplication.AD_SESSION_REQUEST) //�Ự�б�ҳ��
						{
							try
							{
								FileConnection fileConnect = new FileConnection(null);
								byte[] file = fileConnect.getUrl(req.urlPage,head, true);
								if(file!=null&&file.length!=3)	
								{
									if(req.type==MSNApplication.AD_ROSTER_REQUEST)
									{
										MsnUtil.saveDataToFile(msn.RMS_AD_DATA,"roster_ad.png",file);
									}
									else if(req.type==MSNApplication.AD_MAIN_REQUEST)
									{
										MsnUtil.saveDataToFile(msn.RMS_AD_DATA,"main_ad.png",file);
									}
									else if(req.type==MSNApplication.AD_SESSION_REQUEST)
									{
										MsnUtil.saveDataToFile(msn.RMS_AD_DATA,"session_ad.png",file);
									}
									eventListener.eventAction(EventListener.EVENT_AD_UPDATA, null);
								}
								startGetAd = true;
								time_get_ad = System.currentTimeMillis();
							} 
							catch (Exception e)
							{
								e.printStackTrace();
							}
						} 
						else if(req.type==MSNApplication.AD_CLICK_REQUEST)
						{
							try
							{
								// ���ͳ��
								MsnUtil.httpRequest(req.urlPage, req.content,this.adServer,this.adServerPort);
							}
							catch(Exception e)
							{
								e.printStackTrace();
							}
						}
						else if(req.type==MSNApplication.ZIPWAP_REQUEST)
						{
							try
							{
								httpRequest(zipWapServer, req.urlPage,req.bytearrayoutputstream);
							}
							catch(Exception e)
							{
								e.printStackTrace();
							}
						}
						else if(req.type==MSNApplication.AD_GET_REQUEST)
						{
							String receiveAD = null;
							try
							{
								receiveAD = MsnUtil.httpRequest(req.urlPage, req.content,this.adServer,this.adServerPort);
								if (receiveAD != null && receiveAD.length() > 0)
								{
									receiveAD = receiveAD.substring(receiveAD.indexOf("?>") + 2);						
									msn.parser.parse(new XmlHttpReader(receiveAD), null);
								} 
								else
								{
									receiveAD = null;
									System.gc();
								}	
							}
							catch (Exception e)
							{
								e.printStackTrace();
							}
						}
					}
					if (req.command.equals("logoutmas"))
					{
						return;
					}
				}
				Thread.yield();
				Thread.sleep(50);
			} catch (Exception e)
			{
				e.printStackTrace();
				// e.printStackTrace();
				// ֻ����ȷ�����ӶϿ�ʱ���˳�
				//String eMsg = e.getMessage();
//				if (eMsg != null && eMsg.equals("disconnected") || (req != null&&req.type==Constant.JABBER_REQUEST&&req.command.equals("logoutmas")))
//					return;
				String eMsg = e.getMessage();
				if (eMsg != null && eMsg.equals("disconnected") )//|| (req != null&&req.type==Constant.JABBER_REQUEST&&req.command.equals("logoutmas")))
				{
					throw new Exception("disconnected");
				}
				
				if (req != null&&req.type==MSNApplication.JABBER_REQUEST&&req.command.equals("logoutmas"))
					return;

				// ʧ�ܵĻ������·ŵ�����ͷ
				if (req != null && !req.command.equals("cache"))
				{
					synchronized (requestQueue)
					{
						if (requestQueue.size() > 0)
						{
							Request first = (Request) requestQueue.firstElement();
							if (first.command.equals("login"))
								requestQueue.insertElementAt(req, 1);
						} else
							requestQueue.insertElementAt(req, 0);
					}
				}
			}
		}
	}
	
	private void httpRequest(String server, String urlPage,ByteArrayOutputStream bos)
	{
		DataInputStream dataIS = null;
		try
		{
			dataIS = sendData(server, urlPage, bos.toByteArray());	
			Object[] data = new Object[2];
			data[0] = (Object) dataIS;
			data[1] = new Integer(contentLength);
			eventListener.eventAction(EventListener.EVENT_ZIPWAP_OK, data);
		} catch (Exception e)
		{
			e.printStackTrace();
			eventListener.eventAction(EventListener.EVENT_ZIPWAP_ERROR, null);
		}
	}
	
	// sendData��������Ӧ�ĳ���
	private int contentLength;
	public boolean hasAgent;
	public String zipSessionID = null;
	public boolean noCache = false;
	
	private DataInputStream sendData(String server, String urlPage,
			byte[] postData) throws Exception
	{
		URL url = null;
		HttpURLConnection httpConn=null;
		DataOutputStream dataOS = null;
		DataInputStream dataIS = null;

		String URL = "http://" + server + urlPage;

		url = new URL(URL);
		if (Jabber.isUseSocket)
		{
			httpConn = (HttpURLConnection) url.openConnection();
		}
		else
		{
			if (Jabber.proxy == null)
			{
				Jabber.proxy = new Proxy(Proxy.Type.HTTP, new InetSocketAddress("10.0.0.172", 80));
			}
			
			httpConn = (HttpURLConnection) url.openConnection(Jabber.proxy);
		}
		if(postData!=null)
			httpConn.setRequestMethod("POST");		
		else
			httpConn.setRequestMethod("GET");

		httpConn.setRequestProperty("X-Online-Host", server);
		httpConn.setDoOutput(true);
		if (postData != null)
		{
				httpConn.setRequestProperty("Content-Type",
						"application/octet-stream");

		}
		if (msn_ua != null)// �����UA�������
			httpConn.setRequestProperty("User-Agent", msn_ua);
		else
			httpConn.setRequestProperty("User-Agent",
					"Profile/MIDP-1.0 Configuration/CLDC-1.0");
		if (hasAgent)
		{
				// Ϊ֧���������������Ӫ�����ݣ���HTTPͷ��Я��ͷ�ֶ�ZWP_IM_OPERATOR
				if(msn_carrier!=null)
					httpConn.setRequestProperty("ZWP_IM_OPERATOR",msn_carrier);
				
				httpConn.setRequestProperty("Pica-Auth", msn.getPicaNumber() + "/"
						+ msn.GetPicaPW());

				// Ҫ�������IM�汾��ÿһ����ʼ������ HTTP ͷ��Я���ֶ� ZWP_IM_ID��ֵΪ������ IM �û�����ʵ�ʺţ���
				// MSN Ϊ MSN �ʺš���PICAĬ�ϲ�Я�����ֶΡ�
				// �޸��� ֻ����һ�� ����������
				httpConn.setRequestProperty("ZWP_IM_ID", msn.GetLiveID()
						+ "/" + msn.GetPassWord());
				// �����������IM�汾��
				httpConn.setRequestProperty("ZWP_IM_VERSION", msn.VERSION);

				//����Pica-Auth�ĵط� һ�����ܳ���Pica-Session
				
				//if (zipSessionID != null)
				//	httpConn.setRequestProperty("Pica-Session", zipSessionID);	
		} 
		else
		{
			httpConn.setRequestProperty("Pica-Session", zipSessionID);
		}
		dataOS = new DataOutputStream(httpConn.getOutputStream());

		dataOS.write(postData);




		try
		{
			if (httpConn.getResponseCode() != HttpURLConnection.HTTP_OK)
			{
				 if(httpConn.getResponseCode()==480)
				 {
					 hasAgent =true;
				 } 
				 else 
				if (httpConn.getResponseCode() == 404)
					throw new Exception("error:404");

				throw new Exception("invalid response,code:"+ httpConn.getResponseCode());
			}
			
			contentLength = (int) httpConn.getHeaderFieldInt("Content-Length", 0);
			if(contentLength==0)
				contentLength = (int) httpConn.getHeaderFieldInt("Pica-Resplen", 0);
			

			
			msn.tempSave = !noCache;

			

			if (httpConn.getHeaderField("Cache-Control") != null
						&& httpConn.getHeaderField("Cache-Control").startsWith("no-cache"))
				noCache = true;
			else
				noCache = false;
			if (hasAgent)
			{
				zipSessionID = httpConn.getHeaderField("pica-session");
				if (zipSessionID != null)
					hasAgent = false;

			}
			dataIS = new DataInputStream(httpConn.getInputStream());	
			return dataIS;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			throw e;
		} 
/*		finally
		{
			if (dataOS != null)
			{
				try
				{
					dataOS.close();
				} catch (Exception e)
				{
					e.printStackTrace();
				}
			}

			if (httpConn != null)
			{
				try
				{
					httpConn.disconnect();
				} catch (Exception e)
				{
					e.printStackTrace();
				}
			}
			dataIS = null;
			dataOS = null;
			httpConn = null;
		}*/
	}
	
	public  void parseElement(DataInputStream stream, int contentLength) throws Exception
	{
	   int recvLength = 0; 
	   int elementLength = 0;
		   
		try
		{   
		  
			    short Header_id = stream.readShort();		    
			    elementLength = stream.readInt();
			    recvLength = recvLength + 6;
	   }
	   catch(Exception e)
	   {
		   Log.e("Parse Error", e.getMessage());
	       e.printStackTrace();
	       throw e;
	   }
	}
	
	//#ifdef support_head_image
	/**��ȡ��ϵ�˵�ͷ��*/
	public void getContactPortrait(String jid)
	{
	    StringBuffer buf=new StringBuffer();
	    buf.append("<iq type='get' to='msn.pica'>" +
	    		"<query xmlns='jabber:iq:roster' action='getportrait'><item jid='");
	    buf.append(MsnUtil.escapeString(jid));
	    buf.append("'/></query></iq>");
	    addRequest("",buf.toString());
	}
	//#endif
	
	
	/**
	 * ȡ���
	 * 
	 * @param onlyGetRosterAd
	 *            �Ƿ�ֻȡҳ�Ź��
	 * @param imid
	 * @param width
	 *            ��Ļ��
	 * @param height
	 *            ��Ļ��
	 */
	public void getAD(boolean isOnlyGetRosterAd, int width, int height)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<adreq from='");
		buf.append(MsnUtil.escapeString(msn.GetLiveID()));
		buf.append("' source='msn' screen='");
		buf.append(width + "*" + height + "' ");
		buf.append("media='png'>");
		
		if (!isOnlyGetRosterAd)//ȡҳ�Ź�� ��¼������Ϣ�ı����
		{
			if (msn.ad_chat_Vector.size() == 0)
			{
				buf.append("<item type='chat-header-ad' id='");
				buf.append(msn.AD_ID_chat_header);
				buf.append("' count='5'/>");
			}
			buf.append("<item type='login-prog-ad' id='");
			buf.append(msn.AD_ID_login_prog);
			buf.append("' count='1'/>");
		}
		buf.append("<item type='roster-footer-ad' id='");
		buf.append(msn.AD_ID_roster_footer);
		buf.append("' count='1'/>");
			
		buf.append("<item type='main-footer-ad' id='");
		buf.append(msn.AD_ID_main_footer);
		buf.append("' count='1'/>");
			
		buf.append("<item type='session-footer-ad' id='");
		buf.append(msn.AD_ID_session_footer);
		buf.append("' count='1'/>");

		buf.append("</adreq>");
		
		addGetAdRequest( buf.toString());
	}
	
	public void addGetAdRequest(String content)
	{
		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(content,MSNApplication.AD_GET_REQUEST));
		}
	}
	
    
    public void advistClick(String dbid, String sale)
    {
		StringBuffer buf = new StringBuffer();
		buf.append("<advisit from='");
		buf.append(MsnUtil.escapeString(msn.GetLiveID()));
		buf.append(" as in adreq' source='msn' ");
		buf.append("dbid='" + dbid);
		buf.append("' sale='" + sale + "'/>");
    	addAdvistRequest(buf.toString());
    }
	
	/**�����*/
	public void addAdvistRequest(String content)
	{
		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(content,MSNApplication.AD_CLICK_REQUEST ));
		}
	}
	
	
	public static long timeLast = 0;
	public void parse() throws Exception
	{
		try
		{
			int a = 10000;
			//while (reader.next() == XmlReader.START_TAG)
			while((a= reader.next()) != XmlReader.END_DOCUMENT)
			{
				lastActiveTime = System.currentTimeMillis();
				String tmp = reader.getName();
				if(tmp==null)
					return;
				if(cancelLogin)
					return;
				if (tmp.equals("message"))
				{
					msn.parser.parseMessage(reader);
				} 
				else if (tmp.equals("presence"))
				{
					msn.parser.parsePresence(reader);
				} 
				else if (tmp.equals("iq"))
				{
					msn.parser.parseIq(reader);
					
				} 
				else if(tmp.equals("stream:stream"))
				{
					msn.parser.parseStream(reader);
				}
				else
				{
					msn.parser.parseIgnore(reader);
				}
				if(MSNApplication.hasContactOnline)//������ϵ�˵�¼
				{
					eventListener.eventAction(EventListener.EVENT_SOUND_NEW_ONLINE,null);	
				}
				
				//System.gc();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
			throw e;
		}
		finally
		{
			reader.close();
			writer.close();

			reader = null;
			writer = null;
		}
	}
	

	/***
	 * ��ȡPICA�����б�<br>
	 * �������ذ� <iq type='result'><query xmlns='jabber:iq:roster'/></iq> <br>
	 * �ͻ��˲����ٴ���
	 */
	public void getIqRoster()
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<iq type='get'><query xmlns='jabber:iq:roster'/></iq>");
		addRequest("",buf.toString());
	}
	
	/**��������״̬   
	 *���������� <presence type=��available' from=�� '78276741@pica/pica��/>
	 *�ͻ��˸���from���жϴ�presence������pica����msn
	 * */
	public void sendPresenceAvailable()
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<presence type='available'/>");
		addRequest("",buf.toString());
	}
	
	public void setCacheStatus(boolean empty)
	{
		cacheEmpty = empty;
	}
	
	public void sendSocketLogout()
	{
		addRequest("close","</stream:stream>");
	}
	
	

	/** ����dispatch������<br>
	 *  type socket��s http��h*/
	public void dispatch()throws Exception
	{
		StringBuffer sb = new StringBuffer();
		sb.append("imid=");
		sb.append(MSNApplication.getApplication().GetLiveID());
		sb.append("&version=");
		sb.append(MSNApplication.VERSION);
		if (isUseSocket)
		{
			sb.append("&type=s&source=");
		}
		else
		{
			sb.append("&type=h&source=");
		}
		
		sb.append(MSNApplication.SOURCE);
		sb.append("&oem=kuzhan");//�ƹ���������վ��¼���İ汾
		if(msn.GetIMSI()!=null)
		{
			sb.append("&imsi=");
			sb.append(msn.GetIMSI());
		}

		doHttpConn("dispatch", "/dispatch.xml?", sb.toString());
	}
	
	/** ���ӷ����� ��ȡsid <br>
	 * http����ʱ����Ҫ*/
	public void connect()throws Exception // ///////���޸ģ������
	{
		doHttpConn("login", "/login-sid.xml?" + "jid=" + MSNApplication.getApplication().GetPicaID()
					+ "&pass=" +MSNApplication.getApplication().GetPicaPW() + "&version="
					+ MSNApplication.VERSION, null);
	}
	
	public void doHttpConn(String command, String urlPage, String content)throws Exception 
	{
		String response = null;

		if (content != null && !command.equals("dispatch")) 
		{
			content = ((new StringBuffer("<jabber>")) + content + "</jabber>").toString();
		}

		if (!command.equals("login") && !command.equals("dispatch")) 
		{
			urlPage = ((new StringBuffer(urlPage)) + "?sid=" + session_id).toString();
		}
		for (int i = 0; i < 36; i++) 
		{
			try 
			{
				response = MsnUtil.httpRequest(urlPage, content,this.jabberServer,this.jabberServerPort);
				if (command.equals("dispatch")) 
				{
					//response="<sso result='0'><system><notice>ϵͳ��ʾ��Ϣ1</notice><notice>ϵͳ��ʾ��Ϣ2</notice><notice>ϵͳ��ʾ��Ϣn</notice></system><upgrade force='0'><notice>������ʾ����</notice><version>�¿ͻ��˰汾��6.0.0</version><url>http://wap.msn.cn</url></upgrade><login><id>67890001@pica</id><password>666779</password><ip>211.99.191.23:35556</ip></login><config></config><transparent></transparent></sso>";								
					//response="<sso result='0'><system><notice>ϵͳ��ʾ��Ϣ1</notice><notice>ϵͳ��ʾ��Ϣ2</notice><notice>ϵͳ��ʾ��Ϣn</notice></system><upgrade force='0'><version>�¿ͻ��˰汾��6.0.0</version><url>http://wap.msn.cn</url></upgrade><login><id>67890001@pica</id><password>666779</password><ip>211.99.191.23:35556</ip></login><config></config><transparent></transparent></sso>";	
 					msn.parser.parseDispatchResponse(response, urlPage, content);
				}
				else 
				{
					if (response.length() > 0) 
					{
						msn.parser.parse(new XmlHttpReader(response), command);
					}
				}
				break;
			} 
			catch (Exception e) 
			{
				 e.printStackTrace();
				if (command != null&& (command.equals("login") || command.equals("dispatch"))) 
				{
					i = 1000000; // not continue
					if (jabberServer != null && jabberServer.equals("0.0.0.0"))
						throw new Exception("0.0.0.0:0");
					else
						throw new Exception("error network");
					//throw new Exception("��������ʧ��");
				} 
				else if (i == 35)// || jabberError)
				{
					if (i == 35)
					{
						for (int j = 0; j < 20 && !isSendLogoutRequest; j++)
						{
							Thread.sleep(500); // reconnect after 10sec
						}
					}
					throw new Exception("disconnected");
				}

				if (isSendLogoutRequest)
				{
					return;
				}
				for (int j = 0; j < 20 && !isSendLogoutRequest; j++)
				{
					Thread.sleep(500); // reconnect after 10sec
				}

			}
		}
    }
	

	
	/** �����õ�sid */
	public void setSessionId(String sid)
	{
		session_id = sid;
	}
	
	private void addRequest(String command, String content)
	{
		if(isUseSocket)
		{
		    try
		    {
			    writer.write(content.getBytes("UTF-8"));
			    writer.flush();
		    }catch(Exception e)
		    {
		    	e.printStackTrace();
		    }
		}
		else
		{
			synchronized (requestQueue)
			{
				String urlPage=null;
				if(command.equals("logoutmas"))
					urlPage="/logout-sid.xml";
				else
					urlPage="/raw.xml";
				requestQueue.addElement(new Request(command, urlPage, content));
			}
		}
	}
	
	/**��ʼ���������շ��߳�*/
	public boolean startSocket()
	{
		try
		{
			InetAddress serverAddr = InetAddress.getByName(jabberServer);     	 
	   	 	Integer port = new Integer(jabberServerPort);
	   	 	Log.v("port", ""+port);
	   	 	Log.v("serverAddr", ""+serverAddr);
	   	 	socketConnection = new Socket(serverAddr, port.intValue());  	 	
			reader = new XmlSocketReader(socketConnection.getInputStream());
			writer = socketConnection.getOutputStream();
			sendStream();
		}
		catch(Exception e)
		{
			e.printStackTrace();
			if(socketConnection!=null)
			{
				try {
					socketConnection.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				socketConnection=null;
			}
			if(reader!=null)
			{
				reader.close();
				reader=null;
			}
			if(writer!=null)
			{
				try {
					writer.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				writer=null;
			}
			return false;
		}

		return true;
	}
	
	/**socket�ĵ�һ������*/
	public void sendStream() throws Exception
	{
		String sbf="<?xml version='1.0'?>" +
		"<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' to='pica'>"
		+"<iq type='set'><query xmlns='jabber:iq:auth'><username>"+
		MSNApplication.getApplication().getPicaNumber()+"</username><resource>"+
		"pica"+"</resource><password>"+
		MSNApplication.getApplication().GetPicaPW()+"</password><version>"+
		MSNApplication.VERSION+"</version><source>"+
		MSNApplication.SOURCE+"</source></query></iq>";
		
		writer.write(sbf.getBytes("UTF-8"));
		writer.flush();
	}
	
	/**�ͻ��˵�¼
	 * �������<iq type=��result��>
		<query xmlns='jabber:iq:auth'/>
		</iq>
		���ʾ�ɹ�
	 * */
	private void sendIqAuth()throws Exception
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set'><query xmlns='jabber:iq:auth'><username>");
		buf.append(MSNApplication.getApplication().getPicaNumber());
		buf.append("</username><password>");
		buf.append(MSNApplication.getApplication().GetPicaPW());//pica����
		buf.append("</password><version>");
		buf.append(MSNApplication.VERSION);
		buf.append("</version><source>");
		buf.append(MSNApplication.SOURCE);
		buf.append("</source></query></iq>");
		addRequest("", buf.toString());
	}
	
	/** �ж��Ƿ��Ѿ���ȡ����sid�Ự�� */
	boolean isSessionOpen()
	{
		return session_id != null;
	}
	
	/**socket���� ÿ60�뷢�������� ����һ���հ�*/
	public void sendSpace()
	{
		if (isSendLogoutRequest||forceStop)
			return;
		
		if(msn == null || msn.conMgr == null)
		{
		    if(msn.GetMSNHandler().hasMessages(EventListener.EVENT_SEND_HAERT))
		    msn.GetMSNHandler().removeMessages(EventListener.EVENT_SEND_HAERT);
		}
		
        NetworkInfo info = msn.conMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        if(info != null)
        {
            if(!info.isConnected())
            {
                info = msn.conMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
                if(info != null)
                {
                    if(!info.isConnected())
                    {
                        if(msn.GetMSNHandler().hasMessages(EventListener.EVENT_SEND_HAERT))
                            msn.GetMSNHandler().removeMessages(EventListener.EVENT_SEND_HAERT);
                        eventListener.eventAction(EventListener.EVENT_RECEIVE_MSN_LOGOUT, null);
                        return;
                    }
                }
            }
        }

		try
		{
			addRequest(""," ");
	        Message msg = new Message(); 
	        msg.what=EventListener.EVENT_SEND_HAERT ; 
	        msn.GetMSNHandler().sendMessageDelayed(msg, 60000);
		}
		catch(Exception e)
		{
			msn.removeContactGroupVersionFromRMS();
    		msn.saveAllContactListToFile(msn.GetLiveID());
    		msn.setContactGroupVerToRMS(msn.myContactGroupVersion);
			sendLogoutMsn(false);
			e.printStackTrace();
		}
	}
	
	/**���� �����ϵ��
	 * @param imid ҳ��������� �������ϵ�˵�imid ����ϵ�˵�CONTACT_imid<br>
	 * */
	public void sendAddContact(String imid)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:iq:roster' action='add' version='");
		buf.append(msn.myContactGroupVersion);
		buf.append("'>");
		buf.append("<item imid='");
		buf.append(MsnUtil.escapeString(imid));
		buf.append("'/></query></iq>");
		addRequest("", buf.toString());
	}
	
	/**
	 * ɾ��/ɾ������ֹ/��ֹ/�����ֹ��ϵ��
	 * @param String jid ��ϵ�˵�jid  ����ϵ�˵�SUBITEM_JID<br>
	 * @param String action ��������: ɾ��delete/��ֹblock/ɾ������ֹrmblk/�����ֹunblock
	 */
	public void sendOperateContact(String jid, String action)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:iq:roster' action='");
		buf.append(action);
		buf.append("' version='");
		buf.append(msn.myContactGroupVersion);
		buf.append("'><item jid='");
		buf.append(MsnUtil.escapeString(jid));
		buf.append("'/></query></iq>");	
		addRequest("", buf.toString());
	}
	
	/**
	 * �޸ĸ������� �û��ı��Լ����ǳƺ��������
	 * @param String newMyNickname �ǳ�  ԭ�ǳ�Ϊ msn.myVCARD_myNickname
	 * @param String newMyImpresa ������� ԭ������� msn.myVCARD_myImpresa
	 * @param boolean isChangeNickname �Ƿ�ı����ǳ�
	 * @param boolean isChangeImpresa �Ƿ�ı����������
	 */
	public void sendModifyMyVcard(String newMyNickname,String newMyImpresa,boolean isChangeNickname,boolean isChangeImpresa)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='vcard-temp'>");
//		buf.append("<iq type='set' to='msn.pica'><query xmlns='vcard-temp' version='");
//		buf.append(msn.myVcardVersion);
//		buf.append("'>");
		if(isChangeNickname)
		{
			buf.append("<nickname>");
			buf.append(MsnUtil.escapeString(newMyNickname));
			buf.append("</nickname>");
		}
		if(isChangeImpresa)
		{
			buf.append("<impresa>");
			buf.append(MsnUtil.escapeString(newMyImpresa));
			buf.append("</impresa>");
		}
		buf.append("</query></iq>");
		addRequest("", buf.toString());
	}
	
	/**
	 * ���Է����Ϊ��ϵ�ˣ����յ���֤���󣩵�ѡ����� ���� 
	 * @param String handleType=accept(���ܶԷ�����) deny(�ܾ��Է�����)
	 * @param String imid �Է���imid��
	 */
	public void sendAcceptOrDenyVerifyFriend(String handleType,String imid)throws Exception 
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:iq:roster' action='subscriptionanswer'>"+
		           "<item imid='"+MsnUtil.escapeString(imid)+"' subscription='"+handleType+"'/>"+
		           "</query></iq>");
		addRequest("",buf.toString());
	}
	
	/**
	 * ��Э����ע��MSN�ͻ��˵�����<br>
	 * �˳�����ע������ͬ����ϵ��������� ��Ҫ���ô˽ӿ�
	 *@param boolean isExit �Ƿ����˳����� ����msn.isLogout
	 */
	public void sendLogoutMsn(boolean isExit)
	{
		try
		{
			String s = "<iq type='set' to='msn.pica'><query xmlns='jabber:iq:register'><remove/></query></iq>";
			addRequest("", s);
			Thread.sleep(200);
			if (isExit)// ������˳����� ����������logout
			{
				sendEnd();
			}
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	/** �˳����� ����logout�Ͽ�socket���� */
	public void sendEnd()
	{
		if (!isSessionOpen()||isSendLogoutRequest)// ��û�еõ�sid ˵����û�н��뵽ѭ��
			return;
		sendSocketLogout();
		isSendLogoutRequest = true;
	}
	
	/** ��¼�ȴ������е��ȡ�� */
	public void sendCancelLogout()
	{
		try
		{
			if (isSessionOpen())
			{
				sendSocketLogout();
				isSendLogoutRequest = true;
			} 
			else
			{
				forceStop = true;
			}
		} catch (Exception e)
		{
			e.printStackTrace();

		}
	}
	
	/**
	 * �ı��Լ���״̬
	 * @param String show ���͸���������״̬�ֶ� ֵΪMSNApplication.STATE_FLAG_All[i]
	 * */
	public void sendModifyMyState(String show)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<presence type='available' to='msn.pica'><show>");
		buf.append(show);
		buf.append("</show></presence>");
		addRequest("",  buf.toString());
	}
	
	/**
	 * һ��һ ����ϵ�˷�����Ϣ
	 * @param String contactJidorImid ��������ϵ�˵�jid Ҳ������İ���˵�imid
	 * @param String body ������Ϣ������
	 * @param String sid ��ǰ�Ự�ĻỰid
	 * @param boolean isFriend true��jid|false��imid
	 * */
	public void sendMessageRequest(String contactJidorImid,String body,String sid,boolean isFriend)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<message to='msn.pica' ");
		if(isFriend)
		{
			buf.append("jid='");
			buf.append(MsnUtil.escapeString(contactJidorImid));
			buf.append("' type='chat' sid='");
		}
		else 
		{
			buf.append("imid='");
			buf.append(MsnUtil.escapeString(contactJidorImid));
			buf.append("' type='chat' sid='");		
		}
		buf.append(sid);
		buf.append("'><body>");
		buf.append(MsnUtil.escapeString(body));
		buf.append("</body></message>");
		addRequest("", buf.toString());
	}
	
	//#ifdef support_groupmessage
	/**
	 * ����ʱ��Ⱥ������Ϣ
	 * */
	public void sendGroupMessageRequest(String body,String sid)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<message to='msn.pica'");
		buf.append(" type='groupchat' sid='");		
		buf.append(sid);
		buf.append("'><body>");
		buf.append(MsnUtil.escapeString(body));
		buf.append("</body></message>");
		addRequest("",buf.toString());
	}
	
	/**�رջỰ*/
	public void sendCloseSession(String sid)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:x:dialog' sid='");
		buf.append(sid);
		buf.append("' action='end'/></iq>");
		addRequest("",  buf.toString());
	}
	
	/**������ϵ�˼���Ի�*/
	void sendInviteContactAddSession(String jid,String imid,String sid, String[] chaterJid)
	{
	    StringBuffer buf = new StringBuffer();
	    buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:x:dialog' sid='");
	    buf.append(sid);
	    buf.append("' action='add'>");
	    if(jid!=null||imid!=null)
	    {
		    buf.append("<control><firstparticipant ");
		    if(jid==null)//İ����
		    {
		    	buf.append("imid='");
		    	buf.append(MsnUtil.escapeString(imid));
		    }
		    else
		    {
		    	buf.append("jid='");
		    	buf.append(MsnUtil.escapeString(jid));
		    }
	    	buf.append("'/></control>");
	    }
	    ////////////
	    for(int i=0; i<chaterJid.length; i++)
	    {
	        buf.append("<participant jid='");
	        buf.append(chaterJid[i]);
	        buf.append("'/>");
	    }
	    buf.append("</query></iq>");	    
	    addRequest("",  buf.toString());
	}
	
	//#endif
	

	///////////////////////////////////////////////////////////////////////////////////
	///////�ļ����������书��		
	//////////////////////////////////////////////////////////////////////////////////
	
	/**
	 * �����ļ�������
	 * @param ftid �ͻ������ɵ� ����ʱ�� ȷ�����ظ�
	 * @param name �ļ�����
	 * @param size �ļ���С���ֽ�����
	 */
	public void sendTransferInvite(String jid,String sid,String ftid,String name,String size)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' jid='");
		buf.append(MsnUtil.escapeString(jid));
		buf.append("' type='ft' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' action='push'><name>");
		buf.append(MsnUtil.escapeString(name));
		buf.append("</name><size>");
		buf.append(MsnUtil.escapeString(size));
		buf.append("</size></control></message>");
		addRequest("",buf.toString());
	}
	
	
	/**
	 * send transfer invite response �����ļ��Ľ��շ��Ļ�Ӧ
	 * @param transferid
	 * @param sid
	 * @param ftID
	 * @param action ��accept ���� | reject �ܾ�
	 */
	public void sendTransferInviteResponse(String transferid,String sid,String ftID,String action)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft' transferid='");
		buf.append(MsnUtil.escapeString(transferid));
		buf.append("' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftID));
		buf.append("' action='");
		buf.append(action);
		buf.append("'></control></message>");
		addRequest("",buf.toString());
	}
	
	/**
	 * �����ļ������ڷ�ʽ��
	 * @param transferid
	 * @param sid
	 * @param ftid
	 * @param seqid
	 * @param totalLength �ļ��ܳ���
	 * @param startIndex ������ʼλ��
	 * @param endIndex ���ݽ���λ��
	 * @param base64Length Base64���볤��
	 * @param base64 Base64���ݿ�
	 */
	public void sendTransferInner(String transferid,String sid,String ftid,String seqid,
			String totalLength,String startIndex,String endIndex,String base64Length,String base64)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft' transferid='");
		buf.append(MsnUtil.escapeString(transferid));
		buf.append("' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' seqid='");
		buf.append(seqid);
		buf.append("' action='data'><data bytes='");
		buf.append(totalLength);
		buf.append("' start='");
		buf.append(startIndex);
		buf.append("' end='");
		buf.append(endIndex);
		buf.append("' length='");
		buf.append(base64Length);
		buf.append("'>");
		buf.append(base64);
		buf.append("</data></control></message>");
		addRequest("",buf.toString());
	}
	
	/**
	 * ���ͷ������ļ�����
	 * @param transferid
	 * @param sid
	 * @param ftid
	 */
	public void sendTransferInnerComplete(String transferid,String sid,String ftid)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft' transferid='");
		buf.append(MsnUtil.escapeString(transferid));
		buf.append("' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' action='complete'></control></message>");
		addRequest("",buf.toString());
	}
	
	/**���շ������ļ�����*/
	public void sendTransferInnerReceiveComplete(String transferid,String sid,String ftid)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft' transferid='");
		buf.append(MsnUtil.escapeString(transferid));
		buf.append("' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' action='complete_ack'></control></message>");
		addRequest("",buf.toString());
	}
	
	/**
	 * �����ļ������ⷽʽ��
	 * @param transferid
	 * @param sid
	 * @param ftid
	 * @param url ��ȡ���ļ��ĵ�ַ
	 */
	public void sendTransferFileOuterClose(String transferid,String sid,String ftid,String url)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft' transferid='");
		buf.append(MsnUtil.escapeString(transferid));
		buf.append("' sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' action='complete'/><url>");
		buf.append(MsnUtil.escapeString(url));
		buf.append("</url></message>");
		addRequest("",buf.toString());
	}
	

	
	
	/**
	 * ȡ���ļ��Ĵ���
	 * @param transferid
	 * @param sid
	 * @param ftid
	 */
	public void sendTransferCancel(String transferid,String sid,String ftid)
	{
		StringBuffer buf=new StringBuffer();
//		buf.append("<message to='msn.pica' type='ft' transferid='");
//		buf.append(MsnUtil.escapeString(transferid));
		
		buf.append("<message to='msn.pica' type='ft'");//transferid='");
		if(transferid!=null&&transferid.length()!=0)
		{
			buf.append(" transferid='");
			buf.append(MsnUtil.escapeString(transferid));
			buf.append("' sid='");
		}
		else
			buf.append(" sid='");
		buf.append(sid);
		buf.append("'><control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' action='cancel'></control></message>");
		addRequest("",buf.toString());
	}
	
	/**
	 * ������������
	 * @param jid
	 * @param sid
	 * @param mime  audio/wav|audio/amr|audio/qcp
	 * @param bytesLength �ļ��ܳ���
	 * @param base64Length Base64���볤��
	 * @param base64 Base64����ͷ���ļ�����
	 * @param url ���ⷽʽ�ϴ���url
	 * @param isInner �Ƿ��Ǵ���
	 */
	public void sendVoiceClip(String jid,String sid,String mime,String bytesLength,String base64Length,
			String base64,String url,boolean isInner)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' jid='");
		buf.append(jid);
		buf.append("' type='voiceclip' sid='");
		buf.append(sid);
		buf.append("'>");
		if(isInner)
		{
			buf.append("<data mime='");
			buf.append(mime);
			buf.append("' bytes='");
			buf.append(bytesLength);
			buf.append("' lenght='");
			buf.append(base64Length);
			buf.append("'>");
			buf.append(base64);
			buf.append("</data>");
		}
		else
		{
			buf.append("<url mime='");
			buf.append(mime);
			buf.append("'>");
			buf.append(url);
			buf.append("</url>");
		}
		buf.append("</message>");
		addRequest("",buf.toString());
		
	}
	
	/**���� �����ļ���Ļ�Ӧ��*/
	public void sendTransferInnerResponse(String transferid,String sid,String ftid,String seqid)
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<message to='msn.pica' type='ft'");
		if(transferid!=null&&transferid.length()!=0)
		{
			buf.append(" transferid='");
			buf.append(MsnUtil.escapeString(transferid));
			buf.append("' sid='");
		}
		else
			buf.append(" sid='");
		buf.append(sid);
		buf.append("'>");
		buf.append("<control ftid='");
		buf.append(MsnUtil.escapeString(ftid));
		buf.append("' seqid='");
		buf.append(MsnUtil.escapeString(seqid));
		buf.append("' action='ack' status='ok'>");	
		buf.append("</control></message>");
		addRequest("",buf.toString());
	}
	
	/** ��¼����ͼƬ��� */
	public void addLoginADRequest(String urlPage)
	{
		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(urlPage));
		}
	}

	/** ҳ��ͼƬ��� */
	public void addFootADRequest(byte type,String urlPage, String id,
			String action, String target, String param0, String src,
			String dbid, String sale)
	{
		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(type,urlPage, id, action, target, param0, src, dbid, sale));
		}
	}
	
	/**
	 * �޸ĸ������� �û��ı��Լ���ͷ��
	 * @param String mime ͷ���ļ���MIME���ͣ�����JPG
	 * @param String base64Length Base64���볤��
	 * @param String base64Data Base64����ͷ���ļ�����
	 * @param String url �Ƿ�ı����������
	 */
	public void sendModifyMyHead(String mime,String base64Length,String base64Data,String url)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='vcard-temp'>");
		buf.append("<portrait mime='");
		buf.append(MsnUtil.escapeString(mime));
		buf.append("'>");
		if(url!=null&&url.length()!=0)
		{
			buf.append("<url>");
			buf.append(MsnUtil.escapeString(url));
			buf.append("</url>");
		}
		else
		{
			buf.append("<data length='");
			buf.append(base64Length);
			buf.append("'>");
			buf.append(base64Data);
			buf.append("</data>");
		}
		buf.append("</portrait></query></iq>");
		addRequest("", buf.toString());
	}
	
	/**
	 * ������wap��Դ
	 * 
	 * @param url
	 *            ��������Դ��URL��ַ
	 * @param nvPair
	 *            �ύ����-ֵ�Զ�
	 * @param useAgent
	 *            �Ƿ���ӿͻ��˴�������ͷ
	 * @param byte
	 *            0: get����1:post����
	 */
	public void sendZipwapRequest(String url, Vector nvPair,byte method, String strNickname, short[] agent)
	{
		try
		{
	        //��һ�ε�¼ �ͳ�ʱ15��������  �ͻ��˴���Pica-auth�ֶ� ��Я�������������ݰ�
	        //��һ�� System.currentTimeMillis() - firstTime�϶�����900000 15����
	        if(this.hasAgent || (System.currentTimeMillis() - firstTime >= 900000))
	        {
	        	this.hasAgent = true;
	        	firstTime = System.currentTimeMillis();	        	
	        }
			
			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			DataOutputStream dos = new DataOutputStream(bos);
			if(this.hasAgent)
			{
				String agent_version = "j2meV0.1";

				String images = "png";

				String audios = "amr;midi;wav";
				int len = 22 + 2 + agent_version.getBytes("UTF-8").length + 2
						+ images.getBytes("UTF-8").length + 2
						+ audios.getBytes("UTF-8").length + 2
						+ strNickname.getBytes("UTF-8").length;

				dos.writeShort(0x0001); // id
				dos.writeInt(len); // ���ݰ���

				dos.writeByte(1); // ƽ̨����
				dos.writeShort(0x05); // ֧��Э��汾 0.5�汾Ϊ0x05
				dos.writeUTF(agent_version);
				dos.writeShort(agent[0]); // ��Ļ���ؿ�
				dos.writeShort(agent[1]); // ��Ļ���ظ�
				dos.writeShort(agent[2]); // �������ʾ��
				dos.writeShort(agent[3]);// �������ʾ��
				dos.writeByte(agent[4]);// Ӣ�������
				dos.writeByte(agent[5]);// Ӣ�������
				dos.writeByte(0xff);// ��Ļɫ��λ��
				dos.writeUTF(images);// ֧��ͼ���ʽ
				dos.writeUTF(audios);// ֧�ֲ�����Ƶ��ʽ
				dos.writeByte(0);// ֧��cookie
				dos.writeByte(0);// ���ظ���
				dos.writeByte(0);// ¼��
				dos.writeByte(0);// ����
				dos.writeByte(1);// ϵͳ�����
				dos.writeByte(1);// ����绰
				dos.writeByte(1);// ������
				dos.writeByte(1);// ��PICA��Ϣ
				dos.writeUTF(strNickname);// �û��ǳ� 327
			}

			int max_page_zise = 1000;

			dos.writeShort(0x0002);

			if (nvPair != null)
			{// ������Ҫ�ύ�ı���ֵ��
				int len = 0;
				for (int i = 0; i < nvPair.size(); i++)
				{
					len += ((String[]) nvPair.elementAt(i))[0].getBytes("UTF-8").length
							+ 2
							+ ((String[]) nvPair.elementAt(i))[1]
									.getBytes("UTF-8").length + 2 + 1;
				}
				dos.writeInt(8 + len + url.getBytes("UTF-8").length + 2); // ע��õ�utf���ĳ��Ȼ�Ҫ�����ֽڵ�ͷ����
				dos.writeByte(method); // post
				dos.writeUTF(url);
				dos.writeInt(max_page_zise);
				dos.writeByte(0);
				dos.writeByte(0);
				// ������
				dos.writeByte(nvPair.size());
				for (int j = 0; j < nvPair.size(); j++)
				{
					dos.writeUTF(((String[]) nvPair.elementAt(j))[0]);
					dos.writeByte(0);
					dos.writeUTF(((String[]) nvPair.elementAt(j))[1]);
				}

			} else
			{
				dos.writeInt(8 + url.getBytes("UTF-8").length + 2);
				dos.writeByte(0); // get
				dos.writeUTF(url);
				dos.writeInt(max_page_zise);
				dos.writeByte(0);
				dos.writeByte(0);
				dos.writeByte(0);

			}
			addZipWapRequest("", bos);
		} 
		catch (Exception e)
		{
			//e.printStackTrace();
		}
	}
	
	private void addZipWapRequest(String urlPage, ByteArrayOutputStream bos)
	{

		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(urlPage, bos)); 
		}
		
	}
}
