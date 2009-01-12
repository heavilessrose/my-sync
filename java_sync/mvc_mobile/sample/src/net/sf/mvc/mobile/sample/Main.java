package net.sf.mvc.mobile.sample;

import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class Main extends MIDlet {

	protected void startApp() throws MIDletStateChangeException {
		new SampleControler(this).start();
	}

	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
	}

	protected void pauseApp() {
	}
}
