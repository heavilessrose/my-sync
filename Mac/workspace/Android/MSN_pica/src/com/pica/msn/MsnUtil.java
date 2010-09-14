package com.pica.msn;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.SocketAddress;
import java.net.URL;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Vector;

import org.apache.http.HttpConnection;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.net.Uri;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.pica.ui.tools.EmotionTextView;


public class MsnUtil 
{

	public  static String httpRequest(String urlPage, String content,String server,String serverPort) throws Exception
	{

		DataOutputStream dos = null;
		DataInputStream dis = null;

		ByteArrayOutputStream bytearrayoutputstream = null;
		DataOutputStream dataoutputstream1 = null;

		byte[] rcv_bytes = null;

		urlPage="http://"+server+":"+serverPort+urlPage;
		int recvs=-1;
		int resCode;
		HttpURLConnection conn=null;
		URL url = new URL(urlPage);
		try
		{
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
			
			if(content==null)//get方法
			{
				conn.setDoInput(true);
				conn.setConnectTimeout(0);
				resCode = conn.getResponseCode();
				if(resCode!=HttpURLConnection.HTTP_OK)
					throw new Exception("connection error");
				recvs = (int) conn.getContentLength();
				dis = new DataInputStream(conn.getInputStream());
			}
			else //post方法
			{
			    conn.setDoOutput(true);
			    conn.setRequestMethod("POST");			   
			    bytearrayoutputstream = new ByteArrayOutputStream();
				dataoutputstream1 = new DataOutputStream(bytearrayoutputstream);
				dataoutputstream1.writeUTF(content);
				byte abXmlParam[] = bytearrayoutputstream.toByteArray();	     
				conn.setRequestProperty("Content-Type", "text/xml");
			    conn.setRequestProperty("User-Agent", "UNTRUSTED/1.0");
			    conn.setRequestProperty("Content-Length", ""+ (abXmlParam.length - 2));
			     // //始终使用同一个HTTP连接来多次传递数据
			     // 手机传给电脑的数据
			    dos = new DataOutputStream(conn.getOutputStream());
			    dos.write(abXmlParam, 2, abXmlParam.length - 2);			     
			    dos.flush();
				resCode = conn.getResponseCode();
				if(resCode!=HttpURLConnection.HTTP_OK)
					throw new Exception("connection error");	     
			    recvs = (int) conn.getContentLength();
			    dis = new DataInputStream(conn.getInputStream());		
			}

			int k = 0;
			if (recvs > -1) {
				rcv_bytes = new byte[recvs];
				dis.readFully(rcv_bytes);
			} else 
			{
				int l = 1024;
				int i1;
				byte abyte2[];
				for (rcv_bytes = new byte[l]; (i1 = dis.read()) != -1;) {
					rcv_bytes[k] = (byte) i1;
					if (++k == l) {
						l += 1024;
						abyte2 = new byte[l];
						System.arraycopy(rcv_bytes, 0, abyte2, 0, k);
						rcv_bytes = null;
						rcv_bytes = abyte2;
					}
				}
			}
			String sResponse = new String(rcv_bytes, 0, recvs > -1 ? recvs : k,"UTF-8");
			
			if (sResponse.length() > 0) {
	
				return sResponse;
			}
	
		} 
		catch (Exception e) 
		{
		   e.printStackTrace();
			throw new Exception("connection error");
		} 
		finally 
		{
			if (bytearrayoutputstream != null)
				try {
					bytearrayoutputstream.close();
				} catch (Exception e) {
				  e.printStackTrace();
				}

			if (dataoutputstream1 != null)
				try {
					dataoutputstream1.close();
				} catch (Exception e) {
				   e.printStackTrace();
				}

			if (dis != null)
				try {
					dis.close();
				} catch (Exception e) {
				  e.printStackTrace();
				}

			if (dos != null)
				try {
					dos.close();
				} catch (Exception e) {
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
			rcv_bytes = null;
			bytearrayoutputstream = null;
			dataoutputstream1 = null;
			dis = null;
			dos = null;
			conn=null;
			
		}
		
		return null;
	}
	
	public  static String httpRequestRegister(String urlPg, String content,String server,String serverPort) throws Exception
	{

		DataOutputStream dos = null;
		DataInputStream dis = null;
		String urlPage = null;
		ByteArrayOutputStream bytearrayoutputstream = null;
		DataOutputStream dataoutputstream1 = null;

		byte[] rcv_bytes = null;
		
		if(serverPort == null){
			urlPage = "http://"+server+urlPg;
		} else {
			urlPage="http://"+server+":"+serverPort+urlPg;
		}
		
		int recvs=-1;
		int resCode;
		HttpURLConnection conn=null;
		URL url = new URL(urlPage);
		try
		{
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
			
			if(content==null)//get方法
			{
				conn.setDoInput(true);
				conn.setConnectTimeout(0);
				resCode = conn.getResponseCode();
				if(resCode!=HttpURLConnection.HTTP_OK)
					throw new Exception("connection error");
				recvs = (int) conn.getContentLength();
				dis = new DataInputStream(conn.getInputStream());
			}
			else //post方法
			{
			    conn.setDoOutput(true);
			    conn.setRequestMethod("POST");
			    conn.setConnectTimeout(120*1000);
			    bytearrayoutputstream = new ByteArrayOutputStream();
				dataoutputstream1 = new DataOutputStream(bytearrayoutputstream);
				dataoutputstream1.writeUTF(content);
				byte abXmlParam[] = bytearrayoutputstream.toByteArray();
				if(serverPort == null){
					if(!"".equals(urlPg)){
						conn.setRequestProperty("X-Online-Host", server);
					}
					conn.setRequestProperty("Host", "disp1.msn.pica.com:22246");
				}
				conn.setRequestProperty("Content-Type", "text/xml");
			    conn.setRequestProperty("User-Agent", "UNTRUSTED/1.0");
			    conn.setRequestProperty("Content-Length", ""+ (abXmlParam.length - 2));
			     // //始终使用同一个HTTP连接来多次传递数据
			     // 手机传给电脑的数据
			    dos = new DataOutputStream(conn.getOutputStream());
			    dos.write(abXmlParam, 2, abXmlParam.length - 2);			     
			    dos.flush();
				resCode = conn.getResponseCode();
				if(resCode!=HttpURLConnection.HTTP_OK)
					throw new Exception("connection error");	     
			    recvs = (int) conn.getContentLength();
			    dis = new DataInputStream(conn.getInputStream());		
			}

			int k = 0;
			if (recvs > -1) {
				rcv_bytes = new byte[recvs];
				dis.readFully(rcv_bytes);
			} else 
			{
				int l = 1024;
				int i1;
				byte abyte2[];
				for (rcv_bytes = new byte[l]; (i1 = dis.read()) != -1;) {
					rcv_bytes[k] = (byte) i1;
					if (++k == l) {
						l += 1024;
						abyte2 = new byte[l];
						System.arraycopy(rcv_bytes, 0, abyte2, 0, k);
						rcv_bytes = null;
						rcv_bytes = abyte2;
					}
				}
			}
			String sResponse = new String(rcv_bytes, 0, recvs > -1 ? recvs : k,"UTF-8");
			
			if (sResponse.length() > 0) {
	
				return sResponse;
			}
	
		} 
		catch (Exception e) 
		{
		   e.printStackTrace();
			throw new Exception("connection error");
		} 
		finally 
		{
			if (bytearrayoutputstream != null)
				try {
					bytearrayoutputstream.close();
				} catch (Exception e) {
				  e.printStackTrace();
				}

			if (dataoutputstream1 != null)
				try {
					dataoutputstream1.close();
				} catch (Exception e) {
				   e.printStackTrace();
				}

			if (dis != null)
				try {
					dis.close();
				} catch (Exception e) {
				  e.printStackTrace();
				}

			if (dos != null)
				try {
					dos.close();
				} catch (Exception e) {
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
			rcv_bytes = null;
			bytearrayoutputstream = null;
			dataoutputstream1 = null;
			dis = null;
			dos = null;
			conn=null;
			
		}
		
		return null;
	}
	
	public static String escapeString(String str)
	{
		StringBuffer sbf=new StringBuffer();
		char c;
		for(int i=0;i<str.length();i++){
			c = str.charAt(i);
			switch(c){
			case '<':
				sbf.append("&lt;");
				break;
			case '>':
				sbf.append("&gt;");
				break;
			case '&':
				sbf.append("&amp;");
				break;
			case '\'':
				sbf.append("&apos;");
				break;
			case '"':
				sbf.append("&quot;");
				break;
			default:
				sbf.append(c);
			break;
			}
		}

		if(sbf.length()>0){
			return sbf.toString();
		} else{
			return "";
		}
	}
	
	
	/**
	 * 把一个长文本按指定字符分割
	 * @param splitStr 要分割的文本内容
	 * @param delimiter 指定的分割字符
	 * @return 长文本按行存储的字符串数组
	 */
	public static String[] split(String splitStr, char delimiter)
	{
		if (splitStr == null || splitStr.length() == 0)
		{
			return new String[0];
		}

		Vector tokens = new Vector();
		StringBuffer sbuf = new StringBuffer();

		int len = splitStr.length();
		char c;
		String s;
		for (int i = 0; i < len; i++)
		{
			c = splitStr.charAt(i);
			if (c == delimiter)
			{
				s = sbuf.toString().trim();
				if (s.length() > 0)
				{
					tokens.addElement(s);
				}
				sbuf.setLength(0);
			} else
			{
				sbuf.append(c);
			}
		}

		if (sbuf.length() > 0)
		{
			s = sbuf.toString().trim();
			if (s.length() > 0)
			{
				tokens.addElement(s);
			}
			sbuf.setLength(0);
		}
		String[] splitArray = new String[tokens.size()];
		tokens.copyInto(splitArray);
		return splitArray;
	}
	
	/**
	 * 判断该字符是否是字母
	 */
	public static boolean isLetter(char c)
	{
		if((c>=65&&c<=90)||(c>=97&&c<=122))
			return true;
		else
			return false;
	}
	
	
	
	/**传入Date时间对象 生成按照一定格式表示时间的字符串 xx年/xx月/xx日/xx时/xx分*/
	public static String toString(Date dt)
	{
		Calendar cal = Calendar.getInstance();
		cal.setTime(dt);
		StringBuffer str = new StringBuffer();
		// year
		//str.append(Integer.toString(cal.get(Calendar.YEAR)));
		//str.append("/");
		// month
		//append(str,cal.get(Calendar.MONDAY)+1,"/");
		// day
		//append(str,cal.get(Calendar.DAY_OF_MONTH)," ");
		// hour
		append(str,cal.get(Calendar.HOUR_OF_DAY),":");
		// minute
		append(str,cal.get(Calendar.MINUTE),"");
		return str.toString();
	}
	
	/**传入Date时间对象 生成按照一定格式表示时间的字符串 xx年/xx月/xx日/xx时/xx分/xx秒*/
	public static String fileCreatedDate(Date dt)
	{
		Calendar cal = Calendar.getInstance();
		cal.setTime(dt);
		StringBuffer str = new StringBuffer();
		//year
		str.append(Integer.toString(cal.get(Calendar.YEAR)));
		str.append("_");
		// month
		append(str,cal.get(Calendar.MONDAY)+1,"_");
		// day
		append(str,cal.get(Calendar.DAY_OF_MONTH),"_");
		// hour
		append(str,cal.get(Calendar.HOUR_OF_DAY),"_");
		// minute
		append(str,cal.get(Calendar.MINUTE),"_");
		// second
		append(str,cal.get(Calendar.SECOND),"_");
		return str.toString();
	}

	// 将整型数字转换为 xy, 不足两位的数字, 左边加 0
	// data 必须在 0-99 只见
	private static void append(StringBuffer str, int data, String sep)
	{
		int n = str.length();
		str.append(Integer.toString(data));
		if (str.length() - n == 1)
			str.insert(str.length() - 1, "0");

		str.append(sep);
	}
	
	
	/**
	 * 解析带表情的文本 考虑到英文断行情况
	 * @param text
	 * @param width
	 * @param font
	 * @param isOneLine
	 *            为true时只解析一行，其余抛弃
	 * @param isTab 是否考虑英文断行 如果用英文 则true 否则false           
	 * @return Object[]{Vector,int 末尾长度}
	 */
	public static Object[] splitNewEmotionText(String text, int width,Paint paint,boolean isOneLine)
	{
		try
		{

			if (text == null || text.length() == 0)
			{
				return new Object[]{new Vector(),new Integer(0)};//new Vector();
			}

			Vector v1 = (isOneLine ? null : new Vector());
			Vector v2 = new Vector();
			int length = text.length(); // 字符串的长度
			char[] msg_chr = text.toCharArray(); // 存储text的字符形式
			int x = 0;
			int start = 0; // 字符串的开始位置
			int index = 0; // 当前检索字符的索引
			Integer iconIndex = null; // 检索到的表情的索引
			int pace = 1; // 索引index的步长
			boolean flag = false; // 是否画字符串和表情，true 画; false 不画，继续检索

			// 检索text,由于表情符号最少为2个字符，最后一个字符不用检索
			int temp_int = 0;

			while (index <= length - 1)
			{
				Object[] ob=parseEmotion(msg_chr,index,pace,iconIndex,length,temp_int,flag);
				flag=((Boolean)ob[0]).booleanValue();
				iconIndex=(Integer)ob[1];
				pace=((Integer)ob[2]).intValue();

				if (flag)// 遇到表情或者换行 进行判断
				{
					if (index > start)
					{
						v2.addElement(new String(msg_chr, start, index- start));
					}
					if (iconIndex.intValue() == -1)
					{// 遇到回车，换行
						if (isOneLine)
						{
							//return v2;
							
							return new Object[]{v2,new Integer(x)};
						}
						v1.addElement(v2);
						v2 = new Vector();
						v2.addElement("");
						x = 0;
					} else
					{
						if (x + EmotionTextView.emotionWidth +4 > width)
						{
							if (isOneLine)
							{
								//return v2;
								return new Object[]{v2,new Integer(x)};
							}
							v1.addElement(v2);
							v2 = new Vector();
							x = 0;
						}
						v2.addElement(iconIndex);
						x += EmotionTextView.emotionWidth + 4;
					}

					// 变量更新
					index += pace;
					start = index;
					flag = false;
					pace = 1;
				} 
				else
				{
					//Rect rect=new Rect();
					//paint.getTextBounds(msg_chr, index, 1, rect);
					
					//x+=rect.width() + 2;	
					x+=(int)paint.measureText(msg_chr, index, 1);
					
					//x += font.charWidth(msg_chr[index]);
					if (x > width)
					{
						//if (isTab)
						//{
							String temp = new String(msg_chr, start, index
									- start + 1);
							if (isOneLine)
							{
								v2.addElement(temp);
								//return v2;
								return new Object[]{v2,new Integer(x)};
							} else
							{

								int tempLength = temp.length();
								int j = tempLength;
								if (!isLetter(msg_chr[index]))// 如果最后一个字符不是英文
								// 肯定不回溯
								{
									String temp1 = new String(msg_chr, start,
											index - start);
									v2.addElement(temp1);
									v1.addElement(v2);
									v2 = new Vector();
									start = index;
									
									//paint.getTextBounds(msg_chr, index, 1, rect);
									//x=rect.width()  +2;
									x=(int)paint.measureText(msg_chr, index, 1);
									
									//x = font.charWidth(msg_chr[index]);
									
									
								} else
								// 是英文 需要回溯
								{
									boolean find = false;//表示回朔完该行是否找到可以换行的字符
									String oldString;
									String newString;
									for (int n = j - 1; n >= 0; n--)
									{
										if (!isLetter(temp.charAt(n)))// 如果不是英文
										// 则回溯截止
										{
											oldString = temp.substring(
													0, n + 1);
											v2.addElement(oldString);
											v1.addElement(v2);
											v2 = new Vector();
											newString = temp.substring(n + 1);
											start = index - newString.length()+ 1;
											
											//paint.getTextBounds(newString, 0, newString.length(), rect);
											//x=rect.width() + 2;
											x=(int)paint.measureText(newString, 0, newString.length());
											
											//x = font.stringWidth(newString);
											find = true;
											break;
										}
									}
									if(!find)//回朔完该行后没有找到可以换行的字符
									{
										String oldTemp=new String(msg_chr, start,index - start);
										
										//paint.getTextBounds(msg_chr, index, 1, rect);
										//int width1=rect.width();
										int width1=(int)paint.measureText(msg_chr, index, 1);
										//paint.getTextBounds(oldTemp, 0, oldTemp.length(), rect);
										//int width2=rect.width();
										int width2=(int)paint.measureText(oldTemp, 0, oldTemp.length());
											
										//int oldX=x-font.charWidth(msg_chr[index])-font.stringWidth(oldTemp);
					            		int oldX=x-width1-width2;
					            		
										//如果该行是第一行且起始位置不在行首，则需要另起一行

										if(v2.size()!=0&&v2.lastElement() instanceof String)
										{

						            		String temp1 = new String(msg_chr, start,index - start);
											v2.addElement(temp1);
											v1.addElement(v2);
											v2 = new Vector();
											start = index;
											
											//paint.getTextBounds(msg_chr, index, 1, rect);
											//x=rect.width() +2;
											
											x=(int)paint.measureText(msg_chr, index, 1);
											
											//x = font.charWidth(msg_chr[index]);
										}
										else
										{
											if(oldX>0)
											{
												if(v2!=null)
													v1.addElement(v2);
						            			v2=new Vector();
												v2.addElement(temp);
												start = index+1;
												
												//paint.getTextBounds(temp, 0, temp.length(), rect);
												//x=rect.width() + 2;	
												x=(int)paint.measureText(temp, 0, temp.length());
												
												//x = font.stringWidth(temp);
						            		}
						            		else
						            		{			
						            			String temp1 = new String(msg_chr, start,index - start);
												v2.addElement(temp1);
												v1.addElement(v2);
												v2 = new Vector();
												start = index;
												
												//paint.getTextBounds(msg_chr, index, 1, rect);
												//x=rect.width() + 2;	
												x=(int)paint.measureText(msg_chr, index, 1);
												
												//x = font.charWidth(msg_chr[index]);
						            		}
										}

									}
								}
						} 

					}
					index++;
				}
			}

			// 画最后的子字符串
			index = length;
			if (index > start)
			{// 如果字符串不为空
				v2.addElement(new String(msg_chr, start, index - start));
			}

			if (isOneLine) // 如果是画单行，直接返回
			{
				//return v2;
				return new Object[]{v2,new Integer(x)};
			}

			if (v2.size() > 0)
			{
				v1.addElement(v2);
			}
			v2 = null;
			//return v1;
			return new Object[]{v1,new Integer(x)};
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return null;
		}
	
	}
	
	
	/**
	 * 把一个长文本按行分割
	 * 
	 * @param text
	 *            要分割的文本内容
	 * @param width
	 *            需要折行的宽度
	 * @param font
	 *            字体
	 * @return 长文本按行存储的字符串数组
	 */
	public static String[] splitText(String text, int width,Paint paint)
	{
		int x = 0;
		int count = 1;

		char[] temp = new char[text.length()];// 转换成字符数组
		temp = text.toCharArray();

		Vector vector = new Vector();
		int start = 0;
		Rect rect=new Rect();
		for (int i = 0; i < text.length(); i++)
		{

			if (temp[i] == '\n')
			{
				x = 0;
				count++;
				vector.addElement((String) text.substring(start, i));
				start = i + 1;
				continue;
			}
			paint.getTextBounds(temp, i, 1, rect);
			//if (width - x < font.charWidth(temp[i]))
			if (width - x < rect.width())
			{
				x = 0;
				count++;

				vector.addElement((String) text.substring(start, i));
				start = i;
			}
			//x = x + font.charWidth(temp[i]);
			x=x+rect.width();
		}

		vector.addElement((String) text.substring(start));
		String[] str = new String[count];
		for (int j = 0; j < vector.size(); j++)
			str[j] = (String) vector.elementAt(j);

		return str;

	}
	
	/**
	 * 把一个长文本按行分割 考虑英文断行问题
	 * 
	 * @param drawText
	 *            要分割的文本内容
	 * @param width
	 *            需要折行的宽度
	 * @param font
	 *            字体
	 * @return 长文本按行存储的字符串数组
	 */
	public static String[] splitNewText(String drawText, int width, Paint paint)
	{
		try
		{
		char[] text = drawText.toCharArray();
		Vector msg = new Vector(); // 存储这段文字每行内容的向量
		msg.addElement(new StringBuffer(""));
		//Rect rect=new Rect();
		int charW;
		StringBuffer strbuff;
		int length;
		String str;
		int strbuffWidth;
		for (int j = 0; j < drawText.length(); j++)
		{
			if (text[j] == '\n')
			{ // 遇到换行符则换行
				msg.addElement(new StringBuffer(""));
			} 
			else
			{
				//int charW = font.charWidth(text[j]);
				//paint.getTextBounds(text, j, 1, rect);
				charW = (int)paint.measureText(text, j, 1);
				
				strbuff = ((StringBuffer) (msg.lastElement()));
				length = strbuff.length();// 最后一行现有字符串的长度
				str=strbuff.toString();
				//paint.getTextBounds(str, 0, str.length(), rect);
				strbuffWidth=(int)paint.measureText(str, 0, str.length());
				//if (font.stringWidth(strbuff.toString())+ charW > width)
				if (strbuffWidth+ charW > width)
				{
					if (isLetter(text[j]))// 后面是英文字母 要回溯遍历
					{
						boolean find = false;// 表示回朔完该行是否找到可以换行的字符
						for (int n = length - 1; n >= 0; n--)
						{
							if (!isLetter(strbuff.charAt(n)))
							{// 在该字符处换行
								msg.addElement(new StringBuffer(""));
								((StringBuffer) (msg.lastElement())).append(strbuff.toString().substring(n + 1));
								((StringBuffer) (msg.lastElement())).append(text[j]);
								strbuff.delete(n+1, length);
								find = true;
								break;
							}
						}
						if (!find)// 回朔完该行后没有找到可以换行的字符
						{
							msg.addElement(new StringBuffer(""));
								((StringBuffer) (msg.lastElement())).append(text[j]);
						}

					} 
					else
					// 不是英文字母不需要回朔
					{
						msg.addElement(new StringBuffer(""));
						((StringBuffer) (msg.lastElement())).append(text[j]);
					}
				} 
				else
					// 不用换行
					((StringBuffer)(msg.lastElement())).append(text[j]);
			}
		}
		String[] newStr = new String[msg.size()];
		for (int j = 0; j < msg.size(); j++)
			newStr[j] = ((StringBuffer) msg.elementAt(j)).toString();
		return newStr;
		}
		catch(Exception e)
		{
			//e.printStackTrace();
			return null;
		}
		
	}
	
	/**绘制 带表情图标的 超长带省略号的字符串（一行）*/
//	public static void drawOmitOneLineEmotionText(View view,Canvas canvas,Paint paint, String text, int x,int y, int xBorder)
//	{
//		//int oldX = g.getClipX();
//		//int oldY = g.getClipY();
//		//int oldW = g.getClipWidth();
//		//int oldH = g.getClipHeight();
//		int width = xBorder - x;
//		//int startX=x;
//		//g.setClip(startX, oldY, width, oldH);
//		
//		Rect rect=new Rect();
//		String omit="...";
//		paint.getTextBounds(omit, 0, omit.length(), rect);
//		
//		Vector v = getNewVisibleStringAt( width-rect.width(),paint, text);
//		//int endX=
//		drawEmotionText(view, canvas,v, x, y, paint);
//		//g.setClip(oldX, oldY, oldW, oldH);
//		//return endX;
//	}
	
	/**考虑表情图标 获取指定宽度的可见字符 如果有多余 则用'...'表示*/
	public static Vector getNewVisibleStringAt(int width, Paint paint, String text)
	{
		try
		{
			if (text == null || text.length() == 0)
			{
				return new Vector();
			}
			Vector v2 = new Vector();
			int length = text.length(); // 字符串的长度
			char[] msg_chr = text.toCharArray(); // 存储text的字符形式
			int x = 0;
			int start = 0; // 字符串的开始位置
			int index = 0; // 当前检索字符的索引
			Integer iconIndex = null; // 检索到的表情的索引
			int pace = 1; // 索引index的步长
			boolean flag = false; // 是否画字符串和表情，true 画; false 不画，继续检索

			// 检索text,由于表情符号最少为2个字符，最后一个字符不用检索
			int temp_int = 0;
			//Rect rect=new Rect();
			while (index <=length - 1)
			{
				Object[] ob=parseEmotion(msg_chr,index,pace,iconIndex,length,temp_int,flag);
				flag=((Boolean)ob[0]).booleanValue();
				iconIndex=(Integer)ob[1];
				pace=((Integer)ob[2]).intValue();

				if (flag)// 遇到表情或者换行 进行判断
				{
					String temp=new String(msg_chr, start, index- start);

					if (iconIndex.intValue() == -1)
					{// 遇到回车，换行

						if(index<length-1)
						{
							//有省略
							v2.addElement(temp+"...");
						}
						else
						{
							if (index > start)
							{
								v2.addElement(temp);
							}
						}
						return v2;
					} else
					{

						if (x + EmotionTextView.emotionWidth > width)
						{
							if(index<length-1)
							{
								//有省略
								v2.addElement(temp+"...");
							}
							else
							{
								if (index > start)
								{
									v2.addElement(temp);
								}
							}
							return v2;
						}
						else
						{
							if (index > start)
							{
								v2.addElement(temp);
							}
							v2.addElement(iconIndex);
							x += EmotionTextView.emotionWidth;
						}

					}

					// 变量更新
					index += pace;
					start = index;
					flag = false;
					pace = 1;
				} else
				{
					//paint.getTextBounds(msg_chr, index, 1, rect);
					int tempWidth=(int)paint.measureText(msg_chr, index, 1);
					//x += font.charWidth(msg_chr[index]);
					x+=tempWidth;
					if (x > width)
					{
						String temp1=new String(msg_chr, start, index- start);
						if(index<=length-1)
						{
							//有省略
							v2.addElement(temp1+"...");
						}
						else
							v2.addElement(temp1);
						return v2;
						
					}
					index++;
				}
			}

			// 画最后的子字符串
			index = length;
			if (index > start)
			{// 如果字符串不为空
				v2.addElement(new String(msg_chr, start, index - start));
			}
			return v2;

		}catch(Exception e)
		{
			//e.printStackTrace();
			return null;
		}
	}
	
	/**考虑表情图标 获取指定宽度的可见字符 如果有多余 则用'...'表示 还要加上时间戳*/
	public static Vector getNewVisibleStringAt(int width, Paint paint, String text, String timeStamp)
	{
		try
		{
			if (text == null || text.length() == 0)
			{
				return new Vector();
			}
			Vector v2 = new Vector();
			int length = text.length(); // 字符串的长度
			char[] msg_chr = text.toCharArray(); // 存储text的字符形式
			int x = 0;
			int start = 0; // 字符串的开始位置
			int index = 0; // 当前检索字符的索引
			Integer iconIndex = null; // 检索到的表情的索引
			int pace = 1; // 索引index的步长
			boolean flag = false; // 是否画字符串和表情，true 画; false 不画，继续检索

			// 检索text,由于表情符号最少为2个字符，最后一个字符不用检索
			int temp_int = 0;
			//Rect rect=new Rect();
			while (index <=length - 1)
			{
				Object[] ob=parseEmotion(msg_chr,index,pace,iconIndex,length,temp_int,flag);
				flag=((Boolean)ob[0]).booleanValue();
				iconIndex=(Integer)ob[1];
				pace=((Integer)ob[2]).intValue();

				if (flag)// 遇到表情或者换行 进行判断
				{
					String temp=new String(msg_chr, start, index- start);

					if (iconIndex.intValue() == -1)
					{// 遇到回车，换行

						if(index<length-1)
						{
							//有省略
							v2.addElement(temp+"...");
						}
						else
						{
							if (index > start)
							{
								v2.addElement(temp);
							}
						}
						v2.addElement(" 说: ("+timeStamp+")");
						return v2;
					} else
					{

						if (x + EmotionTextView.emotionWidth > width)
						{
							if(index<length-1)
							{
								//有省略
								v2.addElement(temp+"...");
							}
							else
							{
								if (index > start)
								{
									v2.addElement(temp);
								}
							}
							v2.addElement(" 说: ("+timeStamp+")");
							return v2;
						}
						else
						{
							if (index > start)
							{
								v2.addElement(temp);
							}
							v2.addElement(iconIndex);
							x += EmotionTextView.emotionWidth;
						}

					}

					// 变量更新
					index += pace;
					start = index;
					flag = false;
					pace = 1;
				} else
				{
					//paint.getTextBounds(msg_chr, index, 1, rect);
					int tempWidth=(int)paint.measureText(msg_chr, index, 1);
					//x += font.charWidth(msg_chr[index]);
					x+=tempWidth;
					if (x > width)
					{
						String temp1=new String(msg_chr, start, index- start);
						if(index<=length-1)
						{
							//有省略
							v2.addElement(temp1+"...");
						}
						else
						{
							v2.addElement(temp1);
						}
						v2.addElement(" 说: ("+timeStamp+")");
						return v2;
						
					}
					index++;
				}
			}

			// 画最后的子字符串
			index = length;
			if (index > start)
			{// 如果字符串不为空
				v2.addElement(new String(msg_chr, start, index - start));
			}
			v2.addElement(" 说: ("+timeStamp+")");
			return v2;

		}catch(Exception e)
		{
			//e.printStackTrace();
			return null;
		}
	}
	/**
	 * 解析带表情的文本 不考虑到英文断行情况
	 * @param text
	 * @param width
	 * @param font
	 * @param isOneLine
	 *            为true时只解析一行，其余抛弃        
	 * @return Object[]{Vector,int 末尾长度}
	 */
//	public static Object[] splitEmotionText(String text, int width,Paint paint)//,boolean isTab)
//	{
//		try
//		{
//
//			if (text == null || text.length() == 0)
//			{
//				return new Object[]{new Vector(),new Integer(0)};//new Vector();
//			}
//
//			Vector v1 =  new Vector();
//			Vector v2 = new Vector();
//			int length = text.length(); // 字符串的长度
//			char[] msg_chr = text.toCharArray(); // 存储text的字符形式
//			int x = 0;
//			int start = 0; // 字符串的开始位置
//			int index = 0; // 当前检索字符的索引
//			Integer iconIndex = null; // 检索到的表情的索引
//			int pace = 1; // 索引index的步长
//			boolean flag = false; // 是否画字符串和表情，true 画; false 不画，继续检索
//
//			// 检索text,由于表情符号最少为2个字符，最后一个字符不用检索
//			int temp_int = 0;
//			while (index <= length - 1)
//			{
//			Object[] ob=parseEmotion(msg_chr,index,pace,iconIndex,length,temp_int,flag);
//			flag=((Boolean)ob[0]).booleanValue();
//			iconIndex=(Integer)ob[1];
//			pace=((Integer)ob[2]).intValue();
//
//				if (flag)// 遇到表情或者换行 进行判断
//				{
//					if (index > start)
//					{
//						v2.addElement(new String(msg_chr, start, index- start));
//					}
//					if (iconIndex.intValue() == -1)
//					{// 遇到回车，换行
//						//if (isOneLine)
//						//{
//							//return v2;
//							
//							//return new Object[]{v2,new Integer(x)};
//						//}
//						v1.addElement(v2);
//						v2 = new Vector();
//						x = 0;
//					} else
//					{
//						if (x + EmotionTextView.emotionWidth > width)
//						{
//							//if (isOneLine)
//							//{
//								//return v2;
//							//	return new Object[]{v2,new Integer(x)};
//							//}
//							v1.addElement(v2);
//							v2 = new Vector();
//							x = 0;
//						}
//						v2.addElement(iconIndex);
//						x += EmotionTextView.emotionWidth;
//					}
//
//					// 变量更新
//					index += pace;
//					start = index;
//					flag = false;
//					pace = 1;
//				} 
//				else
//				{
//					Rect rect=new Rect();
//					paint.getTextBounds(msg_chr, index, 1, rect);
//					//x += font.charWidth(msg_chr[index]);
//					x+=rect.width();
//					
//					if (x > width)
//					{
//							v2.addElement(new String(msg_chr, start, index
//									- start));
//							//if (isOneLine)
//							//{
//								//return v2;
//							//	return new Object[]{v2,new Integer(x)};
//							//}
//							v1.addElement(v2);
//							v2 = new Vector();
//							start = index;
//							//x = font.charWidth(msg_chr[index]);
//							x=rect.width();
//							////						}
//					}
//					index++;
//				}
//			}
//
//			// 画最后的子字符串
//			index = length;
//			if (index > start)
//			{// 如果字符串不为空
//				v2.addElement(new String(msg_chr, start, index - start));
//			}
//
//			//if (isOneLine) // 如果是画单行，直接返回
//			//{
//				//return v2;
//			//	return new Object[]{v2,new Integer(x)};
//			//}
//
//			if (v2.size() > 0)
//			{
//				v1.addElement(v2);
//			}
//			v2 = null;
//			//return v1;
//			return new Object[]{v1,new Integer(x)};
//		}
//		catch(Exception e)
//		{
//			e.printStackTrace();
//			return null;
//		}
//
//	}
	
	/***
	 * 
	 * @param msg_chr
	 * @param index
	 * @param pace
	 * @param iconIndex
	 * @param length
	 * @param temp_int
	 * @param flag
	 * @return new Boolean(flag),iconIndex, new Integer(pace)
	 */
	private static Object[] parseEmotion(char[] msg_chr,int index,int pace,Integer iconIndex,int length,int temp_int,boolean flag)
	{

		Object[] ob=null;
		switch (msg_chr[index])
		{

		case '\n':
			iconIndex = new Integer(-1);
			flag = true;
			pace = 1;
			break;
		case '(': // 以"("开头的表情, 三、四、五个字符
			if (index < length - 2)
			{
				temp_int = index + 2;
				Object object;
				for (; temp_int < index + 5; temp_int++)
				{
					if (temp_int == length)
					{
						break;
					} else if (msg_chr[temp_int] == ')')
					{
						object = null;
						object = EmotionTextView.faceHashtable.get(new String(
								msg_chr, index, temp_int + 1 - index));
						if (object != null)
						{
							iconIndex = (Integer) object;
							flag = true;
							pace = temp_int + 1 - index;
						}
						break;
					}
				}
			}
			break;
		case '8':// 以"8"开头的表情"8o|" "8-|" "8-)" 全是三个字符
			if (index < length - 2)
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 3));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 3;
				}
			}
			break;
		case ';': // ";)", ";-)"
			if (index < length - 2&&msg_chr[index + 1] == '-' 
					&& msg_chr[index + 2] == ')') // ";-)"
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 3));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 3;
				}
			} else if (index < length - 1&&msg_chr[index + 1] == ')')// ";)"
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 2));

				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 2;
				}
			}
			break;

		case ':': // 以":"开头的表情,如果有'-'则长度位3
			if (index < length - 2&&msg_chr[index + 1] == '-')
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 3));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 3;
				}
			}
			// ":'(" 长度也为3
			else if (index < length - 2&&msg_chr[index + 1] == '\'' 
					&& msg_chr[index + 2] == '(') // ":'("
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 3));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 3;
				}
			}// 其它 长度都为2
			else
			{
				if(index<length-1)
				{
					Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
							index, 2));
					if (object != null)
					{
						iconIndex = (Integer) object;
						flag = true;
						pace = 2;
					}
				}
			}
			break;

		case '<': // 以"<"开头的表情,只有"<:o)"
			if (index < length - 3&&msg_chr[index + 1] == ':'
					&& msg_chr[index + 2] == 'o'
					&& msg_chr[index + 3] == ')')
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 4));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 4;
				}
			}
			break;

		case '*':// '*'打头 3到7个字符长度
			if (index < length - 2)
			{
				temp_int = index + 2;
				Object object;
				for (; temp_int < index + 7; temp_int++)
				{
					if (temp_int == length)
					{
						break;
					}
					object = null;
					object = EmotionTextView.faceHashtable.get(new String(
							msg_chr, index, temp_int + 1 - index));
					if (object != null)
					{
						iconIndex = (Integer) object;
						flag = true;
						pace = temp_int + 1 - index;
						break;
					}
				}
			}
			break;
		default: // 其他表情 "^o)” "|-)” "+o("
			if (index < length - 2)
			{
				Object object = EmotionTextView.faceHashtable.get(new String(msg_chr,
						index, 3));
				if (object != null)
				{
					iconIndex = (Integer) object;
					flag = true;
					pace = 3;
				}
			}
			break;
		}
		ob=new Object[]{new Boolean(flag),iconIndex, new Integer(pace)};
		return ob;
	}
	
	/**
	 * 绘制表情文本
	 * @param g
	 * @param v
	 * @param x
	 * @param y
	 * @param anchor
	 * @return endX 末尾的x坐标
	 */
	public static void drawEmotionText(View view,Canvas canvas, Vector v, int x, int y,Paint paint)
	{
		int drawX = x;
		try
		{
//		int oldX = g.getClipX();
//		int oldY = g.getClipY();
//		int oldW = g.getClipWidth();
//		int oldH = g.getClipHeight();

		int offsetX = 0;
		int offsetY = 0;
		// offsetX的设置
//		if ((anchor & Graphics.LEFT) != 0)
//		{
//			offsetX = 0;
//		} else if ((anchor & Graphics.HCENTER) != 0)
//		{
//			offsetX = emotionWidth / 2;
//		} else if ((anchor & Graphics.RIGHT) != 0)
//		{
//			offsetX = emotionWidth;
//		}
//		// offsetY的设置
//		if ((anchor & Graphics.TOP) != 0)
//		{
//			offsetY = (Constant.ICON_HEIGHT - Constant.msnFontHeight) / 2;
//		} else if ((anchor & Graphics.BASELINE) != 0)
//		{
//			offsetY = emotionWidth / 2;
//		} else if ((anchor & Graphics.BOTTOM) != 0)
//		{
//			offsetY = emotionWidth;
//		}
		offsetY = EmotionTextView.emotionWidth;
		//Rect rect=new Rect();
		Object obj;
		String str;
		int textWidth;
		int drawXIcon;
		int iconIndex;
		Resources r;
		Bitmap bmp;
		for (int i = 0; i < v.size(); i++)
		{
			obj = null;
			obj = v.elementAt(i);
			if (obj instanceof String)
			{ // 字符串
				str = (String) obj;
				canvas.drawText(str, drawX, y, paint);
				//drawX += Constant.msnFont.stringWidth(str);
				//paint.getTextBounds(str, 0, str.length(), rect);
				textWidth = (int)paint.measureText(str, 0, str.length());
				drawX+=textWidth + 2;
			} 
			else
			{ // 表情
				drawXIcon = drawX - offsetX;
				//if (drawXIcon > oldX - EmotionTextView.emotionWidth && drawXIcon < oldX + oldW)
				//{ // 如果表情全部在可视范围外(oldX左面和oldX+oldW右面)，则不用画出
				//	if (drawXIcon < oldX) // 如果有一部分表情在可视范围外，则画出部分
				//		g.setClip(oldX, y - offsetY, drawXIcon + EmotionTextView.emotionWidth
				//				- oldX, EmotionTextView.emotionWidth);
				//	else if (drawXIcon + EmotionTextView.emotionWidth > oldX + oldW)
				//		g.setClip(drawXIcon, y - offsetY, oldX + oldW
				//				- drawXIcon, EmotionTextView.emotionWidth);
				//	else
				//		g.setClip(drawXIcon, y - offsetY, EmotionTextView.emotionWidth,
				//				EmotionTextView.emotionWidth);

					iconIndex = ((Integer) obj).intValue();
					
					//#ifdef support_enough_memory
					
					//g.drawImage(faceImage, drawXIcon - iconIndex % 5* EmotionTextView.emotionWidth, y - offsetY - iconIndex / 5* emotionWidth, Graphics.TOP | Graphics.LEFT);
					r = view.getResources();

					bmp = BitmapFactory.decodeResource(r,iconIndex);
					
					canvas.drawBitmap(bmp, drawXIcon, y - offsetY, paint);
					
					//#else
					//# g.drawImage(faceImage, drawXIcon - iconIndex * emotionWidth, y - offsetY , Graphics.TOP | Graphics.LEFT);
					//#endif
				//	g.setClip(oldX, oldY, oldW, oldH);
			//	}
				drawX += EmotionTextView.emotionWidth + 2;
			}
		}
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * 用户输入的帐号是否是有效的,如果无效 则返回提示信息,有效 则return null
	 * @param allEmail 用户输入的帐号 需要加上后缀
	 * @param isLogin 是否是登录页面的判断 如果不是 则可能提示“您不能对自己进行操作”
	 * */
	public static String getUsernameIsValid(String allEmail,boolean isLogin)
	{
        String username=null;
        String domailname=null;
        if (allEmail.length() == 0) 
        {    
            return (String)MSNApplication.mActivity.getText(R.string.text_input_username_cannot_null);
        }
        if(allEmail.indexOf('@')>=0)
        {
          username=allEmail.substring(0,allEmail.indexOf('@'));
          if(allEmail.length()>=username.length()+1)
              domailname=allEmail.substring(allEmail.indexOf('@')+1);
        }
        else
        	return (String)MSNApplication.mActivity.getText(R.string.text_failure_email_address);
        if (!isLogin&&MSNApplication.getApplication().GetLiveID().equals(allEmail)) 
        {
        	return (String)MSNApplication.mActivity.getText(R.string.text_cannot_work_self);
        }
        ////进行必要的格式判断
        else if(!isNormalUsernameFormat(allEmail)||username.length()>64||domailname.length()>48||username.length()==0||domailname.length()<=0)
        { //无效的电子邮件地址
            return (String)MSNApplication.mActivity.getText(R.string.text_failure_email_address);
        } 
        else
        	return null;
	}
    
	/** 判断输入用户名格式是否有误 
	 * 48-57（数字）97-122（小写字母）65-90（大写字母）95（下划线）64 AT符号（@）46 点符号（.）-符号 45
	 * */
	private static boolean isNormalUsernameFormat(String username)
	{
		boolean isNormal=false;
		char c;
		for(int i=0;i<username.length();i++)
		{
			c=username.charAt(i);
			if((c>=48&&c<=57)||(c>=97&&c<=122)||(c>=65&&c<=90)||c==95||c==64||c==46||c==45)
				isNormal=true;
			else
			{
				isNormal=false;
				break;
			}
		}
		return isNormal;
	}
    

	
	/**字符的ASCII码在33到126之间*/
	public static boolean isNormalPasswordFormat(String psd)
	{
		boolean isNormal=true;
		char c;
		for(int i=0;i<psd.length();i++)
		{
			c = psd.charAt(i);
			if(c<33||c>126)
			{
				isNormal=false;
				break;
			}
		}
		return isNormal;

	}
	
	private static char[] base64EncodeChars = new char[] {  
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  
        'w', 'x', 'y', 'z', '0', '1', '2', '3',  
        '4', '5', '6', '7', '8', '9', '+', '/' };  
	
	private static byte[] base64DecodeChars = new byte[] {  
	     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  
	     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  
	     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,  
	     52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,  
	     -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  
	     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,  
	     -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  
	     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1 };  

	    
	     public static String encode(byte[] data) {  
	         StringBuffer sb = new StringBuffer(); 
	         if(data == null)
	         {
	        	 return null;
	         }
	         int len = data.length;  
	         int i = 0;  
	         int b1, b2, b3;  
	    
	         while (i < len) {  
	             b1 = data[i++] & 0xff;  
	             if (i == len) {  
	                 sb.append(base64EncodeChars[b1 >>> 2]);  
	                 sb.append(base64EncodeChars[(b1 & 0x3) << 4]);  
	                 sb.append("==");  
	                 break;  
	             }  
	             b2 = data[i++] & 0xff;  
	             if (i == len) {  
	                 sb.append(base64EncodeChars[b1 >>> 2]);  
	                 sb.append(  
	                         base64EncodeChars[((b1 & 0x03) << 4) | ((b2 & 0xf0) >>> 4)]);  
	                 sb.append(base64EncodeChars[(b2 & 0x0f) << 2]);  
	                 sb.append("=");  
	                 break;  
	             }  
	             b3 = data[i++] & 0xff;  
	             sb.append(base64EncodeChars[b1 >>> 2]);  
	             sb.append(  
	                     base64EncodeChars[((b1 & 0x03) << 4) | ((b2 & 0xf0) >>> 4)]);  
	             sb.append(  
	                     base64EncodeChars[((b2 & 0x0f) << 2) | ((b3 & 0xc0) >>> 6)]);  
	             sb.append(base64EncodeChars[b3 & 0x3f]);  
	         }  
	         return sb.toString();  
	     }  
	    
	     public static byte[] decode(String str) {  
	         byte[] data = str.getBytes();  
	         int len = data.length;  
	         ByteArrayOutputStream buf = new ByteArrayOutputStream(len);  
	         int i = 0;  
	         int b1, b2, b3, b4;  
	    
	         while (i < len) {  
	    
	             /* b1 */  
	             do {  
	                 b1 = base64DecodeChars[data[i++]];  
	             } while (i < len && b1 == -1);  
	             if (b1 == -1) {  
	                 break;  
	             }  
	    
	             /* b2 */  
	             do {  
	                 b2 = base64DecodeChars[data[i++]];  
	             } while (i < len && b2 == -1);  
	             if (b2 == -1) {  
	                 break;  
	             }  
	             buf.write((int) ((b1 << 2) | ((b2 & 0x30) >>> 4)));  
	    
	             /* b3 */  
	             do {  
	                 b3 = data[i++];  
	                 if (b3 == 61) {  
	                     return buf.toByteArray();  
	                 }  
	                 b3 = base64DecodeChars[b3];  
	             } while (i < len && b3 == -1);  
	             if (b3 == -1) {  
	                 break;  
	             }  
	             buf.write((int) (((b2 & 0x0f) << 4) | ((b3 & 0x3c) >>> 2)));  
	    
	             /* b4 */  
	             do {  
	                 b4 = data[i++];  
	                 if (b4 == 61) {  
	                     return buf.toByteArray();  
	                 }  
	                 b4 = base64DecodeChars[b4];  
	             } while (i < len && b4 == -1);  
	             if (b4 == -1) {  
	                 break;  
	             }  
	             buf.write((int) (((b3 & 0x03) << 6) | b4));  
	         }  
	         return buf.toByteArray();  
	     }  
	     
	 	public static String OutPutFile(Item msg)
	    {  
			FileOutputStream fOut = null;
			try {

					File f =  new File("/sdcard", msg.MESSAGE_FILE_VOICE_NAME);
					fOut = new FileOutputStream(f); 

			} catch (FileNotFoundException e) {
				e.printStackTrace();
				//Log.v("Error", e.getMessage());
				return e.getMessage();
			}
			try {
				fOut.write(msg.MESSAGE_FILE_DATA);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}

			try {
	        	fOut.flush();
			} catch (IOException e) {
				e.printStackTrace();
			}
	        try {
	        	fOut.close();
			} catch (IOException e) {
				e.printStackTrace();
			}	
			return null;
	    }
	 	
		/*获取剩余内存*/
		public static long getFreeMemory()
		{
			return Runtime.getRuntime().freeMemory();
		}
	
		public static byte getAdType(String action)
		{
			byte temp = MSNApplication.AD_WAP_FLAG;
			if(action!=null)
			{
				if (action.equals("wap"))
					temp = MSNApplication.AD_WAP_FLAG;
				else if (action.equals("zwp"))
					temp = MSNApplication.AD_ZWP_FLAG;
				else if (action.equals("tel"))
					temp = MSNApplication.AD_IVR_FLAG;
				else if (action.equals("sms"))
					temp = MSNApplication.AD_SMS_FLAG;
			}
			return temp;
		}	
		
		
		/**将数据存入文件系统
		 * @param 文件夹
		 * @param 文件名
		 * @param 数据
		 * return true成功 false 失败
		 * */
		public static Boolean saveDataToFile(String currentDir,String currentFile,byte[] imgData)
	    {  
			FileOutputStream fileOps = null;
			
			File currentDirFile = new File(currentDir);		
			currentDirFile.mkdirs(); 
			
			File saveFile;
			try 
			{
				saveFile=new File(currentDir+currentFile);
				fileOps = new FileOutputStream(saveFile); 
				fileOps.write(imgData);
				fileOps.flush();
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
				//Log.v("Error", e.getMessage());
				return false;
			}
			finally
			{
		        try 
		        {
		        	if(fileOps!=null)
		        		fileOps.close();
		        	
		        	return true;
				} 
		        catch (IOException e) 
				{
					e.printStackTrace();
					return false;
					
				}
			}

	    }
		
		/**将文件系统里面的某文件删除
		 * @param 文件夹
		 * @param 文件名
		 * @param 数据
		 * */
		public static void deleteDataFormFile(String currentDir,String currentFile)
	    {  
			try
			{
				String fileLocation=currentDir+currentFile;
				File deleteFile = new File(fileLocation);	
				if(deleteFile.isFile())
				{
					deleteFile.delete();
				}	
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
	
	    }
		
		/**删除文件夹*/
		public static void deleteDirectory(String currentDir)
		{
			try
			{
				File deleteFile = new File(currentDir);	
				deleteFile.delete();
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}
		
		
		
		/**得到文件数据 */
		public static byte[] getDataFromFile(String currentDir,String currentFile)
		{
			String fileLocation=currentDir+currentFile;
			File getFile = new File(fileLocation);	
			FileInputStream fileIps=null;
			byte[] data=null;
			int length=0;
			try 
			{
				fileIps=new FileInputStream(getFile);
				length=(int)getFile.length();
			} 
			catch (FileNotFoundException e) 
			{				
				return null;
			}
			if(fileIps!=null&&length!=0)
			{
				data=new byte[length];
				try 
				{
					fileIps.read(data);
					return data;
				} 
				catch (IOException e) 
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
					return null;
				}
				
			}
			return data;
		}
		
		/**得到当前文件夹下面所有的文件名*/
		public static Vector getChildrenFile(String currentDir)
		{
		    Vector strVector = new Vector();
		    try
		    {
			    File tempfile = new File(currentDir);
		    	String[] childname = tempfile.list();
				int i = childname.length;	
				for(int j=0; j<i; j++)
				{
					strVector.add(childname[j]);
				}
				return strVector;
		    }
		    catch(Exception e)
		    {
		    	return null;
		    }
		}
		
		/**删除属于该对话的临时音频文件
		 * */
	    public static void deleteAudioBySession(Context context,Item session)
	    {	    	
			String temp = "justforplay" + session.TOPITEM_JID;
			File filesDir = context.getFilesDir();    				
			String[] childfilesName = filesDir.list(); 
			int n = childfilesName.length;
			for(int i=0; i<n;i++)
				if(childfilesName[i].contains(temp))
				{
					context.deleteFile(childfilesName[i]);
				}		
	    }
	    
	    /**删除所有对话的临时音频文件
		 * */    
	   public static void deleteAllTempAudio(Context context)
	    {    	
	    	String temp = "justforplay";
			File filesDir = context.getFilesDir();
			if(filesDir == null)
			{
				return;
			}
			String[] childfilesName = filesDir.list(); 
			if(childfilesName == null)
			{
				return;
			}
			int n = childfilesName.length;
			for(int i=0; i<n;i++)
				if(childfilesName[i].contains(temp))
				{
					context.deleteFile(childfilesName[i]);
				}			
	    }
	    
	    /**删除包目录下的临时目录的所有文件
	     * 
	     * DirName   目录名
		 * */    
	    public static void deleteTempDir(Context context, String DirName)
	    {
	    	String packageName = context.getPackageName();
			if(packageName == null)
			{
				return;
			}
			File temprecordDir = new File("/data/data/" + packageName + "/" +DirName);
			if(temprecordDir == null)
			{
				return;
			}
			String[] childfilesName = temprecordDir.list(); 
			if(childfilesName == null)
			{
				return;
			}
			int n = childfilesName.length;
			File tempfile;
			for(int i=0; i<n;i++)
			{
				tempfile = null;
				tempfile = new File(temprecordDir.getAbsolutePath(), childfilesName[i]);
				tempfile.delete();
			}		
	    }

	    /**删除包目录下的"files"目录的所有文件
		 * */ 
	    public static void deleteFilesDir(Context context)
	    {	    	
			File filesDir = context.getFilesDir();		
			String[] childfilesName = filesDir.list(); 
			int n = childfilesName.length;
			for(int i=0; i<n;i++)				
				context.deleteFile(childfilesName[i]);			
	    }
	    
		  /**是否是发送消息
		   *@return true 发送 false 接收
		   * */
		  public static boolean getIsSendMessage(Item msg)
		  {
			  if(msg.MESSAGE_type == MSNApplication.MESSAGE_TYPE_FILE_TRANSFER)//文件
			  {
				  if(msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING
						  ||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_WAITING_REFUSED
						  ||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_SENDING
						  ||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_COMPLETE
						  ||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_CANCEL
						  ||msg.MESSAGE_FILE_STATUS_TYPE==MSNApplication.MESSAGE_FILE_TYPE_SEND_EXCEPTION)
					  return true;
				  else
					  return false;
			  }
			  else if(msg.MESSAGE_type ==MSNApplication.MESSAGE_TYPE_VOICECLIP)//语音
			  {
				  if(msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_INNER_SEND_COMPLETE
						  ||msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_COMPLETE
						  ||msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_SENDING
						  ||msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_SEND_CANCEL
						  ||msg.MESSAGE_VOICE_STATUS_TYPE==MSNApplication.MESSAGE_VOICE_TYPE_OUTER_SEND_EXCEPTION)
					  return true;
				  else
					  return false;
			  }
			  else//普通消息
			  {
				  return msg.MESSAGE_IS_SEND;
			  }
		  }
		  
			
			/**
			 * 返回一个能够正确存储到记录库中的有效数据.
			 * @return
			 */
			public static String toValidRs(String obj)
			{
				if (obj == null)
					return "@*@";
				else if(obj.indexOf("'")!=-1)
				{
					return obj.replace("'", "*@*");
				}
				else
					return obj;
			}
			
			/**
			 * 返回原始数据.
			 * @param str
			 */
			public static String getUnvalidFormRs(String obj)
			{

					if (obj.equals("@*@"))
						return null;
					else if(obj.indexOf("*@*")!=-1)
					{
						return obj.replace("*@*", "'");
					}
					else
						return obj;

			}
			
			
		     public static String DebugOutPutFile(String str)
		        {  
//		         File f =  new File("/sdcard", "/debug.txt");
//                 try {
//                    RandomAccessFile raf = new RandomAccessFile(f, "rwd");
//                    raf.seek(raf.length());
//                    raf.writeBytes(str);
//                    raf.close();
//                } catch (Exception e) {
//                    // TODO Auto-generated catch block
//                    e.printStackTrace();
//                }
//                
//                return str;
		         return null;
		        }
}
