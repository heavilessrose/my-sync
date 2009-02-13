package bm.core.fs;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
public class FSException
    extends J2MEException
{
    private static final String FS_ERROR = "FS_ERROR";

    public FSException( final int errorNumber )
    {
        super( errorNumber );
        errorCode = FS_ERROR;
    }

    public FSException( final int errorNumber, final String string )
    {
        super( errorNumber, string );
        errorCode = FS_ERROR;
    }

    public FSException( final int errorNumber, final Throwable source )
    {
        super( errorNumber, source );
        errorCode = FS_ERROR;
    }

    public FSException(
            final int errorNumber, final Throwable source, final String message
    )
    {
        super( errorNumber, source, message );
        errorCode = FS_ERROR;
    }
}
