package bm.storage;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 02-may-2007 17:20:46
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Elimates the dependency on the javax.microedition.rms package.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class InvalidRecordIDException extends J2MEException {
	public InvalidRecordIDException(final int errorNumber) {
		super(errorNumber);
	}

	public InvalidRecordIDException(final int errorNumber, final String string) {
		super(errorNumber, string);
	}

	public InvalidRecordIDException(final int errorNumber,
			final Throwable source) {
		super(errorNumber, source);
	}

	public InvalidRecordIDException(final int errorNumber,
			final Throwable source, final String message) {
		super(errorNumber, source, message);
	}
}
