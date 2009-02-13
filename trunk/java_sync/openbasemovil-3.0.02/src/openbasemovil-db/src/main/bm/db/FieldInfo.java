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

/*
 * File Information
 *
 * Created on       : 13-jul-2005 13:29:29
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-04-26 00:53:43 +0200 (jue, 26 abr 2007) $
 * Revision         : $Revision: 4 $
 */

/**
 * Field information.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 4 $
 */
public class FieldInfo
    implements Serializable
{
    private String      name;
    private byte        type;
    private boolean     nullable;
    private int         length;
    private boolean     disabled;
    private String      fkTable;

    public FieldInfo()
    {
    }

    public FieldInfo(
            final String    name,
            final byte      type,
            final boolean   nullable,
            final int       length
    )
    {
        this.name       = name.toLowerCase();
        this.type       = type;
        this.nullable   = nullable;
        this.length     = length;
    }

    public FieldInfo(
            final String    name,
            final byte      type,
            final boolean   nullable,
            final int       length,
            final String    fkTable
    )
    {
        this.name = name;
        this.type = type;
        this.nullable = nullable;
        this.length = length;
        this.fkTable = fkTable;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.FieldInfo";
    }

    public String getName()
    {
        return name;
    }

    public synchronized void setName( final String name )
    {
        this.name = name != null ? name.toLowerCase() : null;
    }

    public byte getType()
    {
        return type;
    }

    public synchronized void setType( final byte type )
    {
        this.type = type;
    }

    public boolean isNullable()
    {
        return nullable;
    }

    public synchronized void setNullable( final boolean nullable )
    {
        this.nullable = nullable;
    }

    public int getLength()
    {
        return length;
    }

    public synchronized void setLength( final int length )
    {
        this.length = length;
    }

    public boolean isDisabled()
    {
        return disabled;
    }

    public void setDisabled( final boolean disabled )
    {
        this.disabled = disabled;
    }

    public String getFkTable()
    {
        return fkTable;
    }

    public void setFkTable( final String fkTable )
    {
        this.fkTable = fkTable;
    }

    public synchronized void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // Version
        out.writeString( name );
        out.writeByte( type );
        out.writeBoolean( nullable );
        out.writeInt( length );
        out.writeBoolean( disabled );
        out.writeNullableString( fkTable );
    }
/*
            ( FieldInfo:
                {byte:      version}
                {String:    name}
                {byte:      type}
                {boolean:   nullable}
                {int:       length}
                {boolean:   disabled}
                [string:    fkTable]
            )+

 */

    public synchronized void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte();  // Skip version
        name        = in.readString().toLowerCase();
        type        = in.readByte();
        nullable    = in.readBoolean();
        length      = in.readInt();
        disabled    = in.readBoolean();
        fkTable     = in.readNullableString();
    }

    public String toString()
    {
        return "FieldInfo{" +
               "name='" + name + "'" +
               ", type=" + type +
               ", nullable=" + nullable +
               ", length=" + length +
               ", disabled=" + disabled +
               ", fkTable=" + fkTable +
               "}";
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

        final FieldInfo fieldInfo = (FieldInfo) o;

        if( length != fieldInfo.length )
        {
            return false;
        }
        if( nullable != fieldInfo.nullable )
        {
            return false;
        }
        if( type != fieldInfo.type )
        {
            return false;
        }
        if( disabled != fieldInfo.disabled )
        {
            return false;
        }
        if( fkTable != null ? !fkTable.equals( fieldInfo.fkTable ) : fieldInfo.fkTable != null )
        {
            return false;
        }
        //noinspection RedundantIfStatement
        if( name != null ? !name.equals( fieldInfo.name ) : fieldInfo.name != null )
        {
            return false;
        }
        return true;
    }

    public int hashCode()
    {
        int result;
        result = ( name != null ? name.hashCode() : 0 );
        result = 29 * result + ( fkTable != null ? fkTable.hashCode() : 0 );
        result = 29 * result + (int) type;
        result = 29 * result + ( nullable ? 1 : 0 );
        result = 29 * result + length;
        result = 29 * result + ( disabled ? 1 : 0 );
        return result;
    }
}
