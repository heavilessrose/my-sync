package com.sun.j2me.blueprints.smartticket.client.midp;

import javax.microedition.midlet.*;
import com.sun.j2me.blueprints.smartticket.client.midp.model.ModelFacade;
import com.sun.j2me.blueprints.smartticket.client.midp.ui.UIController;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class SmartTicketMIDlet extends MIDlet {

	public static final String PROPERTY_SERVICE_URL = "SmartTicket-Servlet-URL";
	public static final String PROPERTY_LOCALE = "SmartTicket-Locale";
	private UIController controller;
	private ModelFacade model;

	protected void startApp() {
		try {
			model = new ModelFacade(getAppProperty(PROPERTY_SERVICE_URL),
					getAppProperty(PROPERTY_LOCALE));
			controller = new UIController(this, model);

			controller.init();
		} catch (ApplicationException ae) {
			System.err.println(ae.getException() + "/" + ae.getMessage() + "/" + ae.getCode());
		}
	}

	protected void destroyApp(boolean unconditional) {
		try {
			System.out.println(model);
			model.destroy();
		} catch (ApplicationException ae) {
			ae.printStackTrace();
			System.err.println(ae.getException() + "/" + ae.getMessage() + "/" + ae.getCode());
		}
	}

	protected void pauseApp() {
	}

}
