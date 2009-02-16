package bm.net.bind;

import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.net.Net;
import bm.net.NetException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

/*
 * File Information
 *
 * Created on       : 25-nov-2006 18:40:03
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * Direct HttpConnection binding.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class HttpBinding extends Binding {
	private static final Log log = LogFactory.getLog("HttpBinding");

	private HttpConnection connection;
	private DataInputStream in;
	private DataOutputStream out;

	/**
	 * Open underlying connection.<br/>
	 * If the encryption level is set to HIGH and the url is a plain HTTP one,
	 * then it's translated to HTTPS.<br/>
	 * HTTPS urls are always unchanged.
	 * 
	 * @param url
	 *            url to post to
	 * @throws NetException
	 *             on errors
	 */
	public void open(final String url) throws NetException {
		try {
			log.debug("open: " + url);
			String target = url;
			if (Net.getEncryptionLevel() == Net.NET_ENCRYPTION_LEVEL_HIGH
					&& url.startsWith("http:")) {
				target = "https" + url.substring(4);
				log.debug("target: " + target);
			}
			// noinspection ConnectionOpenedButNotSafelyClosed
			connection = (HttpConnection) Connector.open(target,
					Connector.READ_WRITE, true);
			connection.setRequestMethod(HttpConnection.POST);

			// This is a fix to some buggy http connectors that don't send the
			// Host
			// header, making virtual name server hosting to fail
			connection.setRequestProperty("Host", connection.getHost());
		} catch (IOException e) {
			throw new NetException(Net.ERR_HTTP_OPEN, e);
		}
	}

	/**
	 * Simply make a call to an http url and get the response as a string.
	 * 
	 * @param url
	 *            url
	 * @return page text
	 * @throws bm.net.NetException
	 *             on errors
	 */
	public String get(final String url) throws NetException {
		try {
			final HttpConnection conn = (HttpConnection) Connector.open(url,
					Connector.READ, true);
			conn.setRequestMethod(HttpConnection.GET); // ShouldDo: make a HEAD
														// version

			// This is a fix to some buggy http connectors that don't send the
			// Host
			// header, making virtual name server hosting to fail
			conn.setRequestProperty("Host", conn.getHost());

			if (conn.getResponseCode() != HttpConnection.HTTP_OK) {
				return null;
			} else {
				final StringBuffer content = new StringBuffer();
				InputStream is = null;
				try {
					is = conn.openInputStream();
					int c = is.read();
					while (c != -1) {
						content.append((char) c);
						c = is.read();
					}
					return content.toString();
				} finally {
					if (is != null)
						try {
							is.close();
						} catch (Exception e) {
						}
				}
			}
		} catch (IOException e) {
			log.error(e);
			throw new NetException(Net.ERR_HTTP_GET, e);
		}
	}

	/**
	 * Close underlying connection.
	 */
	public void close() {
		super.close();
		log.debug("Closing");
		if (out != null)
			try {
				out.close();
			} catch (Exception e) {
			} finally {
				out = null;
			}
		if (in != null)
			try {
				in.close();
			} catch (Exception e) {
			} finally {
				in = null;
			}
		if (connection != null)
			try {
				connection.close();
			} catch (Exception e) {
			} finally {
				connection = null;
			}
	}

	/**
	 * Begin the post operation, open the output stream to send content to the
	 * server.
	 * 
	 * @return OutputStream to send content.
	 * @throws NetException
	 *             on errors
	 */
	public DataOutputStream beginPost() throws NetException {
		try {
			log.debug("beginPost");
			out = connection.openDataOutputStream();
			return out;
		} catch (IOException e) {
			throw new NetException(Net.ERR_HTTP_BEGIN_POST, e);
		}
	}

	/**
	 * End the post operation, and get server response.
	 * 
	 * @return server response.
	 * @throws NetException
	 *             on errors
	 */
	public DataInputStream endPost() throws NetException {
		try {
			log.debug("endPost");
			out.flush();

			responseCode = connection.getResponseCode();
			responseMessage = connection.getResponseMessage();

			// Any 500 status number (500, 501) means there was a server error
			if (responseCode >= 500) {
				log.error("Server returned error code: " + responseCode + " "
						+ responseMessage);
				throw new NetException(Net.ERR_HTTP_SERVER);
			} else if (responseCode != HttpConnection.HTTP_OK) {
				log.debug("Server returned unhandled code: " + responseCode
						+ " " + responseMessage);
				// ShouldDo: handle redirects?
				throw new NetException(Net.ERR_HTTP_UNHANDLED);
			} else {
				contentType = connection.getType();
				contentLength = (int) connection.getLength();
				log.debug("content-type: " + contentType + ", content-length: "
						+ contentLength);
			}
			in = new NetInputStream(this, connection.openDataInputStream());
			return in;
		} catch (IOException e) {
			throw new NetException(Net.ERR_HTTP_IO, e);
		}
	}

	private class NetInputStream extends DataInputStream {
		private HttpBinding parent;

		public NetInputStream(final HttpBinding parent, final DataInputStream in) {
			super(in);
			this.parent = parent;
		}

		public void close() throws IOException {
			try {
				super.close();
			} catch (IOException e) {
			}
			parent.in = null; // Avoid deadlock
			parent.close();
		}
	}
}
