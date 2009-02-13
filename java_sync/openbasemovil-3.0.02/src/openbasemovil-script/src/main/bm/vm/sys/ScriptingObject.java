package bm.vm.sys;

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

import bm.vm.Instance;
import bm.vm.NativeClass;
import bm.vm.VirtualMachine;
import bm.vm.VirtualMachineException;
import bm.vm.lang.Conversor;/*
 * File Information
 *
 * Created on       : 30-oct-2007 23:25:05
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The base class for any scripting object.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingObject
    extends NativeClass
{
    private static final int    TO_STRING   = 0;
    private static final int    EQUALS      = 1;
    private static final int    HASH_CODE   = 2;
    private static final int    COMPARE     = 3;

    static
    {
        addMethod( "toString", TO_STRING, 0 );
        addMethod( "equals", EQUALS, 1 );
        addMethod( "hashCode", HASH_CODE, 0 );
        addMethod( "compare", COMPARE, 1 );
    }

    /**
     * Constructor.
     *
     * @param vm virtual machine
     */
    public ScriptingObject( final VirtualMachine vm )
    {
        super( vm );
        name = "object";
    }

    /**
     * Invoke the method.
     *
     * @param target target instance on which to run
     * @param args   method arguments
     * @return return value
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance invoke(
            final Instance      target,
            final Instance[]    args
    )
            throws VirtualMachineException
    {
        final int id = checkMethod( methodName, args );
        if( id != -1 )
        {
            switch( id )
            {
                case TO_STRING:
                    return Conversor.toInstance( vm, target.toString() );

                case EQUALS:
                    return Conversor.toInstance(
                            vm,
                            new Boolean( target == args[0] )
                    );

                case HASH_CODE:
                    return Conversor.toInstance(
                            vm,
                            new Integer( System.identityHashCode( target ) )
                    );

                case COMPARE:
                    return Conversor.toInstance(
                            vm,
                            new Integer( target == args[0] ? 0 : -1 )
                    );

                default:
                    return null;
            }
        }
        else
        {
            return null;
        }
    }
}
