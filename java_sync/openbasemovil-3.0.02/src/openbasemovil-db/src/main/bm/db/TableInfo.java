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

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.db.index.IndexInfo;

import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 13:27:13
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-07-17 01:40:16 +0200 (mar, 17 jul 2007) $
 * Revision         : $Revision: 9 $
 */

/*
    Binary format of TableInfo objects:

    {string name}
    {byte   readOnly}
    {int    numfields}
    {fieldInfo}*
    {int    numindexes}
    {indexInfo}*
*/

/**
 * Table information.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 9 $
 */
public class TableInfo
    implements Serializable
{
    public static final byte    WRITABLE        = 0;
    public static final byte    READ_ONLY       = 1;
    public static final byte    LOCALY_WRITABLE = 2;

    private String      name;
    private byte        readWriteType;
    private FieldInfo[] fieldInfo;
    private IndexInfo[] indexInfo;

    private transient int recordId;

    private transient Vector fields;
    private transient Vector indexes;

    public void addField( final FieldInfo field )
    {
        if( fields == null )
        {
            fields = new Vector( 5 );
        }
        fields.addElement( field );
    }

    public void addIndex( final IndexInfo index )
    {
        if( indexes == null )
        {
            indexes = new Vector( 5 );
        }
        indexes.addElement( index );
    }

    void flushVectors()
    {
        if( fields != null && fields.size() > 0 )
        {
            final int newLength = fields.size();
            if( fieldInfo == null )
            {
                fieldInfo = new FieldInfo[ newLength ];
                fields.copyInto( fieldInfo );
            }
            else
            {
                final int curLength = fieldInfo.length;
                final FieldInfo[] aux =
                        new FieldInfo[ curLength + newLength];
                System.arraycopy( fieldInfo, 0, aux, 0, curLength );
                for( int i = 0; i < newLength; i++ )
                {
                    aux[ curLength + i ] = (FieldInfo) fields.elementAt( i );
                }
                fieldInfo = aux;
            }
            if( indexes != null && indexes.size() > 0 )
            {
                if( indexInfo == null )
                {
                    indexInfo = new IndexInfo[ indexes.size() ];
                    indexes.copyInto( indexInfo );
                }
                else
                {
                    final int indexCurLength = indexInfo.length;
                    final IndexInfo[] aux =
                            new IndexInfo[ indexCurLength + indexes.size() ];
                    System.arraycopy( indexInfo, 0, aux, 0, indexCurLength );
                    for( int i = 0; i< indexes.size(); i++ )
                    {
                        aux[ indexCurLength + i ] = (IndexInfo)
                                indexes.elementAt( i );
                    }
                    indexInfo = aux;
                }
            }
        }
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.TableInfo";
    }

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name != null ? name.toLowerCase() : null;
    }

    /**
     * Calculates the maximum row size given the table definition. A negative
     * valule implies that the structure has blob fields, and the calculated
     * size takes only non-blob fields into account.
     * @return maximum row size, 0 if not defined
     */
    public int getMaxRowSize()
    {
        final FieldInfo[] fieldInfo = this.fieldInfo;
        if( fieldInfo != null )
        {
            int size = 22;
            int factor = 1;
            final int count = fieldInfo.length;
            for( int i = 0; i < count; i++ )
            {
                switch( fieldInfo[i].getType() )
                {
                    case Constants.FT_BLOB:
                        factor = -1;
                        break;

                    case Constants.FT_BOOLEAN:
                        size++;
                        break;

                    case Constants.FT_DATE:
                    case Constants.FT_LONG:
                        size += 9;
                        break;

                    case Constants.FT_FIXED_POINT:
                        size += 11;
                        break;

                    case Constants.FT_INT:
                        size += 5;
                        break;

                    case Constants.FT_SHORT:
                        size += 3;
                        break;

                    case Constants.FT_STRING:
                        size += (fieldInfo[i].getLength() * 2) + 3;
                        break;

                    case Constants.FT_BVLOB:
                    case Constants.FT_IMAGE:
                        return 0;
                }
            }
            return size * factor;
        }
        else
        {
            return 0;
        }
    }

    public int getRecordId()
    {
        return recordId;
    }

    public void setRecordId( final int recordId )
    {
        this.recordId = recordId;
    }

    public boolean isReadOnly()
    {
        return readWriteType == READ_ONLY;
    }

    public boolean isLocalyWritable()
    {
        return readWriteType == LOCALY_WRITABLE;
    }

    public byte getReadWriteType()
    {
        return readWriteType;
    }

    public void setReadWriteType( final byte readWriteType )
    {
        this.readWriteType = readWriteType;
    }

    public FieldInfo[] getFieldInfo()
    {
        return fieldInfo;
    }

    public void setFieldInfo( final FieldInfo[] fieldInfo )
    {
        this.fieldInfo = fieldInfo;
    }

    public IndexInfo[] getIndexInfo()
    {
        return indexInfo;
    }

    public void setIndexInfo( final IndexInfo[] indexInfo )
    {
        this.indexInfo = indexInfo;
    }

    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        final FieldInfo[] fieldInfo = this.fieldInfo;
        final IndexInfo[] indexInfo = this.indexInfo;
        out.writeByte( (byte) 1 ); // Version
        out.writeString( name.toLowerCase() );
        out.writeByte( readWriteType ); // Changed from writeBoolean to writeByte, remains compatible as 0 represents false for readBoolean and non-zero represents true
        if( fieldInfo != null )
        {
            final int fieldCount = fieldInfo.length;
            out.writeInt( fieldCount );
            for( int i = 0; i < fieldCount; i++ )
            {
                fieldInfo[i].serialize( out );
            }
        }
        else
        {
            out.writeInt( 0 );
        }
        if( indexInfo != null )
        {
            final int indexCount = indexInfo.length;
            out.writeInt( indexCount );
            for( int i = 0; i < indexCount; i++ )
            {
                indexInfo[i].serialize( out );
            }
        }
        else
        {
            out.writeInt( 0 );
        }
    }
/*
    ( TableInfo :
            {byte:      version}
            {String:    name}
            {byte:      readWriteType}
            {int:       fieldCount}
            ( FieldInfo:
                {byte:      version}
                {String:    name}
                {byte:      type}
                {boolean:   nullable}
                {int:       length}
                {boolean:   disabled}
            )+
            {int:       indexCount}
            ( IndexInfo:
                {byte:      version}
                {int:       order}
                {String:    name}
                {int:       fieldCount}
                {String:    fieldName}+
                {byte:      indexType}
                [ if( indexType == KT_STRING )
                    {boolean:   caseSensitive}
                ]
            )*
    )+
*/

    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // Skip version
        name = in.readString().toLowerCase();
        readWriteType = in.readByte();
        int length = in.readInt();
        if( length > 0 )
        {
            final FieldInfo[] fieldInfo = new FieldInfo[ length ];
            this.fieldInfo = fieldInfo;
            for( int i = 0; i < length; i++ )
            {
                fieldInfo[i] = new FieldInfo();
                fieldInfo[i].deserialize( in );
            }
        }
        else
        {
            fieldInfo = null;
        }
        length = in.readInt();
        if( length > 0 )
        {
            final IndexInfo[] indexInfo = new IndexInfo[ length ];
            this.indexInfo = indexInfo;
            for( int i = 0; i < length; i++ )
            {
                indexInfo[i] = new IndexInfo();
                indexInfo[i].deserialize( in );
            }
        }
        else
        {
            indexInfo = null;
        }
    }

    public String toString()
    {
        final FieldInfo[] fieldInfo = this.fieldInfo;
        final IndexInfo[] indexInfo = this.indexInfo;
        final StringBuffer buffer = new StringBuffer( "TableInfo{" );
        buffer.append( "name=" ).append( name )
                .append( ",readOnly=" ).append( readWriteType )
                .append( ",fieldInfo=" );
        if( fieldInfo != null )
        {
            final int fieldCount = fieldInfo.length;
            buffer.append( "[" ).append( fieldCount ).append( "]{" );
            for( int i = 0; i < fieldCount; i++ )
            {
                buffer.append( fieldInfo[i] );
                if( i < fieldCount - 1 )
                {
                    buffer.append( "," );
                }
            }
            buffer.append( "}" );
        }
        else
        {
            buffer.append( "null" );
        }
        buffer.append( ",indexInfo=" );
        if( indexInfo != null )
        {
            final int indexCount = indexInfo.length;
            buffer.append( "[" ).append( indexCount ).append( "]{" );
            for( int i = 0; i < indexCount; i++ )
            {
                buffer.append( indexInfo[i] );
                if( i < indexCount - 1 )
                {
                    buffer.append( "," );
                }
            }
            buffer.append( "}" );
        }
        else
        {
            buffer.append( "null" );
        }
        buffer.append( "}" );
        return buffer.toString();
    }

    public boolean equals( final Object o )
    {
        if( this == o )
        {
            return true;
        }
        if( o == null || getClass() != o.getClass() )
        {
            return false;
        }

        final TableInfo tableInfo = (TableInfo) o;

        if( readWriteType != tableInfo.readWriteType )
        {
            return false;
        }
        if( !equals( fieldInfo, tableInfo.fieldInfo ) )
        {
            return false;
        }
        if( !equals( indexInfo, tableInfo.indexInfo ) )
        {
            return false;
        }
        //noinspection RedundantIfStatement
        if( name != null ? !name.equals( tableInfo.name ) : tableInfo.name != null )
        {
            return false;
        }

        return true;
    }

    public static boolean equals(Object[] a, Object[] a2) {
        if( a == a2 )
        {
            return true;
        }
        if( a == null || a2 == null )
        {
            return false;
        }

        int length = a.length;
        if( a2.length != length )
        {
            return false;
        }

        for (int i=0; i<length; i++) {
            Object o1 = a[i];
            Object o2 = a2[i];
            if( !( o1 == null ? o2 == null : o1.equals( o2 ) ) )
            {
                return false;
            }
        }

        return true;
    }

    public int hashCode()
    {
        int result;
        result = ( name != null ? name.hashCode() : 0 );
        result = 29 * result + readWriteType;
        return result;
    }
}

