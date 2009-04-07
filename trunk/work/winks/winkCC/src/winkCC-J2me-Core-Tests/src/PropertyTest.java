/**
 * 
 */

import java.util.Hashtable;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import winkCC.core.WinksConstants;
import winkCC.core.property_i18n.I18n;
import winkCC.core.property_i18n.Properties;

public class PropertyTest extends MIDlet implements CommandListener {
	Display display = null;
	Form form = null;
	final Command readSettings = new Command("readSettings", Command.OK, 1);
	final Command readI18N = new Command("readI18N", Command.OK, 1);
	final Command write = new Command("write", Command.OK, 1);
	final Command read = new Command("read", Command.OK, 1);
	final Command list = new Command("list", Command.OK, 1);
	final Command mkFile = new Command("mkFile", Command.OK, 1);
	final Command delete = new Command("delete", Command.OK, 1);
	final Command rename = new Command("rename", Command.OK, 1);
	final Command exit = new Command("Exit", Command.EXIT, 3);
	final Command upgrade = new Command("upgrade", Command.OK, 1);

	// TextField dataField = new TextField("ÄÚÈÝ", "ABC", 100, TextField.ANY);
	//
	// FileUtils fileUtils = new FileUtils();

	public PropertyTest() {
		display = Display.getDisplay(this);
		form = new Form("Property test");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
		System.out.println("destroyApp");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	protected void pauseApp() {
		System.out.println("pauseApp");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#startApp()
	 */
	protected void startApp() throws MIDletStateChangeException {
		System.out.println("startApp");
		form.addCommand(readSettings);
		form.addCommand(readI18N);
		form.addCommand(write);
		form.addCommand(read);
		form.addCommand(list);
		form.addCommand(mkFile);
		form.addCommand(delete);
		form.addCommand(rename);
		form.addCommand(exit);
		form.addCommand(upgrade);
		// form.append(dataField);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == readSettings) {
			Properties
					.loadPropertyBundle(WinksConstants.DEFAULT_PROPERTY_MESSAGES_BUNDLE);
			Hashtable dd = Properties._propertyTable;
			form.append("confServer: " + dd.get("confServer"));
			form.append("commServer: " + dd.get("commServer"));
			form.append("ccServer: " + dd.get("ccServer"));
			form.append("smsAddr: " + dd.get("smsAddr"));
		} else if (cmd == readI18N) {
			I18n.initI18nProperty(WinksConstants.DEFAULT_I18N_MESSAGES_BUNDLE,
					"locale");
			Hashtable dd = I18n._messageTable;
			form.append("VALIDATE: " + dd.get("VALIDATE"));
			form.append("CANCEL: " + dd.get("CANCEL"));
		} else if (cmd == write) {

		} else if (cmd == read) {

		} else if (cmd == list) {

		} else if (cmd == delete) {
		} else if (cmd == mkFile) {
		} else if (cmd == rename) {
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
