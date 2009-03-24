package org.enhydra.khttp;

import java.io.InputStream;
import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.util.Hashtable;
import java.util.Enumeration;

/** Represents an HTTP response **/
public class HttpResponse {
    /** The http response code **/
    protected String responseCode;

    /** The body of the http response **/
    protected byte[] body;

    /** The headers in the response **/
    protected Hashtable headers;

    /** this is computed when either getInputStream() or getInputStreamLength()
	is called **/
    protected byte[] response;

    public HttpResponse() {
    }

    /** Sets the HTTP response code **/
    public void setResponseCode( String responseCode ) throws IOException {
	if( !responseCode.equals( "200" ) && !responseCode.equals( "404" ) ) {
	    throw new IOException( "Unsupported response code: " + 
				   responseCode );
	}

	this.responseCode = responseCode;
    }

    /** Sets the HTTP response body **/
    public void setResponseBody( byte[] body ) {
	this.body = body;
    }

    /** Adds an HTTP header **/
    public void setRequestProperty( String property, String value ) 
	throws IOException {
	
	if( property==null )
	    throw new IOException( "Null property not allowed" );

	if( value==null )
	    throw new IOException( "Null value not allowed" );

	if( headers==null )
	    headers = new Hashtable();
	
	headers.put( property, value );
    }

    /** prints response to console **/
    public void dump() {
	computeResponse();
	if( response != null )
	    System.out.println( new String(response) );
	else
	    System.out.println( "[null response]" );
    }
	    
    /** Rturns an input stream representing the entire HTTP response. This
	can be called multiple times for the same instance **/
    public InputStream getInputStream() {
	return new ByteArrayInputStream( computeResponse() );
    }

    /** Returns the full length of the HTTP response, include the headers **/
    public int getInputStreamLength() {
	return computeResponse().length;
    }

    /** computes the byte[] for the entire response **/
    protected byte[] computeResponse() {
	if( response==null ) {
	    StringBuffer sb = new StringBuffer();

	    String responseMsg = "";
	    if (responseCode.equals("200"))
		responseMsg = "OK";
	    else if (responseCode.equals ("404"))
		responseMsg = "Not Found";

	    sb.append ("HTTP/1.1 " + responseCode + " " + responseMsg +
		       "\r\n");
	    
	    if( headers!=null ) {
		Enumeration keys = headers.keys();
		while( keys.hasMoreElements() ) {
		    String key = (String) keys.nextElement();
		    sb.append( key + ": " + headers.get( key ) + "\r\n" );
		}
	    }

	    sb.append( "Content-length: " + body.length + "\r\n\r\n" );

	    byte[] headers = sb.toString().getBytes();

	    int totallength = headers.length;
	    if( body!=null )
		totallength += body.length;

	    response = new byte[ totallength ];

	    System.arraycopy( headers, 0, response, 0, headers.length );
	    if( body!=null )
		System.arraycopy( body, 0,
				  response, headers.length,
				  body.length );
	}

	return response;
    }
}
