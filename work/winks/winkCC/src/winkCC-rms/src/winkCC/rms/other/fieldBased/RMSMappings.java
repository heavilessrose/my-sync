package winkCC.rms.other.fieldBased;

import java.io.*;
import java.util.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import javax.microedition.rms.*;

public class RMSMappings extends MIDlet implements CommandListener {

	private Display display;

	public static final Command exitCommand = new Command("Exit", Command.EXIT,
			1);
	public static final Command testCommand = new Command("Test",
			Command.SCREEN, 1);

	private static Object[][] empList = {
			new Object[] { "1", "ÍõÓ³»ª", "CEO", "100", "F" },
			new Object[] { "2", "John", "CFO", "200", "M" },
			new Object[] { "3", "Pat", "Closem", "300", "F" },
			new Object[] { "4", "PJ", "Admin", "100", "M" }, };

	private static Object[][] deptList = {
			new Object[] { "100", "Executive", "1" },
			new Object[] { "200", "Operations", "2" },
			new Object[] { "300", "Sales", "1" }, };

	public RMSMappings() {
	}

	public void commandAction(Command c, Displayable d) {
		if (c == exitCommand) {
			exitMIDlet();
		} else if (c == testCommand) {
			runTest();
		}
	}

	protected void destroyApp(boolean unconditional)
			throws MIDletStateChangeException {
		exitMIDlet();
	}

	public void exitMIDlet() {
		notifyDestroyed();
	}

	public Display getDisplay() {
		return display;
	}

	protected void initMIDlet() {
		display.setCurrent(new MainForm());
	}

	protected void pauseApp() {
	}

	private void printRecord(FieldBasedStore store, int recordID) {
		try {
			FieldList list = store.getFieldList();
			Object[] fields = store.readRecord(recordID);

			if (fields.length != list.getFieldCount()) {
				System.out.println("Error: bad count");
				return;
			}

			System.out.println("Record " + recordID + ":");

			for (int i = 0; i < fields.length; ++i) {
				System.out.println("  " + list.getFieldName(i) + ": "
						+ fields[i]);
			}
		} catch (RecordStoreException e) {
		} catch (IOException e) {
		}
	}

	private void runTest() {
		// First delete the record stores...
		System.out.println("Deleting record stores...");

		String[] names = RecordStore.listRecordStores();
		if (names != null && names.length > 0)
			for (int i = 0; i < names.length; ++i) {
				try {
					RecordStore.deleteRecordStore(names[i]);
				} catch (RecordStoreException e) {
					System.out.println("Could not delete " + names[i]);
				}
			}

		// Create two record stores, one with a field list
		// stored in the first record and the second with
		// a field list stored separately (in the app)

		RecordStore empRS = null;
		RecordStore deptRS = null;
		FieldList empFields = new FieldList(5);
		FieldList deptFields = new FieldList(3);
		FieldBasedStore employees;
		FieldBasedStore departments;

		empFields.setFieldType(0, FieldList.TYPE_INT);
		empFields.setFieldName(0, "ID");
		empFields.setFieldType(1, FieldList.TYPE_STRING);
		empFields.setFieldName(1, "Given Name");
		empFields.setFieldType(2, FieldList.TYPE_STRING);
		empFields.setFieldName(2, "Last Name");
		empFields.setFieldType(3, FieldList.TYPE_BOOLEAN);
		empFields.setFieldName(3, "Active");
		empFields.setFieldType(4, FieldList.TYPE_CHAR);
		empFields.setFieldName(4, "Sex");

		System.out.println("Initializing employees");

		try {
			empRS = RecordStore.openRecordStore("empRS", true);
			// now store the field list in the RS
			empFields.toRecordStore(empRS, -1);
			employees = new FieldBasedStore(empRS);
		} catch (RecordStoreException e) {
			System.out.println("Could not create empRS");
			return;
		} catch (IOException e) {
			System.out.println("Error storing field list");
			return;
		}

		System.out.println("Initializing departments");

		deptFields.setFieldType(0, FieldList.TYPE_INT);
		deptFields.setFieldName(0, "ID");
		deptFields.setFieldType(1, FieldList.TYPE_STRING);
		deptFields.setFieldName(1, "Name");
		deptFields.setFieldType(2, FieldList.TYPE_INT);
		deptFields.setFieldName(2, "Manager");

		try {
			deptRS = RecordStore.openRecordStore("deptRS", true);
			departments = new FieldBasedStore(deptRS, deptFields);
		} catch (RecordStoreException e) {
			System.out.println("Could not create deptRS");
			return;
		}

		int[] empRecordID;
		int[] deptRecordID;
		int i;

		// Add the data...

		try {
			empRecordID = new int[empList.length];

			for (i = 0; i < empList.length; ++i) {
				empRecordID[i] = employees.addRecord(empList[i]);
			}

			deptRecordID = new int[deptList.length];

			for (i = 0; i < deptList.length; ++i) {
				deptRecordID[i] = departments.addRecord(deptList[i]);
			}
		} catch (RecordStoreException e) {
			System.out.println("Error adding record");
			return;
		} catch (IOException e) {
			System.out.println("Error writing field");
			return;
		}

		// Now fetch the data back and print it...
		System.out.println("---- Employee data ----");

		for (i = 0; i < empRecordID.length; ++i) {
			printRecord(employees, empRecordID[i]);
		}

		System.out.println("---- Department data ----");

		for (i = 0; i < deptRecordID.length; ++i) {
			printRecord(departments, deptRecordID[i]);
		}

		System.out.println("Closing empRS");

		try {
			empRS.closeRecordStore();
		} catch (RecordStoreException e) {
			System.out.println("Error closing empRS");
		}

		System.out.println("Closing deptRS");

		try {
			deptRS.closeRecordStore();
		} catch (RecordStoreException e) {
			System.out.println("Error closing deptRS");
		}

		System.out.println("Dumping record stores...");

		// Analyze them...

		RMSAnalyzer analyzer = new RMSAnalyzer(new RMSAnalyzer.SystemLogger(10));
		analyzer.analyzeAllRecordStores();
	}

	protected void startApp() throws MIDletStateChangeException {
		if (display == null) {
			display = Display.getDisplay(this);
			initMIDlet();
		}
	}

	public class MainForm extends Form {
		public MainForm() {
			super("RMSMappings");

			addCommand(exitCommand);
			addCommand(testCommand);

			setCommandListener(RMSMappings.this);
		}
	}
}
