package winkCC.net.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

/**
 * 
 * @author WangYinghua
 * 
 */
public class HttpUtil {
	/** 可读写的Http连接 */
	private static HttpConnection _conn;
	private static OutputStream _streamOut;
	private static InputStream _streamIn;

	private static boolean _cmwap = false;

	private static boolean isRun = true;

	/** 默认缓冲区大小 */
	private static int BUFFER_LENGTH = 50 * 1024; // 50k
	/** 默认的分段大小 */
	private static int DATA_SEG = 1;

	/**
	 * 每次联网操作前须进行联网初始化.
	 * 
	 * @param url
	 * @param cmwap
	 * 
	 * @throws IOException
	 *             建立连接, 打开输入输出流时
	 */
	public static void init(String url, boolean cmwap) throws IOException {
		close();
		_cmwap = cmwap;
		if (url.indexOf("://") == -1) {
			url = "http://" + url;
		}

		if (_cmwap) {
			String temp = url.substring(7);
			String host = "";
			if (temp.indexOf("/") > 0) {
				host = temp.substring(0, temp.indexOf("/"));
			} else {
				host = temp;
			}
			String path = "";
			if (temp.indexOf("/") > 0) {
				path = temp.substring(temp.indexOf("/"));
			} else {
				path = "/fengqi"; // 默认path
			}
			_conn = (HttpConnection) Connector.open("http://10.0.0.172:80"
					+ path, Connector.READ_WRITE, true);
			_conn.setRequestProperty("X-Online-Host", host);

			_conn.setRequestMethod(HttpConnection.POST);
			_conn.setRequestProperty("User-Agent",
					"Profile/MIDP-2.0 Configuration/CLDC-1.0");
			// _conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
			// _conn.setRequestProperty("Content-Type", "application/octet-stream");
			_conn.setRequestProperty("Content-Type", "*/*");
			//			_conn.setRequestProperty("Content-Type", "application/json");
			_conn.setRequestProperty("Content-Language", "UTF-8");
			// _conn.setRequestProperty("Content-Length", Integer.toString(body.length));
			// _conn.setRequestProperty("Connection", "Keep-Alive");
			// _conn.setRequestProperty("Connection", "Close");
			// _conn.setRequestProperty("Cache-Control", "no-cache");

		} else {// cmnet
			// 此处如果没有true参数则在读取响应时会抛出异常, 原因不明.
			_conn = (HttpConnection) Connector.open(url, Connector.READ_WRITE,
					true);
			//			_conn.setRequestMethod(HttpConnection.GET);
		}
	}

	/**
	 * http post方法实现.
	 * 
	 * @param data
	 *            发送的数据
	 * @throws IOException
	 */
	public byte[] post(byte[] data) throws IOException {
		_streamOut = _conn.openOutputStream();
		int index = 0;
		while (isRun) {
			// 分段的发送请求数据
			int tmpLen = data.length - index;
			if (index >= data.length)
				break;
			// 每次写DATA_SEG byte
			tmpLen = tmpLen > DATA_SEG ? DATA_SEG : tmpLen;
			_streamOut.write(data, index, tmpLen);
			index += tmpLen;
			System.out.println("post: " + index);
		}

		// 读取响应
		int responseCode = _conn.getResponseCode();
		System.out.println(responseCode);
		index = 0;
		_streamIn = _conn.openInputStream();
		byte[] buffer = null;
		int size = (int) _conn.getLength();
		if (size != (-1)) {// 响应大小已知，确定缓冲区大小
			buffer = new byte[size];
		} else {// 响应大小未知，设定一个固定大小的缓冲区
			buffer = new byte[BUFFER_LENGTH];
		}
		while (isRun) {
			// 分段的接收服务器响应的数据, 如果使用固定缓冲区, 超出缓冲区的字节被丢弃.
			int tmpLen = 0;
			if (size != -1) {
				tmpLen = size - index;
			} else {
				tmpLen = BUFFER_LENGTH - index;
			}
			if (tmpLen <= 0)
				break;
			tmpLen = tmpLen > DATA_SEG ? DATA_SEG : tmpLen;
			_streamIn.read(buffer, index, tmpLen);
			index += tmpLen;

			// System.out.println("rec: " + index);
		}
		System.out.println(new String(buffer));
		return buffer;
	}

	public static byte[] get() throws IOException {

		int index = 0;
		int responseCode = _conn.getResponseCode();
		System.out.println("responseCode: " + responseCode);
		_streamIn = _conn.openInputStream();
		byte[] buffer = null;
		int size = (int) _conn.getLength();
		if (size != (-1)) {// 响应大小已知，确定缓冲区大小
			buffer = new byte[size];
		} else {// 响应大小未知，设定一个固定大小的缓冲区
			buffer = new byte[BUFFER_LENGTH];
		}
		while (isRun) {
			// 分段的接收服务器响应的数据, 如果使用固定缓冲区, 超出缓冲区的字节被丢弃.
			int tmpLen = 0;
			if (size != -1) {
				tmpLen = size - index;
			} else {
				tmpLen = BUFFER_LENGTH - index;
			}
			if (tmpLen <= 0)
				break;
			tmpLen = tmpLen > DATA_SEG ? DATA_SEG : tmpLen;
			_streamIn.read(buffer, index, tmpLen);
			index += tmpLen;

			// System.out.println("rec: " + index);
		}
		System.out.println(new String(buffer));
		return buffer;
	}

	/**
	 * 结束时必须调用.关闭连接, 输入输出流, 注销监听.
	 * 
	 * @throws IOException
	 */
	public static void close() throws IOException {

		if (_streamOut != null) {
			_streamOut.close();
			_streamOut = null;
		}
		if (_streamIn != null) {
			_streamIn.close();
			_streamIn = null;
		}
		if (_conn != null) {
			_conn.close();
			_conn = null;
		}
	}

	/**
	 * 停止联网操作.
	 */
	public void cancel() {
		isRun = false;
	}
}
