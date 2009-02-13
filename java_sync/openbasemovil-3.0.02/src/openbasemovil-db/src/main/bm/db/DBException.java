/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.J2MEException;

/*
 * File Information
 *
 * Created on       : 21-jun-2005 11:58:30
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/**
 * Database exception.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
public class DBException
        extends J2MEException
{
    private static final String ERR_CODE = "ERR_DATABASE";

    public DBException( final int errorNumber )
    {
        super( errorNumber );
        errorCode = ERR_CODE;
    }

    public DBException( final int errorNumber, final String message)
    {
        super(errorNumber, message);
        errorCode = ERR_CODE;
    }

    public DBException( final int errorNumber, final Throwable cause)
    {
        super(errorNumber, cause);
        errorCode = ERR_CODE;
    }

    public DBException(
            final int       errorNumber,
            final String    message,
            final Throwable cause
    )
    {
        super(errorNumber, cause, message);
        errorCode = ERR_CODE;
    }
}
