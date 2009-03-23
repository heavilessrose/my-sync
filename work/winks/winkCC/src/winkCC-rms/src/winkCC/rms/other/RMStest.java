/**
 * 
 */
package winkCC.rms.other;

import java.io.UnsupportedEncodingException;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotOpenException;

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

	TextField nameField = new TextField("名字", "abc", 100, TextField.ANY);
	TextField dataField = new TextField("内容", "ABC", 100, TextField.ANY);

	RmsUtil rms = new RmsUtil(this);
	RecordStore rs = null;

	public RMStest() {
		display = Display.getDisplay(this);
		form = new Form("RMS test");
		rs = rms.openRs("wink_RMS_test");
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
		form.append(nameField);
		form.append(dataField);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	public void commandAction(Command cmd, Displayable disp) {
		Record record = new Record();
		if (cmd == save) {
			record.setName(nameField.getString());
			record.setData(dataField.getString().getBytes());
			rms.addRecord(rs, record);

			RMSAnalyzer analyzer = new RMSAnalyzer();
			analyzer.analyzeAllRecordStores();
		} else if (cmd == read) {
			form.deleteAll();
			form.append(nameField);
			form.append(dataField);

			rs = rms.openRs("wink_RMS_test");
			// recordStore中的所有记录
			RecordEnumeration enum;
			try {
				enum = rs.enumerateRecords(null, null, false);

				//				// 向前遍历
				//				while (enum.hasPreviousElement()) {
				//					int recordId = enum.previousRecordId();
				//					// rs.getRecord(id);
				//					record = rms.getRecord(rs, recordId);
				//					// TODO 对此记录做工作
				//					log.info("name = " + record.getName());
				//					log.info("data = " + new String(record.getData()));
				//				}
				// 向后遍历
				while (enum.hasNextElement()) {
					int recordId = enum.nextRecordId();
					record = rms.getRecord(rs, recordId);
					String name = record.getName();
					String data;
//					try {
						data = new String(record.getData()/*, "UTF-8"*/);
						log.info("name = " + name);
						log.info("data = " + data);
						form.append(rs.getName() + "_" + recordId + "_" + name
								+ ": " + data);
//					} catch (UnsupportedEncodingException e) {
//						e.printStackTrace();
//					}
				}
			} catch (RecordStoreNotOpenException e) {
				e.printStackTrace();
			} catch (InvalidRecordIDException e) {
				e.printStackTrace();
			}
		}else if(cmd == modify){
			rs = rms.openRs("wink_RMS_test");
			rms.modify(rs, 1, "qqqqqq".getBytes());
		}
		else if (cmd == exit) {
			rms.closeRs(rs);
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
