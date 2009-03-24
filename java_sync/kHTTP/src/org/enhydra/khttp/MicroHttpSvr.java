package org.enhydra.khttp;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

import javax.microedition.io.HttpConnection;
import javax.microedition.io.Connector;
import javax.microedition.io.ConnectionNotFoundException;

/** A simple request-response "server-by-proxy" that uses a compliant
    "HTTP Server Proxy" (see http://www.walkingnetworks.com for a compliant
    HTTP Server Proxy.
**/
public class MicroHttpSvr {

    /** URL of a compliant 'HTTP Server Proxy' **/
    String proxyUrl;

    private static final String REQUEST_ID_HEADER =
	"HttpSvrProxy-Request-ID";

    /** Constructs the microserver given a URL for an HTTP Server Proxy **/
    public MicroHttpSvr( String proxyUrl ) {
	this.proxyUrl = proxyUrl;
    }


    /** Fetches a request from the http server proxy **/
    public HttpRequest getRequest() {
	System.out.println( "MicroHttpSvr: getRequest()" );
	boolean receivedValidRequest = false;
	InputStream in = null;
	String requestID = null;

	System.out.println( "getRequest(): Attempting to fetch request" );
	
	String requestUrl  = proxyUrl + "/getrequest";
	
	try {
	    HttpConnection con =
		(HttpConnection)Connector.open(requestUrl);
	    
	    con.setRequestMethod( "POST" );
	    in = con.openInputStream();
	    
	    requestID =
		con.getHeaderField( REQUEST_ID_HEADER );
	    
	    System.out.println( "getRequest(): fetched request with id: " +
				requestID );
	    
	    if( requestID != null )
		receivedValidRequest = true;
	} catch( ConnectionNotFoundException e ) {
	    throw new RuntimeException( "Device does not support http" );
	} catch( IOException e ) {
	    System.out.println( "Error getting request. Trying again. " +
				e.getMessage() );
	}
	
	// XXX play with close() methods... note sure if it will screw things
	// up or not.
	if( receivedValidRequest ) {
	    System.out.println( "MicroHttpSvr: returning HttpRequest" );
	    return new HttpRequest( requestID, in );
	} else {
	    System.out.println( "MicroHttpSvr: returning null" );
	    return null;
	}
    }

    /** Posts a response to the specified requestID. Typically you will 
	obtain the request ID from the HttpRequest object that you 
	are responding to. **/
    public void postResponse( String requestID,
			      HttpResponse response )
	throws IOException {

	System.out.println( "postResponse(): Posting response to request " +
			    requestID );
	
	String responseUrl = proxyUrl + "/postresponse";
	HttpConnection con = (HttpConnection)Connector.open(responseUrl);
	con.setRequestMethod("POST");
	con.setRequestProperty(REQUEST_ID_HEADER, requestID);
	
	// if the server application is posting a null response,
	// at least send back a 404 and close the connection...
	if( response == null ) {
	    response = new HttpResponse();
	    response.setResponseCode( "404" );
	}
	
	OutputStream out = con.openOutputStream();
	transfer( response.getInputStream(),
		  out,
		  response.getInputStreamLength());
	out.flush();
	out.close();
	con.close();
	System.out.println( "Finished posting response" );
    }

    private static final int BUF_SIZE=128;
    void transfer( InputStream in, OutputStream out, int len )
	throws IOException {

	int remain = len;
	byte[] buf;
	int c;
	while( remain > 0 ) {
	    buf = new byte[BUF_SIZE];
	    int readamt = (remain>BUF_SIZE)?BUF_SIZE:remain;
	    c=in.read( buf, 0, readamt );
	    if( c==-1 ) {
		break;
	    }
	    out.write( buf, 0, c );
	    remain-=c;
	}
    }
}
