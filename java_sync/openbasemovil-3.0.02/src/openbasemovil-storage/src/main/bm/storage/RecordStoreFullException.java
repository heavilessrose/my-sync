package bm.storage;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 02-may-2007 17:22:37
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
public class RecordStoreFullException extends J2MEException {
	public RecordStoreFullException(final int errorNumber) {
		super(errorNumber);
	}

	public RecordStoreFullException(final int errorNumber, final String string) {
		super(errorNumber, string);
	}

	public RecordStoreFullException(final int errorNumber,
			final Throwable source) {
		super(errorNumber, source);
	}

	public RecordStoreFullException(final int errorNumber,
			final Throwable source, final String message) {
		super(errorNumber, source, message);
	}
}
