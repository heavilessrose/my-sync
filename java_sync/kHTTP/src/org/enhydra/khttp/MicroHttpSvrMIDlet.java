package org.enhydra.khttp;

import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.microedition.lcdui.*;
import java.io.IOException;
import java.util.Hashtable;

/** Basic example of a micro server-by-proxy **/
public class MicroHttpSvrMIDlet extends MIDlet implements CommandListener {

    private Command exitCommand = new Command( "Exit", Command.EXIT, 2 );
    private boolean isRunning = true;

    protected void startApp() throws MIDletStateChangeException {
	String proxyUrl = getAppProperty( "proxyUrl" );
	if( proxyUrl==null )
	    proxyUrl = "http://localhost/httpsvrproxy";
	System.out.println( "starting up. proxyUrl=" + proxyUrl );
	

	Display display = Display.getDisplay( this );
	Form form = new Form( "MicroHTTPSvr" );
	form.addCommand( exitCommand );
	form.setCommandListener( this );
	display.setCurrent( form );

	try {
	    MicroHttpSvr svr = new MicroHttpSvr(proxyUrl);

	    while( true && isRunning ) {
		HttpRequest req = svr.getRequest();

		if( req == null ) {
		    try {
			Thread.currentThread().sleep( 5*1000 );
		    } catch( InterruptedException e ) {}
		    continue;
		}

		form.append( "rqst revd\n" );
		System.out.println( "MicroHttpSvrMIDlet: received request: "+
				    req.getRequestID() );

		HttpResponse res = onRequest( req );
		
		if( res != null ) {
		    System.out.println( "posting response for token: " +
					req.getRequestID() );
		    res.dump();
		    svr.postResponse( req.getRequestID(),
				      res );
		} else {
		    System.out.println( "Null response" );
		}
	    }
	} catch( IOException e ) {
	    System.out.println( "IOException while server running: " + e );
	    e.printStackTrace();
	    throw new MIDletStateChangeException("cannot start server");
	}
    }

    private HttpResponse onRequest( HttpRequest req ) {
	try {
	    Hashtable headers = HttpRequest.readHeaders( req.getInputStream(),
							 null );
	    HttpResponse res = new HttpResponse();
	    res.setResponseCode( "200" );
	    res.setRequestProperty( "Foo", "Bar" );
	    res.setResponseBody( ("<html><h1>This is your phone talking to you<//h1><p>Your User-Agent is <i>" + headers.get("User-Agent") + "</i></html>").getBytes() );
	    return res; 
	} catch( IOException e ) {
	    System.out.println( "IOException composing response: " + 
				e.getMessage() );
	    return null;
	}
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }   

    public void commandAction( Command c, Displayable s ) {
	if( c == exitCommand ) {
	    System.out.println( "Exit command received" );
	    isRunning = false;
	    destroyApp( false );
	    notifyDestroyed();
	}
    }
}
