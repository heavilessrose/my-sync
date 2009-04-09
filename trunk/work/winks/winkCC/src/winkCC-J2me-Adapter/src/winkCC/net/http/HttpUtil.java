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
	/** �ɶ�д��Http���� */
	private static HttpConnection _conn;
	private static OutputStream _streamOut;
	private static InputStream _streamIn;

	private static boolean _cmwap = false;

	private static boolean isRun = true;

	/** Ĭ�ϻ�������С */
	private static int BUFFER_LENGTH = 50 * 1024; // 50k
	/** Ĭ�ϵķֶδ�С */
	private static int DATA_SEG = 1;

	/**
	 * ÿ����������ǰ�����������ʼ��.
	 * 
	 * @param url
	 * @param cmwap
	 * 
	 * @throws IOException
	 *             ��������, �����������ʱ
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
				path = "/fengqi"; // Ĭ��path
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
			// �˴����û��true�������ڶ�ȡ��Ӧʱ���׳��쳣, ԭ����.
			_conn = (HttpConnection) Connector.open(url, Connector.READ_WRITE,
					true);
			//			_conn.setRequestMethod(HttpConnection.GET);
		}
	}

	/**
	 * http post����ʵ��.
	 * 
	 * @param data
	 *            ���͵�����
	 * @throws IOException
	 */
	public byte[] post(byte[] data) throws IOException {
		_streamOut = _conn.openOutputStream();
		int index = 0;
		while (isRun) {
			// �ֶεķ�����������
			int tmpLen = data.length - index;
			if (index >= data.length)
				break;
			// ÿ��дDATA_SEG byte
			tmpLen = tmpLen > DATA_SEG ? DATA_SEG : tmpLen;
			_streamOut.write(data, index, tmpLen);
			index += tmpLen;
			System.out.println("post: " + index);
		}

		// ��ȡ��Ӧ
		int responseCode = _conn.getResponseCode();
		System.out.println(responseCode);
		index = 0;
		_streamIn = _conn.openInputStream();
		byte[] buffer = null;
		int size = (int) _conn.getLength();
		if (size != (-1)) {// ��Ӧ��С��֪��ȷ����������С
			buffer = new byte[size];
		} else {// ��Ӧ��Сδ֪���趨һ���̶���С�Ļ�����
			buffer = new byte[BUFFER_LENGTH];
		}
		while (isRun) {
			// �ֶεĽ��շ�������Ӧ������, ���ʹ�ù̶�������, �������������ֽڱ�����.
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
		if (size != (-1)) {// ��Ӧ��С��֪��ȷ����������С
			buffer = new byte[size];
		} else {// ��Ӧ��Сδ֪���趨һ���̶���С�Ļ�����
			buffer = new byte[BUFFER_LENGTH];
		}
		while (isRun) {
			// �ֶεĽ��շ�������Ӧ������, ���ʹ�ù̶�������, �������������ֽڱ�����.
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
	 * ����ʱ�������.�ر�����, ���������, ע������.
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
	 * ֹͣ��������.
	 */
	public void cancel() {
		isRun = false;
	}
}
