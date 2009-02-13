/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db.index;
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
import bm.core.tools.Tools;

import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 13:29:56
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/*
    Binary format of IndexInfo:

    {int        order}
    {string     name}
    {string     storeName}
    {string     fieldName}
    {byte       keyType}
    [{boolean   caseSensitive}]

*/

/**
 * Index information.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
public class IndexInfo
    implements Serializable
{
    /**
     * Index keys of type String, with exact match.
     */
    public static final byte KT_STRING      = 0;
    /**
     * Index keys of type Long.
     */
    public static final byte KT_LONG        = 1;
    /**
     * Index keys of type String, with partial match.
     */
    public static final byte KT_FULL_TEXT   = 2;
    /**
     * Minimum order for indexes.
     */
    public static final int MIN_ORDER   = 2;

    private int         order;
    private String      name;
    private String[]    fieldNames;
    private byte        keyType;
    private boolean     caseSensitive;

    private transient String    fieldExpression;
    private transient Hashtable fieldMap = new Hashtable( 10 );

    /**
     * Create an empty index info structure.
     */
    public IndexInfo()
    {
        order = MIN_ORDER;
    }

    /**
     * Create an IndexInfo structure.
     *
     * @param order index order, if below MIN_ORDER then MIN_ORDER is used
     * @param name index name
     * @param fieldName field over wich the index will be constructed
     * @param keyType key type, one of KT_LONG, KT_STRING or KT_FULL_TEXT
     * @param caseSensitive if the keys are considered case sensitive
     */
    public IndexInfo(
            final int       order,
            final String    name,
            final String    fieldName,
            final byte      keyType,
            final boolean   caseSensitive
    )
    {
        this.order = order < MIN_ORDER ? MIN_ORDER : order;
        this.name = name.toLowerCase();
        this.fieldNames = new String[] { fieldName };
        this.keyType = keyType;
        this.caseSensitive = caseSensitive;
        fieldExpression = fieldName;
    }

    /**
     * Create an IndexInfo structure.
     *
     * @param order index order, if below MIN_ORDER then MIN_ORDER is used
     * @param name index name
     * @param fieldNames fields over wich the index will be constructed
     * @param keyType key type, one of KT_LONG, KT_STRING or KT_FULL_TEXT
     * @param caseSensitive if the keys are considered case sensitive
     */
    public IndexInfo(
            final int       order,
            final String    name,
            final String[]  fieldNames,
            final byte      keyType,
            final boolean   caseSensitive
    )
    {
        this.order = order < MIN_ORDER ? MIN_ORDER : order;
        this.name = name.toLowerCase();
        setFieldNames( fieldNames );
        this.keyType = keyType;
        this.caseSensitive = caseSensitive;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.index.IndexInfo";
    }

    /**
     * Get index name.
     *
     * @return index name
     */
    public String getName()
    {
        return name;
    }

    /**
     * Set index name.
     *
     * @param name index name
     */
    public void setName( final String name )
    {
        if( name != null )
        {
            this.name = name.toLowerCase();
        }
        else
        {
            this.name = null;
        }
    }

    /**
     * Get index key type.
     *
     * @return key type
     */
    public byte getKeyType()
    {
        return keyType;
    }

    /**
     * Set index key type.
     *
     * @param keyType key type
     */
    public void setKeyType( final byte keyType )
    {
        this.keyType = keyType;
    }

    /**
     * Check if index is case sensitive.
     *
     * @return case sensitive
     */
    public boolean isCaseSensitive()
    {
        return caseSensitive;
    }

    /**
     * Set if index is case sensitive.
     *
     * @param caseSensitive case sensitive
     */
    public void setCaseSensitive( final boolean caseSensitive )
    {
        this.caseSensitive = caseSensitive;
    }

    /**
     * Get index order.
     *
     * @return order
     */
    public int getOrder()
    {
        return order;
    }

    /**
     * Set index order. If below MIN_ORDER then MIN_ORDER is used.
     *
     * @param order order
     */
    public void setOrder( final int order )
    {
        this.order = order < MIN_ORDER ? MIN_ORDER : order;
    }

    /**
     * Get the field expression for the index. If just one field just the field
     * name, if more than one, then a concatenation of fields names using a plus
     * sign as a separator between them.
     *
     * @return field expression
     */
    public String getFieldExpression()
    {
        return fieldExpression;
    }

    /**
     * Check if a field is indexed in this index.
     *
     * @param name field name
     * @return true if so
     */
    public boolean containsField( final String name )
    {
        return fieldMap.containsKey( name.toLowerCase() );
    }

    /**
     * Get the field names for this index.
     *
     * @return field names
     */
    public String[] getFieldNames()
    {
        return fieldNames;
    }

    /**
     * Set field names
     * @param fieldNames field names
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public void setFieldNames( final String[] fieldNames )
    {
        if( fieldNames != null )
        {
            fieldMap.clear();
            final StringBuffer expression = new StringBuffer();
            final int count = fieldNames.length;
            this.fieldNames = new String[ count ];
            int j = 0;
            for( int i = 0; i < count; i++ )
            {
                if( fieldNames[i] != null )
                {
                    if( j > 0 )
                    {
                        expression.append( "+" );
                    }
                    final String name = fieldNames[i].toLowerCase();
                    this.fieldNames[j++] = name;
                    expression.append( name );
                    fieldMap.put( name, name );
                }
            }
            if( j < count )
            {
                final String[] aux = new String[ j ];
                System.arraycopy( this.fieldNames, 0, aux, 0, j );
                this.fieldNames = aux;
            }
            fieldExpression = expression.toString();
        }
    }

    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // Version
        out.writeInt( order );
        out.writeString( name.toLowerCase() );
        final int length = fieldNames.length;
        out.writeByte( (byte) length );
        for( int i = 0; i < length; i++ )
        {
            out.writeString( fieldNames[i].toLowerCase() );
        }
        out.writeByte( keyType );
        if( keyType == KT_STRING )
        {
            out.writeBoolean( caseSensitive );
        }
    }

    /*
            ( IndexInfo:
                {byte:      version}
                {int:       order}
                {String:    name}
                {byte:      fieldCount}
                {String:    fieldName}+
                {byte:      indexType}
                [ if( indexType == KT_STRING )
                    {boolean:   caseSensitive}
                ]
            )*

     */
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // Skip version info
        order = in.readInt();
        name = in.readString().toLowerCase();
        final int count = in.readByte();
        fieldNames = new String[count];
        final StringBuffer expression = new StringBuffer();
        for( int i = 0; i < count; i++ )
        {
            fieldNames[i] = in.readString().toLowerCase();
            if( i > 0 )
            {
                expression.append( "+" );
            }
            expression.append( fieldNames[i] );
        }
        keyType = in.readByte();
        if( keyType == KT_STRING )
        {
            caseSensitive = in.readBoolean();
        }
        fieldExpression = expression.toString();
    }

    public String toString()
    {
        final String[] fieldNames = this.fieldNames;
        final StringBuffer toString = new StringBuffer();
        toString.append( "IndexInfo{" + "order=" )
                .append( order )
                .append( ", name='" )
                .append( name )
                .append( "'" + ", fieldNames='" );
        if( fieldNames != null )
        {
            toString.append( "[" );
            final int length = fieldNames.length;
            for( int i = 0; i < length; i++ )
            {
                if( i > 0 )
                {
                    toString.append( "," );
                }
                toString.append( fieldNames[i] );
            }
            toString.append( "]" );
        }
        else
        {
            toString.append( "null" );
        }
        toString.append( "'" + ", keyType=" )
                .append( keyType )
                .append( ", caseSensitive='" )
                .append( caseSensitive )
                .append( "', fieldExpression='" )
                .append( fieldExpression )
                .append( "'}" );
        return toString.toString();
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

        final IndexInfo indexInfo = (IndexInfo) o;

        if( caseSensitive != indexInfo.caseSensitive )
        {
            return false;
        }
        if( keyType != indexInfo.keyType )
        {
            return false;
        }
        if( order != indexInfo.order )
        {
            return false;
        }
        if( (fieldNames == null && indexInfo.fieldNames != null) ||
                (fieldNames != null && indexInfo.fieldNames == null)
                )
        {
            return false;
        }
        if( fieldNames.length != indexInfo.fieldNames.length )
        {
            return false;
        }
        final int count = fieldNames.length;
        for( int i = 0; i < count; i++ )
        {
            if( !Tools.objectEquals( fieldNames[i], indexInfo.fieldNames[i] ) )
            {
                return false;
            }
        }
        if( name != null ? !name.equals( indexInfo.name ) : indexInfo.name != null )
        {
            return false;
        }

        return true;
    }

    public int hashCode()
    {
        int result;
        result = order;
        result = 29 * result + ( name != null ? name.hashCode() : 0 );
        result = 29 * result + ( fieldNames != null ? fieldNames.hashCode() : 0 );
        result = 29 * result + (int) keyType;
        result = 29 * result + ( caseSensitive ? 1 : 0 );
        return result;
    }
}
