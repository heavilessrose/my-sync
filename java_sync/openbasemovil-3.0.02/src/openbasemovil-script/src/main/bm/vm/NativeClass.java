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

import java.util.Hashtable;
import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 23-oct-2007 18:40:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Common utilities for Native Classes.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class NativeClass
        extends ScriptingClass
        implements Method
{
    protected static Hashtable lookup = new Hashtable( );

    protected static void addMethod(
            final String    name,
            final int       code,
            final int       argCount
    )
    {
        lookup.put( name, new int[] { code, argCount } );
    }

    protected static int checkMethod( final String name, final Instance[] args )
            throws VirtualMachineException
    {
        final int[] method = (int[]) lookup.get( name );
        if( method != null )
        {
            final int length = args != null ? args.length : 0;
            if( length < method[1] )
            {
                throw new VirtualMachineException(
                        0,
                        "Method " + name + " needs " + method[1] + " parameters"
                );
            }
            else
            {
                return method[0];
            }
        }
        else
        {
            return -1;
        }
    }

    /** @noinspection FieldCanBeLocal*/
    protected String methodName;

    /**
     * Constructor.
     *
     * @param vm virtual machine
     */
    public NativeClass( final VirtualMachine vm )
    {
        super( vm );

        methods = new Hashtable( lookup.size() );
        for( Enumeration i = lookup.keys(); i.hasMoreElements(); )
        {
            methods.put( i.nextElement(), this );
        }
    }

    /**
     * Get the class to which this method belongs.
     *
     * @return scripting class
     */
    public ScriptingClass getClazz()
    {
        return this;
    }

    /**
     * Set the class to which this method belongs.
     *
     * @param clazz scripting class
     */
    public void setClazz( ScriptingClass clazz )
    {
    }

    /**
     * Set the method name.
     *
     * @param name name
     */
    public void setMethodName( String name )
    {
        this.methodName = name;
    }

    /**
     * Get the method name.
     *
     * @return method name
     */
    public String getMethodName()
    {
        return methodName;
    }
}
