/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db;
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

import java.io.*;
import java.util.Date;

import bm.err.ErrorLog;
import bm.core.J2MEException;
import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializerInputStream;
import bm.core.tools.Tools;
import bm.core.math.FixedPoint;
import bm.storage.RSException;

import bm.storage.RecordStoreFullException;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 14:47:00
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-09-11 14:25:53 +0200 (mar, 11 sep 2007) $
 * Revision         : $Revision: 14 $
 */

/**
 * A Row is a record in a table, which contains common data for synchronization
 * and custom data for each table.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 14 $
 */
public class Row
    implements Serializable
{
    static final int VERSION_BYTE    = 0;
    static final int STATUS_BYTE     = 1;
    static final int SENT_BYTE       = 2;

    /**
     * The row is up to date and valid.
     */
    public static final byte STATUS_OK       = 0;
    /**
     * The row is new, and has not been sent to the server.
     */
    public static final byte STATUS_NEW      = 1;
    /**
     * The row has been localy modified.
     */
    public static final byte STATUS_MODIFIED = 2;
    /**
     * The row has been localy deleted.
     */
    public static final byte STATUS_DELETED  = 3;
    /**
     * The row has been marked as erroneus by the server after sending an
     * update.
     */
    public static final byte STATUS_ERROR    = 4;
    /**
     * The row has been remotely deleted, and thus no update was possible.
     */
    public static final byte STATUS_ERROR_DELETED     = 5;

//    private static Log log = LogFactory.getLog( "Row" );

    byte        status;         // Row status, one of STATUS_XXX
    long        id;             // Row remote id
    int         version;        // Row version
    boolean     sent;           // The row has been sent to the server
    Object[]    fields;         // Field data

    transient Integer       recordId;   // Non persistent local record Id
    transient Table         parent;     // Non persistent parent Table
    transient boolean       dirty;      // Non persistent dirty marker

    /**
     * Constructor.
     *
     * @param parent parent table
     */
    Row( final Table parent )
    {
        this.parent = parent;
    }

    /**
     * Empty constructor suitable for deserialization.<br/>
     * Rows should never be created directly with this constructor.
     */
    public Row()
    {
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.Row";
    }

    /**
     * Get row status.
     *
     * @return row status
     */
    public byte getStatus()
    {
        return status;
    }

    /**
     * Set row status.
     *
     * @param status row status, one of the STATUS_* constants
     */
    public synchronized void setStatus( final byte status )
    {
        this.status = status;
    }

    /**
     * Get the id of this row.<br/>
     * The id has a positive, non-zero value if the row has been sent to the
     * server, and in that case it will not change as it will be shared among
     * devices.<br/>
     * If the row is locally new to a device, the id will have a negative,
     * non-zero value that will allow it to be indexed and referenced prior to
     * sending it to the server. When the server receives the row it will
     * generate a central id for it, and will notify the device in the
     * confirmation message.
     *
     * @return id of this row
     */
    public long getId()
    {
        return id;
    }

    /**
     * Set the id of this row.
     *
     * @param id row id
     */
    public synchronized void setId( final long id )
    {
        this.id = id;
    }

    /**
     * Get the version of this row.
     *
     * @return row version
     */
    public int getVersion()
    {
        return version;
    }

    /**
     * Set this row version.
     *
     * @param version row version
     */
    public synchronized void setVersion( final int version )
    {
        this.version = version;
    }

    /**
     * Get the local record id of this row.
     *
     * @return local record id of this row, or null if not created yet
     */
    public Integer getRecordId()
    {
        return recordId;
    }

    /**
     * Set the local record id.
     *
     * @param recordId local record id
     */
    public synchronized void setRecordId( final int recordId )
    {
        this.recordId = new Integer( recordId );
    }

    /**
     * Set the local record id.
     *
     * @param recordId local record id
     */
    public synchronized void setRecordId( final Integer recordId )
    {
        this.recordId = recordId;
    }

    /**
     * The row has been sent to the server.
     * @return row sent to the server
     */
    public boolean isSent()
    {
        return sent;
    }

    public synchronized void setSent( final boolean sent )
    {
        this.sent = sent;
    }

    /**
     * Get parent table.
     * @return parent table.
     */
    public Table getParent()
    {
        return parent;
    }

    /**
     * Shortcut to getField that returns a String.
     *
     * @param name field name
     * @return string representation of field value or null
     */
    public String getString( final String name )
    {
        final Object value = getField( name );
        return value != null ? value.toString() : null;
    }

    /**
     * Shortcut to getField that returns a String.
     *
     * @param index field index
     * @return string representation of field value or null
     */
    public String getString( final int index )
    {
        final Object value = getField( index );
        return value != null ? value.toString() : null;
    }

    /**
     * Shortcut to getField that returns an int.
     * @param name field name
     * @return int value or 0 if not applyable
     */
    public int getInt( final String name )
    {
        final Object value = getField( name );
        return getIntValue( value );
    }

    /**
     * Shortcut to getField that returns an int.
     * @param index field index
     * @return int value or 0 if not applyable
     */
    public int getInt( final int index )
    {
        final Object value = getField( index );
        return getIntValue( value );
    }

    /**
     * Shortcut to getField that returns a long.
     * @param name field name
     * @return long value or 0 if not applyable
     */
    public long getLong( final String name )
    {
        final Object value = getField( name );
        return getLongValue( value );
    }

    /**
     * Shortcut to getField that returns a long.
     * @param index field index
     * @return long value or 0 if not applyable
     */
    public long getLong( final int index )
    {
        final Object value = getField( index );
        return getLongValue( value );
    }

    /**
     * Shortcut to getField that returns a short.
     * @param name field name
     * @return short value or 0 if not applyable
     */
    public short getShort( final String name )
    {
        final Object value = getField( name );
        return getShortValue( value );
    }

    /**
     * Shortcut to getField that returns a short.
     * @param index field index
     * @return short value or 0 if not applyable
     */
    public short getShort( final int index )
    {
        final Object value = getField( index );
        return getShortValue( value );
    }

    /**
     * Shortcut to getField that returns a FixedPoint.
     * @param name field name
     * @return FixedPoint or null if not appyable
     */
    public FixedPoint getFixedPoint( final String name )
    {
        final Object value = getField( name );
        if( value != null && value instanceof FixedPoint )
        {
            return (FixedPoint) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Shortcut to getField that returns a FixedPoint.
     * @param index field index
     * @return FixedPoint or null if not appyable
     */
    public FixedPoint getFixedPoint( final int index )
    {
        final Object value = getField( index );
        if( value != null && value instanceof FixedPoint )
        {
            return (FixedPoint) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Shortcut to getField that returns a Date.
     * @param name field name
     * @return Date or null if not appyable
     */
    public Date getDate( final String name )
    {
        final Object value = getField( name );
        if( value != null && value instanceof Date )
        {
            return (Date) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Shortcut to getField that returns a Date.
     * @param index field index
     * @return Date or null if not appyable
     */
    public Date getDate( final int index )
    {
        final Object value = getField( index );
        if( value != null && value instanceof Date )
        {
            return (Date) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Shortcut to getField that returns a boolean.<br/>
     * If the value is a Boolean object return it's primitive value, if not,
     * converts it to int and returns true if the result is not zero.
     * @param name field name
     * @return boolean or false if not appyable or null
     */
    public boolean getBoolean( final String name )
    {
        return getBooleanValue( getField( name ) );
    }

    /**
     * Shortcut to getField that returns a boolean.<br/>
     * If the value is a Boolean object return it's primitive value, if not,
     * converts it to int and returns true if the result is not zero.
     * @param index field index
     * @return boolean or false if not appyable or null
     */
    public boolean getBoolean( final int index )
    {
        return getBooleanValue( getField( index ) );
    }

    private boolean getBooleanValue( final Object value )
    {
        if( value != null && value instanceof Boolean )
        {
            return ((Boolean) value).booleanValue();
        }
        else
        {
            return getIntValue( value ) != 0;
        }
    }

    /**
     * Shortcut to getField that returns a blob (byte array).
     * @param name field name
     * @return byte array or null if not applyable
     */
    public byte[] getBlob( final String name )
    {
        final Object value = getField( name );
        if( value != null && value instanceof byte[] )
        {
            return (byte[]) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Shortcut to getField that returns a boolean.
     * @param index field index
     * @return byte array or null if not applyable
     */
    public byte[] getBlob( final int index )
    {
        final Object value = getField( index );
        if( value != null && value instanceof byte[] )
        {
            return (byte[]) value;
        }
        else
        {
            return null;
        }
    }

    /**
     * Get the content of a field.
     *
     * @param name field name
     * @return field value, possibly null
     * @throws InvalidFieldException if a field with this name is not defined for this row
     */
    public Object getField( final String name )
            throws InvalidFieldException
    {
        if( !name.toLowerCase().equals( "remote_id" ) )
        {
            final Integer index = (Integer) parent.fieldMap.get(
                    name.toLowerCase()
            );
            if( index == null )
            {
                throw new InvalidFieldException( name );
            }
            else
            {
                return getField( index.intValue() );
            }
        }
        else
        {
            return new Long( id );
        }
    }

    /**
     * Get the content of a field.
     *
     * @param index field index (from 0 to fieldCount -1)
     * @return field value, possibly null
     * @throws InvalidFieldException if the index is invalid
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public Object getField( final int index )
            throws InvalidFieldException
    {
        if( fields != null )
        {
            if( index >= 0 && index < fields.length )
            {
                return fields[ index ];
            }
            else
            {
                throw new InvalidFieldException( Integer.toString( index ) );
            }
        }
        else
        {
            return null;
        }
    }

    /**
     * Set the value of a field.
     *
     * @param name field name
     * @param value value
     * @throws RequiredFieldCantBeNullException if the value is null but the field is non-nullable
     * @throws InvalidDataTypeException if the data type does not match and can't be converted
     * @throws InvalidFieldException if the name does not represent a valid field
     */
    public synchronized void setField( final String name, final Object value )
            throws RequiredFieldCantBeNullException,
                   InvalidDataTypeException,
                   InvalidFieldException
    {
        final Integer index = (Integer) parent.fieldMap.get( name.toLowerCase() );
        if( index != null )
        {
            setField( index.intValue(), value );
        }
        else
        {
            throw new InvalidFieldException( name );
        }
    }

    /**
     * Set the value of a field.
     *
     * @param index field index (from 0 to fieldCount -1)
     * @param value value
     * @throws RequiredFieldCantBeNullException if the value is null but the field is non-nullable
     * @throws InvalidDataTypeException if the data type does not match and can't be converted
     * @throws InvalidFieldException if the index does not represent a valid field
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void setField( final int index, final Object value )
            throws RequiredFieldCantBeNullException,
                   InvalidDataTypeException,
                   InvalidFieldException
    {
        if( fields == null )
        {
            fields = new Object[ parent.tableInfo.getFieldInfo().length ];
        }
        if( index >= 0 && index < fields.length )
        {
            final Object val = checkType( index, value );
            if( !Tools.objectEquals( fields[ index ], val ) )
            {
                fields[ index ] = val;
                dirty = true;
            }
        }
        else
        {
            throw new InvalidFieldException( Integer.toString( index ) );
        }
    }

    /**
     * Check if the row has changed since it was last read/written.
     * @return true if it's dirty
     */
    public boolean isDirty()
    {
        return dirty;
    }

    /**
     * Return the number of fields in this row.<br/>
     * If the field collection is not null, it's length is returned.
     * Else, if the parent table is not null, the value returned corresponds to
     * the number of fields defined for a row.
     * Otherwise 0.
     * @return number of fields or 0 if unable to know.
     */
    public int getFieldCount()
    {
        if( fields != null )
        {
            return fields.length;
        }
        else if( parent != null )
        {
            return parent.getTableInfo().getFieldInfo().length;
        }
        else
        {
            return 0;
        }
    }

    /**
     * Return the name of the field for a given index.<br/>
     * Only works if the parent table is setProperty
     * @param index field index
     * @return name or null if table no setProperty or index is wrong
     */
    public String getFieldName( final int index )
    {
        if( parent != null )
        {
            final FieldInfo[] fieldInfo = parent.getTableInfo().getFieldInfo();
            if( index >= 0 && index < fieldInfo.length )
            {
                return fieldInfo[index].getName();
            }
            else
            {
                return null;
            }
        }
        else
        {
            return null;
        }
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     *
     * @throws SerializationException on errors
     */
    public synchronized void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // version
        out.writeByte( status );
        out.writeByte( (byte) (sent ? 1 : 0) );
        serializeHeader( out );
        serializeFields( out );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     *
     * @throws SerializationException on errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // version
        status = in.readByte();
        sent = in.readByte() == 1;
        deserializeHeader( in );
        deserializeFields( in, true );
        dirty = false;
    }

    public boolean equals( final Object o )
    {
        if( this == o )
        {
            return true;
        }
        if( o == null || !(o instanceof Row) )
        {
            return false;
        }

        final Row row = (Row) o;

        if( id != row.id )
        {
            return false;
        }
        if( status != row.status )
        {
            return false;
        }
        if( version != row.version )
        {
            return false;
        }
        // Local variable to improve speed on CDC
        final Object[] fields = this.fields;
        if(
                (fields != null && row.fields == null) ||
                (fields == null && row.fields != null)
        )
        {
            return false;
        }
        if( fields != null )
        {
            final int fieldCount = fields.length;
            if( fieldCount != row.fields.length )
            {
                return false;
            }
            for( int i = 0; i < fieldCount; i++ )
            {
                if(
                        (fields[i] != null && row.fields[i] == null) ||
                        (fields[i] == null && row.fields[i] != null)
                )
                {
                    return false;
                }
                if( fields[i] != null )
                {
                    if(
                            !fields[i].getClass().isInstance( row ) ||
                            !fields[i].equals( row )
                    )
                    {
                        return false;
                    }
                }
            }
        }
        return !(
                recordId != null ?
                        !recordId.equals( row.recordId ) :
                        row.recordId != null
        );

    }

    public int hashCode()
    {
        // Local variable to improve speed on CDC
        final Object[] fields = this.fields;
        int result;
        result = (int) status;
        result = 29 * result + (int) ( id ^ ( id >>> 32 ) );
        result = 29 * result + version;
        result = 29 * result + ( recordId != null ? recordId.hashCode() : 0 );
        if( fields != null )
        {
            final int fieldCount = fields.length;
            for( int i = 0; i < fieldCount; i++ )
            {
                result = 29 * result + ( fields[i] != null ? fields[i].hashCode() : 0 );
            }
        }
        return result;
    }

    public String toString()
   {
        final StringBuffer buffer = new StringBuffer( "Row{" )
               .append( "status=" ).append( status )
               .append( ", id=" ).append( id )
               .append( ", version=" ).append( version )
               .append( ", fields=" );
        // Local variable to improve speed on CDC
        final Object[] fields = this.fields;
        if( fields == null )
        {
            buffer.append( "null" );
        }
        else
        {
            final FieldInfo[] fieldInfo = parent.getTableInfo().getFieldInfo();
            buffer.append( "[" );
            final int fieldCount = fields.length;
            for( int i = 0; i < fieldCount; i++ )
            {
                buffer.append( "(" ).append( fieldInfo[i].getName() ).append( "=" );
                if( fields[i] instanceof byte[])
                {
                    buffer.append( "blob(" )
                            .append( fields[i] != null ?
                                     ((byte[])fields[i]).length
                                     : 0
                            ).append( ")" );
                }
                else
                {
                    buffer.append( fields[i] );
                }
                buffer.append( ")" );
                if( i < fieldCount -1 )
                {
                    buffer.append( ", " );
                }
            }
            buffer.append( "]" );
        }
        buffer.append( ", recordId=" ).append( recordId )
                .append( "}" );
        return buffer.toString();
    }

    /**
     * Create a new Row object that is a clone of this one.
     * @return new clonned row
     */
    public Row cloneRow()
    {
        // Local variable to improve speed on CDC
        final Object[] fields = this.fields;

        final Row newRow = new Row( parent );
        newRow.status   = status;
        newRow.id       = id;
        newRow.version  = version;
        newRow.recordId = recordId != null ? new Integer( recordId.intValue() ) : null;
        if( fields != null )
        {
            final FieldInfo[] fieldInfo = parent.getTableInfo().getFieldInfo();
            final int length = fields.length;
            newRow.fields   = new Object[ length ];
            for( int i = 0; i < length; i++ )
            {
                if( fields[i] != null )
                {
                    switch( fieldInfo[i].getType() )
                    {
                        case Constants.FT_BOOLEAN:
                            newRow.fields[i] = new Boolean(
                                    ((Boolean) fields[i]).booleanValue()
                            );
                            break;

                        case Constants.FT_DATE:
                            newRow.fields[i] = new Date(
                                    ((Date) fields[i]).getTime()
                            );
                            break;

                        case Constants.FT_FIXED_POINT:
                            newRow.fields[i] = ((FixedPoint) fields[i]).clone();
                            break;

                        case Constants.FT_INT:
                            newRow.fields[i] = new Integer(
                                    ((Integer) fields[i]).intValue()
                            );
                            break;

                        case Constants.FT_LONG:
                            newRow.fields[i] = new Long(
                                    ((Long) fields[i]).longValue()
                            );
                            break;

                        case Constants.FT_SHORT:
                            newRow.fields[i] = new Short(
                                    ((Short) fields[i]).shortValue()
                            );
                            break;

                        case Constants.FT_STRING:
                            //noinspection RedundantStringConstructorCall
                            newRow.fields[i] = new String(
                                    ((String) fields[i])
                            );
                            break;

                        case Constants.FT_BVLOB:
                        case Constants.FT_IMAGE:
                        case Constants.FT_BLOB:
                            final byte[] source = (byte[]) fields[i];
                            final byte[] dest = new byte[ source.length ];
                            System.arraycopy( source, 0, dest, 0, source.length );
                            newRow.fields[i] = dest;
                            break;
                    }
                }
            }
        }
        return newRow;
    }

    /**
     * Remove the row from it's parent table.<br/>
     * This is a shortcut for row.getParent().remove( row ).
     *
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public void remove()
            throws SerializationException,
                   RSException,
                   RecordStoreFullException,
                   DBException
    {
        parent.remove( this );
    }

    /**
     *
     * Save/update the row at it's parent table.<br/>
     * This is a shortcut for row.getParent().save( row ).
     *
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public void save()
            throws SerializationException,
                   RSException,
                   RecordStoreFullException,
                   DBException
    {
        if( dirty )
        {
            parent.save( this );
            dirty = false;
        }
    }

    // -------------------------------------------------------------------------
    // Private methods
    // -------------------------------------------------------------------------

    private void prepareFields()
    {
        final int length = parent.tableInfo.getFieldInfo().length;
        final Object[] fields = this.fields;
        if( fields == null )
        {
            this.fields = new Object[ length ];
        }
        else
        {
            for( int i = 0; i < length; i++ )
            {
                fields[i] = null;
            }
        }
    }

    private void serializeHeader( final SerializerOutputStream out )
            throws SerializationException
    {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final DataOutputStream dos = new DataOutputStream( baos );

        try
        {
            dos.writeInt( version );
            dos.writeLong( id );

            final byte[] header = baos.toByteArray();
            final int count = header.length;
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                out.writeByte( header[i] );
            }
        }
        catch( IOException e )
        {
            ErrorLog.addError(
                    "Row",
                    "serializeHeader",
                    null,
                    null,
                    e
            );
            throw new SerializationException( Constants.ERR_ROW_SERIALIZE_HEADER, e );
        }
    }

    private void deserializeHeader( final SerializerInputStream in )
            throws SerializationException
    {
        try
        {
            final int count = in.readInt();
            final byte[] buffer;
            try
            {
                buffer = Tools.secureAlloc( count );
            }
            catch( J2MEException e )
            {
                ErrorLog.addError(
                        "Row",
                        "deserializeHeader",
                        null,
                        null,
                        e
                );
                throw new SerializationException( Constants.ERR_ROW_DESERIALIZE_HEADER,  e );
            }
            for( int i = 0; i < count; i++ )
            {
                buffer[i] = in.readByte();
            }

            final ByteArrayInputStream bais = new ByteArrayInputStream( buffer );
            final DataInputStream dis = new DataInputStream( bais );

            version     = dis.readInt();
            id          = dis.readLong();
        }
        catch( IOException e )
        {
            ErrorLog.addError(
                    "Row",
                    "deserializeHeader",
                    null,
                    null,
                    e
            );
            throw new SerializationException( Constants.ERR_ROW_DESERIALIZE_HEADER, e );
        }
    }

    private void serializeFields( final SerializerOutputStream out )
            throws SerializationException
    {
        // Local variables to improve speed on CDC
        final Object[] fields = this.fields;
        final FieldInfo[] fieldInfo = parent.tableInfo.getFieldInfo();
        final int fieldCount = fieldInfo.length;
        for( int i = 0; i < fieldCount; i++ )
        {
            if( fields[i] != null )
            {
                out.writeInt( i );
                serializeField( out, i );
            }
        }
        out.writeInt( -1 );
    }

    void deserializeFields(
            final SerializerInputStream in,
            final boolean               cleanUp
    )
            throws SerializationException
    {
        if( cleanUp )
        {
            prepareFields();
        }
        int fieldIndex = in.readInt();
        while( fieldIndex != -1 )
        {
            deserializeField( in, fieldIndex );
            fieldIndex = in.readInt();
        }
    }

    private void serializeField( final SerializerOutputStream out, final int i )
            throws SerializationException
    {
        // Local variables to improve speed on CDC
        final Object[] fields = this.fields;
        final FieldInfo[] fieldInfo = parent.tableInfo.getFieldInfo();
        switch( fieldInfo[i].getType() )
        {
            case Constants.FT_BOOLEAN:
                out.writeBoolean( ((Boolean)fields[i]).booleanValue() );
                break;

            case Constants.FT_DATE:
                out.writeDate( (Date) fields[i] );
                break;

            case Constants.FT_FIXED_POINT:
                FixedPoint.serialize( out, (FixedPoint) fields[i], false );
                break;

            case Constants.FT_INT:
                out.writeInt( ((Integer) fields[i]).intValue() );
                break;

            case Constants.FT_LONG:
                out.writeLong( ((Long) fields[i]).longValue() );
                break;

            case Constants.FT_SHORT:
                out.writeShort( ((Short) fields[i]).shortValue() );
                break;

            case Constants.FT_STRING:
                out.writeString(
                        (String) fields[i],
                        fieldInfo[i].getLength()
                );
                break;

            case Constants.FT_BVLOB:
            case Constants.FT_IMAGE:
            case Constants.FT_BLOB:
                out.writeBlob( (byte[]) fields[i] );
                break;
        }
    }


    private void deserializeField( final SerializerInputStream in, final int i )
            throws SerializationException
    {
        // Local variables to improve speed on CDC
        final Object[] fields = this.fields;
        if( i < 0 || i >= fields.length )
        {
            ErrorLog.addError(
                    "Row",
                    "deserializeField",
                    new Object[] { new Integer( i ) },
                    "Invalid field index (" + toString() + "}",
                    null
            );
            throw new SerializationException( Constants.ERR_ROW_DESERIALIZE_FIELD, "Invalid field index: " + i );
        }
        try
        {
            // Local variables to improve speed on CDC
            final FieldInfo[] fieldInfo = parent.tableInfo.getFieldInfo();
            switch( fieldInfo[i].getType() )
            {
                case Constants.FT_BOOLEAN:
                    fields[i] = new Boolean( in.readBoolean() );
                    break;

                case Constants.FT_DATE:
                    fields[i] = in.readDate();
                    break;

                case Constants.FT_FIXED_POINT:
                    fields[i] = FixedPoint.deserialize( in, false );
                    break;

                case Constants.FT_INT:
                    fields[i] = new Integer( in.readInt() );
                    break;

                case Constants.FT_LONG:
                    fields[i] = new Long( in.readLong() );
                    break;

                case Constants.FT_SHORT:
                    fields[i] = new Short( in.readShort() );
                    break;

                case Constants.FT_STRING:
                    fields[i] = in.readString();
                    break;

                case Constants.FT_BVLOB:
                case Constants.FT_IMAGE:
                case Constants.FT_BLOB:
                    fields[i] = in.readBlob();
                    break;
            }
        }
        catch( Exception e )
        {
            throw new SerializationException(
                    Constants.ERR_ROW_DESERIALIZE_FIELD,
                    e
            );
        }
    }

    private int getIntValue( final Object value )
    {
        if( value != null )
        {
            if( value instanceof Integer )
            {
                return ((Integer) value).intValue();
            }
            else if( value instanceof Long )
            {
                return (int) ((Long) value).longValue();
            }
            else if( value instanceof Short )
            {
                return ((Short) value).shortValue();
            }
            else
            {
                try
                {
                    return Integer.parseInt( value.toString() );
                }
                catch( NumberFormatException e )
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }

    private long getLongValue( final Object value )
    {
        if( value != null )
        {
            if( value instanceof Long )
            {
                return ((Long) value).longValue();
            }
            else if( value instanceof Integer )
            {
                return ((Integer) value).intValue();
            }
            else if( value instanceof Short )
            {
                return ((Short) value).shortValue();
            }
            else
            {
                try
                {
                    return Long.parseLong( value.toString() );
                }
                catch( NumberFormatException e )
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }

    private short getShortValue( final Object value )
    {
        if( value != null )
        {
            if( value instanceof Short )
            {
                return ((Short) value).shortValue();
            }
            else if( value instanceof Integer )
            {
                return (short) ((Integer) value).intValue();
            }
            else if( value instanceof Long )
            {
                return (short) ((Long) value).longValue();
            }
            else
            {
                try
                {
                    return Short.parseShort( value.toString() );
                }
                catch( NumberFormatException e )
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }

    /**
     * Check and possibly convert the value into the appropriate type.
     *
     * @param index field index
     * @param value value to check (and convert)
     * @return appropriate value for field
     * @throws RequiredFieldCantBeNullException if value is null and field is non-nullable
     * @throws InvalidDataTypeException if the data type is wrong and can't be converted
     */
    private Object checkType( final int index, final Object value )
            throws RequiredFieldCantBeNullException,
                   InvalidDataTypeException
    {
        // Local variable to improve speed on CDC
        final FieldInfo[] fieldInfo = parent.tableInfo.getFieldInfo();
        if( value == null )
        {
            if( fieldInfo[index].isNullable() )
            {
                return value;
            }
            else
            {
                throw new RequiredFieldCantBeNullException(
                        Constants.ERR_ROW_CHECK_TYPE,
                        fieldInfo[index].getName()
                );
            }
        }
        switch( fieldInfo[index].getType() )
        {
            case Constants.FT_BOOLEAN:
                if( value instanceof Boolean )
                {
                    return value;
                }
                else if(
                        value.toString().toLowerCase().equals( "true") ||
                        value.toString().toLowerCase().equals( "1") ||
                        value.toString().toLowerCase().equals( "yes")
                )
                {
                    return new Boolean( true );
                }
                else
                {
                    return new Boolean( false );
                }

            case Constants.FT_DATE:
                if( value instanceof Date )
                {
                    return value;
                }
                else if( value instanceof Long )
                {
                    return new Date( ((Long) value).longValue() );
                }
                else
                {
                    return new Date( Long.parseLong( value.toString() ) );
                }

            case Constants.FT_FIXED_POINT:
                if( value instanceof FixedPoint )
                {
                    return value;
                }
                else if( value instanceof String )
                {
                    try
                    {
                        return FixedPoint.parse( (String) value );
                    }
                    catch( NumberFormatException e )
                    {
                        throw new InvalidDataTypeException(
                                Constants.ERR_ROW_CHECK_TYPE,
                                fieldInfo[index].getName()
                        );
                    }
                }
                else if( value instanceof Long )
                {
                    return new FixedPoint( ((Long) value).longValue() );
                }
                else if( value instanceof Integer )
                {
                    return new FixedPoint( ((Integer) value).longValue() );
                }
                else if( value instanceof Short )
                {
                    return new FixedPoint( ((Short) value).shortValue() );
                }
                else
                {
                    throw new InvalidDataTypeException(
                            Constants.ERR_ROW_CHECK_TYPE,
                            fieldInfo[index].getName()
                    );
                }

            case Constants.FT_INT:
                if( value instanceof Integer )
                {
                    return value;
                }
                else if( value instanceof Short )
                {
                    return new Integer( ((Short) value).shortValue() );
                }
                else if( value instanceof String )
                {
                    try
                    {
                        return new Integer( Integer.parseInt( (String) value ) );
                    }
                    catch( NumberFormatException e )
                    {
                        throw new InvalidDataTypeException(
                                Constants.ERR_ROW_CHECK_TYPE,
                                fieldInfo[index].getName()
                        );
                    }
                }
                else
                {
                    throw new InvalidDataTypeException(
                            Constants.ERR_ROW_CHECK_TYPE,
                            fieldInfo[index].getName()
                    );
                }

            case Constants.FT_LONG:
                if( value instanceof Long )
                {
                    return value;
                }
                else if( value instanceof Integer )
                {
                    return new Long( ((Integer) value).intValue() );
                }
                else if( value instanceof Short )
                {
                    return new Long( ((Short) value).shortValue() );
                }
                else if( value instanceof String )
                {
                    try
                    {
                        return new Long( Long.parseLong( (String) value ) );
                    }
                    catch( NumberFormatException e )
                    {
                        throw new InvalidDataTypeException(
                                Constants.ERR_ROW_CHECK_TYPE,
                                fieldInfo[index].getName()
                        );
                    }
                }
                else
                {
                    throw new InvalidDataTypeException(
                            Constants.ERR_ROW_CHECK_TYPE,
                            fieldInfo[index].getName()
                    );
                }

            case Constants.FT_SHORT:
                if( value instanceof Short )
                {
                    return value;
                }
                else if( value instanceof String )
                {
                    try
                    {
                        return new Short( Short.parseShort( (String) value ) );
                    }
                    catch( NumberFormatException e )
                    {
                        throw new InvalidDataTypeException(
                                Constants.ERR_ROW_CHECK_TYPE,
                                fieldInfo[index].getName()
                        );
                    }
                }
                else
                {
                    throw new InvalidDataTypeException(
                            Constants.ERR_ROW_CHECK_TYPE,
                            fieldInfo[index].getName()
                    );
                }

            case Constants.FT_STRING:
                return value.toString();

            case Constants.FT_BVLOB:
            case Constants.FT_IMAGE:
            case Constants.FT_BLOB:
                if( value instanceof byte[] )
                {
                    return value;
                }
                else if( value instanceof String )
                {
                    return ((String) value).getBytes();
                }
                else
                {
                    throw new InvalidDataTypeException(
                            Constants.ERR_ROW_CHECK_TYPE,
                            fieldInfo[index].getName()
                    );
                }
        }
        return null; // will never happen
    }
}
