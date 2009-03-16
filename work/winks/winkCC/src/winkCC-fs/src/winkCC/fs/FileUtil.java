package winkCC.fs;

import java.io.IOException;
import java.io.InputStream;
import java.util.Calendar;
import java.util.Date;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.io.file.FileSystemRegistry;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextBox;
import javax.microedition.lcdui.TextField;

public class FileUtil {

	private static final String[] attrList = { "Read", "Write", "Hidden" };
	private static final String[] typeList = { "Regular File", "Directory" };
	private static final String[] monthList = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
			"Nov", "Dec" };

	/* special string denotes upper directory */
	private static final String UP_DIRECTORY = "..";

	/* special string that denotes upper directory accessible by this browser.
	 * this virtual directory contains all roots.
	 */
	private static final String MEGA_ROOT = "/";

	/* separator string as defined by FC specification */
	private static final String SEP_STR = "/";

	/* separator character as defined by FC specification */
	private static final char SEP = '/';




	public void commandAction(Command c, Displayable d) {
		if (c == view) {// 显示目录或文件
			List curr = (List) d;
			final String currFile = curr.getString(curr.getSelectedIndex());
			new Thread(new Runnable() {

				public void run() {
					if (currFile.endsWith(SEP_STR) || currFile.equals(UP_DIRECTORY)) {// 目录
						traverseDirectory(currFile);
					} else {
						// Show file contents
						showFile(currFile);
					}
				}
			}).start();
		} else if (c == prop) {// 属性
			List curr = (List) d;
			String currFile = curr.getString(curr.getSelectedIndex());

			showProperties(currFile);
		} else if (c == creat) {// 新建
			createFile();
		} else if (c == creatOK) {
			String newName = nameInput.getString();

			if ((newName == null) || newName.equals("")) {
				Alert alert = new Alert("Error!", "File Name is empty. Please provide file name", null, AlertType.ERROR);
				alert.setTimeout(Alert.FOREVER);
				Display.getDisplay(this).setCurrent(alert);
			} else {
				// Create file in a separate thread and disable all commands
				// except for "exit"
				executeCreateFile(newName, typeInput.getSelectedIndex() != 0);
				Display.getDisplay(this).getCurrent().removeCommand(creatOK);
				Display.getDisplay(this).getCurrent().removeCommand(back);
			}
		} else if (c == delete) {
			List curr = (List) d;
			String currFile = curr.getString(curr.getSelectedIndex());
			executeDelete(currFile);
		}
	}

	void delete(String currFile) {
		if (!currFile.equals(UP_DIRECTORY)) {
			if (currFile.endsWith(SEP_STR)) {
				checkDeleteFolder(currFile);
			} else {
				deleteFile(currFile);
//				showCurrDir();
			}
		} else {
			// 
		}
	}

	private void executeDelete(String currFile) {
		final String file = currFile;
		new Thread(new Runnable() {

			public void run() {
				delete(file);
			}
		}).start();
	}

	private void checkDeleteFolder(String folderName) {
		try {
			FileConnection fcdir = (FileConnection) Connector.open("file:///" + folderName);
			Enumeration content = fcdir.list("*", true);

			//only empty directory can be deleted
			if (!content.hasMoreElements()) {
				fcdir.delete();
//				showCurrDir();
			} else {
				// 提示非空文件夹无法删除
			}
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	/** Starts creatFile with another Thread */
	private void executeCreateFile(final String name, final boolean val) {
		new Thread(new Runnable() {

			public void run() {
				createFile(name, val);
			}
		}).start();
	}

	/** Show file list in the current directory. */
//	void showCurrDir() {
//		Enumeration e;
//		FileConnection currDir = null;
//		List browser;
//
//		try {
//			if (MEGA_ROOT.equals(currDirName)) {
//				e = FileSystemRegistry.listRoots();
//				browser = new List(currDirName, List.IMPLICIT);
//			} else {
//				currDir = (FileConnection) Connector.open("file://localhost/" + currDirName);
//				e = currDir.list();
//				browser = new List(currDirName, List.IMPLICIT);
//				// not root - draw UP_DIRECTORY
//				browser.append(UP_DIRECTORY, dirIcon);
//			}
//
//			while (e.hasMoreElements()) {
//				String fileName = (String) e.nextElement();
//
//				if (fileName.charAt(fileName.length() - 1) == SEP) {
//					// This is directory
//					browser.append(fileName, dirIcon);
//				} else {
//					// this is regular file
//					browser.append(fileName, fileIcon);
//				}
//			}
//
//			browser.setSelectCommand(view);
//
//			//Do not allow creating files/directories beside root
//			if (!MEGA_ROOT.equals(currDirName)) {
//				browser.addCommand(prop);
//				browser.addCommand(creat);
//				browser.addCommand(delete);
//			}
//
//			browser.addCommand(exit);
//
//			browser.setCommandListener(this);
//
//			if (currDir != null) {
//				currDir.close();
//			}
//
//			Display.getDisplay(this).setCurrent(browser);
//		} catch (IOException ioe) {
//			ioe.printStackTrace();
//		}
//	}

	/** 进入目录 */
	void traverseDirectory(String fileName) {
		/* In case of directory just change the current directory and show it */
		if (currDirName.equals(MEGA_ROOT)) {
			if (fileName.equals(UP_DIRECTORY)) {
				// can not go up from MEGA_ROOT
				return;
			}

			currDirName = fileName;
		} else if (fileName.equals(UP_DIRECTORY)) {
			// Go up one directory
			int i = currDirName.lastIndexOf(SEP, currDirName.length() - 2);

			if (i != -1) {
				currDirName = currDirName.substring(0, i + 1);
			} else {
				currDirName = MEGA_ROOT;
			}
		} else {
			currDirName = currDirName + fileName;
		}

		showCurrDir();
	}

	void showFile(String fileName) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file://localhost/" + currDirName + fileName);

			if (!fc.exists()) {
				throw new IOException("File does not exists");
			}

			InputStream fis = fc.openInputStream();
			byte[] b = new byte[1024];

			int length = fis.read(b, 0, 1024);

			fis.close();
			fc.close();

			TextBox viewer = new TextBox("View File: " + fileName, null, 1024, TextField.ANY | TextField.UNEDITABLE);

			viewer.addCommand(back);
			viewer.addCommand(exit);
			viewer.setCommandListener(this);

			if (length > 0) {
				viewer.setString(new String(b, 0, length));
			}

			Display.getDisplay(this).setCurrent(viewer);
		} catch (Exception e) {
			Alert alert = new Alert("Error!", "Can not access file " + fileName + " in directory " + currDirName
					+ "\nException: " + e.getMessage(), null, AlertType.ERROR);
			alert.setTimeout(Alert.FOREVER);
			Display.getDisplay(this).setCurrent(alert);
		}
	}

	void deleteFile(String fileName) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///" + fileName);
			fc.delete();
		} catch (Exception e) {
		}
	}

	void showProperties(String fileName) {
		try {
			if (fileName.equals(UP_DIRECTORY)) {
				return;
			}

			FileConnection fc = (FileConnection) Connector.open("file:///" + fileName);

			if (!fc.exists()) {
				throw new IOException("File does not exists");
			}

			Form props = new Form("Properties: " + fileName);
			ChoiceGroup attrs = new ChoiceGroup("Attributes:", Choice.MULTIPLE, attrList, null);

			attrs.setSelectedFlags(new boolean[] { fc.canRead(), fc.canWrite(), fc.isHidden() });

			props.append(new StringItem("Location:", currDirName));
			props.append(new StringItem("Type: ", fc.isDirectory() ? "Directory" : "Regular File"));
			props.append(new StringItem("Modified:", myDate(fc.lastModified())));
			props.append(attrs);

			props.addCommand(back);
			props.addCommand(exit);
			props.setCommandListener(this);

			fc.close();

			Display.getDisplay(this).setCurrent(props);
		} catch (Exception e) {
			Alert alert = new Alert("Error!", "Can not access file " + fileName + " in directory " + currDirName
					+ "\nException: " + e.getMessage(), null, AlertType.ERROR);
			alert.setTimeout(Alert.FOREVER);
			Display.getDisplay(this).setCurrent(alert);
		}
	}

	void createFile() {
		Form creator = new Form("New File");
		nameInput = new TextField("Enter Name", null, 256, TextField.ANY);
		typeInput = new ChoiceGroup("Enter File Type", Choice.EXCLUSIVE, typeList, iconList);
		creator.append(nameInput);
		creator.append(typeInput);
		creator.addCommand(creatOK);
		creator.addCommand(back);
		creator.addCommand(exit);
		creator.setCommandListener(this);
		Display.getDisplay(this).setCurrent(creator);
	}

	void createFile(String newName, boolean isDirectory) {
		try {
			FileConnection fc = (FileConnection) Connector.open("file:///" + currDirName + newName);

			if (isDirectory) {
				fc.mkdir();
			} else {
				fc.create();
			}

			showCurrDir();
		} catch (Exception e) {
			String s = "Can not create file '" + newName + "'";

			if ((e.getMessage() != null) && (e.getMessage().length() > 0)) {
				s += ("\n" + e);
			}

			Alert alert = new Alert("Error!", s, null, AlertType.ERROR);
			alert.setTimeout(Alert.FOREVER);
			Display.getDisplay(this).setCurrent(alert);
			// Restore the commands that were removed in commandAction()
			Display.getDisplay(this).getCurrent().addCommand(creatOK);
			Display.getDisplay(this).getCurrent().addCommand(back);
		}
	}

	/** 得到日期时间字符串 */
	private String myDate(long time) {
		Calendar cal = Calendar.getInstance();

		cal.setTime(new Date(time));

		StringBuffer sb = new StringBuffer();

		sb.append(cal.get(Calendar.HOUR_OF_DAY));
		sb.append(':');
		sb.append(cal.get(Calendar.MINUTE));
		sb.append(':');
		sb.append(cal.get(Calendar.SECOND));
		sb.append(',');
		sb.append(' ');
		sb.append(cal.get(Calendar.DAY_OF_MONTH));
		sb.append(' ');
		sb.append(monthList[cal.get(Calendar.MONTH)]);
		sb.append(' ');
		sb.append(cal.get(Calendar.YEAR));

		return sb.toString();
	}
}
