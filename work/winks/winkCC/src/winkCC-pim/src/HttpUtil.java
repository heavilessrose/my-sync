import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

public class HttpUtil {
	/*
	 * 缺省过滤器实例
	 */
	protected static HttpConnectionFilter filter = new HttpConnectionFilter() {
		public boolean accept(HttpConnection con) {
			/* 资费提示wap页面长度=438+目的地URL字符串字节长度 */
			return con != null
					&& con.getLength() != 438 + con.getURL().getBytes().length;
		}
	};

	/**
	 * 打开一个HttpConnection
	 * 
	 * @param url
	 */
	public static HttpConnection open(final String url) {
		/* 一个HttpConnection的代理实现 */
		return new HttpConnection() {
			/*
			 * 连接
			 */
			private javax.microedition.io.HttpConnection con;

			/*
			 * 连接次数，超过次数则认为连接失败
			 */
			private int connectCount = 0;
			/*
			 * 输出缓冲
			 */
			private ByteArrayOutputStream baos = new ByteArrayOutputStream(1024);

			/*
			 * 请求属性
			 */
			private Hashtable requestProperties = new Hashtable();

			/*
			 * 请求方式
			 */
			private String method = GET;

			/*
			 * 连接服务器
			 */
			private void connect() {
				try {
					if (this.con != null)
						this.con.close();
				} catch (Exception ex) {
				}
				try {
					this.con = (javax.microedition.io.HttpConnection) Connector
							.open(url);
					this.con.setRequestMethod(this.method);
					Enumeration propertyKeys = this.requestProperties.keys();
					while (propertyKeys.hasMoreElements()) {
						String propertyKey = (String) propertyKeys
								.nextElement();
						String propertyValue = (String) requestProperties
								.get(propertyKey);
						this.con.setRequestProperty(propertyKey, propertyValue);
					}
				} catch (Exception ex) {
					this.con = null;
				}
			}

			/*
			 * 确认连接
			 */
			private void ensureConnect() {
				while (this.con == null || filter != null
						&& !filter.accept(this.con)) {
					if (connectCount >= 5)
						throw new RuntimeException("Connection failed");
					connectCount++;
					this.connect();
				}
			}

			public long getDate() throws IOException {
				ensureConnect();
				return this.con.getDate();
			}

			public long getExpiration() throws IOException {
				ensureConnect();
				return this.con.getExpiration();
			}

			public String getFile() {
				ensureConnect();
				return this.con.getFile();
			}

			public String getHeaderField(String arg0) throws IOException {
				ensureConnect();
				return this.con.getHeaderField(arg0);
			}

			public String getHeaderField(int arg0) throws IOException {
				ensureConnect();
				return this.con.getHeaderField(arg0);
			}

			public long getHeaderFieldDate(String arg0, long arg1)
					throws IOException {
				ensureConnect();
				return this.con.getHeaderFieldDate(arg0, arg1);
			}

			public int getHeaderFieldInt(String arg0, int arg1)
					throws IOException {
				ensureConnect();
				return this.con.getHeaderFieldInt(arg0, arg1);
			}

			public String getHeaderFieldKey(int arg0) throws IOException {
				ensureConnect();
				return this.con.getHeaderFieldKey(arg0);
			}

			public String getHost() {
				ensureConnect();
				return this.con.getHost();
			}

			public long getLastModified() throws IOException {
				ensureConnect();
				return this.con.getLastModified();
			}

			public int getPort() {
				ensureConnect();
				return this.con.getPort();
			}

			public String getProtocol() {
				ensureConnect();
				return this.con.getProtocol();
			}

			public String getQuery() {
				ensureConnect();
				return this.con.getQuery();
			}

			public String getRef() {
				ensureConnect();
				return this.con.getRef();
			}

			public String getRequestMethod() {
				ensureConnect();
				return this.con.getRequestMethod();
			}

			public String getRequestProperty(String arg0) {
				ensureConnect();
				return this.con.getRequestProperty(arg0);
			}

			public int getResponseCode() throws IOException {
				ensureConnect();
				return this.con.getResponseCode();
			}

			public String getResponseMessage() throws IOException {
				ensureConnect();
				return this.con.getResponseMessage();
			}

			public String getURL() {
				ensureConnect();
				return this.con.getURL();
			}

			public void setRequestMethod(String arg0) throws IOException {
				this.method = arg0;
			}

			public void setRequestProperty(String arg0, String arg1)
					throws IOException {
				requestProperties.put(arg0, arg1);
			}

			public String getEncoding() {
				ensureConnect();
				return this.con.getEncoding();
			}

			public long getLength() {
				ensureConnect();
				return this.con.getLength();
			}

			public String getType() {
				ensureConnect();
				return this.con.getType();
			}

			public DataInputStream openDataInputStream() throws IOException {
				ensureConnect();
				return this.con.openDataInputStream();
			}

			public InputStream openInputStream() throws IOException {
				ensureConnect();
				return this.con.openInputStream();
			}

			public void close() throws IOException {
				ensureConnect();
				this.con.close();
			}

			public DataOutputStream openDataOutputStream() throws IOException {
				return new DataOutputStream(baos);
			}

			public OutputStream openOutputStream() throws IOException {
				return baos;
			}
		};
	}

	/*
	 * 设置过滤器,可设置自定义过滤器
	 */
	public static void setHttpConnectionFilter(HttpConnectionFilter filter) {
		HttpUtil.filter = filter;
	}

	/**
	 * 过滤器接口
	 */
	public static interface HttpConnectionFilter {
		boolean accept(HttpConnection con);
	}
}