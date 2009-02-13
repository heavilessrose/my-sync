package bm.vm;

import bm.core.J2MEException;

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
 * Created on       : 07-oct-2007 1:43:16
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Base exception for virtual machine.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class VirtualMachineException
    extends J2MEException
{
    public VirtualMachineException( final int errorNumber )
    {
        super( errorNumber );
    }

    public VirtualMachineException(
            final int       errorNumber,
            final String    string
    )
    {
        super( errorNumber, string );
    }

    public VirtualMachineException(
            final int       errorNumber,
            final Throwable source
    )
    {
        super( errorNumber, source );
    }

    public VirtualMachineException(
            final int       errorNumber,
            final Throwable source,
            final String    message
    )
    {
        super( errorNumber, source, message );
    }
}
