package winkCC.net;

import java.io.IOException;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDletStateChangeException;

import winkCC.core.event.Event;
import winkCC.core.midp.MidpApp;
import winkCC.log.ILog;
import winkCC.log.LogFactory;
import winkCC.push.PushUtil;

public class HttpPushTestMidlet extends MidpApp implements
		winkCC.push.IPushListener {
	private static ILog log = LogFactory.getLog("HttpPushTestMidlet");
	Form form = new FormShow("LogForm");

	public HttpPushTestMidlet() {
		super(240);
		// TODO Auto-generated constructor stub
	}

	// @Override
	protected void destroyApp(boolean uncondition)
			throws MIDletStateChangeException {
		// TODO Auto-generated method stub
		display = null;
	}

	// @Override
	protected void pauseApp() {
		// TODO Auto-generated method stub

	}

	private static int i = 0;

	// @Override
	protected void startApp() throws MIDletStateChangeException {
		// TODO Auto-generated method stub
		// if (PushUtil.isPushActivated()) {
		// display.setCurrent(/*new Alert("push start")*/null);
		// work();
		// } else if (!PushUtil.isPushActivated()) {
		display.setCurrent(form);
		work();
		Alert alert = new Alert("Tip", "Push start", null, AlertType.ERROR);
		alert.setTimeout(Alert.FOREVER);
		display.setCurrent(alert);
		// }

		log.info("i = " + (++i));
	}

	private void work() {
		new Thread() {
			public void run() {
				try {
					final HttpConnection conn = HttpUtil
							.connect("http://www.google.com");
					HttpUtil.getPage(conn);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					log.debug(e);
				}
			}
		}.start();
	}

	public void handleEvent(Event event) {
		// TODO Auto-generated method stub

	}

	public Command backCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command cancelCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command exitCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command getCommand(int id) {
		// TODO Auto-generated method stub
		return null;
	}

	public Command helpCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command homeCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command okCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public Command stopCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	public void handlePushActivation() {
		// TODO push启动后的处理
	}

}
