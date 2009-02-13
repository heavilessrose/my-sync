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
import bm.core.CoreConstants;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Date;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 29-mar-2007 16:54:45
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Handles simple and complex deserialization of objects.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SerializerInputStream
    extends InputStream
{
    protected DataInputStream in;

    public SerializerInputStream( final InputStream in )
    {
        this.in = in instanceof DataInputStream ?
                  (DataInputStream) in :
                  new DataInputStream( in );
    }

    public int read() throws IOException
    {
        return in.read();
    }

    public int readInt()
            throws SerializationException
    {
        try
        {
            return in.readInt();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Integer readNullableInt()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Integer( in.readInt() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public byte readByte()
            throws SerializationException
    {
        try
        {
            return in.readByte();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Byte readNullableByte()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Byte( in.readByte() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public boolean readBoolean()
            throws SerializationException
    {
        try
        {
            return in.readBoolean();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Boolean readNullableBoolean()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Boolean( in.readBoolean() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public short readShort()
            throws SerializationException
    {
        try
        {
            return in.readShort();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Short readNullableShort()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Short( in.readShort() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public long readLong()
            throws SerializationException
    {
        try
        {
            return in.readLong();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Long readNullableLong()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Long( in.readLong() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Date readDate()
            throws SerializationException
    {
        try
        {
            return new Date( in.readLong() );
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Date readNullableDate()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return new Date( in.readLong() );
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public String readString()
            throws SerializationException
    {
        try
        {
            return in.readUTF();
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public String readNullableString()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return in.readUTF();
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public byte[] readBlob()
            throws SerializationException
    {
        try
        {
            final int length = in.readInt();
            if( length > 0 )
            {
                if( length < CoreConstants.MAX_BLOB_SIZE )
                {
                    // Reading the block at once should be faster, but it does
                    // not work properly under certain emulator versions
                    // this way it always works
                    final byte[] data = new byte[ length ];
                    for( int i = 0; i < length; i++ )
                    {
                        data[i] = in.readByte();
                    }
                    return data;
                }
                else
                {
                    throw new SerializationException(
                            CoreConstants.ERR_SOS,
                            "Invalid blob length: " + length
                    );
                }
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public byte[] readNullableBlob()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return readBlob();
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS );
        }
    }

    public Object readObject()
            throws SerializationException
    {
        try
        {
            final byte type = in.readByte();
            return parseObject( type );
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS, e );
        }
    }

    /**
     * This method receives the type and parses the following object.<br/>
     * This allows subclasses to override readObject, read the object type with
     * in.readByte(), make their work and then fall back to the default parser
     * that this method implements.
     *
     * @param type object type
     * @return object
     * @throws SerializationException on serialization errors
     * @throws IOException on io exceptions
     */
    protected Object parseObject( final byte type )
            throws SerializationException, IOException
    {
        switch( type )
        {
            case CoreConstants.BOB:
                return readBlob();

            case CoreConstants.ARRAY:
                {
                    final short size = in.readShort();
                    final Object[] array = new Object[ size ];
                    for( int i = 0; i < size; i++ )
                    {
                        array[i] = readNullableObject();
                    }
                    return array;
                }

            case CoreConstants.VECTOR:
                {
                    final short size = in.readShort();
                    final Vector list = new Vector( size );
                    for( int i = 0; i < size; i++ )
                    {
                        list.addElement( readObject() );
                    }
                    return list;
                }

            case CoreConstants.MAP:
                {
                    final short size = in.readShort();
                    final Hashtable map = new Hashtable( size );
                    for( int i = 0; i < size; i++ )
                    {
                        map.put( readObject(), readObject() );
                    }
                    return map;
                }

            case CoreConstants.SERIALIZABLE:
                try
                {
                    final String className = readString();
                    Class clazz = SerializableFactory.lookup( className );
                    if( clazz == null )
                    {
                        clazz = Class.forName( className );
                    }
                    final Serializable serializable = (Serializable)
                            clazz.newInstance();
                    serializable.deserialize( this );
                    return serializable;
                }
                catch( Exception e )
                {
                    throw new SerializationException(
                            CoreConstants.ERR_TOL_INVALID_SERIALIZABLE_OBJECT,
                            e
                    );
                }

            case CoreConstants.STRING:
                return readString();

            case CoreConstants.LONG:
                return new Long( in.readLong() );

            case CoreConstants.BYTE:
                return new Byte( in.readByte() );

            case CoreConstants.BOOLEAN:
                return new Boolean( in.readBoolean() );

            case CoreConstants.INTEGER:
                return new Integer( in.readInt() );

            case CoreConstants.SHORT:
                return new Short( in.readShort() );

            case CoreConstants.DATE:
                return new Date( in.readLong() );

            default:
                return readString();
        }
    }

    public Object readNullableObject()
            throws SerializationException
    {
        try
        {
            final boolean isNull = in.readBoolean();
            if( !isNull )
            {
                return readObject();
            }
            else
            {
                return null;
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS, e );
        }
    }

    public void close()
            throws IOException
    {
        super.close();
        in.close();
    }
}
