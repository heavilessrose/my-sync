package winkCC.net.http2;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

/**
 * ������Http���ӵ��߳��࣬������һ��URL��HttpListener.
 * 
 * @author WangYinghua
 * 
 */
class HttpThread extends Thread {
	private static final int MAX_LENGTH = 20 * 1024; // 20K
	private boolean cancel = false;
	private String url;
	private byte[] buffer = null;
	private HttpListener listener;

	public HttpThread(String url, HttpListener listener) {
		this.url = url;
		this.listener = listener;
	}

	public void cancel() {
		cancel = true;
	}

	public void run() {
		get();
	}

	/**
	 * GET����. GET����ֻ�������������һ��URL,Ȼ��ȡ�÷�������Ӧ.
	 */
	private void get() {
		HttpConnection conn = null;
		InputStream input = null;
		try {
			conn = (HttpConnection) Connector.open(url);
			conn.setRequestMethod(HttpConnection.GET); // Ĭ�ϼ�ΪGET
			conn.setRequestProperty("User-Agent",
					"Profile/MIDP-2.0 Configuration/CLDC-1.0");
			// get response code:
			int code = conn.getResponseCode();
			if (code != HttpConnection.HTTP_OK) {
				listener.onError(code, conn.getResponseMessage());
				return;
			}
			// get size:
			int size = (int) conn.getLength(); // ������Ӧ��С������-1�����С�޷�ȷ��
			listener.onSetSize(size);
			// ��ʼ����Ӧ��
			input = conn.openInputStream();
			int percent = 0; // percentage
			int tmp_percent = 0;
			int index = 0; // buffer index
			int reads; // each byte
			if (size != (-1))
				buffer = new byte[size]; // ��Ӧ��С��֪��ȷ����������С
			else
				buffer = new byte[MAX_LENGTH]; // ��Ӧ��Сδ֪���趨һ���̶���С�Ļ�����
			while (!cancel) {
				int len = buffer.length - index;
				len = len > 128 ? 128 : len;
				reads = input.read(buffer, index, len);
				if (reads <= 0)
					break;
				index += reads;
				if (size > 0) { // ���½���
					tmp_percent = index * 100 / size;
					if (tmp_percent != percent) {
						percent = tmp_percent;
						listener.onProgress(percent);
					}
				}
			}
			if (!cancel && input.available() > 0) // �������������޷�������ȡ
				listener.onError(601, "Buffer overflow.");
			if (!cancel) {
				if (size != (-1) && index != size)
					listener.onError(102, "Content-Length does not match.");
				else
					listener.onFinish(buffer, index);
			}
		} catch (IOException ioe) {
			listener.onError(101, "IOException: " + ioe.getMessage());
		} finally { // ������Դ
			if (input != null)
				try {
					input.close();
				} catch (IOException ioe) {
				}
			if (conn != null)
				try {
					conn.close();
				} catch (IOException ioe) {
				}
		}
	}
	
	private void post(){
		
	}
}