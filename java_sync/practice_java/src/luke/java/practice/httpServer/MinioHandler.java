package luke.java.practice.httpServer;

import java.io.File;
import java.io.IOException;
import java.net.URLEncoder;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Enumeration;
import java.util.StringTokenizer;
import java.util.concurrent.ThreadPoolExecutor;

// Minio Http Handler per connection
public class MinioHandler extends MinioCommon implements Runnable {

	private static final int BUFFER_SIZE = 96; // size in bytes

	private ThreadPoolExecutor pool;
	private SelectionKey key;
	private SocketChannel channel;
	private ByteBuffer inputBuffer;
	private ByteBuffer outputBuffer;
	private MinioRequest request;
	private MinioResponse response;

	// Constructor
	public MinioHandler(ThreadPoolExecutor pool_, SocketChannel channel_)
			throws IOException {
		pool = pool_;
		channel = channel_;
		channel.configureBlocking(false);
		// channel.socket().setTcpNoDelay(true);
		// use allocateDirect() may increase the overhead on creation and
		// disposing
		inputBuffer = ByteBuffer.allocateDirect(BUFFER_SIZE);
		outputBuffer = ByteBuffer.allocateDirect(BUFFER_SIZE);
		request = newRequest();
		response = newResponse();
	}

	public void assignKey(SelectionKey key_) {
		key = key_;
	}

	// return a new Request object
	protected MinioRequest newRequest() {
		return new MinioRequest();
	}

	// return a new Response object
	protected MinioResponse newResponse() {
		return new MinioResponse();
	}

	// return the socket channel
	public SocketChannel getChannel() {
		return channel;
	}

	// return the request object
	public MinioRequest getRequest() {
		return request;
	}

	// return the response object
	public MinioResponse getResponse() {
		return response;
	}

	// check if the request is complete
	public boolean hasCompleteRequest() {
		log(NOTE, request.isComplete() + "");
		return request.isComplete();
	}

	// check if the response is complete
	public boolean hasReadyResponse() {
		log(NOTE, response.isReady() + "");
		return response.isReady();
	}

	// check if the response has sent completely
	public boolean hasFinishedResponse() {
		log(NOTE, response.isFinished() + "");
		return response.isFinished();
	}

	// read the byte buffer from channel into the request object
	public void readRequest() throws Exception {
		int nbyte;
		try {
			inputBuffer.clear();
			nbyte = channel.read(inputBuffer);
			log(SPEC, "num of byte read = " + nbyte);
			if (nbyte > 0) { // incoming data
				inputBuffer.flip();
				request.load(inputBuffer);
			} else if (nbyte == -1) { // connection closed
				log(SPEC, "Close Connection");
				key.cancel();
				channel.close();
			}
		} catch (Exception e) {
			log(SPEC, "Close Connection");
			key.cancel();
			channel.close();
			getResponse().message(HTTP_BADREQUEST,
					"Error in handling the request.");
			log(SPEC, e.getMessage());
		}
	}

	// write the byte buffer from response object to channel
	public void writeResponse() {
		try {
			outputBuffer.clear();
			getResponse().save(outputBuffer);
			outputBuffer.flip();
			int n = channel.write(outputBuffer);
			log(SPEC, "num of bytes write = " + n);
		} catch (Exception e) {
			try {
				log(SPEC, "Close Connection");
				key.cancel();
				channel.close();

				log(SPEC, e.getMessage());
				e.printStackTrace();
			} catch (Exception ex) {
			}
		}
	}

	protected class MinioProcessor implements Runnable {
		public void run() {
			try {
				readRequest();
				if (hasCompleteRequest()) {
					serve();
				}
				if (hasReadyResponse()) {
					writeResponse();
					if (hasFinishedResponse()) {
						getResponse().reset();
						key.cancel();
						channel.close();
					} else {
						key.interestOps(SelectionKey.OP_WRITE);
					}
				}
			} catch (Exception e) {
				log(SPEC, e.getMessage());
				try {
					key.cancel();
					channel.close();
				} catch (Exception ex) {
				}
			}
		}
	}

	protected class MinioWriter implements Runnable {
		public void run() {
			try {
				if (hasReadyResponse()) {
					writeResponse();
					if (hasFinishedResponse()) {
						getResponse().reset();
						key.cancel();
						channel.close();
					}
				}
			} catch (Exception e) {
				log(ERRO, e.getMessage());
				try {
					key.cancel();
					channel.close();
				} catch (Exception ex) {
					log(ERRO, ex.getMessage());
				}
			}
		}
	}

	public void run() {
		try {
			log(100, key + " -> read=" + key.isReadable() + ", write="
					+ key.isWritable() + ", accept=" + key.isAcceptable());
			if (!hasCompleteRequest() && key.isReadable()) {
				new MinioProcessor().run();

			}
			if (hasReadyResponse() && key.isWritable()) {
				new MinioWriter().run();
			}

		} catch (Exception e) {
			log(ERRO, e.getMessage());
		}
	}

	public synchronized void serve() {
		log("Serve: " + request.getMethod() + " '" + request.getURI() + "'");
		Enumeration e = request.getHeaderNames();
		while (e.hasMoreElements()) {
			String value = (String) e.nextElement();
			log("  HDR: '" + value + "' = '" + request.getHeader(value) + "'");
		}
		e = request.getParamNames();
		while (e.hasMoreElements()) {
			String value = (String) e.nextElement();
			log("  PRM: '" + value + "' = '" + request.getParam(value) + "'");
		}
		try {
			serveFile(new File("."), true);
		} catch (Exception ex) {
			log(ex.getMessage());
		}
	}

	protected void serveFile(File homeDir, boolean allowDirectoryListing)
			throws Exception {
		String uri;

		// Make sure we won't die of an exception later
		log("serveFile: " + getRequest().getURI());
		if (!homeDir.isDirectory())
			getResponse()
					.exception(HTTP_INTERNALERROR,
							"INTERNAL ERRROR: serveFile(): given homeDir is not a directory.");

		// normalize uri
		uri = getRequest().getURI().getPath().trim().replace(
				File.separatorChar, '/');

		// Prohibit getting out of current directory
		if (uri.startsWith("..") || uri.endsWith("..")
				|| uri.indexOf("../") >= 0)
			getResponse().exception(HTTP_FORBIDDEN,
					"FORBIDDEN: Won't serve ../ for security reasons.");

		File f = new File(homeDir, uri);
		if (!f.exists())
			getResponse()
					.exception(HTTP_NOTFOUND, "Error 404, file not found.");

		// List the directory, if necessary
		if (f.isDirectory()) {
			// Browsers get confused without '/' after the
			// directory, send a redirect.
			if (!uri.endsWith("/")) {
				uri += "/";
				getResponse().redirect(uri);
				return;
			}

			// First try index.html and index.htm
			if (new File(f, "index.html").exists())
				f = new File(homeDir, uri + "/index.html");
			else if (new File(f, "index.htm").exists())
				f = new File(homeDir, uri + "/index.htm");

			// No index file, list the directory
			else if (allowDirectoryListing) {
				String[] files = f.list();
				String msg = "<html><body><h1>Directory " + uri + "</h1><br/>";

				if (uri.length() > 1) {
					String u = uri.substring(0, uri.length() - 1);
					int slash = u.lastIndexOf('/');
					if (slash >= 0 && slash < u.length())
						msg += "<b><a href=\"" + uri.substring(0, slash + 1)
								+ "\">..</a></b><br/>";
				}

				for (int i = 0; i < files.length; ++i) {
					File curFile = new File(f, files[i]);
					boolean dir = curFile.isDirectory();
					if (dir) {
						msg += "<b>";
						files[i] += "/";
					}

					msg += "<a href=\"" + encodeUri(uri + files[i]) + "\">"
							+ files[i] + "</a>";

					// Show file size
					if (curFile.isFile()) {
						long len = curFile.length();
						msg += " &nbsp;<font size=2>(";
						if (len < 1024)
							msg += curFile.length() + " bytes";
						else if (len < 1024 * 1024)
							msg += curFile.length() / 1024 + "."
									+ (curFile.length() % 1024 / 10 % 100)
									+ " KB";
						else
							msg += curFile.length() / (1024 * 1024) + "."
									+ curFile.length() % (1024 * 1024) / 10
									% 100 + " MB";

						msg += ")</font>";
					}
					msg += "<br/>";
					if (dir)
						msg += "</b>";
				}
				getResponse().exception(HTTP_OK, msg);
			} else {
				getResponse().exception(HTTP_FORBIDDEN,
						"FORBIDDEN: No directory listing.");
			}
		}

		try {
			// Get MIME type from file name extension, if possible
			String mime = null;
			int dot = f.getCanonicalPath().lastIndexOf('.');
			if (dot >= 0)
				mime = (String) theMimeTypes.get(f.getCanonicalPath()
						.substring(dot + 1).toLowerCase());
			if (mime == null)
				mime = MIME_DEFAULT_BINARY;

			// Support (simple) skipping:
			long startFrom = 0;
			String range = getRequest().getHeader("Range");
			if (range != null) {
				if (range.startsWith("bytes=")) {
					range = range.substring("bytes=".length());
					int minus = range.indexOf('-');
					if (minus > 0)
						range = range.substring(0, minus);
					try {
						startFrom = Long.parseLong(range);
					} catch (NumberFormatException nfe) {
					}
				}
			}

			getResponse().fileStream(HTTP_OK, mime, f, startFrom);
			return;
		} catch (IOException ioe) {
			getResponse().exception(HTTP_FORBIDDEN,
					"FORBIDDEN: Reading file failed.");
		}
	}

	private String encodeUri(String uri) {
		String newUri = "";
		StringTokenizer st = new StringTokenizer(uri, "/ ", true);
		while (st.hasMoreTokens()) {
			String tok = st.nextToken();
			if (tok.equals("/"))
				newUri += "/";
			else if (tok.equals(" "))
				newUri += "%20";
			else {
				newUri += URLEncoder.encode(tok);
				// For Java 1.4 you'll want to use this instead:
				// try { newUri += URLEncoder.encode( tok, "UTF-8" ); } catch (
				// UnsupportedEncodingException uee )
			}
		}
		return newUri;
	}

}
