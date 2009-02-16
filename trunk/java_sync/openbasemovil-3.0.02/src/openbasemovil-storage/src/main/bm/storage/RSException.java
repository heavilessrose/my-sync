package bm.storage;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 07-dic-2006 23:35:45
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Store exception.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 10 $
 */
public class RSException extends J2MEException {
	private static final String ERR_CODE = "ERR_RECORDSTORE";

	public RSException(final int errorNumber) {
		super(errorNumber);
		errorCode = RSException.ERR_CODE;
	}

	public RSException(final int errorNumber, final String message) {
		super(errorNumber, message);
		errorCode = RSException.ERR_CODE;
	}

	public RSException(final int errorNumber, final Throwable cause) {
		super(errorNumber, cause);
		errorCode = RSException.ERR_CODE;
	}

	public RSException(final int errorNumber, final String message,
			final Throwable cause) {
		super(errorNumber, cause, message);
		errorCode = RSException.ERR_CODE;
	}
}
