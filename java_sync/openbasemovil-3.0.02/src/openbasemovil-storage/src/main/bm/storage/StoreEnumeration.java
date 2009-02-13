package bm.storage;

/* -----------------------------------------------------------------------------
    OpenBaseMovil Storage Library
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

import javax.microedition.rms.RecordEnumeration;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 3:51:54
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * StoreEnumeration is very simimilar to RecordStoreEnumeration..
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StoreEnumeration
{
    private Store               store;
    private RecordEnumeration enumeration;
    private Vector              buffer;

    public StoreEnumeration(
            final Store                 store,
            final RecordEnumeration     enumeration
    )
    {
        this.store = store;
        this.enumeration = enumeration;
    }

    public boolean hasNext()
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        feed();
        return (buffer != null && buffer.size() > 0);
    }

    public int numRecords()
    {
        return store.getNumRecords();
    }

    public int nextId()
            throws InvalidRecordIDException,
                   RSException,
                   RecordStoreFullException
    {
        feed();
        if( buffer != null && buffer.size() > 0 )
        {
            final Integer id = (Integer) buffer.firstElement();
            buffer.removeElementAt( 0 );
            return id.intValue();
        }
        else
        {
            throw new InvalidRecordIDException( Constants.ERR_STE_NEXT_ID );
        }
    }

    private void feed()
            throws InvalidRecordIDException,
                   RSException,
                   RecordStoreFullException
    {
        try
        {
            while( (buffer == null || buffer.size() == 0) && enumeration.hasNextElement() )
            {
                final int id = enumeration.nextRecordId();
                buffer = store.mapValidRecordIds( id );
            }
        }
        catch( javax.microedition.rms.InvalidRecordIDException e )
        {
            throw new InvalidRecordIDException( Constants.ERR_STE_FEED, e );
        }
    }

    public byte[] next()
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        return store.getRecord( nextId() );
    }

    public void destroy()
            throws RSException
    {
        enumeration.destroy();
        store.close();
    }
}
