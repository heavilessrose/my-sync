/**
 * 
 */
package winkCC.rms;

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

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * @author WangYinghua
 * 
 */
public class RMStest extends MIDlet implements RecordListener, CommandListener {
	private static ILog log = LogFactory.getLog("RMStest");
	Display display = null;
	Form form = null;
	final Command save = new Command("Save", Command.OK, 1);
	final Command read = new Command("Read", Command.OK, 1);
	final Command modify = new Command("Modify", Command.OK, 1);
	final Command exit = new Command("Exit", Command.EXIT, 3);

	TextField dataField = new TextField("ÄÚÈÝ", "ABC", 100, TextField.ANY);

	RmsUtils rms = new RmsUtils();
	RecordStore rs = null;
	RMSAnalyzer analyzer;

	public RMStest() {
		display = Display.getDisplay(this);
		form = new Form("RMS test");
		//		rs = rms.openRs("wink_RMS_test");
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
		form.addCommand(save);
		form.addCommand(read);
		form.addCommand(modify);
		form.addCommand(exit);
		form.append(dataField);
		form.setCommandListener(this);
		display.setCurrent(form);

		// init rms
		if (RecordStore.listRecordStores() == null
				|| RecordStore.listRecordStores().length <= 0)
			for (int i = 0; i < 8; i++)
				rms.add();
		analyzer = new RMSAnalyzer();
	}

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == save) {
			rms.writeString(1, dataField.getString());
			rms.writeBoolean(2, true);
			rms.writeChars(3, new char[] { 'a', 'b', 'c' });
			rms.writeInt(4, 456);
			rms.writeLong(5, 456L);
			rms.writeShort(6, (short) 456);

			analyzer.analyzeAllRecordStores();
		} else if (cmd == read) {
			form.deleteAll();
			dataField.setString(rms.getString(1));
			form.append(dataField);

			form.append("1: " + rms.getString(1) + "\n");
			form.append("2: " + rms.getBoolean(2) + "\n");
			form.append("3: " + new String(rms.getChars(3)) + "\n");
			form.append("4: " + rms.getInt(4) + "\n");
			form.append("5: " + rms.getLong(5) + "\n");
			form.append("6: " + rms.getShort(6) + "\n");
			analyzer.analyzeAllRecordStores();
		} else if (cmd == exit) {
			rms.close();
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
