package bm.vm;

/* -----------------------------------------------------------------------------
    bmScript Scripting language for Mobile Devices
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
 * Created on       : 07-oct-2007 1:44:10
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Exception thrown by a method.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class InvocationException
    extends VirtualMachineException
{
    public InvocationException( final int errorNumber )
    {
        super( errorNumber );
    }

    public InvocationException(
            final int errorNumber, final String string
    )
    {
        super( errorNumber, string );
    }

    public InvocationException(
            final int errorNumber, final Throwable source
    )
    {
        super( errorNumber, source );
    }

    public InvocationException(
            final int errorNumber, final Throwable source, final String message
    )
    {
        super( errorNumber, source, message );
    }
}
