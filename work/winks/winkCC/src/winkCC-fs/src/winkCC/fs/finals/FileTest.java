/**
 * 
 */
package winkCC.fs.finals;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;

public class FileTest extends MIDlet implements RecordListener, CommandListener {
	Display display = null;
	Form form = null;
	final Command mkdirs = new Command("mkdirs", Command.OK, 1);
	final Command read = new Command("Read", Command.OK, 1);
	final Command modify = new Command("Modify", Command.OK, 1);
	final Command exit = new Command("Exit", Command.EXIT, 3);

	TextField dataField = new TextField("ÄÚÈÝ", "ABC", 100, TextField.ANY);

	FileUtils fileUtils = new FileUtils();

	public FileTest() {
		display = Display.getDisplay(this);
		form = new Form("File test");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	protected void pauseApp() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#startApp()
	 */
	protected void startApp() throws MIDletStateChangeException {
		form.addCommand(mkdirs);
		form.addCommand(read);
		form.addCommand(modify);
		form.addCommand(exit);
		form.append(dataField);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == mkdirs) {
			new Thread() {
				public void run() {
					fileUtils.mkDirs("root1/q1q/q2q/q3q/q4q/");
				}
			}.start();
		} else if (cmd == read) {

		} else if (cmd == exit) {
			//			rms.close();
			try {
				destroyApp(true);
			} catch (MIDletStateChangeException e) {
				e.printStackTrace();
			}
			notifyDestroyed();
		}
	}

	//// record listener
	public void recordAdded(RecordStore rs, int id) {
		System.out.println("record added");
	}

	public void recordChanged(RecordStore rs, int id) {
		System.out.println("record Changed");
	}

	public void recordDeleted(RecordStore rs, int id) {
		System.out.println("record Deleted");
	}

}
