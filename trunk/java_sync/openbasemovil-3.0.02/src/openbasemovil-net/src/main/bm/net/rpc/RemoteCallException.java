package bm.net.rpc;

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 26-nov-2006 13:50:31
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * Error performing remote call.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class RemoteCallException extends J2MEException {
	public RemoteCallException(final int errorNumber) {
		super(errorNumber);
	}

	public RemoteCallException(final int errorNumber, final String string) {
		super(errorNumber, string);
	}

	public RemoteCallException(final int errorNumber, final Throwable source) {
		super(errorNumber, source);
	}

	public RemoteCallException(final int errorNumber, final Throwable source,
			final String message) {
		super(errorNumber, source, message);
	}
}
