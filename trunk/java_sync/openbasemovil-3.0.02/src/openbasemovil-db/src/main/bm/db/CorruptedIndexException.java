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

/*
 * File Information
 *
 * Created on       : 12-abr-2006 12:02:58
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/**
 * A corrupted index is detected during update of a table.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
class CorruptedIndexException
        extends DBException
{
    public CorruptedIndexException( final int errorNumber )
    {
        super( errorNumber );
    }

    public CorruptedIndexException(
            final String message,
            final int errorNumber
    )
    {
        super( errorNumber, message );
    }

    public CorruptedIndexException(
            final Throwable cause,
            final int errorNumber
    )
    {
        super( errorNumber, cause );
    }

    public CorruptedIndexException( final String message,
                                    final Throwable cause,
                                    final int errorNumber
    )
    {
        super( errorNumber, message, cause );
    }
}
