package luke.java.practice.httpServer;

import java.net.URI;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.text.ParseException;
import java.util.Enumeration;
import java.util.Properties;
import java.util.StringTokenizer;

// Minio Http Request
public class MinioRequest extends MinioCommon {
	// Constants
	public static final int REQ_BUFFER_SIZE = 512; // String Buffer Initial Size
	public static final String REQ_DEFAULT_CHARSET = "us-ascii"; // Default
	// Charset
	// for
	// decode
	public static final int REQ_INIT = 1; // Initial state: Empty Request
	public static final int REQ_HEAD = 2; // Header state: Reading Headers
	public static final int REQ_POST = 3; // Post state: Reading POST data
	public static final int REQ_DONE = 4; // Done state: Request is ready

	private StringBuffer requestBuffer;
	private CharsetDecoder decoder;
	private int state;
	private String requestStr;
	private String method;
	private URI uri;
	private String protocol;
	private Properties params;
	private Properties headers;

	// Constructor
	public MinioRequest() {
		requestBuffer = new StringBuffer(REQ_BUFFER_SIZE);
		decoder = Charset.forName(REQ_DEFAULT_CHARSET).newDecoder();
		params = new Properties();
		headers = new Properties();
		reset();
	}

	// Reset the parsing state and clear all content in the Request object
	public void reset() {
		try {
			requestBuffer.delete(0, requestBuffer.length());
			decoder.reset();
			state = REQ_INIT;
			requestStr = "";
			method = "";
			uri = new URI(requestStr);
			protocol = "";
			params.clear();
			headers.clear();
		} catch (Exception e) {
			log(ERRO, e.getMessage());
		}
	}

	// Load byte buffer into request object
	public void load(ByteBuffer buffer) throws CharacterCodingException,
			ParseException {
		int index = 0;
		String line = "";
		requestBuffer.append(decode(buffer));
		do {
			if ((index = requestBuffer.indexOf("\r\n")) >= 0) {
				line = requestBuffer.substring(0, index);
				requestBuffer.delete(0, index + 2);
			} else if ((index = requestBuffer.indexOf("\n")) >= 0) {
				line = requestBuffer.substring(0, index);
				requestBuffer.delete(0, index + 1);
			}
			if (index >= 0)
				parseLine(line);
		} while (index >= 0);
	}

	// Decode the byte buffer to string
	protected String decode(ByteBuffer buffer) throws CharacterCodingException {
		CharBuffer charBuffer = decoder.decode(buffer);
		return charBuffer.toString();
	}

	// Parse a arbitrary line
	public void parseLine(String line) throws ParseException {
		log(line);
		switch (state) {
		case (REQ_INIT):
			parseRequestLine(line);
			if (method.equals("POST"))
				state = REQ_POST;
			if (isHeaderAcceptable())
				state = REQ_HEAD;
			else
				state = REQ_DONE;
			break;
		case (REQ_HEAD):
			if (!line.trim().isEmpty())
				parseHeaderLine(line);
			else {
				if (method.equals("POST"))
					state = REQ_POST;
				else
					state = REQ_DONE;
			}
			break;
		case (REQ_POST):
			parsePostLine(line);
			state = REQ_DONE;
			break;
		}
	}

	// Parse the single request line
	protected void parseRequestLine(String line) throws ParseException {
		log(line);
		requestStr = line;
		StringTokenizer st = new StringTokenizer(line);
		if (!st.hasMoreTokens())
			throw new ParseException(
					"BAD REQUEST: Syntax error. Usage: GET /example/file.html",
					0);
		method = st.nextToken().toUpperCase();
		if (!st.hasMoreTokens())
			throw new ParseException(
					"BAD REQUEST: Missing URI. Usage: GET /example/file.html",
					0);
		uri = URI.create("http://example" + st.nextToken());
		if (uri.getQuery() != null)
			params.putAll(parseUriQuery(uri.getQuery()));
		if (st.hasMoreTokens())
			protocol = st.nextToken();
	}

	// Check protocol to accept header or not
	protected boolean isHeaderAcceptable() {
		return (protocol.isEmpty() ? false : true);
	}

	// Parse and append each header line to Headers properties
	protected void parseHeaderLine(String line) throws ParseException {
		log(line);
		int p = line.indexOf(':');
		headers.put(line.substring(0, p).trim().toLowerCase(), line.substring(
				p + 1).trim());
	}

	// Parse the POST parameter line
	protected void parsePostLine(String line) throws ParseException {
		log(line);
		params.putAll(parseUriQuery(URI.create(
				"http://example/index.htm?" + line).getQuery()));
	}

	// return state
	public int getState() {
		return state;
	}

	// Indicate the request message is completely loaded
	public boolean isComplete() {
		return (state == REQ_DONE ? true : false);
	}

	// return method
	public String getMethod() {
		return method;
	}

	// return URI
	public URI getURI() {
		return uri;
	}

	// return Protocol
	public String getProtocol() {
		return protocol;
	}

	// return parameter by key
	public String getParam(String key) {
		return params.getProperty(key);
	}

	// return parameters
	public Properties getParams() {
		return params;
	}

	// return parameter keys
	public Enumeration getParamNames() {
		return params.propertyNames();
	}

	// return header by key
	public String getHeader(String key) {
		return headers.getProperty(key);
	}

	// return headers
	public Properties getHeaders() {
		return headers;
	}

	// return header keys
	public Enumeration getHeaderNames() {
		return headers.propertyNames();
	}

	// Parse the parameters in URI
	private Properties parseUriQuery(String query) {
		Properties prop = new Properties();

		String param[] = query.split("&");
		String pair[];
		String k, v;
		for (int i = 0; i < param.length; i++) {
			if (param[i].contains("=")) {
				pair = param[i].split("=");
				k = pair[0];
				if (pair.length > 1)
					v = pair[1];
				else
					v = "";
				prop.setProperty(k, v);
			}
		}
		return prop;
	}

} // end of MinioRequest
