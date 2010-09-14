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
	/**客户端dispatch服务器分配的jabber服务器地址*/
	public String jabberServer;
	/**客户端dispatch服务器分配的jabber服务器端口*/
	public String jabberServerPort;
	
	public static boolean isUseSocket=true;
	public static Proxy proxy;//http连接需使用代理
	private MSNApplication msn;
	private String session_id;
	private long firstTime = 0;
	/**dispatch返回的notice(system notice在最前面)按顺序存在这个向量中*/
	public Vector noticeVector=new Vector();
	public String force=null;//froce=0/1 建议升级/强制升级
	public String result=null;//0/1/2 不许登 /强制升级 /成功
	public Vector requestQueue= new Vector();
	String updateVersion = "";
	String updateUrl = "";
	ParserNotifier parserNotifier;
	EventListener eventListener;
	/////////////
	private Socket socketConnection ;  
	XmlReader reader;
	OutputStream writer;
	public long lastActiveTime = 0;//最后一次收到包的时间
	//////////////
	public boolean receive_logout_unavailable;// 是否收到注销返回包
	
	/** 是否发注销包 */
	public boolean isSendLogoutRequest;
	private boolean cacheEmpty;
	/** 强制退出收发包函数 */
	public boolean forceStop;

	public String zwp_addFriendUrl = null,// msn.cn用户添加联系人的地址
			msn_ua = null,// UA
			msn_carrier = null,// 运营商
			transParent = null;// dispatch返回的配置给网关
	
	/**类wap服务器地址*/
	public String zipWapServer=null;
	
	/**广告服务器地址*/
	public String adServer = null;
	/**广告服务器端口*/
	public String adServerPort=null;

	
	/**是否点击取消登录*/
	public boolean cancelLogin=false;
	
	private int interval = 10000;
	
	
	Jabber(ParserNotifier parserNotifier,EventListener el) 
	{
		this.parserNotifier=parserNotifier;
		eventListener=el;
		msn=MSNApplication.getApplication();
		msn.parser=new Parser(parserNotifier);
		new Thread(this).start();//开启联网线程
	}
	
	/**
	 * 发起登录MSN的请求（登录msp） 发送email和密码，下载联系人数量，版本号 jabber:iq:register
	 * @param username 用户email
	 * @param password 密码
	 * @param show  状态
	 * @param uastring 
	 * @param version
	 * @param source
	 * @param xroster all
	 * @param carrier
	 * @param capability
	 * @param profile_portraitsize 自己头像显示的大小-整数  如果支持头像，需要带此节点
	 * @param contact_portraitsize 联系人头像显示的大小-整数 如果支持头像，需要带此节点
	 * @param deviceid  由网关生成，本地保存；下次登录时带上，和同步版本类似
	 * @param sync_contacts  联系人同步版本
	 * @param resourceId 个人资料的resourceid 如果本地没有此参数，则不带
	 * @param transparent 透传dispatch返回的配置给网关
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
		//android版支持 :
		//0.多人会话（临时群组）      							支持
		//1.支持头像                                                                                              	          支持
		//2.支持文件传送                     							  支持
		//3.支持录音剪辑									 支持
		//4.支持接收nudge                             	         支持
		//5.支持本地保存联系人和分组							支持
		//6.支持接收联系人头像变化（仅指presence）                            取决diaptch和本地RMS设置
		//7.支持接收联系人个人信息变化（仅指presence）                 取决diaptch和本地RMS设置
		//8.支持离线消息										取决diaptch
		//9.支持编辑和显示联系人昵称							取决diaptch
		
		char[] capabilityCharArray=new char[32];
		for(int i=capabilityCharArray.length-1;i>=0;i--)//初始值 即"0000003F" 
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

		if (carrier != null)// 运营商
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
			//先显示notice

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

			if(result!=null&&result.equals("0"))//系统不允许登录
			{
				eventListener.eventAction(EventListener.EVENT_DISPATCH_REFUSE_LOGIN,null);
				return;//直接退回到登录页面
			}
			else
			{	
				if (updateUrl != null && updateUrl.length() > 0&&!cancelLogin)// /升级版本
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
				if(result!=null&&result.equals("1"))//强制升级
				{
					eventListener.eventAction(EventListener.EVENT_DISPATCH_MUST_UPGRADE,null);						
					return;//直接退回到登录页面
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
							// 由于网络问题连接中断，请重新登录
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
							cycle();//类wap和广告
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
				//开始发送客户端登录包
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
	
	/** 循环收发包 */
	public void cycle() throws Exception
	{
		long timerFetchCache = System.currentTimeMillis();
		boolean startGetAd = false;// 是否成功取过页脚广告
		long time_get_ad = 0;// 计算时间
		Request req;
		String[][] head;
		while (true)
		{
			req = null;
			/** 循环取广告 */
			if (adServer != null
					&& startGetAd
					&& (System.currentTimeMillis() - time_get_ad > Long
							.parseLong(msn.getAdInterval + "000")))
			{
				time_get_ad = System.currentTimeMillis();
				getAD(true, msn.windowWidth, msn.windowHeight);//取广告
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
					//发心跳包
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
						if (req.type==MSNApplication.JABBER_REQUEST)// 普通jabber包
						{
							doHttpConn(req.command, req.urlPage, req.content);
							if (!req.command.startsWith("cache"))
							{
								timerFetchCache = System.currentTimeMillis()- (interval - 4000);
							}
						} 
						else if (req.type==MSNApplication.AD_LOGIN_REQUEST)// 登录页面广告
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
						else if(req.type==MSNApplication.HEAD_VCARD_REQUEST)//自己的头像
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
						else if(req.type==MSNApplication.HEAD_CONTACT_REQUEST)//联系人头像
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
						else if (req.type==MSNApplication.AD_ROSTER_REQUEST   // 联系人页脚广告
								||req.type==MSNApplication.AD_MAIN_REQUEST    //主页面页脚     
								||req.type==MSNApplication.AD_SESSION_REQUEST) //会话列表页脚
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
								// 广告统计
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
				// 只有明确是连接断开时才退出
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

				// 失败的话，重新放到队列头
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
	
	// sendData服务器响应的长度
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
		if (msn_ua != null)// 如果有UA，则带上
			httpConn.setRequestProperty("User-Agent", msn_ua);
		else
			httpConn.setRequestProperty("User-Agent",
					"Profile/MIDP-1.0 Configuration/CLDC-1.0");
		if (hasAgent)
		{
				// 为支持向服务器报告运营商数据，在HTTP头中携带头字段ZWP_IM_OPERATOR
				if(msn_carrier!=null)
					httpConn.setRequestProperty("ZWP_IM_OPERATOR",msn_carrier);
				
				httpConn.setRequestProperty("Pica-Auth", msn.getPicaNumber() + "/"
						+ msn.GetPicaPW());

				// 要求第三方IM版本的每一个初始请求都在 HTTP 头中携带字段 ZWP_IM_ID，值为第三方 IM 用户的真实帐号，对
				// MSN 为 MSN 帐号。对PICA默认不携带该字段。
				// 修改了 只发送一次 并带上密码
				httpConn.setRequestProperty("ZWP_IM_ID", msn.GetLiveID()
						+ "/" + msn.GetPassWord());
				// 向服务器报告IM版本号
				httpConn.setRequestProperty("ZWP_IM_VERSION", msn.VERSION);

				//出现Pica-Auth的地方 一定不能出现Pica-Session
				
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
	/**获取联系人的头像*/
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
	 * 取广告
	 * 
	 * @param onlyGetRosterAd
	 *            是否只取页脚广告
	 * @param imid
	 * @param width
	 *            屏幕宽
	 * @param height
	 *            屏幕高
	 */
	public void getAD(boolean isOnlyGetRosterAd, int width, int height)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<adreq from='");
		buf.append(MsnUtil.escapeString(msn.GetLiveID()));
		buf.append("' source='msn' screen='");
		buf.append(width + "*" + height + "' ");
		buf.append("media='png'>");
		
		if (!isOnlyGetRosterAd)//取页脚广告 登录广告和信息文本广告
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
	
	/**广告点击*/
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
				if(MSNApplication.hasContactOnline)//有新联系人登录
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
	 * 获取PICA好友列表<br>
	 * 服务器回包 <iq type='result'><query xmlns='jabber:iq:roster'/></iq> <br>
	 * 客户端不用再处理
	 */
	public void getIqRoster()
	{
		StringBuffer buf=new StringBuffer();
		buf.append("<iq type='get'><query xmlns='jabber:iq:roster'/></iq>");
		addRequest("",buf.toString());
	}
	
	/**发送上线状态   
	 *服务器返回 <presence type=’available' from=’ '78276741@pica/pica’/>
	 *客户端根据from来判断此presence是来自pica还是msn
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
	
	

	/** 连接dispatch服务器<br>
	 *  type socket用s http用h*/
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
		sb.append("&oem=kuzhan");//推广渠道“酷站收录”的版本
		if(msn.GetIMSI()!=null)
		{
			sb.append("&imsi=");
			sb.append(msn.GetIMSI());
		}

		doHttpConn("dispatch", "/dispatch.xml?", sb.toString());
	}
	
	/** 连接服务器 获取sid <br>
	 * http连接时候需要*/
	public void connect()throws Exception // ///////已修改，需调整
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
					//response="<sso result='0'><system><notice>系统提示信息1</notice><notice>系统提示信息2</notice><notice>系统提示信息n</notice></system><upgrade force='0'><notice>升级提示内容</notice><version>新客户端版本号6.0.0</version><url>http://wap.msn.cn</url></upgrade><login><id>67890001@pica</id><password>666779</password><ip>211.99.191.23:35556</ip></login><config></config><transparent></transparent></sso>";								
					//response="<sso result='0'><system><notice>系统提示信息1</notice><notice>系统提示信息2</notice><notice>系统提示信息n</notice></system><upgrade force='0'><version>新客户端版本号6.0.0</version><url>http://wap.msn.cn</url></upgrade><login><id>67890001@pica</id><password>666779</password><ip>211.99.191.23:35556</ip></login><config></config><transparent></transparent></sso>";	
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
					//throw new Exception("网络连接失败");
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
	

	
	/** 解析得到sid */
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
	
	/**开始启动网络收发线程*/
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
	
	/**socket的第一个连接*/
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
	
	/**客户端登录
	 * 如果返回<iq type=’result’>
		<query xmlns='jabber:iq:auth'/>
		</iq>
		则表示成功
	 * */
	private void sendIqAuth()throws Exception
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set'><query xmlns='jabber:iq:auth'><username>");
		buf.append(MSNApplication.getApplication().getPicaNumber());
		buf.append("</username><password>");
		buf.append(MSNApplication.getApplication().GetPicaPW());//pica密码
		buf.append("</password><version>");
		buf.append(MSNApplication.VERSION);
		buf.append("</version><source>");
		buf.append(MSNApplication.SOURCE);
		buf.append("</source></query></iq>");
		addRequest("", buf.toString());
	}
	
	/** 判断是否已经获取到了sid会话号 */
	boolean isSessionOpen()
	{
		return session_id != null;
	}
	
	/**socket连接 每60秒发送心跳包 发送一个空包*/
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
	
	/**发送 添加联系人
	 * @param imid 页面所输入的 被添加联系人的imid 即联系人的CONTACT_imid<br>
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
	 * 删除/删除并阻止/阻止/解除阻止联系人
	 * @param String jid 联系人的jid  即联系人的SUBITEM_JID<br>
	 * @param String action 操作类型: 删除delete/阻止block/删除并阻止rmblk/解除阻止unblock
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
	 * 修改个人资料 用户改变自己的昵称和心情短语
	 * @param String newMyNickname 昵称  原昵称为 msn.myVCARD_myNickname
	 * @param String newMyImpresa 心情短语 原心情短语 msn.myVCARD_myImpresa
	 * @param boolean isChangeNickname 是否改变了昵称
	 * @param boolean isChangeImpresa 是否改变了心情短语
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
	 * 被对方添加为联系人（即收到验证请求）的选择操作 发包 
	 * @param String handleType=accept(接受对方请求) deny(拒绝对方请求)
	 * @param String imid 对方的imid号
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
	 * 新协议中注销MSN客户端的请求<br>
	 * 退出程序、注销程序、同步联系人三种情况 需要调用此接口
	 *@param boolean isExit 是否是退出操作 调用msn.isLogout
	 */
	public void sendLogoutMsn(boolean isExit)
	{
		try
		{
			String s = "<iq type='set' to='msn.pica'><query xmlns='jabber:iq:register'><remove/></query></iq>";
			addRequest("", s);
			Thread.sleep(200);
			if (isExit)// 如果是退出操作 则马上连接logout
			{
				sendEnd();
			}
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	/** 退出操作 发送logout断开socket连接 */
	public void sendEnd()
	{
		if (!isSessionOpen()||isSendLogoutRequest)// 还没有得到sid 说明还没有进入到循环
			return;
		sendSocketLogout();
		isSendLogoutRequest = true;
	}
	
	/** 登录等待过程中点击取消 */
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
	 * 改变自己的状态
	 * @param String show 发送给服务器的状态字段 值为MSNApplication.STATE_FLAG_All[i]
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
	 * 一对一 给联系人发送消息
	 * @param String contactJidorImid 可能是联系人的jid 也可能是陌生人的imid
	 * @param String body 发送消息的内容
	 * @param String sid 当前会话的会话id
	 * @param boolean isFriend true用jid|false用imid
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
	 * 给临时组群发送消息
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
	
	/**关闭会话*/
	public void sendCloseSession(String sid)
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:x:dialog' sid='");
		buf.append(sid);
		buf.append("' action='end'/></iq>");
		addRequest("",  buf.toString());
	}
	
	/**邀请联系人加入对话*/
	void sendInviteContactAddSession(String jid,String imid,String sid, String[] chaterJid)
	{
	    StringBuffer buf = new StringBuffer();
	    buf.append("<iq type='set' to='msn.pica'><query xmlns='jabber:x:dialog' sid='");
	    buf.append(sid);
	    buf.append("' action='add'>");
	    if(jid!=null||imid!=null)
	    {
		    buf.append("<control><firstparticipant ");
		    if(jid==null)//陌生人
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
	///////文件及语音传输功能		
	//////////////////////////////////////////////////////////////////////////////////
	
	/**
	 * 传送文件的请求
	 * @param ftid 客户端生成的 根据时间 确保不重复
	 * @param name 文件名称
	 * @param size 文件大小（字节数）
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
	 * send transfer invite response 传送文件的接收方的回应
	 * @param transferid
	 * @param sid
	 * @param ftID
	 * @param action ’accept 接受 | reject 拒绝
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
	 * 传送文件（带内方式）
	 * @param transferid
	 * @param sid
	 * @param ftid
	 * @param seqid
	 * @param totalLength 文件总长度
	 * @param startIndex 数据起始位置
	 * @param endIndex 数据结束位置
	 * @param base64Length Base64编码长度
	 * @param base64 Base64数据块
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
	 * 发送方发送文件结束
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
	
	/**接收方接收文件结束*/
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
	 * 传送文件（带外方式）
	 * @param transferid
	 * @param sid
	 * @param ftid
	 * @param url 获取该文件的地址
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
	 * 取消文件的传送
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
	 * 语音剪辑发送
	 * @param jid
	 * @param sid
	 * @param mime  audio/wav|audio/amr|audio/qcp
	 * @param bytesLength 文件总长度
	 * @param base64Length Base64编码长度
	 * @param base64 Base64编码头像文件内容
	 * @param url 带外方式上传的url
	 * @param isInner 是否是带内
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
	
	/**带内 接收文件后的回应包*/
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
	
	/** 登录过程图片广告 */
	public void addLoginADRequest(String urlPage)
	{
		synchronized (requestQueue)
		{
			requestQueue.addElement(new Request(urlPage));
		}
	}

	/** 页脚图片广告 */
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
	 * 修改个人资料 用户改变自己的头像
	 * @param String mime 头像文件的MIME类型，比如JPG
	 * @param String base64Length Base64编码长度
	 * @param String base64Data Base64编码头像文件内容
	 * @param String url 是否改变了心情短语
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
	 * 请求类wap资源
	 * 
	 * @param url
	 *            所请求资源的URL地址
	 * @param nvPair
	 *            提交的名-值对儿
	 * @param useAgent
	 *            是否添加客户端代理配置头
	 * @param byte
	 *            0: get或是1:post方法
	 */
	public void sendZipwapRequest(String url, Vector nvPair,byte method, String strNickname, short[] agent)
	{
		try
		{
	        //第一次登录 和超时15分钟以上  客户端带上Pica-auth字段 并携带代理配置数据包
	        //第一次 System.currentTimeMillis() - firstTime肯定大于900000 15分钟
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
				dos.writeInt(len); // 数据包长

				dos.writeByte(1); // 平台类型
				dos.writeShort(0x05); // 支持协议版本 0.5版本为0x05
				dos.writeUTF(agent_version);
				dos.writeShort(agent[0]); // 屏幕象素宽
				dos.writeShort(agent[1]); // 屏幕象素高
				dos.writeShort(agent[2]); // 浏览器显示宽
				dos.writeShort(agent[3]);// 浏览器显示高
				dos.writeByte(agent[4]);// 英文字体宽
				dos.writeByte(agent[5]);// 英文字体高
				dos.writeByte(0xff);// 屏幕色彩位数
				dos.writeUTF(images);// 支持图像格式
				dos.writeUTF(audios);// 支持播放音频格式
				dos.writeByte(0);// 支持cookie
				dos.writeByte(0);// 上载附件
				dos.writeByte(0);// 录音
				dos.writeByte(0);// 拍照
				dos.writeByte(1);// 系统浏览器
				dos.writeByte(1);// 拨打电话
				dos.writeByte(1);// 发短信
				dos.writeByte(1);// 发PICA消息
				dos.writeUTF(strNickname);// 用户昵称 327
			}

			int max_page_zise = 1000;

			dos.writeShort(0x0002);

			if (nvPair != null)
			{// 加入需要提交的表单名值对
				int len = 0;
				for (int i = 0; i < nvPair.size(); i++)
				{
					len += ((String[]) nvPair.elementAt(i))[0].getBytes("UTF-8").length
							+ 2
							+ ((String[]) nvPair.elementAt(i))[1]
									.getBytes("UTF-8").length + 2 + 1;
				}
				dos.writeInt(8 + len + url.getBytes("UTF-8").length + 2); // 注意得到utf串的长度还要加两字节的头长度
				dos.writeByte(method); // post
				dos.writeUTF(url);
				dos.writeInt(max_page_zise);
				dos.writeByte(0);
				dos.writeByte(0);
				// 表单数据
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
