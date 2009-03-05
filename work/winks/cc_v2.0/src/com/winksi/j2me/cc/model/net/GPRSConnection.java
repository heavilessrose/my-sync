package com.winksi.j2me.cc.model.net;

import com.sun.midp.io.SystemOutputStream;
import com.winksi.j2me.cc.common.CPProperty;
//import com.winksi.j2me.cc.tools.StringTools;
import java.io.*;

import javax.microedition.io.*;
import javax.microedition.lcdui.*;

/** 联网处理 */
public class GPRSConnection extends Thread {

	//    Thread self=null;
	HttpListener listener;
	String url;
	byte[] body;
	String result = "";
	public int percent = 0;
	private boolean isCmwap = true;//true: cmwap;  false: cmnet
	private boolean running = false;

	//Thread self=null;
	public GPRSConnection(HttpListener listener, String url, byte[] body) {
		this.listener = listener;
		this.url = url;
		this.body = body;
		isCmwap = false;
		running = true;
	}

	//    public void start(){
	//        if(self==null){
	//            self=new Thread(this);
	//            self.start();
	//        }
	//    }
	public void cancel() {
		running = false;
	}

	public boolean isRunning() {
		return running;
	}

	public void run() {
		if (running) {
			try {
				// 这个方法写了太多逻辑了,需要重构一下
				getResourcesFile(url, body);
				Thread.sleep(100);
			} catch (Exception e) {
				//e.printStackTrace();
			}
		}
	}

	/** 将按协议组织好的数据发送到服务器，并从服务器拿回返回的数据 */
	public synchronized void getResourcesFile(String url, byte[] body) {
		HttpConnection conn = null;
		DataInputStream in = null;
		DataOutputStream outputStream = null;
		byte[] bytesFromServer = null;
		//        if (body.length > 5 * 1024) {//debug connection haibo
		//            url = "http://219.143.46.226:8088/ClientServerMss/send";
		//        }

		if (!url.startsWith("http://")) {
			url = "http://" + url;
		}

		// 试6次?
		for (int index = 0; index < 6; index++) {
			if (index > 2) {
				isCmwap = false;
			}
			try {
				if (running) {
					listener.setGprs(true);
					if (!isCmwap) { // cmnet连接
						System.out.println("GPRSConnection:run(): cmnet: open = " + url);
						try {
							conn = (HttpConnection) Connector.open(url, Connector.READ_WRITE, true);
						} catch (Exception e) {
							System.out.println("GPRSConnection:run(): open cmnet throws an Exception: ");
							conn.close();
							e.printStackTrace();
						}
						listener.setGprs(true);
						System.out.println("GPRSConnection:run(): cmnet opened: ok");
					} else { // cmwap连接需要特殊的处理
						String temp = url.substring(7);
						//                        System.out.println(" temp : " + temp);
						String host = "";
						if (temp.indexOf("/") > 0) {
							host = temp.substring(0, temp.indexOf("/"));
						} else {
							host = temp;
						}
						//                        System.out.println(" host : " + host);
						String path = "";
						if (temp.indexOf("/") > 0) {
							path = temp.substring(temp.indexOf("/"));
						} else {
							path = "/fengqi";
						}
						System.out.println("cmwap:open=" + ("http://10.0.0.172:80" + path));
						//                        System.out.println("open=" + "http://10.0.0.172:80" + path);
						try{
							conn = (HttpConnection) Connector.open("http://10.0.0.172:80" + path, Connector.READ_WRITE,
									true);
						}catch(Exception e){
							conn.close();
							e.printStackTrace();
						}
						//                        System.out.println("ok");
						conn.setRequestProperty("X-Online-Host", host);
					}

					try {
						conn.setRequestMethod(HttpConnection.POST);
						conn.setRequestProperty("User-Agent", "Profile/MIDP-2.0 Configuration/CLDC-1.0");
						// conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
						//                conn.setRequestProperty("Content-Type", "application/octet-stream");
						conn.setRequestProperty("Content-Type", "*/*");
						conn.setRequestProperty("Content-Language", "UTF-8");
						//                    conn.setRequestProperty("Content-Length", Integer.toString(body.length));
						//                    conn.setRequestProperty("Connection", "Keep-Alive");
						//                    conn.setRequestProperty("Connection", "Close");
					} catch (Exception e) {
						e.printStackTrace();
					}
				}

				// 向通讯服务器写
				if (running) {
					System.out.println("GPRSConnection:run(): write to server: " + body.length);
					listener.setGprs(true);
					try {
						outputStream = conn.openDataOutputStream();
					} catch (Exception e) {
						outputStream.close();
						System.out.println("GPRSConnection:run(): openDataOutputStream Exception");
						e.printStackTrace();
					}
					try {
						outputStream.write(body);
//						outputStream.flush();
					} catch (Exception e) {
						try{
							outputStream.close();
						}catch(IOException ee){
							System.out.println("GPRSConnection:run(): IOException while close outputsstream ++++ ");
							ee.printStackTrace();
						}
						System.out.println("GPRSConnection:run(): write to server Exception");
						e.printStackTrace();
					}
					//                    outputStream.close();
					//                    outputStream.flush();
					System.out.println("GPRSConnection:run(): write to server ok: ");
//					for(int i = 0; i < body.length; i++)
//						System.out.println("," + body[i]);
				}

				// 从通讯服务器读
				if (running) {
					System.out.println("GPRSConnection:run(): read from server");
					listener.setGprs(true);

					System.out.println("GPRSConnection:run(): conn: " + conn + "conn: " + (conn != null));
					try{
						in = conn.openDataInputStream();
					}catch(Exception e){
						in.close();
						e.printStackTrace();
					}
					System.out.println("GPRSConnection:run(): open inputstream");
					int ch = -1;
					//                    int i = 0;

					StringBuffer sb = new StringBuffer();
					try{
						while ((ch = in.read()) != -1) {
							//                        i++;
							sb.append((char) ch);
						}
					}catch(Exception e){
						in.close();
						e.printStackTrace();
					}
					System.out.println("GPRSConnection:run(): data read from server: " + sb);
					bytesFromServer = sb.toString().getBytes();

					//                    System.out.println("len: " + b.length);
					//                    System.out.println("get : ");
					//
					//                    for (int ii = 0; ii < b.length; ii++) {
					//                        System.out.print(" " + b[ii]);
					//                    }
					//                    System.out.println(" ");

					if (outputStream != null) {
						outputStream.close();
						outputStream = null;
					}
					if (in != null) {
						in.close();
						in = null;
					}
					if (conn != null) {
						conn.close();
						conn = null;
					}
					System.out.println("read from server success! ");
				}

				// 
				if (running) {
					try {
						String responseString = new String(bytesFromServer, CPProperty.UTF8);
						System.out.println("GPRSConnection:run(): response from commuServer: " + responseString);
						// 是否是移动资费页(简单判断)
						if (responseString.indexOf("DTD") > 0) {
							System.out.println("GPRSConnection:run(): get the CMCC payment page, retry request ++");
							continue;
						}
					} catch (Exception e) {
					}
					//                    listener.setGprs(false);

					System.out.println("GPRSConnection:run(): response from commuServer OK");
					running = false;
					//                    index = 6;
					listener.setGprs(false);
					// 去解析返回的数据
					listener.onFinish(bytesFromServer);
					//                    System.out.println("over");
				}
				break;
			} catch (SecurityException se) {
				listener.setGprs(false);
				listener.onError(12);
			} catch (IOException e) {
				System.out.println("IO  exception");
				e.printStackTrace();
				listener.setGprs(false);

				if (index > 4) {
					listener.onError(11);
				} else {
					continue;
				}
				//                listener.onError(0);
			} finally {
				// ??
				System.out.println("GPRSConnection:run():getResourcesFile(): finally{}");
//				if (index > 4) {
					System.out.println("GPRSConnection:run():getResourcesFile(): close streams and connection");
					listener.setGprs(false);
					try {
						if (conn != null) {
							conn.close();
							conn = null;
						}
						if (outputStream != null) {
							outputStream.close();
							outputStream = null;
						}
						if (in != null) {
							in.close();
							in = null;
						}

					} catch (IOException e) {
						System.out.println("GPRSConnection:run():getResourcesFile(): Exception while close streams and connection ");
						listener.onError(11);
						e.printStackTrace();
					}
//					running = false;
//				}
					// luke add
					if (index > 4){ // 如果超过尝试次数
						running = false;
					}
			}
		}
	}
}
