package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import javax.microedition.midlet.MIDlet;


public class MobileVideoApp extends MIDlet {

	private Display display;
	private PropertyForm form;
	private String serverAddress;
	private boolean offline = false;

	public MobileVideoApp() {

		form = new PropertyForm("Mobile Video App",this);
	}

	public void startApp() {
		display = Display.getDisplay(this);
		display.setCurrent(form);
	}

	public void pauseApp() {
	}

	public void destroyApp(boolean unconditional) {

	}
	public Display getDisplay() {
		return display;
	}

	public void setServerAddress(String serverAddress_) {
		this.serverAddress = serverAddress_;
	}

	public String getServerAddress() {
		return this.serverAddress;
	}

	public boolean isOffline() {
		return offline;
	}

	public void setOffline(boolean offline_) {
		this.offline = offline_;
	}

}