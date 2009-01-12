/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.midlet;

import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import com.astrientlabs.jobs.StartupJob;
import com.astrientlabs.prefs.PropertyMap;
import com.astrientlabs.threads.JobRunner;
import com.astrientlabs.ui.SplashCanvas;

public class Yipi extends MIDlet {
	public static Yipi instance;

	public Display display;
	public PropertyMap systemPrefs = new PropertyMap("system");

	public Yipi() {
		instance = this;
	}

	protected void startApp() throws MIDletStateChangeException {
		if (display == null) {
			display = Display.getDisplay(this);
			SplashCanvas splashScreen = new SplashCanvas();
			display.setCurrent(splashScreen);

			JobRunner.instance.run(new StartupJob(splashScreen));
		}
	}

	protected void pauseApp() {
	}

	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
		exit();
	}

	public void exit() {
		try {
			systemPrefs.write();
		} catch (Exception e) {

		}
		notifyDestroyed();
	}
}