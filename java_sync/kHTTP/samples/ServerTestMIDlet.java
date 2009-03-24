/*
 * The Walking Networks License, Version 1.0
 *
 * Copyright (c) 2001 Walking Networks, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by
 *        Walking Networks (http://www.walkingnetworks.com/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The name "Walking Networks" must not be used to endorse or
 *    promote products derived from this software without prior written
 *    permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by 
 * individuals on behalf of Walking Networks.  For more
 * information on the Walking Networks, please see 
 * <http://www.walkingnetworks.com/>.
 */


import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import java.io.*;
import org.enhydra.khttp.*;


/** Basic example of a micro server **/

public class ServerTestMIDlet extends MIDlet implements CommandListener {
    
    private Command exitCommand = new Command( "Exit", Command.EXIT, 2 );
    private boolean isRunning = true;
    private Server server;
    Form form = new Form ("ServerTestMIDlet");
    
    class Server extends Thread {

	public void run () {
	    try {
		SocketHttpServer svr = new SocketHttpServer (8008);
		
		while (isRunning) {
		    ServerHttpConnection con = svr.accept ();
		    
		    if (con == null) {
			try {
			    Thread.currentThread().sleep( 5*1000 );
			} 
			catch( InterruptedException e ) {}
			continue;
		    }
		    
		    form.append( "rqst revd: "+con.getURL ());
		    
		    PrintStream out = new PrintStream (con.openOutputStream ());
		    out.println ("<html>");
		    out.println (" <head><title>MIDlet Server</title></head>");
		    out.println (" <body>");
		    out.println ("  <h1>MIDlet Server</h1>");
		    out.println ("  <p>Test successfull</p>");
		    out.println (" </body>");
		    out.println ("</html>");
		    
		    con.close ();
		}
		svr.close ();
	    } 
	    catch( IOException e ) {
		System.out.println( "IOException while server running: " + e );
		e.printStackTrace();
		throw new RuntimeException ("cannot start server");
	    }
	}
    }
    
    
    protected void startApp() throws MIDletStateChangeException {
	

	if (server == null) {
	    server = new Server ();
	    server.start ();
	    
	    form.addCommand( exitCommand );
	    form.setCommandListener( this );
	}

	Display display = Display.getDisplay( this );
	display.setCurrent( form );
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
	isRunning = false;
    }   

    public void commandAction( Command c, Displayable s ) {
	if( c == exitCommand ) {
	    System.out.println( "Exit command received" );
	    destroyApp (true);
	    notifyDestroyed();
	}
    }
}
