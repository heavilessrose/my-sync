package bm.core.io;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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

/*
 * File Information
 *
 * Created on       : 29-mar-2007 18:50:02
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Dummy serializable object for testing.
*
* @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
* @version $Revision: 6 $
*/
public class DummySerializable
        implements Serializable
{
    private String value;

    public DummySerializable()
    {
    }

    public DummySerializable( final String value )
    {
        this.value = value;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getName()
    {
        return "DummySerializable";
    }

    public String getValue()
    {
        return value;
    }

    public void setValue( final String value )
    {
        this.value = value;
    }

    /**
 * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        out.writeString( value );
    }

    /**
 * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException on errors
     */
    public void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        value = in.readString();
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

        final DummySerializable that = (DummySerializable) o;

        if( value != null ? !value.equals( that.value ) : that.value != null )
        {
            return false;
        }

        return true;
    }

    public int hashCode()
    {
        return ( value != null ? value.hashCode() : 0 );
    }
}
