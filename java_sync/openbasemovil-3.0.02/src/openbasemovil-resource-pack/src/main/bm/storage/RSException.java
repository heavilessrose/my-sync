package bm.storage;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ResourcePacker, packs and checks resource files
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
 * Created on       : 07-dic-2006 23:35:45
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Database exception.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 10 $
 */
public class RSException
        extends J2MEException
{
    private static final String ERR_CODE = "ERR_RECORDSTORE";

    public RSException( final int errorNumber )
    {
        super( errorNumber );
        errorCode = RSException.ERR_CODE;
    }

    public RSException( final int errorNumber, final String message)
    {
        super(errorNumber, message);
        errorCode = RSException.ERR_CODE;
    }

    public RSException( final int errorNumber, final Throwable cause)
    {
        super(errorNumber, cause);
        errorCode = RSException.ERR_CODE;
    }

    public RSException(
            final int       errorNumber,
            final String    message,
            final Throwable cause
    )
    {
        super(errorNumber, cause, message);
        errorCode = RSException.ERR_CODE;
    }
}
