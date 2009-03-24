package org.enhydra.khttp;

import java.io.InputStream;
import java.io.IOException;
import java.util.Hashtable;

/** Represents an HttpRequest that came from an HTTP Server Proxy **/
public class HttpRequest {
    private InputStream in;
    private String requestID;

    HttpRequest( String requestID, InputStream in ) {
	this.requestID = requestID;
	this.in = in;
    }

    /** Returns the request ID associated with this HTTP request.  This
	is needed to respond to this request. **/
    public String getRequestID() {
	return requestID;
    }

    /** Returns the inputstream for the request. This includes the http
	headers and everything **/
    public InputStream getInputStream() {
	return in;
    }

    public void stripHeaders() throws IOException {
	HttpRequest.readHeaders( in, null );
    }

    /** Reads 'in' until a \r\n line. Stores the header lines in buf if
	buf is not null. Returns a Hashtable of the headers **/
    public static Hashtable readHeaders( InputStream in, 
					 StringBuffer buf )
    throws IOException {
	
	Hashtable map = new Hashtable();
	String line;
	String headerValue = null;
	while( !(line = readline( in )).equals( "\r\n" ) ) {
	    int colonIndex = line.indexOf( ':' );
	    if( colonIndex != -1 ) {
		String key = line.substring( 0, colonIndex);
		String value = line.substring( colonIndex+1,
					       line.length() ).trim();
		map.put( key, value );
		System.out.println( "HttpUtil: readHeaders: put: " +
				    key + " = " + value );
	    }

	    if( buf!=null ) {
		buf.append( line );
	    }
	}

	return map;
    }

    /** A very inefficient line reader :( TODO **/
    private static String readline( InputStream in ) throws IOException {

	StringBuffer buf = new StringBuffer();
	int c;
	while( (c=in.read()) != -1 ) {
	    buf.append( (char) c );
	    if( c == '\n' ) {
		return buf.toString();
	    }
	}
	return null;
    }
}
