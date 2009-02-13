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

import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.vm.Context;
import bm.core.math.FixedPoint;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:45:24
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Adds two numbers.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Plus
    extends MathOperator
{
    public String getName()
    {
        return CommandFactory.PLUS;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Plus";
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

            final Object o1 = value1.get( "value" );
            final Object o2 = value2.get( "value" );
            if( ( o1 instanceof String) || ( o2 instanceof String) )
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "String" );
                instance.set( "value", o1.toString() + o2.toString() );
                return instance;
            }
            else
            {
                return super.run();
            }
        }
    }

    protected Instance doMath( final Object val1, final Object val2 )
            throws VirtualMachineException
    {
        if( val1 instanceof FixedPoint )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "double" );
            instance.set(
                    "value",
                    ((FixedPoint) val1).add( (FixedPoint) val2 )
            );
            return instance;
        }
        else if( val1 instanceof Long )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "long" );
            instance.set(
                    "value",
                    new Long(
                            ((Long) val1).longValue() +
                            ( (Long) val2 ).longValue()
                    )
            );
            return instance;
        }
        else if( val1 instanceof Integer )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "int" );
            instance.set(
                    "value",
                    new Integer(
                            ((Integer) val1).intValue() +
                            ( (Integer) val2 ).intValue()
                    )
            );
            return instance;
        }
        else if( val1 instanceof Short )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "short" );
            instance.set(
                    "value",
                    new Short( (short)
                            (((Short) val1).shortValue() +
                             ( (Short) val2 ).shortValue() )
                    )
            );
            return instance;
        }
        else
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "byte" );
            instance.set(
                    "value",
                    new Byte( (byte)
                            (((Byte) val1).byteValue() +
                             ( (Byte) val2 ).byteValue() )
                    )
            );
            return instance;
        }
    }
}
