package bm.core.io;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 21-jun-2005 12:02:39
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Persistence Exception.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class SerializationException extends J2MEException {
	private static final String ERR_CODE = "ERR_SERIALIZATION";

	public SerializationException(final int errorNumber) {
		super(errorNumber);
		errorCode = ERR_CODE;
	}

	public SerializationException(final int errorNumber, final String message) {
		super(errorNumber, message);
		errorCode = ERR_CODE;
	}

	public SerializationException(final int errorNumber, final Throwable cause) {
		super(errorNumber, cause);
		errorCode = ERR_CODE;
	}

	public SerializationException(final int errorNumber, final String message,
			final Throwable cause) {
		super(errorNumber, cause, message);
		errorCode = ERR_CODE;
	}
}
