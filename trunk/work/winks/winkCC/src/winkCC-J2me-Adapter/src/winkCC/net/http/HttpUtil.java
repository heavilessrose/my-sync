package winkCC.net.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

public class HttpUtil {
	/** 可读写的Http连接 */
	private HttpConnection _conn;
	private OutputStream _streamOut;
	private InputStream _streamIn;

	private IHttpListener _listener;
	private boolean _cmwap = false;

	public void init(String url, IHttpListener listener, boolean cmwap)
			throws IOException {

		_listener = listener;
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
			_conn.setRequestProperty("Content-Language", "UTF-8");
			// _conn.setRequestProperty("Content-Length", Integer.toString(body.length));
			// _conn.setRequestProperty("Connection", "Keep-Alive");
			// _conn.setRequestProperty("Connection", "Close");
		} else {
			// cmnet
			_conn = (HttpConnection) Connector.open(url, Connector.READ_WRITE);
			_streamOut = _conn.openOutputStream();
			_streamIn = _conn.openInputStream();
		}
	}

	public void send(byte[] data) throws IOException {
		if (_streamOut != null) {
			_streamOut.write(data);
		}
	}

	public byte[] receive() throws IOException {
		byte[] data = null;
		if (_streamIn != null) {
			_streamIn.read(data);
		}
		return data;
	}

	/**
	 * 关闭连接.
	 * 
	 * @throws IOException
	 */
	public void close() throws IOException {

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
		if (_listener != null) {
			_listener.dispose();
			_listener = null;
		}
	}
}
