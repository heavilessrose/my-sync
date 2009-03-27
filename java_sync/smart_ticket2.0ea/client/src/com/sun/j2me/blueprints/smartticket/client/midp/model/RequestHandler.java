package com.sun.j2me.blueprints.smartticket.client.midp.model;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public interface RequestHandler {

	RequestHandler getNextHandler();

	void init() throws ApplicationException;

	void destroy() throws ApplicationException;
}
