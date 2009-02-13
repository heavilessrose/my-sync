package bm.net;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Network Library, provides an abstraction layer for simplified
    network access over different channels.
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
public class NetException
    extends J2MEException
{
    public NetException( final int errorNumber )
    {
        super( errorNumber );
    }

    public NetException( final int errorNumber, final String string )
    {
        super( errorNumber, string );
    }

    public NetException( final int errorNumber, final Throwable source )
    {
        super( errorNumber, source );
    }

    public NetException(
            final int errorNumber, final Throwable source, final String message
    )
    {
        super( errorNumber, source, message );
    }
}
