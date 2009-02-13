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
 * Created on       : 07-dic-2006 11:30:05
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-08-06 14:58:22 +0200 (lun, 06 ago 2007) $
 * Revision         : $Revision: 10 $
 */

/**
 * Database header record.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 10 $
 */
class DatabaseHeader
    implements Serializable
{
    private short   versionMajor;
    private short   versionMinor;
    private short   engineVersion = Database.ENGINE_VERSION;

    public DatabaseHeader()
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
        return "bm.db.DatabaseHeader";
    }

    public short getVersionMajor()
    {
        return versionMajor;
    }

    public void setVersionMajor( final short versionMajor )
    {
        this.versionMajor = versionMajor;
    }

    public short getVersionMinor()
    {
        return versionMinor;
    }

    public void setVersionMinor( final short versionMinor )
    {
        this.versionMinor = versionMinor;
    }

    public short getEngineVersion()
    {
        return engineVersion;
    }

    public void setEngineVersion( final short engineVersion )
    {
        this.engineVersion = engineVersion;
    }


    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 2 ); // Version
        out.writeShort( versionMajor );
        out.writeShort( versionMinor );
        out.writeShort( engineVersion );
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        final byte version = in.readByte();
        versionMajor        = in.readShort();
        versionMinor        = in.readShort();
        engineVersion       = in.readShort();
    }
}
