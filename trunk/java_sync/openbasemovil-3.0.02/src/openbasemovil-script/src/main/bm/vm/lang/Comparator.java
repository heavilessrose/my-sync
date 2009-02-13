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

import bm.vm.*;
import bm.core.CoreConstants;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.math.FixedPoint;
/*
 * File Information
 *
 * Created on       : 20-oct-2007 3:02:44
 * Created by       : narciso
 * Last modified by : \$Author$
 * Last modified on : \$Date$
 * Revision         : \$Revision$
 */

/**
 * Performs a logical and of two expressions.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Comparator
        extends BinaryOperator
{
    public static final byte LT          = 0;
    public static final byte LTE         = 1;
    public static final byte GT          = 2;
    public static final byte GTE         = 3;
    public static final byte EQUAL       = 4;
    public static final byte DIFFERENT   = 5;

    protected byte mode;

    public Comparator()
    {
    }

    public Comparator( final byte mode )
    {
        this.mode = mode;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Comparator";
    }

    public String getName()
    {
        return CommandFactory.COMP;
    }

    /**
     * Serializes command common properties, must be called by subclasses.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        super.serialize( out );
        out.writeByte( (byte) 1 );
        out.writeByte( mode );
    }

    /**
     * Read command common properties, must be called by subclasses.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        super.deserialize( in );
        in.readByte(); // skip version
        mode = in.readByte();
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination children or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( operand1 == null || operand2 == null )
        {
            throw new IllegalArgumentException( "And needs exactly two arguments" );
        }
        else
        {
            final Context context = getContext();
            operand1.setContext( context );
            operand2.setContext( context );
            final Instance val1 = operand1.run();
            final Instance val2 = operand2.run();
            return compare( val1, val2, mode, getVirtualMachine() );
        }
    }

    public static Instance compare(
            final Instance val1,
            final Instance val2,
            final int mode,
            final VirtualMachine virtualMachine
    )
            throws VirtualMachineException
    {
        Boolean value = null;
        if( val1 == null && val2 == null )
        {
            value = CoreConstants.FALSE;
        }
        else if( val1 == null )
        {
            value = CoreConstants.FALSE;
        }
        else if( val2 == null )
        {
            value = CoreConstants.TRUE;
        }
        else
        {
            final ScriptingClass clazz = val1.getScriptingClass();
            if( clazz.isSystemClass() )
            {
                value = comparePrimitives( val1, val2, mode );
            }
            else if( clazz.equals( val2.getScriptingClass()))
            {
                if( (clazz.hasMethod( "equals" ) && mode == EQUAL) ||
                    clazz.hasMethod( "compare" )
                )
                {
                    value = compareInstances( val1, val2, mode );
                }
            }
            else
            {
                value = CoreConstants.FALSE; // FixMe: this sould not happend, but anyway is surely not the best solution
            }
        }

        Instance instance = null;
        try
        {
            instance = virtualMachine.newInstance( "boolean" );
            instance.set( "value", value );
        }
        catch( ScriptingClassNotFoundException e )
        {
        }
        return instance;
    }

    private static Boolean compareInstances( final Instance val1, final Instance val2, final int mode )
            throws VirtualMachineException
    {
        final int result = Conversor.toInteger( val1.invoke(
                    (mode == EQUAL &&
                     val1.getScriptingClass().hasMethod( "equals" )) ?
                           "equals" :
                           "compare",
                    new Instance[] { val2 }
            ).get( "value" ) ).intValue();

        switch( mode )
        {
            case LT:
                return result < 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return result <= 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return result > 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return result >= 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return result == 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return result != 0 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean comparePrimitives(
            final Instance  val1,
            final Instance  val2,
            final int       mode
    )
    {
        final Boolean value;
        final Object o1 = val1.get( "value" );
        final Object o2 = val2.get( "value" );
        if( o1 == null && o2 == null )
        {
            value = CoreConstants.FALSE;
        }
        else if( o1 == null )
        {
            value = CoreConstants.FALSE;
        }
        else if( o2 == null )
        {
            value = CoreConstants.TRUE;
        }
        else
        {
            if( o1.getClass().isInstance( o2 ) )
            {
                if( o1 instanceof Boolean )
                {
                    value = compare(
                            ((Boolean) o1).booleanValue(),
                            ((Boolean) o2).booleanValue(),
                            mode
                    );
                }
                else if( o1 instanceof Integer )
                {
                    value = compare(
                            ((Integer) o1).intValue(),
                            ((Integer) o2).intValue(),
                            mode
                    );
                }
                else if( o1 instanceof Long )
                {
                    value = compare(
                            ((Long) o1).longValue(),
                            ((Long) o2).longValue(),
                            mode
                    );
                }
                else if( o1 instanceof Short )
                {
                    value = compare(
                            ((Short) o1).shortValue(),
                            ((Short) o2).shortValue(),
                            mode
                    );
                }
                else if( o1 instanceof Byte )
                {
                    value = compare(
                            ((Byte) o1).byteValue(),
                            ((Byte) o2).byteValue(),
                            mode
                    );
                }
                else if( o1 instanceof FixedPoint )
                {
                    value = compare(
                            (FixedPoint) o1,
                            (FixedPoint) o2,
                            mode
                    );
                }
                else
                {
                    value = compare(
                            o1.toString(),
                            o2.toString(),
                            mode
                    );
                }
            }
            else
            {
                value = CoreConstants.FALSE;
            }
        }
        return value;
    }

    private static Boolean compare( final int i1, final int i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1 < i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1 <= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1 >= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1 != i2 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean compare( final long i1, final long i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1 < i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1 <= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1 >= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1 != i2 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;

    }
    private static Boolean compare( final short i1, final short i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1 < i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1 <= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1 >= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1 != i2 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean compare( final byte i1, final byte i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1 < i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1 <= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1 >= i2 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1 == i2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1 != i2 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean compare( final FixedPoint i1, final FixedPoint i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1.compareTo( i2 ) < 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1.compareTo( i2 ) <= 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1.compareTo( i2 ) > 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1.compareTo( i2 ) >= 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1.compareTo( i2 ) == 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1.compareTo( i2 ) != 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean compare( final String i1, final String i2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return i1.compareTo( i2 ) < 0 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return i1.compareTo( i2 ) <= 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return i1.compareTo( i2 ) > 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return i1.compareTo( i2 ) >= 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return i1.compareTo( i2 ) == 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return i1.compareTo( i2 ) != 0 ?
                       CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }

    private static Boolean compare( final boolean b1, final boolean b2, final int mode )
    {
        switch( mode )
        {
            case LT:
                return !b1 && b2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case LTE:
                return (!b1 && b2) || ( b1 == b2 ) ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case GT:
                return b1 && !b2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case GTE:
                return (b1 && !b2) || ( b1 == b2 ) ?
                       CoreConstants.TRUE : CoreConstants.FALSE;

            case EQUAL:
                return b1 == b2 ? CoreConstants.TRUE : CoreConstants.FALSE;

            case DIFFERENT:
                return b1 != b2 ? CoreConstants.TRUE : CoreConstants.FALSE;
        }
        return CoreConstants.FALSE;
    }


}
