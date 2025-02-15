/*
 * Fire (Flexible Interface Rendering Engine) is a set of graphics widgets for creating GUIs for j2me applications. 
 * Copyright (C) 2006-2008 Bluevibe (www.bluevibe.net)
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/**
 * 
 */
package gr.fire.browser.util;

import gr.fire.browser.Browser;
import gr.fire.util.FireConnector;
import gr.fire.util.Log;
import gr.fire.util.StringUtil;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.HttpsConnection;

/**
 * This class implements the Http functionality for the Browser module. 
 * It makes http requests using the supplied FireConnector instance and handles 
 * cookies and redirects approprietly. 
 * 
 * It also supports local file urls (starting with file://)
 * 
 * The HttpClient instance can remember the previous request made and can handle appropriatelly 
 * relative URLs based on that information.
 * 
 * @see FireConnector
 * @see Browser
 * 
 * @author padeler
 *
 */
public class HttpClient
{
	private String[][] defaultHeaderValues = new String[][] {
         {"User-Agent","Fire v2.0 Mozilla/4.0 (compatible; MSIE 6.0)"},
		 {"Accept","text/xml, application/xhtml+xml, text/css, multipart/mixed, ,*/*;q=0.8"},
		 {"Accept-Language","en-us,en;q=0.5"},
		 {"Accept-Charset","utf-8, ISO-8859-1;q=0.8, *;q=0.7"}
	   };
	
	
	protected FireConnector connector;

	private Vector savedCookies=new Vector();
	private Request currentPage=null;

	/**
	 * Constructor for the HttpClient. The new instance uses the supplied FireConnector instance 
	 * to open input/output streams to the requested resource.   
	 * @param connector
	 */
	public HttpClient(FireConnector connector)
	{
		this.connector = connector;
	}
	
	/**
	 * Makes a http request to URL using the set method (HttpConnection.GET or HttpConnection.POST) and the supplied request parameters.
	 *  
	 * @param url
	 * @param requestMethod
	 * @param requestProperties User supplied Http request header parameters. 
	 * @param data if method is POST then the post data are in this array. 
	 * @param updateCurrentPage if set to true the HttpClient will remember the URL of this request in order to handle feature relative path requests
	 * @return
	 */
	public Request requestResource(String url, String requestMethod, Hashtable requestProperties, byte[]data,boolean updateCurrentPage)
	{
		if(url==null) throw new IllegalArgumentException("Request for resource to null url?");
		
		url = StringUtil.proccessUrl(url,false);
		
		if(url.indexOf("://")==-1 && currentPage!=null) // url path is relevative to current. 
		{
			Log.logDebug("Resolving URL: "+url);
			if(url.startsWith("/"))
			{
				url = currentPage.getBaseURL()+url;
			}
			else
			{
				String base = currentPage.getBaseURL();
				String file = currentPage.getFile();

				int idx=-1;
				if(file!=null)
				{
					idx =file.lastIndexOf('/');
					if(idx==0) idx=-1; // file starts with "/" ignore that
				}
				
				if(idx==-1)
				{
					String s = "";
					if(base.endsWith("/")==false) s="/"; 
					
					url = base+s+url;
				}
				else
				{
					int start = 0;
					if(file.startsWith("/")) start=1;
					String sep="/";
					if(base.endsWith("/")) sep="";
					url = base+sep+file.substring(start,idx)+"/"+url;
				}
			}
			Log.logDebug("URL Resolved to: "+url);
		}
		
		int idx = url.indexOf("://");
		if(idx==-1) throw new IllegalArgumentException("URL must start with the protocol.");
		
		String protocol = url.substring(0,idx);
		
		if(protocol.equals("file"))
		{
			Log.logInfo("File: ["+url+"]");
			InputStream in=null;
			try
			{
				in = connector.openInputStream(url);
			} catch (Exception e)
			{
				Log.logDebug("Exception:" +e.getClass()+": "+e.getMessage());
			}
			
			if(in==null)
			{
				Log.logWarn("Failed to load local resource "+url);
				return null;
			}
			
			Request result = new Request("","file",url.substring(7),Request.defaultEncoding,in);
			if(updateCurrentPage) currentPage = result;
			
			return result;
		}
		
		Log.logInfo(requestMethod+" ["+url+"]");
		HttpConnection conn=null; 
		try{ // the resource is located remotelly. Try to retrieve it.
			int mode = Connector.READ;
			if(HttpConnection.POST.equals(requestMethod) && data!=null) mode=Connector.READ_WRITE;

			conn = (HttpConnection)connector.open(url,mode,true);
			conn.setRequestMethod(requestMethod);
			for(int i=0;i<defaultHeaderValues.length;++i)
			{
				conn.setRequestProperty(defaultHeaderValues[i][0], defaultHeaderValues[i][1]);
			}
            
			if(requestProperties!=null)
			{
				Enumeration propKeys = requestProperties.keys();
				while(propKeys.hasMoreElements())
				{
					String key = (String)propKeys.nextElement();
					String val = (String)requestProperties.get(key);
					conn.setRequestProperty(key,val);		
				}
			}
			
			// add cookies 
			String cookies = getCookies(conn,conn.getFile());
			if(cookies.length()>0)
			{
				conn.setRequestProperty("Cookie",cookies);
			}
			
			// add referer
			if(currentPage!=null && currentPage.getProtocol().equals("file")==false)
			{
				String q = currentPage.getQuery();
				if(q!=null) q = "?"+q;
				else q="";
				String referer = currentPage.getBaseURL()+currentPage.getFile()+q;
				conn.setRequestProperty("Referer",referer);
			}
			if(mode==Connector.READ_WRITE && data!=null) // exoume na grapsoume kiolas.
			{
				conn.setRequestProperty("Content-Length",""+data.length);				
				OutputStream out = conn.openOutputStream();
				out.write(data);
				Log.logDebug("Post data["+data.length+"] sent.");
				Log.logDebug("["+new String(data)+"]");
			}
			Log.logDebug("Attempting to retrieve response code..");
			int code = conn.getResponseCode();
			
			Log.logInfo("Response "+code+" "+conn.getResponseMessage());
			
			for(int i=0;i<100;++i)
			{
				String key = conn.getHeaderFieldKey(i);
				if(key==null) break;
				
				if(key.toLowerCase().equals("set-cookie")) 
				{
					// the cookieStr may be more than one cookies seperated by commas. 
					// First we split the cookies and then we parse them.
					// this is a bit tricky since a cookie may contain commas as well...(who thought of that delimiter anyway?)
					Vector cookieStrings = Cookie.splitCookies(conn.getHeaderField(i));
					for(int j=0;j<cookieStrings.size();++j)					
						saveCookie((String)cookieStrings.elementAt(j),conn.getHost());
				}
				else Log.logDebug(key+": "+conn.getHeaderField(i));
			}
			
			if(code==HttpConnection.HTTP_MOVED_PERM) // 301
			{ 				
				if(updateCurrentPage)
					currentPage = new Request(conn);
				String redirect = conn.getHeaderField("location");
				Log.logInfo("Redirect "+code+" to location: " +redirect);
				conn.close();
				return requestResource(redirect,requestMethod,requestProperties,data,updateCurrentPage);
			}
			else if(code==HttpConnection.HTTP_MOVED_TEMP || code==HttpConnection.HTTP_SEE_OTHER)
			{// must redirect using the GET method (see protocol)
				if(updateCurrentPage)
					currentPage = new Request(conn);
				String redirect = conn.getHeaderField("location");
				Log.logInfo("Redirect "+code+" to location: " +redirect);
				conn.close();
				return requestResource(redirect,HttpConnection.GET,requestProperties,data,updateCurrentPage);
			}
			else // responce is 200 or another http code.
			{
				Request result = new Request(conn);			
				
				if(updateCurrentPage && code==HttpConnection.HTTP_OK) // updateCurrentPage only when responce is 200 (OK)
					currentPage = result;

				return result;
			}
		}catch(Exception ex)
		{
			Log.logError("Request to "+url+" failed.",ex);
			if(conn!=null){
				try{conn.close();}catch(IOException e){
					Log.logWarn("Failed to close the connection.",e);
				}
			}
		}
		return null; 
	}
	
	private void saveCookie(String cookieStr,String host)
	{
		
		try{		
			Cookie newCookie = new Cookie(cookieStr,host);
		
			int idx = savedCookies.indexOf(newCookie); // check to see if cookie already exists
			Date expires = newCookie.getExpires();
			if(idx>-1)
			{
				savedCookies.removeElementAt(idx);
			}
			
			if(expires==null ||  System.currentTimeMillis()< (expires.getTime()))
			{
				savedCookies.addElement(newCookie);
				Log.logDebug("Saved Cookie "+newCookie.getName());
			}
			else // expiration date is in the past.
				Log.logDebug("Removed (expired: "+expires+") Cookie: "+newCookie.getName()); 
		}catch(Exception e)
		{
			Log.logWarn("Failed to save cookie: "+cookieStr,e);
		}
	}
	
	private String getCookies(HttpConnection req,String path)
	{
		String host = req.getHost();
		if(path==null) path="/";
		boolean includeSecure = req instanceof HttpsConnection;
		String res = "";
		for(int i =0 ;i<savedCookies.size();++i)
		{
			Cookie c = (Cookie)savedCookies.elementAt(i);
			if(c.match(host,path) && (includeSecure || !c.isSecure()))
			{
				res += c.toString()+"; ";
				Log.logDebug("Sending Cookie: "+c.getName());
			}
		}
		return res;
	}
	

	/**
	 * Stores the cookies currently saved in memory, to the record store.
	 * 
	 * @param cookiesStorage the name of the record store to hold the cookies.
	 * @throws IOException
	 */
	public void storeCookies(String cookiesStorage) throws IOException
	{
		if(savedCookies.size()==0)
		{
			Log.logInfo("No cookies to store in recordstore.");
			return; // nothing to save.
		}
		try{
			try{
				connector.rmsDelete(cookiesStorage);
			}catch(Throwable e){ // try to delete last save cookies.
				Log.logWarn("Failed to delete old saved cookies file. "+e.getMessage());
			}
			
			ByteArrayOutputStream bout = new ByteArrayOutputStream();
			DataOutputStream dout = new DataOutputStream(bout);
			for(int i=0;i<savedCookies.size();++i)
			{
				Cookie c = (Cookie)savedCookies.elementAt(i);
				Cookie.serialize(c,dout);
			}
			byte []buf = bout.toByteArray();
			int free = connector.rmsFree();
			if(free<buf.length) throw new IOException("Not enough free storage space to save cookies.");
			connector.rmsWrite(cookiesStorage,buf);
		}catch(Throwable e)
		{
			if(e instanceof IOException) throw (IOException)e;
			else throw new IOException("Store cookies failed. "+e.getMessage());
		}
		Log.logInfo("Stored "+savedCookies.size()+" to record store: "+cookiesStorage);
	}
	
	/**
	 * Load the cookies previously stored in the specified recordstore using the {@link #storeCookies(String)} method.
	 * @param cookiesStorage
	 * @throws IOException
	 */
	public void loadCookies(String cookiesStorage) throws IOException
	{
		try{
			InputStream in = connector.rmsRead(cookiesStorage);
			if(in==null) return; // no stored cookies found
			
			DataInputStream din = new DataInputStream(in);
			int cookiesCount=0;
			while(din.available()>0)
			{
				Cookie c = Cookie.deSerialize(din);
				savedCookies.addElement(c);
				cookiesCount++;
			}
			Log.logInfo("Loaded "+cookiesCount+" cookies from record store: "+cookiesStorage);
		}catch(Throwable e)
		{
			if(e instanceof IOException) throw (IOException)e;
			else throw new IOException("Load cookies failed. "+e.getMessage());
		}
	}
	
	/**
	 * If a call to the {@link #requestResource(String, String, Hashtable, byte[], boolean)} was made earlies with the updateCurrentPage==true 
	 * then this method will return that URL.
	 * @return
	 */
	public String getCurrentURL()
	{
		return (currentPage!=null)?currentPage.getURL():null;
	}

	public String[][] getDefaultHeaderValues()
	{
		return defaultHeaderValues;
	}

	public void setDefaultHeaderValues(String[][] defaultHeaderValues)
	{
		this.defaultHeaderValues = defaultHeaderValues;
	}
}
