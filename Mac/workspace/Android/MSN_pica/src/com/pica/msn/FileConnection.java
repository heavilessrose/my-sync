package com.pica.msn;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.URL;

import android.util.Log;


public class FileConnection 
{

	int charNum;
	int connectCode;
	Object msgOrContact;
	//Item contact;
	//boolean pause = false;// 意外中断
	private MSNApplication app;
	//String uploadUrl;
	public FileConnection(Object msgOrContact)
	{
		this.msgOrContact=msgOrContact;
		//this.contact=contact;
		app = MSNApplication.getApplication();
	}
	
	/*
	 * url:连接地址 head:头信息
	 */
	public byte[] postUrl(int endIndex,int startIndex, byte[] bytes,
			String urlPage, String[][] head) throws Exception
	{
		int sendBytes=endIndex - startIndex + 1;
		DataOutputStream os = null;
		InputStream is = null;
		byte reply[];
		
		HttpURLConnection conn=null;
		URL url = null;

		int resCode;
		try
		{

		    url = new URL(urlPage);
		    if (Jabber.isUseSocket)
			{
				conn = (HttpURLConnection) url.openConnection();
			}
			else
			{
				if (Jabber.proxy == null)
				{
					Jabber.proxy = new Proxy(Proxy.Type.HTTP, new InetSocketAddress("10.0.0.172", 80));
				}
				
				conn = (HttpURLConnection) url.openConnection(Jabber.proxy);
			}
		    conn.setDoOutput(true);
		    conn.setRequestMethod("POST");			        
			conn.setRequestProperty("Content-Type", "text");
		    conn.setRequestProperty("Connection", "close");		
		    conn.setRequestProperty("User-Agent", "UNTRUSTED/1.0");
			if (head != null)
				setRequestHeaders(conn, head);
			else
			{
				conn.setRequestProperty("Content-Length",  Integer.toString(sendBytes));
			}
			os = new DataOutputStream(conn.getOutputStream());
			if(msgOrContact instanceof Contact)
			{
				if(((Contact)msgOrContact).CONTACT_OUTER_HEAD_STATUS==MSNApplication.MESSAGE_HEAD_TYPE_SEND_CANCEL)
				{
					app.isTransferingOuterSendHead=false;
					return "402".getBytes();
				}
			}
			else
			{
				if (((Item)msgOrContact).MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL||
						((Item)msgOrContact).MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL)
				{
					//app.isTransfering=false;
					if(app.isTransferingMessageVector.contains(msgOrContact))
						app.isTransferingMessageVector.removeElement(msgOrContact);
					return "402".getBytes();
				}
			}
			

			os.write(bytes, startIndex, sendBytes);
   
			resCode = conn.getResponseCode();
			if(resCode!=HttpURLConnection.HTTP_OK)
				throw new Exception("invalid response");
			is =conn.getInputStream();

			reply = new byte[is.available()];
			int ch = 0;
			charNum = 0;
			while ((ch = is.read()) != -1)
			{
				if(msgOrContact instanceof Contact)
				{
					if(((Contact)msgOrContact).CONTACT_OUTER_HEAD_STATUS==MSNApplication.MESSAGE_HEAD_TYPE_SEND_CANCEL)
					{
						app.isTransferingOuterSendHead=false;
						return "402".getBytes();
					}
//					else if (pause)
//						return "403".getBytes();
				}
				else
				{
					if (((Item)msgOrContact).MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL||
							((Item)msgOrContact).MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL)
					{
						//app.isTransfering=false;
						if(app.isTransferingMessageVector.contains(msgOrContact))
							app.isTransferingMessageVector.removeElement(msgOrContact);
						return "402".getBytes();
					}
//					else if (pause)
//						return "403".getBytes();
				}
				reply[charNum++] = (byte) ch;
			}
			return reply;
		} 
		catch (Exception e)
		{
			String s=e.getMessage();
			if(s.equals("invalid response"))
				throw new Exception("invalid response");//抛出异常
			return null;
		}
		finally
		{
			if (is != null)
				try
				{
					is.close();
				}
				catch(Exception e)
				{		
					e.printStackTrace();
				}
				if(os!=null)
				try{
					os.close();
				}catch(Exception e)
				{
					e.printStackTrace();
				}

				if(conn!=null)	
				{
					try
					{
						conn.disconnect();
					}catch(Exception e)
					{
						e.printStackTrace();
					}
				}
				is=null;
				os=null;
				conn=null;
		}
	}

	public int total=0;
	public int start=0;
	public int end=0;
	public int length=0;
	
	/**
	 * 下载附件
	 * @param url 附件所在url地址
	 * @param head 为空
	 * @return byte[] 附件字节数组
	 */
	public byte[] getUrl(String urlPage, String[][] head, boolean bAdDownLoad)throws Exception
	{
		HttpURLConnection conn = null;
		URL url = null;
		DataInputStream is = null;
		start=0;
		total=0;
		end=0;
		length=0;
		
		try
		{

			url = new URL(urlPage);
			if (Jabber.isUseSocket)
			{
				conn = (HttpURLConnection) url.openConnection();
			}
			else
			{
				if (Jabber.proxy == null)
				{
					Jabber.proxy = new Proxy(Proxy.Type.HTTP, new InetSocketAddress("10.0.0.172", 80));
				}
				
				conn = (HttpURLConnection) url.openConnection(Jabber.proxy);
			}
			conn.setDoInput(true);
			conn.setConnectTimeout(0);
			conn.setRequestProperty("X-Online-Host", urlPage.substring(7, urlPage.indexOf("/", 7)));
			if (head != null)
				setRequestHeaders(conn, head);
			try
			{
				length = Integer.parseInt(conn.getHeaderField("Content-Length"));
				if(length==0)
					length = Integer.parseInt(conn.getHeaderField("Pica-Resplen"));
			}
			catch(Exception e)
			{
				return "400".getBytes();
			}
			
			if(bAdDownLoad == false)
			{
				if(length>(int)(app.per_file_outer_download_size))
				{
					return "400".getBytes();
				}
			}
			
			if(length>MsnUtil.getFreeMemory()+20000)
			{
				return "401".getBytes();
			}		
			if (conn.getResponseCode() != HttpURLConnection.HTTP_OK)
			{
				throw new Exception("invalid response");
			}
			try
			{
				String content_range=conn.getHeaderField("Content-Range");
				if(content_range!=null&&content_range.length()!=0)
				{
					content_range=content_range.substring("bytes ".length());
					start=Integer.parseInt(content_range.substring(0,content_range.indexOf("-")));
					end=Integer.parseInt(content_range.substring(content_range.indexOf("-")+1,content_range.indexOf("/")));
					total=Integer.parseInt(content_range.substring(content_range.indexOf("/")+1));
				}
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
			
			is = new DataInputStream(conn.getInputStream());
			byte[] rcv_bytes = null;
			if(length>0)
	        {
				rcv_bytes = new byte[(int)length];
				is.readFully(rcv_bytes);
				if(msgOrContact!=null)
				{
					if(msgOrContact instanceof Item)
					{
						if (((Item)msgOrContact).MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL||
								((Item)msgOrContact).MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)					
			        	{    		
							//app.isTransfering=false;
							if(app.isTransferingMessageVector.contains(msgOrContact))
								app.isTransferingMessageVector.removeElement(msgOrContact);
			        		return "402".getBytes();
			        	}
					}
//
//		            if(pause)
//		            	return "403".getBytes();
				}
			} 
			else
			{
				int k = 0;
				int l = 1024;
				int i1;
				byte abyte2[];
				for (rcv_bytes = new byte[l]; (i1 = is.read()) != -1;) 
				{
					if(msgOrContact!=null)
					{
						if(msgOrContact instanceof Item)
						{
							if (((Item)msgOrContact).MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_RECEIVE_CANCEL||
									((Item)msgOrContact).MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_RECEIVE_CANCEL)				
							{
								//app.isTransfering=false;
								if(app.isTransferingMessageVector.contains(msgOrContact))
									app.isTransferingMessageVector.removeElement(msgOrContact);
								return "402".getBytes();
							}
						}

//			            else if(pause)
//			            	return "403".getBytes();
					}

					rcv_bytes[k] = (byte) i1;
					if (++k == l) 
					{
						l += 1024;
						abyte2 = new byte[l];
						System.arraycopy(rcv_bytes, 0, abyte2, 0, k);
						rcv_bytes = null;
						rcv_bytes = abyte2;
						System.gc();
					}

				}
			}

			return rcv_bytes;
		} 
		catch (Exception e)
		{
			String s=e.getMessage();
			if(s.equals("invalid response"))
				throw new Exception("invalid response");
			e.printStackTrace();
			return null;
		}
		finally
		{
			if (is != null)
			try
			{
				is.close();
			}catch(Exception e)
			{
				e.printStackTrace();
			}
			if (conn != null)
			try
			{
				conn.disconnect();
			}catch(Exception e)
			{
				e.printStackTrace();
			}
			is=null;
			conn=null;
		}
	}

//	public void setRequestHeaders(HttpURLConnection c) throws IOException
//	{
//
//	}

	public void setRequestHeaders(HttpURLConnection c, String[][] title)
			throws IOException
	{
		for (int i = 0; i < title.length; i++)
		{
			c.setRequestProperty(title[i][0], title[i][1]);
		}
	}
}
