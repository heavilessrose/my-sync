package passkeep.control;

import bm.err.ErrorLog;
import bm.mvc.ViewFactory;

/*
 * File Information
 *
 * Created on       : 27-abr-2007 0:09:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Custom ErrorHandler.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ErrorHandler extends bm.core.ErrorHandler {
	/**
	 * Do the actual error handling. Subclasses should override this method.
	 * 
	 * @param source
	 *            error source
	 * @param e
	 *            error
	 */
	protected void handle(final Object source, final Exception e) {
		ErrorLog.addError("ErrorHandler", "handle", new Object[] { source },
				null, e);
		((MainController) ViewFactory.getController("main")).handleError(
				source, e, null);
	}

	/**
	 * Do the actual error handling. Subclasses should override this method.
	 * 
	 * @param source
	 *            error source
	 * @param message
	 *            additional message
	 * @param e
	 *            error
	 */
	protected void handle(final Object source, final String message,
			final Exception e) {
		ErrorLog.addError("ErrorHandler", "handle", new Object[] { source },
				message, e);
		((MainController) ViewFactory.getController("main")).handleError(
				source, e, message);
	}
}
