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

import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;
import bm.core.CoreConstants;

import java.io.InputStream;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 23-may-2007 20:01:59
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Extension to SerializerInputStream that allows saving Row objects so they
 * can be afterwards fully restored and reattached to the right table and
 * database.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DBSerializerInputStream
    extends SerializerInputStream
{
    public DBSerializerInputStream( final InputStream in )
    {
        super( in );
    }

    public Object readObject()
            throws SerializationException
    {
        try
        {
            final byte type = in.readByte();
            if( type == DBSerializerOutputStream.ROW )
            {
                final Integer rid = readNullableInt();
                final String tableName = readString();
                final String dbName = readString();
                final Database db = Database.connect( dbName );
                if( db == null )
                {
                    throw new SerializationException(
                            CoreConstants.ERR_SOS,
                            "Invalid database: " + dbName
                    );
                }
                final Table table = db.getTable( tableName );
                if( table == null )
                {
                    throw new SerializationException(
                            CoreConstants.ERR_SOS,
                            "Invalid table: " + tableName
                    );
                }
                final Row row = table.createRow();
                row.deserialize( this );
                row.setRecordId( rid );
                return row;
            }
            else
            {
                return parseObject( type );
            }
        }
        catch( IOException e )
        {
            throw new SerializationException( CoreConstants.ERR_SOS, e );
        }
    }
}
