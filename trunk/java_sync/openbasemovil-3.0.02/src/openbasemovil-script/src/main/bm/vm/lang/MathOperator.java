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
import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.vm.Context;

import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 10-oct-2007 16:03:58
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Performs math operations.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class MathOperator
        extends BinaryOperator
{
    private static final Hashtable precission = new Hashtable( 5 );
    private static final Hashtable numericClasses = new Hashtable( 5 );
    static
    {
        numericClasses.put( "byte", new Byte( (byte) 1 ) );
        numericClasses.put( "short", new Byte( (byte) 2 ) );
        numericClasses.put( "int", new Byte( (byte) 3 ) );
        numericClasses.put( "long", new Byte( (byte) 4 ) );
        numericClasses.put( "double", new Byte( (byte) 5 ) );
        precission.put( new Byte( (byte) 0 ).getClass(), new Byte( (byte) 1 ) );
        precission.put( new Short( (short) 0 ).getClass(), new Byte( (byte) 2 ) );
        precission.put( new Integer( 0 ).getClass(), new Byte( (byte) 3 ) );
        precission.put( new Long( 0 ).getClass(), new Byte( (byte) 4 ) );
        precission.put( FixedPoint.ONE.getClass(), new Byte( (byte) 5 ) );
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.MathOperator";
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination command or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( operand1 == null || operand2 == null )
        {
            throw new IllegalArgumentException( "Operation needs exactly two arguments" );
        }
        else
        {
            final Context context = getContext();
            operand1.setContext( context );
            operand2.setContext( context );
            Instance value1 = operand1.run();
            Instance value2 = operand2.run();

            Object val1 = parseValue( value1 );
            Object val2 = parseValue( value2 );

            final int precission1 = ((Byte)
                    precission.get( val1.getClass() )).byteValue();
            final int precission2 = ((Byte)
                    precission.get( val2.getClass() )).byteValue();

            if( precission1 > precission2 )
            {
                val2 = raisePrecission( val2, val1);
            }
            else if( precission1 < precission2 )
            {
                val1 = raisePrecission( val1, val2 );
            }
            return doMath( val1, val2 );
        }
    }

    protected abstract Instance doMath( final Object val1, final Object val2 )
            throws VirtualMachineException;

    private Object raisePrecission( final Object value, final Object target)
    {
        if( target instanceof FixedPoint )
        {
            return Conversor.toDouble( value );
        }
        else if( target instanceof Long )
        {
            return Conversor.toLong( value );
        }
        else if( target instanceof Integer )
        {
            return Conversor.toInteger( value );
        }
        else
        {
            return Conversor.toShort( value );
        }
    }

    private Object parseValue( final Instance value1 )
    {
        Object val1 = value1.get( "value" );
        if( val1 == null )
        {
            val1 = new Integer( 0 );
        }
        final String className = value1.getScriptingClass().getName();
        final String alias =
                getVirtualMachine().getClassLoader().getAlias( className );
        if( !numericClasses.containsKey( className ) &&
            ( alias != null || !numericClasses.containsKey( alias ) )
        )
        {
            String s1 = val1.toString();
            try
            {
                if( s1.indexOf( '.' ) > -1 )
                {
                    val1 = FixedPoint.parse( s1, '.' );
                }
                else
                {
                    val1 = Integer.valueOf( s1 );
                }
            }
            catch( NumberFormatException e )
            {
                val1 = new Integer( 0 );
            }
        }
        return val1;
    }
}
