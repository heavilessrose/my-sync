package winkCC.net;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.file.FileConnection;

import winkCC.log.ILog;
import winkCC.log.LogFactory;
import winkCC.push.PushUtil;

public class HttpUtil {
	private static ILog log = LogFactory.getLog("HttpUtil");

	public static HttpConnection connect(String url) throws IOException {
		HttpConnection conn = (HttpConnection) Connector.open(url,
				Connector.READ_WRITE);
		log.info("get connection success");
		return conn;
	}

	public void send() {

	}

	static final int MAX_LENGTH = 1024;

	public static void getPage(HttpConnection conn) throws IOException {
		log.info("start get page");
		InputStream is = conn.openInputStream();
		int code = conn.getResponseCode();
		String type = conn.getType();
		// int pageLength = (int) conn.getLength();
		String requestMethod = HttpConnection.GET;

		if (code == HttpConnection.HTTP_OK/* && type.equals("text/html") */) {

			// String htmlPage = getText(is);
			byte[] htmlPage = getBytes(is);
			log.info("get page success");
			// 设置定时激活
			PushUtil.scheduleMIDlet(60000, HttpPushTestMidlet.getInstance()
					.getClass().getName());
			writeToFile(htmlPage/* .getBytes() */);
		}
	}

	public static String getText(InputStream is) {
		char ch;
		StringBuffer buff = new StringBuffer();
		try {
			while ((ch = (char) is.read()) != -1) {
				buff.append(ch);
			}
		} catch (IOException e) {
			log.debug("read from server error", e);
		}
		return buff.toString();
	}

	public static byte[] getBytes(InputStream is) {
		// 读取页面并写入文件, 最多读取MAX_LENGTH个字节
		byte[] b = new byte[MAX_LENGTH];
		// byte[] b = new byte[pageLength];
		int total = 0;
		while (total < MAX_LENGTH) {
			try {
				int count = is.read(b, total, MAX_LENGTH - total);
				log.info("count = " + count);
				if (count < 0) {
					log.info("count = " + count
							+ " get page finish but trunked");
					break;
				}
				total += count;
			} catch (IOException e) {
				log.debug("get bytes from server error ", e);
			}
		}

		return b;
	}

	private static boolean workDone = false;

	public static boolean getState() {
		return workDone;
	}

	public static void writeToFile(byte[] data) {
		if (data == null || data.length == 0)
			return;
		FileConnection fc = null;
		DataOutputStream out = null;
		String fileName = null;
		try {
			fileName = "google.html";
//			fc = (FileConnection) Connector.open("file:///root1/");
			 fc = (FileConnection) Connector.open("file:///e:/ccpp/");
			if (!fc.exists())
				fc.mkdir();
//			fc = (FileConnection) Connector.open("file:///root1/" + fileName);
			 fc = (FileConnection) Connector.open("file:///e:/ccpp/" +
			 fileName);
			if (!fc.exists())
				fc.create();

			out = fc.openDataOutputStream();
			out.write(data);
			out.flush();
			log.info("write to file success");
		} catch (IOException e1) {
			log.debug(e1);
		} finally {
			try {
				if (out != null) {
					out.close();
					out = null;
				}
				if (fc != null) {
					fc.close();
					fc = null;
				}
			} catch (IOException e) {
				log.debug("exception on close", e);
			}
			workDone = true;
		}
	}
}
