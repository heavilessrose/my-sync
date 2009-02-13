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

import bm.storage.RSException;
import bm.storage.StoreEnumeration;

import bm.storage.InvalidRecordIDException;
import bm.storage.RecordStoreFullException;
/*
 * File Information
 *
 * Created on       : 07-dic-2006 11:33:53
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-09-11 14:25:53 +0200 (mar, 11 sep 2007) $
 * Revision         : $Revision: 14 $
 */

/**
 * Miscellaneous database utility methods.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 14 $
 */
class DbTool
{
    public static int[] getSortedRecordIds( final StoreEnumeration re )
            throws InvalidRecordIDException,
                   RSException,
                   RecordStoreFullException
    {
        final int[] rids = new int[ re.numRecords() ];
        rids[0] = re.nextId();
        int length = 1;
        //noinspection MethodCallInLoopCondition
        while( re.hasNext() )
        {
            insertRecordId( rids, re.nextId(), 0, length - 1 );
            length++;
        }
        return rids;
    }

    private static void insertRecordId(
            final int[] rids,
            final int   recordId,
            final int   first,
            final int   last
    )
    {
        if( first == last )
        {
            if( rids[first] > recordId )
            {
                insertAt( rids, recordId, first );
            }
            else
            {
                insertAt( rids, recordId, first + 1 );
            }
        }
        else
        {
            final int middle = first + ((last - first)/2);
            if( rids[middle] < recordId )
            {
                insertRecordId( rids, recordId, middle + 1, last );
            }
            else
            {
                insertRecordId( rids, recordId, first, middle );
            }
        }
    }

    private static void insertAt( final int[] rids, final int recordId, final int pos )
    {
        System.arraycopy( rids, pos, rids, pos + 1, rids.length - pos - 1 );
        rids[ pos ] = recordId;
    }
}
