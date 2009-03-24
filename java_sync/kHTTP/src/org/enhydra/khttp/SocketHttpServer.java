package org.enhydra.khttp;

import java.io.IOException;
import javax.microedition.io.*;

/** A J2ME HTTP server class for devices providing server sockets. */


public class SocketHttpServer implements HttpServer {

    StreamConnectionNotifier notifier;

    class SocketSvrHttpCon extends ServerHttpConnection {

	StreamConnection con;

	SocketSvrHttpCon (StreamConnection con) throws IOException {
	    super (con.openInputStream ());
	    this.con = con;
	}


	public void close () throws IOException {
	    write (con.openOutputStream ());
	    con.close (); 
	}
    }


    public SocketHttpServer (int port) throws IOException {
	notifier = (StreamConnectionNotifier) 
	    Connector.open ("serversocket://:"+port);
    }


    public ServerHttpConnection accept () throws IOException {
	StreamConnection conn = notifier.acceptAndOpen ();
	return conn == null ? null : new SocketSvrHttpCon (conn);
    }

   
    public void close () throws IOException {
	notifier.close ();
    }

}


