package winkCC.push.test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.PushRegistry;
import javax.microedition.io.ServerSocketConnection;
import javax.microedition.io.SocketConnection;
import javax.microedition.io.UDPDatagramConnection;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Ticker;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * PushMIDlet - a Push sample MIDlet
 */
public class BasicPushMIDlet extends MIDlet implements CommandListener,
		Runnable {

	// Default values to use.
	String midletClassName = this.getClass().getName();
	String defaultURL = "socket://:6000";
	String defaultFilter = "*";
	long defaultDeltaTime = 15000;
	long REFRESH_TIME = 1000 * 60 * 5; // every 5 minutes

	// User Interface related
	private Command scheduleCmd = new Command("Schedule", Command.SCREEN, 1);
	private Command regCmd = new Command("Register", Command.SCREEN, 2);
	private Command unregCmd = new Command("UnRegister", Command.SCREEN, 3);
	private Command listCmd = new Command("List", Command.SCREEN, 4);
	private Command thCmd;
	protected Display display;
	private Ticker ticker;
	private Form form;

	/** Constructor */
	public BasicPushMIDlet() {
	}

	/**
	 * Initial state.
	 * 
	 * @throw MIDletStateChangeException to indicate a transient error has
	 *        occured.
	 */
	public void startApp() throws MIDletStateChangeException {
		try {
			if (display == null) {
				System.out.println("Setting up...");
				display = Display.getDisplay(this);
				// Create a Ticker and a Form, add commands
				// to the form to test PushRegistry alarms and
				// inbound connections, set the Form'scommand
				// listener, and the Ticker to the Form.
				ticker = new Ticker("");
				form = new Form("Basic Push Test");
				form.addCommand(scheduleCmd);
				form.addCommand(regCmd);
				form.addCommand(unregCmd);
				form.addCommand(listCmd);
				form.setCommandListener(this);
				form.setTicker(ticker);
				display.setCurrent(form);

				// Discover if activation is due to an inbound
				// connection.
				if (isPushActivated() == true) {
					ticker.setString("Push Activated");
				}
			}
		} catch (Exception e) {
			System.out.println("Exception during startApp()");
			e.printStackTrace();
			// If some kind of transient error ocurrs, throw a
			// MIDledStateChangeException.
			throw new MIDletStateChangeException("Error Starting...");
		}
	}

	/** Paused state. Release resources (connection, threads, etc). */
	public void pauseApp() {
		display = null;
	}

	/**
	 * Destroyed state. Release resources (connection, threads, etc). Also,
	 * schedule the future launch of the MIDlet. @param uc If true when this
	 * method is called, the MIDlet must cleanup and release all resources. If
	 * false the MIDlet may throw @throw MIDletStateChangeException to indicate
	 * it does not want to be destroyed at this time.
	 */
	public void destroyApp(boolean uc) throws MIDletStateChangeException {
		display = null;
		try {
			// Set up the alarm and force the MIDlet to exit.
			scheduleMIDlet(defaultDeltaTime);
		} catch (ClassNotFoundException e) {
			System.out.println("Exception during destroyApp()");
			e.printStackTrace();
		} catch (ConnectionNotFoundException e) {
			System.out.println("Exception during destroyApp()");
			e.printStackTrace();
		}
	}

	/**
	 * Command/button listener.
	 * 
	 * @param c
	 *            the LCDUI Command to process.
	 * @param d
	 *            the Displayable source of the Command.
	 */
	public void commandAction(Command c, Displayable d) {
		thCmd = c;
		// Dispatch a thread to process commands.
		if (c == scheduleCmd || c == regCmd || c == unregCmd || c == listCmd) {
			Thread th = new Thread(this);
			th.start();
		}
	} // commandAction()

	/** Thread run method. */
	public void run() {
		try {
			// Do alarm or inbound connection test based
			// on value of last Command
			if (thCmd == scheduleCmd) {
				// cyclic background task info.
				Timer aTimer = new Timer();
				MyTask myTask = new MyTask();
				aTimer.schedule(myTask, 0, defaultDeltaTime);
			} else if (thCmd == regCmd) {
				// Register connection
				// PushRegistry.registerConnection(defaultURL, midletClassName,
				// defaultFilter);
				registerDynSocket();
			} else if (thCmd == unregCmd) {
				// Unregister connectioun
				// boolean status =
				// PushRegistry.unregisterConnection(defaultURL);
			} else if (thCmd == listCmd) {
				// Dump push info
				outputPushInfo();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 是否push启动. Determine if activated due to inbound connection.
	 * 
	 * @return true if MIDlet was activated due to inbound connection, false
	 *         otherwise
	 */
	private boolean isPushActivated() {
		// Discover if there are pending push inbound connections and if so,
		// dispatch a PushProcessor for each one.
		String[] connections = PushRegistry.listConnections(true);
		if (connections != null && connections.length > 0) {
			return (true);
		}
		return (false);
	}

	/**
	 * Determine if activated due to inbound connection and if so dispatch a
	 * PushProcessor to handle incoming connection(s).
	 * 
	 * @return true if MIDlet was activated due to inbound connection, false
	 *         otherwise
	 */
	private boolean handlePushActivation() {
		// Discover if there are pending push
		// inbound connections and if so, dispatch
		// a PushProcessor for each one.
		String[] connections = PushRegistry.listConnections(true);
		if (connections != null && connections.length > 0) {
			System.out.println("is PushActivated");
			for (int i = 0; i < connections.length; i++) {
				PushProcessor pp = new PushProcessor(connections[i]);
			}
			return (true);
		}
		return (false);
	}

	/**
	 * Output push info for all push connections for the MIDlet suite
	 */
	private void outputPushInfo() {
		// For all registered inbound connections output
		// the push info for each one.
		String[] connections = PushRegistry.listConnections(false);
		if (connections != null && connections.length > 0) {
			for (int i = 0; i < connections.length; i++) {
				String midlet = PushRegistry.getMIDlet(connections[i]);
				String filter = PushRegistry.getFilter(connections[i]);
				// Output the info.
				System.out.println("PushInfo -> " + connections[i] + " "
						+ midlet + " " + filter);
				form.append(connections[i] + " " + midlet + " " + filter);
			}
		}
	}

	/**
	 * 设置定时启动. Set up a PUSH alarm event.
	 * 
	 * @param et
	 *            is the expiration delta time in milliseconds.
	 */
	private void scheduleMIDlet(long et) throws ClassNotFoundException,
			ConnectionNotFoundException, SecurityException {
		Date alarm = new Date();
		String midletClassName = this.getClass().getName();
		long t = PushRegistry.registerAlarm(midletClassName, alarm.getTime()
				+ et);
	}

	/**
	 * Example on registering a dynamic socket connection.
	 */
	private void registerDynSocket() {

		// MIDlet class name.
		String midletClassName = this.getClass().getName();
		// Register a dynamic connection.
		String url = "socket://";
		// Use an unrestricted filter.
		String filter = "*";

		try {
			// Open the connection.
			ServerSocketConnection ssc = (ServerSocketConnection) Connector
					.open(url);
			// Discover the system-assigned port.
			int port = ssc.getLocalPort();
			url = "socket://:" + port;
			System.out.println("TCP: " + port);
			// Register the connection now. The AMS will remember the registered
			// URL even when the MIDlet is not active.
			PushRegistry.registerConnection(url, midletClassName, filter);
			// Now publish the push URL. We can use an HTTP POST or a socket or
			// datagram for this.
			String purl;
			int port1 = ssc.getLocalPort();
			purl = "socket://" + ssc.getLocalAddress() + ":" + port1;
			System.out.println("TCP: " + port1);
			// 发布
			publishInboundConnection(purl, midletClassName);
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Example on registering a dynamic datagram connection.
	 */
	private void registerDynDatagram() {

		// MIDlet class name.
		String midletClassName = this.getClass().getName();
		// Register a dynamic connection.
		String url = "datagram://";
		// Use an unrestricted filter.
		String filter = "*";

		try {
			// Open the connection.
			UDPDatagramConnection udgc = (UDPDatagramConnection) Connector
					.open(url);

			// Discover the system-assigned port.
			int port = udgc.getLocalPort();
			url = "datagram://:" + port;
			System.out.println("udp: " + port);
			// Register the connection now. The AMS will remember
			// the registered URL even when the MIDlet is not
			// active.
			PushRegistry.registerConnection(url, midletClassName, filter);
			// Now publish the push URL. We can use an HTTP
			// POST or a socket or datagram for this.
			String purl;
			int port1 = udgc.getLocalPort();
			purl = "datagram://" + udgc.getLocalAddress() + ":" + port1;
			System.out.println("udp: " + port1);
			// 发布
			publishInboundConnection(purl, midletClassName);
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	/**
	 * An example of a method to publish dynamic inbound address.
	 * 
	 * @param url
	 *            is the inbound address
	 * @param midlet
	 *            is the name of the midlet
	 */
	private void publishInboundConnection(String url, String midlet) {
		String hostToPostTo = "j2medeveloper:7000/pushagent";
		OutputStream os = null;
		HttpConnection hc = null;
		try {
			if (hostToPostTo.startsWith("http://") == false) {
				hostToPostTo = "http://" + hostToPostTo;
			}
			hc = (HttpConnection) Connector.open(hostToPostTo);
			// Set HTTP request method to POST,set Content-Type
			// and send body.
			int len = url.length() + midlet.length();
			String pushinfo = "pushurl=" + url + ", pushmidlet=" + midlet;
			hc.setRequestMethod(HttpConnection.POST);
			hc.setRequestProperty("User-Agent",
					"Profile/MIDP-1.0 Configuration/CLDC-1.0");
			hc.setRequestProperty("Content-Language", "en-US");
			hc.setRequestProperty("Accept", "text/xml");
			hc.setRequestProperty("Connection", "close");

			hc.setRequestProperty("Content-Type",
					"application/x-www-form-urlencoded");
			hc.setRequestProperty("Content-Length", "" + pushinfo.length());
			os = hc.openOutputStream();
			os.write(pushinfo.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (os != null)
					os.close();
				if (hc != null)
					hc.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	// -----------------------------------------------------

	/**
	 * MyTask - Thread responsible of cyclic processing while the MIDlet is
	 * active. Add your cyclic logic to the run() method.
	 */
	class MyTask extends TimerTask {
		// Constructor.
		public MyTask() {
		}

		// .....
		// Thread run method.
		public void run() {
			// ..... Your Task Logic
			System.out.println("Cycling...");
		}
	}

	// -----------------------------------------------------

	/**
	 * PushProcessor - Thread of execution responsible of receiving and
	 * processing push events.
	 */
	class PushProcessor implements Runnable {

		Thread th = new Thread(this);
		String url;
		boolean done = false;
		String midletClassName;

		/** Constructor */
		public PushProcessor(String url) {
			this.url = url;
			th.start();
		}

		public void notifyDone() {
			done = true;
		}

		/**
		 * Thread's run method to wait for and process received messages.
		 */
		public void run() {
			ServerSocketConnection ssc = null;
			SocketConnection sc = null;
			UDPDatagramConnection udgc = null;
			Datagram dg = null;
			InputStream is = null;
			try {
				while (!done) {
					if (url.startsWith("socket://")) {
						// "Open" connection.
						ssc = (ServerSocketConnection) Connector.open(url);
						// Wait for (and accept) inbound connection.
						sc = (SocketConnection) ssc.acceptAndOpen();
						is = sc.openInputStream();
						// Read data from inbound connection.
						int ch;
						byte[] data = null;
						ByteArrayOutputStream tmp = new ByteArrayOutputStream();
						while ((ch = is.read()) != -1) {
							tmp.write(ch);
						}
						data = tmp.toByteArray();
						// --------------------------------------+
						// Here do something with received data|
						// --------------------------------------+
						System.out.print(new String(data));
					} else {
						if (url.startsWith("datagram://")) {
							// Open the connection.
							udgc = (UDPDatagramConnection) Connector.open(url);
							// Now wait for inbound network
							// activity.
							dg = udgc.newDatagram(udgc.getNominalLength());
							// Read the inbound messages
							while (!done) {
								udgc.receive(dg);
								// -------------------------+
								// Here do something with |
								// received data |
								// -------------------------+
								System.out.print(new String(dg.getData()));
							}
						}
					}
				}
			} catch (IOException e) {
				System.out.println("PushProcessor.run Exception" + e);
				e.printStackTrace();
			} finally {
				try {
					if (is != null)
						is.close();
					if (sc != null)
						sc.close();
					if (ssc != null)
						ssc.close();
					if (udgc != null)
						udgc.close();
				} catch (Exception e) {
				}
			}
		}
	} // PushProcessor
} // PushMIDlet
