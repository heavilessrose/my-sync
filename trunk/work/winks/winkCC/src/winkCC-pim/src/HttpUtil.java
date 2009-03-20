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
	 * ȱʡ������ʵ��
	 */
	protected static HttpConnectionFilter filter = new HttpConnectionFilter() {
		public boolean accept(HttpConnection con) {
			/* �ʷ���ʾwapҳ�泤��=438+Ŀ�ĵ�URL�ַ����ֽڳ��� */
			return con != null
					&& con.getLength() != 438 + con.getURL().getBytes().length;
		}
	};

	/**
	 * ��һ��HttpConnection
	 * 
	 * @param url
	 */
	public static HttpConnection open(final String url) {
		/* һ��HttpConnection�Ĵ���ʵ�� */
		return new HttpConnection() {
			/*
			 * ����
			 */
			private javax.microedition.io.HttpConnection con;

			/*
			 * ���Ӵ�����������������Ϊ����ʧ��
			 */
			private int connectCount = 0;
			/*
			 * �������
			 */
			private ByteArrayOutputStream baos = new ByteArrayOutputStream(1024);

			/*
			 * ��������
			 */
			private Hashtable requestProperties = new Hashtable();

			/*
			 * ����ʽ
			 */
			private String method = GET;

			/*
			 * ���ӷ�����
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
			 * ȷ������
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
	 * ���ù�����,�������Զ��������
	 */
	public static void setHttpConnectionFilter(HttpConnectionFilter filter) {
		HttpUtil.filter = filter;
	}

	/**
	 * �������ӿ�
	 */
	public static interface HttpConnectionFilter {
		boolean accept(HttpConnection con);
	}
}