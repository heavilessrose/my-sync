package bm.net;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 26-nov-2006 2:17:39
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * Generic Network Exception.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class NetException extends J2MEException {
	public NetException(final int errorNumber) {
		super(errorNumber);
	}

	public NetException(final int errorNumber, final String string) {
		super(errorNumber, string);
	}

	public NetException(final int errorNumber, final Throwable source) {
		super(errorNumber, source);
	}

	public NetException(final int errorNumber, final Throwable source,
			final String message) {
		super(errorNumber, source, message);
	}
}
