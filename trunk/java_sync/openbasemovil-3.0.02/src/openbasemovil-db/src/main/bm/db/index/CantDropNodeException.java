package bm.db.index;

import bm.db.DBException;

/*
 * File Information
 *
 * Created on       : 25-sep-2005 18:38:36
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/**
 * The index node can't be dropped.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
public class CantDropNodeException extends DBException {
	private static final String ERR_CODE = "ERR_IDX_CANT_DROP_NODE";

	public CantDropNodeException(final int errorNumber) {
		super(errorNumber);
		errorCode = ERR_CODE;
	}

	public CantDropNodeException(final int errorNumber, final String message) {
		super(errorNumber, message);
		errorCode = ERR_CODE;
	}

	public CantDropNodeException(final int errorNumber, final Throwable cause) {
		super(errorNumber, cause);
		errorCode = ERR_CODE;
	}

	public CantDropNodeException(final int errorNumber, final String message,
			final Throwable cause) {
		super(errorNumber, message, cause);
		errorCode = ERR_CODE;
	}
}
