package se.rupy.http;

import java.io.*;
import java.nio.*;
import java.util.*;

/**
 * HTTP response. Inherently non-blocking asynchronous; even if you don't write
 * any output, the worker will still be released immediately. The order of
 * execution is {@link #header(String, String)}, {@link #code(String)} and
 * finally {@link #output()}. If you call the {@link #code(String)} or
 * {@link #output()} method, the reply will flush output, so then you won't be
 * able to do an asynchronous reply. To wakeup a dormant asynchronous event use
 * {@link #wakeup()}.
 * 
 * @author marc
 */
public class Reply {
	private String type = "text/html; charset=UTF-8";
	private HashMap headers;
	private Output output;
	private Event event;
	private long modified;
	private String code;

	Reply(Event event) throws IOException {
		this.event = event;
		output = new Output.Chunked(this);
		reset();
	}

	void done() throws IOException {
		event.log("done", Event.DEBUG);

		output.end();

		if (headers != null) {
			headers.clear();
		}

		reset();
	}

	void reset() {
		modified = 0;
		type = "text/html; charset=UTF-8";
		code = "200 OK";
	}

	Event event() {
		return event;
	}

	HashMap headers() {
		return headers;
	}

	public String code() {
		return code;
	}

	/**
	 * Important: call {@link #header(String, String)} before you call this. If
	 * you manually set a code, the reply will flush even if empty. So do not
	 * call this if you wan't to reply asynchronously. For example if you want
	 * to redirect a browser.
	 * 
	 * <pre>
	 * public void filter(Event event) throw Event {
	 *     event.reply().header(&quot;Location&quot;, &quot;/login&quot;);
	 *     event.reply().code(&quot;302 Found&quot;);
	 *     throw event; // stop the chain
	 * }
	 * </pre>
	 * 
	 * @param code
	 */
	public void code(String code) throws IOException {
		event.log("code", Event.DEBUG);
		this.code = code;
		output.init(0);
	}

	public String type() {
		return type;
	}

	public void type(String type) {
		this.type = type;
	}

	public void header(String name, String value) {
		if (headers == null) {
			headers = new HashMap();
		}

		headers.put(name, value);
	}

	long modified() {
		return modified;
	}

	void modified(long modified) {
		this.modified = modified;
	}

	/**
	 * Calls {@link #output(0)}.
	 * @return the output stream.
	 * @throws IOException
	 */
	public Output output() throws IOException {
		return output(0);
	}
	
	/**
	 * Important: call {@link #header(String, String)} and {@link #code(String)}
	 * first, in that order, this method is the point of no return for delivery
	 * of a request. It enables OP_WRITE and writes the headers immediately if
	 * your reply is chunked, ie. if your client is HTTP/1.1. If your client is
	 * HTTP/1.0, the headers will be prepended automatically after the
	 * {@link Service#filter(Event)} method returns.
	 * 
	 * @param length if you want to write fixed length data
	 * @return the output stream.
	 * @throws IOException
	 */
	public Output output(long length) throws IOException {
		event.log("output " + length, Event.DEBUG);
		output.init(length);
		return output;
	}

	/**
	 * To send data asynchronously, call this and the event will be re-filtered.
	 * Just make sure you didn't already flush the reply and that are ready to
	 * catch the event when it recycles!
	 * 
	 * @throws IOException
	 */
	public void wakeup() throws IOException {
		if (output.complete()) {
			throw new IOException("Reply already complete.");
		}

		if (event.worker() != null) {
			throw new IOException("Reply still processing.");
		}
		
		event.push(true);
		event.daemon().employ(event);
	}
}
