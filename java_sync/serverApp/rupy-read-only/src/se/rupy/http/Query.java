package se.rupy.http;

import java.io.*;
import java.net.*;
import java.text.*;
import java.util.*;

/**
 * HTTP request and query in one.
 * 
 * @author marc
 */
public class Query extends Hash {
	static URLDecoder decoder = new URLDecoder();
	public final static int GET = 1 << 0, POST = 1 << 1;
	private String path, version, parameters;
	private HashMap headers;
	private Input input;
	private int length, method;
	private long modified;
	private boolean done, parsed;

	Query(Event event) throws IOException {
		headers = new HashMap();
		input = new Input.Chunked(event);
	}

	void headers() throws IOException {
		headers.clear();

		String line = input.line();
		StringTokenizer http = new StringTokenizer(line, " ");
		String method = http.nextToken();

		if (method.equalsIgnoreCase("get")) {
			this.method = GET;
		} else if (method.equalsIgnoreCase("post")) {
			this.method = POST;
			parsed = false;
		} else {
			throw new IOException("Unsupported method.");
		}

		String get = http.nextToken();
		int index = get.indexOf('?');

		if (index > 0) {
			path = get.substring(0, index);
			parameters = get.substring(index + 1);
			parsed = false;
		} else {
			path = get;
			parameters = null;
		}

		version = http.nextToken();
		line = input.line();
		int lines = 0;

		while (line != null && !line.equals("")) {
			int colon = line.indexOf(":");

			if (colon > -1) {
				String name = line.substring(0, colon).toLowerCase();
				String value = line.substring(colon + 1).trim();

				headers.put(name, value);
			}

			line = input.line();
			lines++;

			if (lines > 30) {
				throw new IOException("Too many headers.");
			}
		}

		String encoding = header("transfer-encoding");
		
		if (encoding != null && encoding.equalsIgnoreCase("chunked")) {
			length = -1;
		} else {
			String content = header("content-length");
			
			if(content != null) {
				length = Integer.parseInt(content);
			}
			else {
				length = 0;
			}
		}

		String since = header("if-modified-since");

		if (since != null && since.length() > 0) {
			try {
				modified = input.event().DATE.parse(since).getTime();
			} catch (ParseException e) {
				e.printStackTrace();
				modified = 0;
			}
		}

		String connection = header("connection");
		
		if (connection != null && connection.equalsIgnoreCase("close")) {
			input.event().close(true);
		}

		clear();

		input.event().log(
				method + " " + (length > -1 ? "" + length : "*") + " " + path
						+ (parameters != null ? "?" + parameters : ""),
				Event.VERBOSE);
		input.init();
	}

	/**
	 * Parse the parameters from GET or POST. This method will only parse once
	 * per query and cache the result so don't be afraid of calling this method.
	 * 
	 * @throws Exception
	 */
	public void parse() throws Exception {
		parse(input.event().daemon().size);
	}

	/**
	 * Parse the parameters from GET or POST. This method will only parse once
	 * per query and cache the result so don't be afraid of calling this method.
	 * 
	 * @param size Maximum amount of bytes.
	 * @throws Exception
	 */
	public void parse(int size) throws Exception {
		if (parsed) {
			return;
		} else {
			parsed = true;
		}

		if (method == POST) {
			ByteArrayOutputStream out = new ByteArrayOutputStream();
			if (Deploy.pipe(input, out, size, size) > 0)
				parameters = new String(out.toByteArray());
		}

		input.event().log("query " + parameters, Event.VERBOSE);

		if (parameters != null) {
			StringTokenizer amp = new StringTokenizer(parameters, "&");

			while (amp.hasMoreTokens()) {
				StringTokenizer equ = new StringTokenizer(amp.nextToken(), "=");

				String key = equ.nextToken();
				String value = "";

				if (equ.hasMoreTokens()) {
					value = decoder.decode(equ.nextToken(), "UTF-8");
				}

				put(key, value);
			}
		}
	}

	void done() throws IOException {
		input.end();
		modified = 0;
	}

	public int method() {
		return method;
	}

	public String path() {
		return path;
	}

	public String version() {
		return version;
	}

	public String type() {
		return header("content-type");
	}

	public long modified() {
		return modified;
	}

	public int length() {
		return length;
	}

	/**
	 * The headers are stored and fetched as lower case.
	 * 
	 * @param name
	 * @return the header value.
	 */
	public String header(String name) {
		return (String) headers.get(name.toLowerCase());
	}
	
	void header(String name, String value) {
		headers.put(name, value);
	}

	public HashMap header() {
		return headers;
	}
	
	public Input input() {
		return input;
	}
}
