package luke.java.practice.httpServer;

import java.util.Hashtable;
import java.util.StringTokenizer;

// Minio Common Class

public class MinioCommon {
	// Global Constants - Log level
	public static final int INFO = 10; // Information
	public static final int NOTE = 15; // Notes
	public static final int WARN = 20; // Warning
	public static final int ERRO = 30; // Error
	public static final int SPEC = 90;// Special

	// Global Constants - Common Values
	public static final int THREAD_POOL_SIZE = 50;
	public static final String NEWLINE = "\r\n";

	// HTTP status
	public static final String HTTP_OK = "200 OK",
			HTTP_REDIRECT = "301 Moved Permanently",
			HTTP_FORBIDDEN = "403 Forbidden", HTTP_NOTFOUND = "404 Not Found",
			HTTP_BADREQUEST = "400 Bad Request",
			HTTP_INTERNALERROR = "500 Internal Server Error",
			HTTP_NOTIMPLEMENTED = "501 Not Implemented";

	// Global Constants - MIME types
	public static final String MIME_PLAINTEXT = "text/plain",
			MIME_HTML = "text/html",
			MIME_DEFAULT_BINARY = "application/octet-stream";

	public static Hashtable theMimeTypes = new Hashtable();
	static {
		StringTokenizer st = new StringTokenizer("htm		text/html "
				+ "html		text/html " + "txt		text/plain " + "asc		text/plain "
				+ "gif		image/gif " + "jpg		image/jpeg " + "jpeg		image/jpeg "
				+ "png		image/png " + "mp3		audio/mpeg "
				+ "m3u		audio/mpeg-url " + "pdf		application/pdf "
				+ "doc		application/msword " + "ogg		application/x-ogg "
				+ "zip		application/octet-stream "
				+ "exe		application/octet-stream "
				+ "class		application/octet-stream ");
		while (st.hasMoreTokens())
			theMimeTypes.put(st.nextToken(), st.nextToken());
	}

	protected static int debug = INFO;

	// set log level
	public static void setLogLevel(int level) {
		debug = level;
	}

	// Logging routine, can override for other logger
	public static void log(int level, String msg) {
		if (level < debug)
			return;
		/*
		 * Exception e = new Exception(); String logMethod =
		 * e.getStackTrace()[0].getMethodName(); String methodName, className;
		 * int i; for (i=1;
		 * (methodName=e.getStackTrace()[i].getMethodName()).equals(logMethod);
		 * i++); className = e.getStackTrace()[i].getClassName();
		 * System.out.println("[" + level + "] " +
		 * Thread.currentThread().getName() + "*" + className + "." + methodName
		 * + ":  " + msg);
		 */
		System.out.println("[" + level + "] "
				+ Thread.currentThread().getName() + ":  " + msg);
	}

	// Default log level
	public static void log(String msg) {
		log(INFO, msg);
	}

}
