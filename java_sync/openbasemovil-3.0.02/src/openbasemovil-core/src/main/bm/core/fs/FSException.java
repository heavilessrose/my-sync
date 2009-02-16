package bm.core.fs;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 11-dic-2006 11:29:28
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * File System Exception.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class FSException extends J2MEException {
	private static final String FS_ERROR = "FS_ERROR";

	public FSException(final int errorNumber) {
		super(errorNumber);
		errorCode = FS_ERROR;
	}

	public FSException(final int errorNumber, final String string) {
		super(errorNumber, string);
		errorCode = FS_ERROR;
	}

	public FSException(final int errorNumber, final Throwable source) {
		super(errorNumber, source);
		errorCode = FS_ERROR;
	}

	public FSException(final int errorNumber, final Throwable source,
			final String message) {
		super(errorNumber, source, message);
		errorCode = FS_ERROR;
	}
}
