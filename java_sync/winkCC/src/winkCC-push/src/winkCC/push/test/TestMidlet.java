package winkCC.push.test;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.PushRegistry;
import javax.microedition.io.ServerSocketConnection;
import javax.microedition.io.SocketConnection;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class TestMidlet extends MIDlet {

	// @Override
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {

	}

	// @Override
	protected void pauseApp() {

	}

	// @Override
	protected void startApp() throws MIDletStateChangeException {

	}

	// ---¶¯Ì¬°ó¶¨-----------------------------------------
	// MIDlet class name.
	String midletClassName = this.getClass().getName();
	// Register a static connection.
	String url = "socket://:5000";
	// Use an unrestricted filter.
	String filter = "*";

	public void RegisteringAnInboundConnection() {
		try {
			// Open the connection.
			ServerSocketConnection ssc = (ServerSocketConnection) Connector
					.open(url);

			PushRegistry.registerConnection(url, midletClassName, filter);
			// Now wait for inbound network activity.
			SocketConnection sc = (SocketConnection) ssc.acceptAndOpen();
			// Read data from inbound connection.
			InputStream is = sc.openInputStream();
			// ..... read from the input stream.

			// Here process the inbound data.
			// .....
		} catch (SecurityException e) {
			System.out.println("SecurityException,insufficient permissions");
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			System.out.println("ClassNotFoundException,MIDlet name not found");
			e.printStackTrace();
		} catch (IOException e) {
			System.out.println("IOException,possibly port already in use.");
			e.printStackTrace();
		}
	}

	// ---------------------------------------------------
	// ---------------------------------------------------
	// ---¾²Ì¬°ó¶¨-----------------------------------------

}