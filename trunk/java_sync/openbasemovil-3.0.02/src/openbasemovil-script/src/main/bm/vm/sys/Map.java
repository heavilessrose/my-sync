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
import bm.core.CoreConstants;

import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 11-oct-2007 18:29:49
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A map.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Map
    extends NativeClass
{
    private static final int SET            = 0;
    private static final int CLEAR          = 1;
    private static final int CONTAINS       = 2;
    private static final int CONTAINS_KEY   = 3;
    private static final int GET            = 4;
    private static final int REMOVE         = 5;
    private static final int SIZE           = 6;
    private static final int TO_STRING      = 7;
    private static final int EQUALS         = 8;
    private static final int HASHCODE       = 9;

    static
    {
        addMethod( "set", SET, 2 );
        addMethod( "clear", CLEAR, 0 );
        addMethod( "contains", CONTAINS, 1 );
        addMethod( "containsKey", CONTAINS_KEY, 1 );
        addMethod( "get", GET, 1 );
        addMethod( "remove", REMOVE, 1 );
        addMethod( "size", SIZE, 0 );
        addMethod( "toString", TO_STRING, 0 );
        addMethod( "equals", EQUALS, 1 );
        addMethod( "hashCode", HASHCODE, 0 );
    }

    public Map( final VirtualMachine vm )
    {
        super( vm );
        name = "map";
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
            Instance target, Instance[] args
    )
            throws VirtualMachineException
    {
        final int id = checkMethod( methodName, args );
        if( id != -1 )
        {
            Hashtable map = (Hashtable)
                    target.getAttachment( "map" );
            if( map == null )
            {
                map = new Hashtable( 10 );
                target.setAttachment( "map", map );
            }
            switch( id )
            {
                case SET:
                    map.put( args[0], args[1] );
                    return null;

                case CLEAR:
                    map.clear();
                    return null;

                case CONTAINS:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            map.contains( args[0] ) ?
                                CoreConstants.TRUE :
                                CoreConstants.FALSE
                    );

                case CONTAINS_KEY:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            map.containsKey( args[0] ) ?
                                CoreConstants.TRUE :
                                CoreConstants.FALSE
                    );

                case GET:
                    return (Instance) map.get( args[0] );

                case REMOVE:
                    return (Instance) map.remove( args[0] );

                case SIZE:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            new Integer( map.size() )
                    );

                case TO_STRING:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            map.toString()
                    );

                case EQUALS:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            map.equals( args[0].getAttachment( "map" ) ) ?
                                CoreConstants.TRUE :
                                CoreConstants.FALSE
                    );

                case HASHCODE:
                    return Conversor.toInstance(
                            getVirtualMachine(),
                            new Integer( map.hashCode() )
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
