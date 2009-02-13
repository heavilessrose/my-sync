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
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:45:42
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Divides two numeric expressions.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Div
    extends MathOperator
{

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Div";
    }

    public String getName()
    {
        return CommandFactory.DIV;
    }

    protected Instance doMath( final Object val1, final Object val2 )
            throws VirtualMachineException
    {
        if( val1 instanceof FixedPoint )
        {
            if( val2.equals( FixedPoint.ZERO ) )
            {
                throw new VirtualMachineException( 0, "Division by zero" );
            }
            else
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "double" );
                instance.set(
                        "value",
                        ((FixedPoint) val1).div( (FixedPoint) val2 )
                );
                return instance;
            }
        }
        else if( val1 instanceof Long )
        {
            if( ((Long) val2).longValue() == 0 )
            {
                throw new VirtualMachineException( 0, "Division by zero" );
            }
            else
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "long" );
                instance.set(
                        "value",
                        new Long(
                                ((Long) val1).longValue() /
                                ( (Long) val2 ).longValue()
                        )
                );
                return instance;
            }
        }
        else if( val1 instanceof Integer )
        {
            if( ((Integer) val2).intValue() == 0 )
            {
                throw new VirtualMachineException( 0, "Division by zero" );
            }
            else
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "int" );
                instance.set(
                        "value",
                        new Integer(
                                ((Integer) val1).intValue() /
                                ( (Integer) val2 ).intValue()
                        )
                );
                return instance;
            }
        }
        else if( val1 instanceof Short )
        {
            if( ((Short) val2).shortValue() == 0 )
            {
                throw new VirtualMachineException( 0, "Division by zero" );
            }
            else
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "short" );
                instance.set(
                        "value",
                        new Short( (short)
                                (((Short) val1).shortValue() /
                                 ( (Short) val2 ).shortValue() )
                        )
                );
                return instance;
            }
        }
        else
        {
            if( ((Byte) val2).byteValue() == 0 )
            {
                throw new VirtualMachineException( 0, "Division by zero" );
            }
            else
            {
                final Instance instance =
                        getVirtualMachine().newInstance( "byte" );
                instance.set(
                        "value",
                        new Byte( (byte)
                                (((Byte) val1).byteValue() /
                                 ( (Byte) val2 ).byteValue() )
                        )
                );
                return instance;
            }
        }
    }
}
