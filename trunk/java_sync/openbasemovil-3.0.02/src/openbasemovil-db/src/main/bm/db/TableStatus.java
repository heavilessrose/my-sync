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
import bm.core.tools.Tools;
import bm.storage.RSException;

import bm.storage.RecordStoreFullException;
import java.util.Date;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 13:49:17
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-05-15 03:11:14 +0200 (mar, 15 may 2007) $
 * Revision         : $Revision: 6 $
 */

/**
 * Table status information (mutable information)
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class TableStatus
    implements Serializable
{
    private boolean     changed;
    private Date        lastUpdate;
    private String      tableName;
    private boolean     locked;
    private boolean     damaged;
    private boolean     requestFullTable;
    private int         modificationCount;

    private transient Database    parent;
    private transient int       recordId;

    public TableStatus( final Database parent )
    {
        this.parent = parent;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.TableStatus";
    }

    public boolean isChanged()
    {
        return changed;
    }

    public void setChanged( final boolean changed )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( this.changed != changed )
        {
            this.changed = changed;
            parent.updateTableStatus( this );
        }
    }

    public Date getLastUpdate()
    {
        return lastUpdate;
    }

    public void setLastUpdate( final Date lastUpdate )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( !Tools.objectEquals( this.lastUpdate, lastUpdate ) )
        {
            this.lastUpdate = lastUpdate;
            parent.updateTableStatus( this );
        }
    }

    public boolean isLocked()
    {
        return locked;
    }

    public void setLocked( final boolean locked )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        if( locked != this.locked )
        {
            this.locked = locked;
            parent.updateTableStatus( this );
        }
    }

    public boolean isDamaged()
    {
        return damaged;
    }

    public void setDamaged( final boolean damaged )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        if( damaged != this.damaged )
        {
            this.damaged = damaged;
            parent.updateTableStatus( this );
        }
    }

    public boolean isRequestFullTable()
    {
        return requestFullTable;
    }

    public void setRequestFullTable( final boolean requestFullTable )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        if( requestFullTable != this.requestFullTable )
        {
            this.requestFullTable = requestFullTable;
            parent.updateTableStatus( this );
        }
    }

    /**
     * Modification count keeps track of how many changes (deletes and updates)
     * have been performed on the associated table.
     * @return modification count
     */
    public int getModificationCount()
    {
        return modificationCount;
    }

    public void setModificationCount( final int modificationCount )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        if( modificationCount != this.modificationCount )
        {
            this.modificationCount = modificationCount;
            parent.updateTableStatus( this );
        }
    }

    public void incModificationCount()
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        this.modificationCount++;
        parent.updateTableStatus( this );
    }

    public int getRecordId()
    {
        return recordId;
    }

    public void setRecordId( final int recordId )
    {
        this.recordId = recordId;
    }

    public String getTableName()
    {
        return tableName;
    }

    public void setTableName( final String tableName )
    {
        this.tableName = tableName;
    }

    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // Version
        out.writeNullableString( tableName, Constants.MAX_TABLE_NAME_LENGTH );
        out.writeNullableDate( lastUpdate );
        out.writeBoolean( locked );
        out.writeBoolean( changed );
        out.writeBoolean( damaged );
        out.writeBoolean( requestFullTable );
        out.writeInt( modificationCount );
    }

    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // Skip version
        tableName           = in.readNullableString();
        lastUpdate          = in.readNullableDate();
        locked              = in.readBoolean();
        changed             = in.readBoolean();
        damaged             = in.readBoolean();
        requestFullTable    = in.readBoolean();
        modificationCount   = in.readInt();
    }
}
