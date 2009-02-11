package luke.java.practice.httpServer;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.charset.CharsetEncoder;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;
import java.util.StringTokenizer;

// Minio Http Response class
public class MinioResponse extends MinioCommon {
	// Constants
	public static final int RES_BUFFER_SIZE = 512; // Repsonse StringBuffer
	// initial size
	public static final int RES_NONE = 0; // NONE state: Empty Response
	public static final int RES_HEAD = 2; // HEADER state: Writting Headers
	public static final int RES_DATA = 3; // DATA state: Writting Data segments
	public static final int RES_DONE = 4; // DONE state: Repsonse is completely
	// written

	private int state;
	private String httpStatus;
	private Properties headers;
	private StringBuffer headersBuffer;
	private InputStream data;
	private BufferedInputStream bufStream;
	private byte[] readBuffer;

	// Constructor
	public MinioResponse() {
		headers = new Properties();
		headersBuffer = new StringBuffer();
		reset();
	}

	// reset the repsonse object to initial state
	public void reset() {
		state = RES_NONE;
		httpStatus = HTTP_OK;
		data = null;
		bufStream = null;
		headers.clear();
		headersBuffer.delete(0, headersBuffer.length());
		setMimeType(MIME_PLAINTEXT);
	}

	// set header
	public void setHeader(String name, String value) {
		headers.setProperty(name, value);
	}

	// get header
	public String getHeader(String name) {
		return headers.getProperty(name);
	}

	// set the Mime-type into header properties
	public void setMimeType(String mime) {
		setHeader("Content-Type", mime);
	}

	// get the Mime-type from header properties
	public String getMimeType() {
		return getHeader("Content-Type");
	}

	// get Header Buffer
	public StringBuffer getHeadersBuffer() {
		return headersBuffer;
	}

	// return true if the response is ready to send
	public boolean isReady() {
		return (state > RES_NONE ? true : false);
	}

	// set the repsonse is ready to be sent
	public void setReady() {
		state = RES_HEAD;
	}

	// return true if the reponse is finished sending
	public boolean isFinished() {
		return (state == RES_DONE ? true : false);
	}

	// set message response with mime type
	public void message(String status, String mime, String msg) {
		httpStatus = status;
		setMimeType(mime);
		data = new ByteArrayInputStream(msg.getBytes());
		setReady();
	}

	// set message response
	public void message(String status, String msg) {
		message(status, getMimeType(), msg);
	}

	// set exceptional response
	public void exception(String status, String msg) throws Exception {
		message(status, msg);
		log(WARN, msg);
		throw new Exception(msg);
	}

	// set redirect response
	public void redirect(String location) {
		String html = "<html><body>Redirected: <a href=\"" + location + "\">"
				+ location + "</a></body></html>";
		httpStatus = HTTP_REDIRECT;
		setMimeType(MIME_HTML);
		setHeader("Location", location);
		data = new ByteArrayInputStream(html.getBytes());
		setReady();
		log("Response.redirect: " + location);
	}

	// set file stream response
	public void fileStream(String status, String mime, File f, long startFrom)
			throws IOException {
		FileInputStream fis = new FileInputStream(f);
		fis.skip(startFrom);
		httpStatus = status;
		setMimeType(mime);
		data = fis;
		bufStream = new BufferedInputStream(data);
		setHeader("Content-length", "" + (f.length() - startFrom));
		setHeader("Content-range", "" + startFrom + "-" + (f.length() - 1)
				+ "/" + f.length());
		setReady();
		log(f.toString());
	}

	// set data stream response
	public void dataStream(String status, String mime, InputStream stream,
			long startFrom) throws IOException {
		stream.skip(startFrom);
		httpStatus = status;
		setMimeType(mime);
		data = stream;
		bufStream = new BufferedInputStream(data);
		setReady();
		log("data stream done");
	}

	public void save(ByteBuffer buffer) throws Exception {
		int size = buffer.remaining();
		log("state = " + state);
		switch (state) {
		case (RES_NONE):
			return;
		case (RES_HEAD):
			StringBuffer hb = getHeadersBuffer();
			if (hb.length() == 0)
				fillHeadersBuffer();
			if (size > hb.length())
				size = hb.length();
			buffer.put(ByteBuffer.wrap(hb.substring(0, size).getBytes()));
			hb.delete(0, size);
			log("Response.save: " + size
					+ " bytes of headers buffer saved to byte buffer.");
			if (hb.length() == 0)
				state = RES_DATA;
			return;
		case (RES_DATA):
			if (data == null) {
				state = RES_DONE;
				return;
			} else {
				if (bufStream == null) {
					bufStream = new BufferedInputStream(data);
				}
				readBuffer = new byte[size];
				int read = bufStream.read(readBuffer, 0, size);
				if (read <= 0) {
					state = RES_DONE;
					return;
				}
				buffer.put(readBuffer, 0, read);
				if (bufStream.available() <= 0) {
					state = RES_DONE;
					return;
				}
			}
			break;
		}
	}

	private void fillHeadersBuffer() {
		StringBuffer sb = getHeadersBuffer();
		sb.append("HTTP/1.1 ").append(httpStatus).append(NEWLINE);
		for (Enumeration e = headers.keys(); e.hasMoreElements();) {
			String key = (String) e.nextElement();
			String value = headers.getProperty(key);
			sb.append(key).append(": ").append(value).append(NEWLINE);
		}
		sb.append(NEWLINE);
	}

} // end of MinioResponse
