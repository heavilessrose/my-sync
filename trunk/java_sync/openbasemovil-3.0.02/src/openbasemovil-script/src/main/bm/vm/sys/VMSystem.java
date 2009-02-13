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

import bm.vm.*;
import bm.vm.lang.Conversor;
import bm.core.Application;

/*
 * File Information
 *
 * Created on       : 20-oct-2007 8:56:24
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Builtin class to access system information and resources.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class VMSystem
    extends NativeClass
{
    private static final int PRINT      = 0;
    private static final int PRINTLN    = 1;
    private static final int CURTIME    = 2;
//    private static final int GC         = 3;
    private static final int GET_PROP   = 4;

    static
    {
        addMethod( "print", PRINT, 1 );
        addMethod( "println", PRINTLN, 1 );
        addMethod( "currentTimeMillis", CURTIME, 0 );
//        addMethod( "gc", GC, 0 );
        addMethod( "getProperty", GET_PROP, 1 );
    }

    public VMSystem( final VirtualMachine vm )
    {
        super( vm );
        name = "system";
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
            Instance    target,
            Instance[]  args
    )
            throws VirtualMachineException
    {
        final int id = checkMethod( methodName, args );
        if( id != -1 )
        {
            switch( id )
            {
                case PRINT:
                    if( args != null && args.length > 0 )
                    {
                        System.out.print( Conversor.toString( args[0] ) );
                    }
                    return null;

                case PRINTLN:
                    if( args == null || args.length == 0 )
                    {
                        System.out.println();
                    }
                    else
                    {
                        System.out.println( Conversor.toString( args[0] ) );
                    }
                    return null;

                case CURTIME:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            new Long( System.currentTimeMillis() )
                    );

//                case GC:
//                    System.gc();
//                    return null;

                case GET_PROP:
                    if( args != null && args.length > 0 )
                    {
                        return Conversor.toInstance(
                                getVirtualMachine(),
                                Application.getManager().getProperty(
                                        Conversor.toString( args[0] )
                                )
                        );
                    }
                    else
                    {
                        return null;
                    }

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
