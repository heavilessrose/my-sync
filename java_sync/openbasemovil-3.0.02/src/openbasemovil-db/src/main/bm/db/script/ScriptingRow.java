package bm.db.script;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
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
import bm.db.Row;
import bm.core.CoreConstants;
import bm.core.math.FixedPoint;
/*
 * File Information
 *
 * Created on       : 23-oct-2007 22:34:21
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Scripting interface to the Row class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingRow
    extends NativeClass
{
/*
    public Date getDate( final String name )
    public Date getDate( final int index )
    public byte[] getBlob( final String name )
    public byte[] getBlob( final int index )

    public byte getStatus()
    public long getId()
    public int getVersion()
    public Integer getRecordId()
    public boolean isSent()
    public Table getParent()
    public String getString( final String name )
    public String getString( final int index )
    public int getInt( final String name )
    public int getInt( final int index )
    public long getLong( final String name )
    public long getLong( final int index )
    public short getShort( final String name )
    public short getShort( final int index )
    public FixedPoint getFixedPoint( final String name )
    public FixedPoint getFixedPoint( final int index )
    public boolean getBoolean( final String name )
    public boolean getBoolean( final int index )
    public synchronized void setField( final String name, final Object value )
    public synchronized void setField( final int index, final Object value )
    public boolean isDirty()
    public int getFieldCount()
    public String getFieldName( final int index )
    public boolean equals( final Object o )
    public int hashCode()
    public String toString()
    public Row cloneRow()
    public void remove()
    public void save()



 */
    private static final int GET_STATUS         = 0;
    private static final int GET_ID             = 1;
    private static final int GET_VERSION        = 2;
    private static final int GET_RECORD_ID      = 3;
    private static final int IS_SENT            = 4;
    private static final int GET_PARENT         = 5;
    private static final int GET_FIELD          = 12;
    private static final int IS_DIRTY           = 13;
    private static final int GET_FIELD_COUNT    = 14;
    private static final int GET_FIELD_NAME     = 15;
    private static final int EQUALS             = 16;
    private static final int HASH_CODE          = 17;
    private static final int TO_STRING          = 18;
    private static final int CLONE_ROW          = 19;
    private static final int REMOVE             = 20;
    private static final int SAVE               = 21;
    private static final int SET_FIELD          = 21;

    static
    {
        addMethod( "getStatus", GET_STATUS, 0 );
        addMethod( "getParent", GET_PARENT, 0 );
        addMethod( "getId", GET_ID, 0 );
        addMethod( "getVersion", GET_VERSION, 0 );
        addMethod( "getRecordId", GET_RECORD_ID, 0 );
        addMethod( "isSent", IS_SENT, 0 );
        addMethod( "getField", GET_FIELD, 1 );
        addMethod( "isDirty", IS_DIRTY, 0 );
        addMethod( "getFieldCount", GET_FIELD_COUNT, 0 );
        addMethod( "getFieldName", GET_FIELD_NAME, 1 );
        addMethod( "equals", EQUALS, 1 );
        addMethod( "hashCode", HASH_CODE, 0 );
        addMethod( "toString", TO_STRING, 0 );
        addMethod( "cloneRow", CLONE_ROW, 0 );
        addMethod( "save", SAVE, 0 );
        addMethod( "remove", REMOVE, 0 );
        addMethod( "setField", SET_FIELD, 2 );
    }

    /**
     * Constructor.
     *
     * @param vm virtual machine
     */
    public ScriptingRow( final VirtualMachine vm )
    {
        super( vm );
        name = "Row";
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
            final Row row = (Row) target.getAttachment( "row" );
            if( row != null )
            {
                try
                {
                    switch( id )
                    {
                        case GET_STATUS:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    new Byte( row.getStatus() )
                            );

                        case GET_ID:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    new Long( row.getId() )
                            );

                        case GET_VERSION:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    new Integer( row.getVersion() )
                            );

                        case GET_RECORD_ID:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.getRecordId()
                            );

                        case IS_SENT:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.isSent() ?
                                        CoreConstants.TRUE :
                                        CoreConstants.FALSE
                            );

                        case GET_PARENT:
                            return (Instance) target.getAttachment( "table" );

                        case GET_FIELD:
                        {
                            final Object locator = getLocator( args[0] );
                            Object value = null;
                            if( locator != null )
                            {
                                if( locator instanceof String )
                                {
                                    value = row.getField( (String) locator );
                                }
                                else
                                {
                                    value = row.getField(
                                            ((Integer) locator).intValue()
                                    );
                                }
                            }
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    value
                            );
                        }

                        case IS_DIRTY:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.isDirty() ?
                                        CoreConstants.TRUE :
                                        CoreConstants.FALSE
                            );

                        case GET_FIELD_COUNT:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    new Integer( row.getFieldCount() )
                            );

                        case GET_FIELD_NAME:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.getFieldName( Conversor.toInteger(
                                            args[0].get( "value" )
                                    ).intValue() )
                            );

                        case EQUALS:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.equals(
                                            args[0].get( "value" )
                                    ) ? CoreConstants.TRUE : CoreConstants.FALSE
                            );

                        case HASH_CODE:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    new Integer( row.hashCode() )
                            );

                        case TO_STRING:
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    row.toString()
                            );

                        case CLONE_ROW:
                        {
                            final Row r = row.cloneRow();
                            final Instance i =
                                    getVirtualMachine().newInstance( "Row" );
                            i.setAttachment( "row", row );
                            i.setAttachment(
                                    "parent",
                                    target.getAttachment( "parent" )
                            );
                            return i;
                        }

                        case REMOVE:
                            row.remove();
                            break;

                        case SAVE:
                            row.save();
                            break;
                    }
                }
                catch( Exception e )
                {
                    throw new VirtualMachineException( 0, e.getMessage() );
                }
            }
        }
        return null;
    }

    static Object getLocator( final Instance instance )
            throws VirtualMachineException
    {
        if( instance == null )
        {
            return null;
        }
        else
        {
            final ScriptingClass clazz = instance.getScriptingClass();
            if( clazz.isSystemClass() )
            {
                final Object o = instance.get( "value" );
                if( o instanceof Integer || o instanceof String )
                {
                    return o;
                }
                else if( o instanceof Long )
                {
                    return new Integer( (int) ((Long)o).longValue() );
                }
                else if( o instanceof Short )
                {
                    return new Integer( (int) ((Short)o).shortValue() );
                }
                else if( o instanceof Byte )
                {
                    return new Integer( (int) ((Byte)o).byteValue() );
                }
                else if( o instanceof FixedPoint )
                {
                    return new Integer( (int) ((FixedPoint) o).trunc() );
                }
                else
                {
                    return null;
                }
            }
            else if( clazz.hasMethod( "intValue" ) )
            {
                return instance.invoke( "toString", null ).get( "value" );
            }
            else if( clazz.hasMethod( "toString" ) )
            {
                return instance.invoke( "toString", null ).get( "value" );
            }
            else
            {
                return instance.toString();
            }
        }
    }

}
