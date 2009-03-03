package com.winksi.j2me.cc.model.main;

/**
 *
 * @author jerry
 */
import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import java.io.IOException;
//import java.io.InputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.io.file.FileSystemRegistry;
import javax.microedition.lcdui.Alert;

public class FileModel extends Thread {

	private GUIController controller;
	private int event = 0;
	private static Enumeration e;
	private static FileConnection currDir;
	private OutputStream fos;
	private InputStream is;
	private String currDirName = "/";
	private String currPath;
	private static final String UP_DIRECTORY = "..";
	private static final String SEP_STR = "/";
	private static final char SEP = '/';
	Vector vector = new Vector();
	private boolean isread = false;

	//    private byte[] data;
	public FileModel(GUIController controller) {
		this.controller = controller;
		//        this.currDirName = currDirName;
		currPath = CPProperty.SAVEPATH;
		vector = new Vector();
		//        running = true;
	}

	public void run() {
		if (!isread) {
			traverseDirectory(currDirName);
		}
	}

	private synchronized void showCurrDir() {
		Enumeration e = null;
		FileConnection currDir = null;

		if (!vector.isEmpty()) {
			vector.removeAllElements();
			vector.trimToSize();
		}
		try {
			if (SEP_STR.equals(currDirName)) {
				e = FileSystemRegistry.listRoots();
			} else {
				currDir = (FileConnection) Connector.open("file:///" + currDirName);

				boolean isexist = currDir.exists();

				if (!isexist) {
					currDir.mkdir();
				}

				if (currDirName.lastIndexOf('/') > 0) {
					e = currDir.list();
					//                    vector.addElement(UP_DIRECTORY);
				} else {
					is = currDir.openInputStream();
				}

			}
			if (e != null) {
				int i = 0;
				while (e.hasMoreElements()) {
					i++;
					String fileName = ((String) e.nextElement()).toLowerCase();
					if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg") || fileName.endsWith(".gif")/*|| fileName.endsWith(".png")*/) {
						//                        System.out.println("read : "+"file:///" + currDirName+"/"+fileName);
						vector.addElement("file:///" + currDirName + "/" + fileName);
					}
				}
			}
			if (currDir != null) {
				currDir.close();
			}
			isread = false;
		} catch (IOException ioe) {
			isread = false;
			Alert alert = new Alert(ioe.toString());
			alert.setTimeout(Alert.FOREVER);
			controller.setAlertDis(alert);
		}
	}

	/** 遍历彩像文件夹 */
	public void traverseDirectory(String fileName) {
		isread = true;
		if (currDirName.equals(SEP_STR)) {
			if (fileName.equals(UP_DIRECTORY)) {
				// can not go up from MEGA_ROOT
				return;
			}
			currDirName = fileName;
		} else if ((fileName.substring(fileName.length() - 2)).equals(UP_DIRECTORY)) {
			int i = currDirName.lastIndexOf(SEP, currDirName.length() - 2);

			if (i != -1) {
				currDirName = currDirName.substring(0, i + 1);
			} else {
				currDirName = SEP_STR;
			}
		} else {
			currDirName = fileName;
		}
		showCurrDir();
	}

	public String getCurrDirName() {
		return currDirName;
	}

	public void setCurrDirName(String s) {
		currDirName = s;
	}

	public Vector getList() {
		return vector;
	}

	/** 得到图片文件输入流 */
	public static InputStream getFileInputStream(String path) throws IOException {
		FileConnection currDir = null;
		InputStream is = null;
		currDir = (FileConnection) Connector.open(path);
		is = currDir.openInputStream();

		return is;
	}

	public InputStream getInputStream() {
		return is;
	}

	public synchronized void createFile(String path, byte[] data) throws Exception {
		//        iswrite = true;
		FileConnection fc = (FileConnection) Connector.open("file:///" + path);
		OutputStream fos = null;

		if (!fc.exists()) {
			fc.create();
		}

		fos = fc.openOutputStream();

		fos.write(data, 0, data.length);

		fos.flush();

		if (fos != null) {
			fos.close();
		}
		if (fc != null) {
			fc.close();
		}
		//        iswrite = false;
	}

	public synchronized void closeed() throws Exception {
		currDir.close();
	}

	public synchronized int write(byte[] b, int nStart, int nLen) {
		int n = -1;
		try {
			fos = currDir.openOutputStream();
			fos.write(b, nStart, nLen);
			n = nLen;
		} catch (IOException e) {
			e.printStackTrace();
		}

		return n;
	}

	public FileConnection getFile() {
		return currDir;
	}

	public void setEvent(int event) {
		this.event = event;
	}

	public boolean isRead() {
		return this.isread;
	}
}
