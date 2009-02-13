package bm.vm.lang;

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

import bm.core.math.FixedPoint;
import bm.vm.Context;
import bm.vm.Instance;
import bm.vm.VirtualMachine;
import bm.vm.VirtualMachineException;

import java.util.Enumeration;
import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 18:23:57
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Convert values from one type to others.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Conversor
{
    public static FixedPoint toDouble( final Object o )
    {
        if( o instanceof FixedPoint )
        {
            return (FixedPoint) o;
        }
        else if( o instanceof Long )
        {
            return new FixedPoint( ((Long) o).longValue() );
        }
        else if( o instanceof Integer )
        {
            return new FixedPoint( ((Integer)o).intValue() );
        }
        else if( o instanceof Short )
        {
            return new FixedPoint( ((Short)o).shortValue() );
        }
        else if( o instanceof Byte )
        {
            return new FixedPoint( ((Byte)o).byteValue() );
        }
        else if( o instanceof Boolean )
        {
            return ((Boolean)o).booleanValue() ? FixedPoint.ONE : FixedPoint.ZERO;
        }
        else
        {
            try
            {
                return FixedPoint.parse( o.toString(), '.' );
            }
            catch( NumberFormatException e )
            {
                return FixedPoint.ZERO;
            }
        }
    }

    public static Long toLong( final Object o )
    {
        if( o instanceof Long )
        {
            return (Long) o;
        }
        else if( o instanceof FixedPoint )
        {
            return new Long( ((FixedPoint) o).trunc() );
        }
        else if( o instanceof Integer )
        {
            return new Long( ((Integer)o).intValue() );
        }
        else if( o instanceof Short )
        {
            return new Long( ((Short)o).shortValue() );
        }
        else if( o instanceof Byte )
        {
            return new Long( ((Byte)o).byteValue() );
        }
        else if( o instanceof Boolean )
        {
            return new Long( ((Boolean)o).booleanValue() ? 1 : 0 );
        }
        else
        {
            try
            {
                return new Long( Long.parseLong( o.toString() ) );
            }
            catch( NumberFormatException e )
            {
                return new Long( 0 );
            }
        }
    }

    public static Integer toInteger( final Instance o )
    {
        return toInteger( o, 0 );
    }

    public static Integer toInteger( final Instance o, final int defVal )
    {
        return toInteger( o.get( "value" ), defVal );
    }

    public static Integer toInteger( final Object o )
    {
        return toInteger( o, 0 );
    }

    public static Integer toInteger( final Object o, final int defVal )
    {
        if( o == null )
        {
            return new Integer( defVal );
        }
        else if( o instanceof Integer )
        {
            return (Integer) o;
        }
        else if( o instanceof FixedPoint )
        {
            return new Integer( (int) ((FixedPoint) o).trunc() );
        }
        else if( o instanceof Long )
        {
            return new Integer( (int) ((Long)o).longValue() );
        }
        else if( o instanceof Short )
        {
            return new Integer( ((Short)o).shortValue() );
        }
        else if( o instanceof Byte )
        {
            return new Integer( ((Byte)o).byteValue() );
        }
        else if( o instanceof Boolean )
        {
            return new Integer( ((Boolean)o).booleanValue() ? 1 : 0 );
        }
        else
        {
            try
            {
                return new Integer( Integer.parseInt( o.toString() ) );
            }
            catch( NumberFormatException e )
            {
                return new Integer( defVal );
            }
        }
    }

    public static Short toShort( final Object o )
    {
        if( o instanceof Short )
        {
            return (Short) o;
        }
        else if( o instanceof FixedPoint )
        {
            return new Short( (short) ((FixedPoint) o).trunc() );
        }
        else if( o instanceof Long )
        {
            return new Short( (short) ((Long)o).longValue() );
        }
        else if( o instanceof Integer )
        {
            return new Short( (short) ((Integer)o).intValue() );
        }
        else if( o instanceof Byte )
        {
            return new Short( ((Byte)o).byteValue() );
        }
        else if( o instanceof Boolean )
        {
            return new Short( (short) (((Boolean)o).booleanValue() ? 1 : 0) );
        }
        else
        {
            try
            {
                return new Short( Short.parseShort( o.toString() ) );
            }
            catch( NumberFormatException e )
            {
                return new Short( (short) 0 );
            }
        }
    }

    public static boolean toBoolean( final Instance instance )
    {
        if( instance == null || !instance.getScriptingClass().isSystemClass() )
        {
            return false;
        }
        else
        {
            final Object o = instance.get( "value" );
            if( o instanceof Boolean )
            {
                return ((Boolean)o).booleanValue();
            }
            else if( o instanceof Integer )
            {
                return ((Integer)o).intValue() != 0;
            }
            else if( o instanceof Long )
            {
                return ((Long)o).longValue() != 0;
            }
            else if( o instanceof Short )
            {
                return ((Short)o).shortValue() != 0;
            }
            else if( o instanceof Byte )
            {
                return ((Byte)o).byteValue() != 0;
            }
            else if( o instanceof FixedPoint )
            {
                return !o.equals( FixedPoint.ZERO );
            }
            else
            {
                return o.toString().toLowerCase().equals( "true" );
            }
        }
    }

    public static String toString( final Instance instance )
            throws VirtualMachineException
    {
        if( instance == null )
        {
            return null;
        }
        else if( instance.getScriptingClass().isSystemClass() )
        {
            final Object o = instance.get( "value" );
            return o != null ? o.toString() : null;
        }
        else if( instance.getScriptingClass().getMethod( "toString" ) != null )
        {
            return toString( instance.invoke( "toString", null ) );
        }
        else
        {
            return null;
        }
    }

    public static Instance toInstance( final Context context, final Object o )
            throws VirtualMachineException
    {
        return toInstance( context.getVirtualMachine(), o );
    }

    public static Instance toInstance( final VirtualMachine vm, final Object o )
            throws VirtualMachineException
    {
        Instance instance;
        if( o == null )
        {
            return null;
        }
        else if( o instanceof Instance )
        {
            return (Instance) o;
        }
        else if( o instanceof String )
        {
            instance = vm.newInstance( "String" );
        }
        else if( o instanceof Integer )
        {
            instance = vm.newInstance( "int" );
        }
        else if( o instanceof Long )
        {
            instance = vm.newInstance( "long" );
        }
        else if( o instanceof Short )
        {
            instance = vm.newInstance( "short" );
        }
        else if( o instanceof Byte )
        {
            instance = vm.newInstance( "byte" );
        }
        else if( o instanceof Boolean )
        {
            instance = vm.newInstance( "boolean" );
        }
        else
        {
            throw new VirtualMachineException(
                    0,
                    "Type not supported: " + o.getClass()
            );
        }
        instance.set( "value", o );
        return instance;
    }

    public static String[] mapToStringArray( final Instance map )
    {
        Hashtable m = (Hashtable) map.getAttachment( "map" );
        int max = -1;
        for( Enumeration i = m.keys(); i.hasMoreElements(); )
        {
            final Instance key = (Instance) i.nextElement();
            final Integer value = toInteger( key, -1 );
            if( value.intValue() > max )
            {
                max = value.intValue();
            }
        }
        if( max > -1 )
        {
            final String[] retVal = new String[max + 1];
            for( Enumeration i = m.keys(); i.hasMoreElements(); )
            {
                final Instance key = (Instance) i.nextElement();
                final Integer value = toInteger( key, -1 );
                if( value.intValue() > -1 )
                {
                    retVal[ value.intValue() ] = m.get( key ).toString();
                }
            }
            return retVal;
        }
        else
        {
            return null;
        }
    }
}
