package winkCC.push.test;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.PushRegistry;
import javax.microedition.io.ServerSocketConnection;
import javax.microedition.io.SocketConnection;
import javax.microedition.io.UDPDatagramConnection;
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

	public void unregisterConnection(String url) {
		try {
			boolean success;
			success = PushRegistry.unregisterConnection(url);
		} catch (SecurityException e) {
			System.out.println("SecurityException, insufficient permissions");
			e.printStackTrace();
		}
	}

	/**
	 * Determine if activated due to inbound connection and if so dispatch a
	 * PushProcessor to handle incoming connection(s). @return true if MIDlet
	 * was activated due to inbound connection, false otherwise
	 */
	private boolean handlePushActivation() {
		// Discover if there are pending push inbound
		// connections and if so, dispatch a
		// PushProcessor for each one.
		String[] connections = PushRegistry.listConnections(true);
		if (connections != null && connections.length > 0) {
			for (int i = 0; i < connections.length; i++) {
//				PushProcessor pp = new PushProcessor(connections[i]);
			}
			return (true);
		}
		return (false);
	}

	/**
	 * Output push info for all push connections for the MIDlet suite
	 */
	private void outputPushInfo() {
		// Discover if there are pending push inbound
		// connections and if so, output the push info
		// for each one.
		String[] connections = PushRegistry.listConnections(false);
		if (connections != null && connections.length > 0) {
			for (int i = 0; i < connections.length; i++) {
				String midlet = PushRegistry.getMIDlet(connections[i]);
				String filter = PushRegistry.getFilter(connections[i]);
				// Output the info.
				System.out.println("PushInfo -> " + connections[i] + " "
						+ midlet + " " + filter);
//				form.append(connections[i] + " " + midlet + " " + filter);
			}
		}
	}

	// ---动态绑定--TCP---------------------------------------
	// MIDlet class name.
	String midletClassName = this.getClass().getName();
	// Register a static connection.
	String url = "socket://:5000";
	// Use an unrestricted filter.
	String filter = "*";

	public void RegisteringAnInboundTcpConnection() {
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

	// ---动态绑定--UDP---------------------------------------
	public void RegisteringAnInboundUdpDatagramConnection() {

		try {
			// Open the connection.
			UDPDatagramConnection udgc = (UDPDatagramConnection) Connector
					.open(url);
			// Register the connection now so that when this
			// MIDlet exits (is destroyed), the AMS can activate
			// our MIDlet when network activity is detected. The
			// AMS will remember the registered URL even
			// when the MIDlet is not active.
			PushRegistry.registerConnection(url, midletClassName, filter);
			// Now wait for inbound network activity.
			Datagram dg = udgc.newDatagram(udgc.getNominalLength());
			udgc.receive(dg);
			// Read the inbound messages
//			while (!done) {
//				udgc.receive(dg);
//				// --------------------------------------+
//				// Here do something with received data|
//				// --------------------------------------+
//				System.out.print(new String(dg.getData()));
//			}
		} catch (SecurityException e) {
			System.out.println("SecurityException, insufficient permissions");
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			System.out.println("ClassNotFoundException, MIDlet name not found");
			e.printStackTrace();
		} catch (IOException e) {
			System.out.println("IOException, possibly port already in use.");
			e.printStackTrace();
		}
	}
	// ---------------------------------------------------
	// ---------------------------------------------------
	// ---静态绑定-----------------------------------------

}