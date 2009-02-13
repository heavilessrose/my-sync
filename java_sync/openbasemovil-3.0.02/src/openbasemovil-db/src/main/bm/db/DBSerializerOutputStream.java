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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.CoreConstants;

import java.io.OutputStream;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 23-may-2007 19:50:35
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Extension to SerializerOutputStream that allows saving Row objects so they
 * can be afterwards fully restored and reattached to the right table and
 * database.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DBSerializerOutputStream
    extends SerializerOutputStream
{
    public static final int ROW = 20;

    public DBSerializerOutputStream( final OutputStream out )
    {
        super( out );
    }


    /**
     * Write an non-null object to a Stream.<br/>
     * Objects can be of one of these types:
     * Row, Serializable, String, Long, Byte, Date, Boolean, Integer, Short, byte[].<br/>
     * They can also be collections of objects of the above types, to any nesting
     * level. Collections can be simple arrays, Vector or Hashtable.
     *
     * @param o value to write
     * @throws SerializationException on serialization exceptions
     */
    public void writeObject( final Object o )
            throws SerializationException
    {
        if( o instanceof Row )
        {
            try
            {
                out.writeByte( ROW );
                final Row row = (Row) o;
                writeInt( row.getRecordId() );
                writeString( row.getParent().getName() );
                writeString( row.getParent().getParent().getName() );
                row.serialize( this );
            }
            catch( IOException e )
            {
                throw new SerializationException( CoreConstants.ERR_SOS, e );
            }
        }
        else
        {
            super.writeObject( o );
        }
    }
}
