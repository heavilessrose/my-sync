package winkCC.pim;

import java.util.Enumeration;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.wireless.messaging.MessageConnection;

public class Test extends MIDlet implements CommandListener {
	Display display = null;
	Form form = null;
	final Command test = new Command("test", Command.OK, 1);
	final Command showContacts = new Command("showContacts", Command.BACK, 2);
	final Command exit = new Command("Exit", Command.EXIT, 3);

	TextField numField = new TextField("µÁª∞", "15801630382", 100,
			TextField.PHONENUMBER);
	TextField contentField = new TextField("ƒ⁄»›", "test123≤‚ ‘", 100,
			TextField.ANY);

	String address = "sms://15801630382:5050";
	String smsMonitorPort = "5050";
	MessageConnection mc = null;

	public Test() {
		display = Display.getDisplay(this);
		form = new Form("sms test");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	//	@Override
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	//	@Override
	protected void pauseApp() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#startApp()
	 */
	//	@Override
	protected void startApp() throws MIDletStateChangeException {
		form.addCommand(test);
		form.addCommand(showContacts);
		form.addCommand(exit);
		form.append(numField);
		form.append(contentField);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	ContactUtil util = new ContactUtil();

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == test) {
			new Thread() {
				public void run() {
					ContactUtil.test();
				}
			}.start();
		} else if (cmd == showContacts) {
			
			new Thread() {
				public void run() {
					for (Enumeration peoples = util.getAllPeople().elements(); peoples
							.hasMoreElements();)
						peoples.nextElement().toString();
				}
			}.start();

		} else if (cmd == exit) {
			try {
				destroyApp(true);
			} catch (MIDletStateChangeException e) {
				e.printStackTrace();
			}
			notifyDestroyed();
		}
	}
}
