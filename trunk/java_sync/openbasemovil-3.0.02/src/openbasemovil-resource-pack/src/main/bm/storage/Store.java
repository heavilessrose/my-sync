package bm.storage;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ResourcePacker, packs and checks resource files
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

import java.util.HashMap;
import java.util.Map;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 0:51:53
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * This Store implementation is an in memory table, useful for server data
 * preparation purpouses and serialization.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Store
{
    private String              name;
    private int                 multiplexer;
    private int                 size;
    private long                lastModified = System.currentTimeMillis();
    private int                 nextRecordId = 1;
    private Map                 records = new HashMap( 10 );

    public static Store get( final String name, final int multiplexer )
    {
        return new Store( name, multiplexer );
    }

    public Store( final String name, final int multiplexer )
    {
        this.name = name;
        this.multiplexer = multiplexer;
    }

    public int getSize()
    {
        return size;
    }

    public String getName()
    {
        return name;
    }

    public int getMultiplexer()
    {
        return multiplexer;
    }

    public int getNextRecordId()
    {
        return nextRecordId;
    }

    public int getNumRecords()
    {
        return records.size();
    }

    public synchronized void shutdown()
            throws RecordStoreFullException,
                   RSException
    {
    }

    public synchronized void drop()
            throws RSException,
                   RecordStoreFullException
    {
    }

    public synchronized void open()
            throws RSException,
                   RecordStoreFullException
    {
    }

    public synchronized void close()
            throws RSException
    {
    }

    public long getLastModified()
    {
        return lastModified;
    }

    public synchronized int getRecordSize( final int recordId )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        final Integer rid = new Integer( recordId );
        if( records.containsKey( rid ) )
        {
            return ((byte[]) records.get( rid )).length;
        }
        else
        {
            throw new InvalidRecordIDException( 0, Integer.toString( recordId ) );
        }
    }

    public synchronized byte[] getRecord( final int recordId )
            throws RSException,
                   InvalidRecordIDException
    {
        final Integer rid = new Integer( recordId );
        if( records.containsKey( rid ) )
        {
            return (byte[]) records.get( rid );
        }
        else
        {
            throw new InvalidRecordIDException( 0, Integer.toString( recordId ) );
        }
    }

    public synchronized void setRecord(
            final int       recordId,
            final byte[]    data,
            final int       offset,
            final int       length
    )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        final Integer rid = new Integer( recordId );
        if( records.containsKey( rid ) )
        {
            final byte[] old = (byte[]) records.get( rid );
            final byte[] target = new byte[ length ];
            System.arraycopy( data, offset, target, 0, length );
            records.put( rid, target );
            size = size - old.length + length;
            lastModified = System.currentTimeMillis();
        }
        else
        {
            throw new InvalidRecordIDException( 0, Integer.toString( recordId ) );
        }
    }

    public synchronized int addRecord( final byte[] data )
        throws RSException,
               RecordStoreFullException
    {
        return addRecord( data, 0, data.length );
    }

    public synchronized int addRecord(
            final byte[]    data,
            final int       offset,
            final int       length
    )
        throws RSException,
               RecordStoreFullException
    {
        final byte[] target = new byte[ length ];
        System.arraycopy( data, offset, target, 0, length );
        int recordId = nextRecordId;
        final Integer rid = new Integer( recordId );
        records.put( rid, target );
        size += length;
        nextRecordId++;
        lastModified = System.currentTimeMillis();
        return recordId;
    }

    public synchronized void deleteRecord( final int recordId )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        final Integer rid = new Integer( recordId );
        if( records.containsKey( rid ) )
        {
            final byte[] old = (byte[]) records.get( rid );
            records.remove( rid );
            size -= old.length;
            lastModified = System.currentTimeMillis();
        }
        else
        {
            throw new InvalidRecordIDException( 0, Integer.toString( recordId ) );
        }
    }


    public synchronized int getRecord(
            final int       recordId,
            final byte[]    buffer,
            final int       offset
    )
            throws RSException,
                   InvalidRecordIDException
    {
        final byte[] data = getRecord( recordId );
        System.arraycopy( data, 0, buffer, offset, data.length );
        return data.length;
    }

    public interface Listener
    {
        /**
         * Called by the Store when the underlying record store is really
         * opened and listener should perform a set up.
         */
        void rsOpen();

        /**
         * Called by the Store when the underlying record store is really
         * closed and listener should perform clean up.
         */
        void rsClose();
    }
}
