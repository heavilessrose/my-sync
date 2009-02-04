package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.io.*;
import javax.microedition.midlet.*;
import java.util.*;

public class JMEUtility {

	private static Hashtable imageTable = new Hashtable();

	public static synchronized Image getImage(String path) {

		try {
			Object objImage = imageTable.get(path);
			if ( objImage != null ) {
				return (Image)objImage;
			}
			else {
				Image aImage = Image.createImage(path);
				imageTable.put(path, aImage);
				return aImage;
			}
		}
		catch(Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	public static void showErrorAlert(String hdrMsg,
									String msg,
									int timeout,
									Form fromForm,
									MIDlet midlet) {

		Alert anAlert = new Alert(hdrMsg,
								msg,
								null,
								AlertType.ERROR );
		anAlert.setTimeout(timeout);
		(Display.getDisplay(midlet)).setCurrent(anAlert, fromForm);

	}
	public static void showInfoAlert(String hdrMsg,
									String msg,
									int timeout,
									Form fromForm,
									MIDlet midlet) {

		Alert anAlert = new Alert(hdrMsg,
								msg,
								null,
								AlertType.INFO );
		anAlert.setTimeout(timeout);
		(Display.getDisplay(midlet)).setCurrent(anAlert, fromForm);
	}
    public static String findContentType(String ext) throws Exception {
		// guess content type
		String ct = "";
		if (ext.equals("mpg") || ext.equals("avi") ) {
			ct = "video/mpeg";
		} else if (ext.equals("3gp")) {
			ct = "video/3gpp";
		} else if (ext.equals("wav")) {
			ct = "audio/x-wav";
		} else if (ext.equals("gif")) {
			ct = "image/gif";
		} else if (ext.equals("png")) {
			ct = "image/png";
		}

		return ct;
    }

    public static String findFileExtension(String contentType) {
		String ext = "";
		System.out.println("contentType=" + contentType);
		if (contentType.equals("video/mpeg") || contentType.equals("video/vnd.sun.rgb565")) {
			ext = "mpg";
		} else if (contentType.equals("video/3gpp")) {
			ext = "3gp";
		} else if (contentType.equals("audio/x-wav")) {
			ext = "wav";
		} else if (contentType.equals("image/gif")) {
			ext = "gif";
		} else if (contentType.equals("image/png")) {
			ext = "png";
		}
		return ext;

	}

}