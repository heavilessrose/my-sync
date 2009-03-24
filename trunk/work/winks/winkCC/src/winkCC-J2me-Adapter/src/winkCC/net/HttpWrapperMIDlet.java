package winkCC.net;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.TextBox;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * A simple MIDlet that does nothing except prompt for a URL and then connects to it and reads the data it sends back. 
 * The connection is logged using the HttpConnectionWrapper.
 * @author WangYinghua
 *
 */
public class HttpWrapperMIDlet extends MIDlet implements CommandListener, Runnable {

	private Display display;
	private String url = "http://localhost:8080/httpLog/";

	public static final Command exitCommand = new Command("Exit", Command.EXIT, 1);
	public static final Command okCommand = new Command("OK", Command.OK, 1);

	public HttpWrapperMIDlet() {
	}

	/* Process commands. If the OK command is invoked, need to determine which screen was active. */
	public void commandAction(Command c, Displayable d) {
		if (c == exitCommand) {
			exitMIDlet();
		} else if (c == okCommand && d instanceof Prompter) {
			// Start the page loading....
			url = ((Prompter) d).getString();
			if (url.indexOf("://") == -1) {
				url = "http://" + url;
			}
			display.setCurrent(new Wait());
		} else if (c == okCommand && d instanceof Lister) {
			display.setCurrent(new Prompter());
		}
	}

	protected void destroyApp(boolean unconditional) throws MIDletStateChangeException {
		exitMIDlet();
	}

	/**
	 * Displays an error message using an alert.
	 * @param title
	 * @param msg
	 * @param e
	 */
	private void error(String title, String msg, Throwable e) {
		Alert a = new Alert(title);
		if (msg == null && e != null) {
			msg = "Exception:";
		}
		a.setString(msg + " " + (e != null ? e.toString() : ""));
		a.setType(AlertType.ERROR);
		a.setTimeout(Alert.FOREVER);
		display.setCurrent(a, new Prompter());
	}

	public void exitMIDlet() {
		notifyDestroyed();
	}

	public Display getDisplay() {
		return display;
	}

	/** Start by prompting the user... */
	protected void initMIDlet() {
		display.setCurrent(new Prompter());
	}

	protected void pauseApp() {
	}

	/* Does the actual parsing. Invoked indirectly by the wait screen using display.callSerially. 
	 * Loads the page and then parses it for the META tags. */
	public void run() {
		Wait w = (Wait) display.getCurrent();
		HttpConnection conn = null;
		InputStream in = null;

		try {
			conn = new HttpConnectionWrapper((HttpConnection) Connector.open(url));
			conn.setRequestProperty("User-Agent", "Profile/MIDP-1.0 Configuration/CLDC-1.0");
			conn.setRequestProperty("Connection", "close");
			conn.setRequestProperty("Send-HTTP-Log-To", "hit456@gmail.com");
			int rc = conn.getResponseCode();
			w.update("Response code " + rc);
			in = conn.openInputStream();

			for (int i = 0; i < 100; ++i) {
				if (in.read() == -1)
					break;
			}

			display.setCurrent(new Prompter());
		} catch (IOException e) {
			error("Exception", "Error reading page", e);
		} finally {
			if (in != null) {
				try {
					in.close();
				} catch (IOException e) {
				}
			}
			if (conn != null) {
				try {
					conn.close();
				} catch (IOException e) {
				}
			}
		}
	}

	protected void startApp() throws MIDletStateChangeException {
		if (display == null) {
			display = Display.getDisplay(this);
			initMIDlet();
		}
	}

	/**
	 * A simple class that prompts for a URL.
	 * @author WangYinghua
	 *
	 */
	class Prompter extends TextBox {

		Prompter() {
			super("Enter a URL:", url, 200, 0);
			addCommand(okCommand);
			addCommand(exitCommand);
			setCommandListener(HttpWrapperMIDlet.this);
		}
	}

	/** 
	 * Displays our list of tags. The tags are added to the list in the run method.
	 * @author WangYinghua
	 *
	 */
	class Lister extends List {

		Lister() {
			super("Meta tags:", List.IMPLICIT);
			addCommand(okCommand);
			setCommandListener(HttpWrapperMIDlet.this);
		}
	}

	/**
	 * Asks the user to the wait while a lengthy operation takes place. 
	 * The operation does not start until the canvas has been painted once, 
	 * at which point display.callSerially is invoked to start it.
	 * @author WangYinghua
	 *
	 */
	class Wait extends Canvas {

		boolean called = false;
		String msg = "Connecting...";

		/* Draw the message on screen.... fancier code would center and wrap the text... */
		protected void paint(Graphics g) {
			g.setColor(255, 255, 255);
			g.fillRect(0, 0, getWidth(), getHeight());
			g.setColor(0, 0, 0);
			g.drawString(msg, 0, 0, Graphics.TOP | Graphics.LEFT);

			if (!called) {
				Thread t = new Thread(HttpWrapperMIDlet.this);
				t.start();
				called = true;
			}
		}

		/**
		 * Update the string and immediately repaint the screen.
		 * @param newMsg
		 */
		public void update(String newMsg) {
			msg = newMsg;
			repaint(0, 0, getWidth(), getHeight());
			serviceRepaints();
		}
	}
}
