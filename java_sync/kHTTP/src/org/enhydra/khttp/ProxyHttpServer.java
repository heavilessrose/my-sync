package org.enhydra.khttp;

import java.io.*;
import java.util.*;
import javax.microedition.io.*;

/** A J2ME proxy based HTTP server class for devices <b>not</b>
    providing server sockets. Replacement for MicroHttpSvr implementing
    the HttpServer interface. */

public class ProxyHttpServer implements HttpServer {

    /** URL of a compliant 'HTTP Server Proxy', such as Locumi **/
    private String proxyUrl;

    /** HTTP header used for communicating request IDs back and forth **/
    private static final String REQUEST_ID_HEADER =
	"HttpSvrProxy-Request-ID";

    public ProxyHttpServer( String proxyUrl ) {
	this.proxyUrl = proxyUrl;
    }

    class ProxySvrHttpCon extends ServerHttpConnection {

	private String requestID = null;

	ProxySvrHttpCon (String requestID, InputStream in)
	    throws IOException {

	    super( in );
	    this.requestID = requestID;
	}


	public void close () throws IOException {
	    // HTTP POST back to the proxy, sending the request ID
	    String responseUrl = proxyUrl + "/postresponse";
	    HttpConnection con = (HttpConnection)Connector.open(responseUrl);
	    con.setRequestMethod("POST");
	    con.setRequestProperty(REQUEST_ID_HEADER, requestID);

	    // write the HTTP response back
	    OutputStream out = con.openOutputStream();
	    write( out );
	    out.close();
	    con.close();
	}
    }


    public ServerHttpConnection accept () throws IOException {
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

	if( receivedValidRequest )
	    return new ProxySvrHttpCon( requestID, in );
	else
	    return null;
    }

    public void close () throws IOException {
	// No code intended.  There is nothing to close in the proxy case
	// because we do not remain connected to the proxy.
    }
}
