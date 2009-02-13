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

import bm.core.ResourceManager;
import bm.core.event.ProgressEvent;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.math.FixedPoint;
import bm.err.ErrorLog;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;

import java.util.Date;

/*
 * File Information
 *
 * Created on       : 13-jul-2005 17:04:42
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-05-15 03:11:14 +0200 (mar, 15 may 2007) $
 * Revision         : $Revision: 6 $
 */

/**
 * A set of rows, a result of a query.<br/>
 * A RowSet can be filtered and sorted.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class RowSet
        implements ScrollSet
{
    public static final int ASCENDING   = 1;
    public static final int DESCENDING  = -1;

    public static final byte SM_EXACT           = 0;
    public static final byte SM_STARTS_WITH     = 1;

    private static Log log = LogFactory.getLog( "RowSet" );

    int[]   recordIds;
    int[]   filtered;
    int     filteredCount;
    int     index = -1;
    Table   source;

    private Object[]    keys;

    /**
     * Constructor.
     *
     * @param source source table
     * @param recordIds collection of record ids
     * @param searchField search field
     * @param searchValue search value
     * @param searchMode search mode
     * @noinspection UnusedDeclaration
     */
    public RowSet(
            final Table source,
            final int[] recordIds,
            final String searchField,
            final Object searchValue,
            final byte searchMode
    )
    {
        this.source = source;
        this.recordIds = recordIds;
    }

    /**
     * Sort the row set, using ASCENDING mode.<br/>
     * Valid field types for sorting: String, int, short, long, date.
     *
     * @param fieldName fied name of the columnt to sort with, ascending
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public void sort( final String fieldName )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        sort( fieldName, ASCENDING );
    }

    /**
     * Sort the row set.<br/>
     * Valid field types for sorting: String, int, short, long, date.
     *
     * @param fieldName fied name of the columnt to sort with, ascending
     * @param mode ASCENDING or DESCENDING
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public void sort( final String fieldName, final int mode )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        final Integer index = (Integer) source.fieldMap.get(
                fieldName.toLowerCase()
        );
        if( index == null )
        {
            throw new InvalidFieldException( fieldName );
        }
        else
        {
            sort( index.intValue(), mode );
        }
    }

    /**
     * Sort the row set, using ASCENDING mode.<br/>
     * Valid field types for sorting: String, int, short, long, date.
     *
     * @param fieldIndex fied index of the columnt to sort with, ascending
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public void sort( final int fieldIndex )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        sort( fieldIndex, ASCENDING );
    }

    /**
     * Sort the row set.<br/>
     * Valid field types for sorting: String, int, short, long, date.
     *
     * @param fieldIndex fied index of the columnt to sort with, ascending
     * @param mode ASCENDING or DESCENDING
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public void sort( final int fieldIndex, final int mode )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( recordIds != null && recordIds.length > 0 )
        {
            try
            {
                if( keys == null )
                {
                    getKeys( fieldIndex );
                }
                // Local variable to improve speed on CDC
                if( keys != null )
                {
                    final int length = keys.length;
                    quickSort( mode, 0, length - 1 );
                }
            }
            finally
            {
                System.gc();
            }
        }
    }

    /**
     * Open the associated table, to improve sequential movement operations.
     * @throws DBException on errors
     * @throws RSException on recordstore errors
     * @throws RecordStoreFullException if no space left
     */
    public void openSource()
            throws RSException,
                   DBException,
                   RecordStoreFullException
    {
        if( source != null )
        {
            source.open();
        }
    }

    /**
     * Close the associated table.
     * @throws DBException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public void closeSource()
            throws DBException,
                   RSException
    {
        if( source != null )
        {
            source.close();
        }
    }

    private void getKeys( final int fieldIndex )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        final int[] recordIds = this.recordIds;
        final int length = recordIds.length;
        final Table source = this.source;
        try
        {
            source.open();
            keys = new Object[ length ];
            for( int i = 0; i < length; i++ )
            {
                try
                {
                    final Row row = source.fetch( recordIds[ i ] );
                    keys[ i ] = row.getField( fieldIndex );
                }
                catch( RecordNotFoundException e )
                {
                    ErrorLog.addError(
                            "RowSet",
                            "getKeys",
                            new Object[] { new Integer( fieldIndex ) },
                            "RecordNotFound@" + i + ": " + recordIds[i],
                            e
                    );
                    log.warn( e );
                    shift( i );
                }
            }
        }
        finally
        {
            source.close();
        }
    }

    /**
     * Sort the RowSet.<br/>
     *
     * Based on:<br/>
     * QuickSort.java by Henk Jan Nootenboom, 9 Sep 2002 Copyright 2002-2005
     * SUMit. All Rights Reserved.
     * <p/>
     * Algorithm designed by prof C. A. R. Hoare, 1962 See
     * http://www.sum-it.nl/en200236.html for algorithm improvement by Henk Jan
     * Nootenboom, 2002.
     * <p/>
     * Recursive Quicksort, sorts (part of) a Vector by 1.  Choose a pivot, an
     * element used for comparison 2.  dividing into two parts: - less than-equal
     * pivot - and greater than-equal to pivot. A element that is equal to the
     * pivot may end up in any part. See www.sum-it.nl/en200236.html for the
     * theory behind this. 3. Sort the parts recursively until there is only one
     * element left.
     * <p/>
     * www.sum-it.nl/QuickSort.java this source code www.sum-it.nl/quicksort.php3
     * demo of this quicksort in a java applet
     * <p/>
     * Permission to use, copy, modify, and distribute this java source code and
     * its documentation for NON-COMMERCIAL or COMMERCIAL purposes and without fee
     * is hereby granted. See http://www.sum-it.nl/security/index.html for
     * copyright laws.
     *
     * @param mode search mode (ASCENDING or DESCENDING)
     * @param lowIndex start index on the array
     * @param highIndex end index on the array
     * @noinspection MethodCallInLoopCondition
     */
    private void quickSort( final int mode, final int lowIndex, final int highIndex )
    {
        // We should exchange values no just in the cached array, but also
        // in the recordIds array to keep indexes in the same positions in
        // both arrays, but we don't do it because it's not really necessary
        // since once the array is filled, and it is previuosly to this call,
        // the original recordIds array won't be used any more.

        // Local variable to improve speed on CDC
        final Object[] keys = this.keys;
        final int[] recordIds = this.recordIds;

        int lowToHighIndex;
        int highToLowIndex;
        final int pivotIndex;
        final Object pivotValue;
        Object lowToHighValue;
        Object highToLowValue;
        Object parking;
        int newLowIndex;
        int newHighIndex;
        int compareResult;

        lowToHighIndex = lowIndex;
        highToLowIndex = highIndex;
        /*  Choose a pivot, remember it's value
         *  No special action for the pivot element itself.
         *  It will be treated just like any other element.
         */
        pivotIndex = ( lowToHighIndex + highToLowIndex ) / 2;
        pivotValue = keys[pivotIndex];

        /*  Split the Vector in two parts.
         *
         *  The lower part will be lowIndex - newHighIndex,
         *  containing elements <= pivot Value
         *
         *  The higher part will be newLowIndex - highIndex,
         *  containting elements >= pivot Value
         */
        newLowIndex = highIndex + 1;
        newHighIndex = lowIndex - 1;
        // loop until low meets high
        while( ( newHighIndex + 1 ) < newLowIndex )
        { // loop from low to high to find a candidate for swapping
            lowToHighValue = keys[lowToHighIndex];
            while( lowToHighIndex < newLowIndex
                   && compare( mode, lowToHighValue, pivotValue ) < 0 )
            {
                newHighIndex = lowToHighIndex; // add element to lower part
                lowToHighIndex ++;
                lowToHighValue = keys[lowToHighIndex];
            }

            // loop from high to low find other candidate for swapping
            highToLowValue = keys[highToLowIndex];
            while( newHighIndex <= highToLowIndex
                   && ( compare( mode, highToLowValue, pivotValue ) > 0 )
                    )
            {
                newLowIndex = highToLowIndex; // add element to higher part
                highToLowIndex --;
                highToLowValue = keys[highToLowIndex];
            }

            // swap if needed
            if( lowToHighIndex == highToLowIndex ) // one last element, may go in either part
            {
                newHighIndex = lowToHighIndex; // move element arbitrary to lower part
            }
            else if( lowToHighIndex < highToLowIndex ) // not last element yet
            {
                compareResult = compare( mode, lowToHighValue, highToLowValue );
                if( compareResult >= 0 ) // low >= high, swap, even if equal
                {
                    parking = lowToHighValue;
                    final int parkRecordId = recordIds[lowToHighIndex];
                    keys[lowToHighIndex] = highToLowValue;
                    recordIds[lowToHighIndex] = recordIds[highToLowIndex];
                    keys[highToLowIndex] = parking;
                    recordIds[highToLowIndex] = parkRecordId;

                    newLowIndex = highToLowIndex;
                    newHighIndex = lowToHighIndex;

                    lowToHighIndex ++;
                    highToLowIndex --;
                }
            }
        }

        // Continue recursion for parts that have more than one element
        if( lowIndex < newHighIndex )
        {
            this.quickSort( mode, lowIndex, newHighIndex ); // sort lower subpart
        }
        if( newLowIndex < highIndex )
        {
            this.quickSort( mode, newLowIndex, highIndex ); // sort higher subpart
        }
    }

    /**
     * Compare to rows on a given field.
     *
     * @param mode ASCENDING or DESCENDING
     * @param value1 first value
     * @param value2 second value
     * @return 0 if both fields are null, -1 if first is null,
     * 1 if second is null, otherwise -1 if field 1 is smaller,
     * 0 if both are equal, 1 if field2 is smaller
     */
    private int compare(
            final int       mode,
            final Object    value1,
            final Object    value2
    )
    {
        if( value1 == null && value2 == null )
        {
            return 0;
        }
        else if( value1 == null )
        {
            return -1 * mode;
        }
        else if( value2 == null )
        {
            return mode;
        }
        else
        {
            int result;
            if( value1 instanceof Short )
            {
                final short short1 = ((Short) value1).shortValue();
                final short short2 = ((Short) value2).shortValue();
                result = short1 == short2 ?
                       0 :
                       short1 < short2 ? -1 : 1;
            }
            else if( value1 instanceof Integer )
            {
                final int int1 = ((Integer) value1).intValue();
                final int int2 = ((Integer) value2).intValue();
                result = int1 == int2 ?
                       0 :
                       int1 < int2 ? -1 : 1;
            }
            else if( value1 instanceof Long )
            {
                final long long1 = ((Long) value1).longValue();
                final long long2 = ((Long) value2).longValue();
                result = long1 == long2 ?
                       0 :
                       long1 < long2 ? -1 : 1;
            }
            else if( value1 instanceof Date )
            {
                final long long1 = ((Date) value1).getTime();
                final long long2 = ((Date) value2).getTime();
                result = long1 == long2 ?
                       0 :
                       long1 < long2 ? -1 : 1;
            }
            else if( value1 instanceof FixedPoint )
            {
                final FixedPoint fp1 = (FixedPoint) value1;
                final FixedPoint fp2 = (FixedPoint) value2;
                result = fp1.compareTo( fp2 );
            }
            else
            {
                result = value1.toString().compareTo( value2.toString() );
            }
            return result * mode;
        }
    }

    /**
     * Apply a fiter on the Rowset.
     *
     * @param filter filter
     * @throws DBException on errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public void applyFilter( final RowFilter filter )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        clearFilter();
        // Local variables to improve speed on CDC
        final int[]     recordIds   = this.recordIds;
        if( recordIds != null && recordIds.length > 0 )
        {
            // Local variables to improve speed on CDC
            final int       count       = recordIds.length;
            final int[]     filtered    = new int[ recordIds.length ];

            final ProgressEvent event = new ProgressEvent(
                    this,
                    new Integer( count ),
                    0
            );
            event.setPhase( ResourceManager.getResource(
                    "db.table.rowSet.Filtering"
            ) );
            event.dispatch();
            int filterCount = 0;
            for( int i = 0; i < count; i++ )
            {
                final Row row;
                try
                {
                    row = source.fetch( recordIds[ i ] );
//                    sanityCheck( row );
                    if( filter.matches( row ) )
                    {
                        filtered[ filterCount++ ] = row.getRecordId().intValue();
                    }
                }
                catch( RecordNotFoundException e )
                {
                    ErrorLog.addError(
                            "RowSet",
                            "applyFilter",
                            new Object[] { filter },
                            "RecordNotFound@" + i + ": " + recordIds[i],
                            e
                    );
                    log.warn( e );
                    shift( i );
                }
                event.increment();
                event.dispatch();
            }
            this.filteredCount  = filterCount;
            if( filterCount > 0 )
            {
                this.filtered = new int[ filterCount ];
                System.arraycopy( filtered, 0, this.filtered, 0, filterCount );
            }
            else
            {
                this.filtered = new int[1];
            }
        }
    }

    /**
     *
     * @param pointer location to shift
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    private void shift( final int pointer )
    {
        final int[] aux = new int[ recordIds.length - 1 ];
        System.arraycopy( recordIds, 0, aux, 0, pointer );
        System.arraycopy( recordIds, pointer + 1, aux, pointer, aux.length - pointer );
        recordIds = aux;
    }

    /**
     * If there's a filter applied, clear it.
     */
    public void clearFilter()
    {
        if( filtered != null )
        {
            filtered = null;
            index = -1;
            // Make some root (needed by pocket pc)
            System.gc();
            Thread.yield();
        }
    }

    /**
     * Check if there's a next row in the setProperty.
     *
     * @return true if there's a next row
     */
    public boolean hasNext()
    {
        if( filtered == null )
        {
            return recordIds != null && index < recordIds.length - 1;
        }
        else
        {
            return index < filteredCount - 1;
        }
    }

    /**
     * Check if there's a previous row in the setProperty.
     *
     * @return true if there's a previous row in the setProperty
     */
    public boolean hasPrevious()
    {
        if( filtered == null )
        {
            return recordIds != null && index > 0;
        }
        else
        {
            return index > 0;
        }
    }

    /**
     * Move to the next row.
     *
     * @return true if there was a next row
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public boolean next()
    {
        if( filtered == null )
        {
            if( recordIds != null )
            {
                index++;
                if( index >= recordIds.length )
                {
                    index = recordIds.length;
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else if( filteredCount > 0 )
        {
            index++;
            if( index >= filteredCount )
            {
                index = filteredCount;
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    /**
     * Move to the previous row.
     *
     * @return true if there was a previus row
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public boolean previous()
    {
        if( recordIds != null )
        {
            index--;
            if( index <= -1 )
            {
                index = -1;
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    /**
     * Get the size of the rowset. If it's filtered returns the size of the
     * filtered result, not the original size.
     *
     * @return rowset size
     */
    public int size()
    {
        if( filtered == null )
        {
            return recordIds != null ? recordIds.length : 0;
        }
        else
        {
            return filteredCount;
        }
    }

    /**
     * Get the size of the underlying row setProperty, no matter if it's filtered or not.
     *
     * @return real size of the rowset
     */
    public int realSize()
    {
        return recordIds != null ? recordIds.length : 0;
    }

    /**
     * Get the current row.
     *
     * @return a Row object or null if there no rows or the pointer is at EOF
     * or BOF
     * @throws DBException on errors
     */
    public Row getCurrent()
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        return getRowAt( index );
    }

    /**
     * Get the current row.
     *
     * @return an int or -1 if there no rows or the pointer is at EOF or BOF
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     */
    public int getCurrentRecordId()
            throws DBException,
                   RecordStoreFullException
    {
        return getRecordIdAt( index );
    }

    /**
     * Get the row at a given position.
     *
     * @param position index of the row, from 0 to size() - 1
     * @return row or null if the position is invalid
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     */
    public Row getRowAt( final int position )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        final int recordId = getRecordIdAt( position );
        if( position == -1 )
        {
            return null;
        }
        else
        {
            return source.fetch( recordId );
        }
    }

    /**
     * Get the recordId at a given position.
     *
     * @param position index of the row, from 0 to size() - 1
     * @return row or -1 if the position is invalid
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     */
    public int getRecordIdAt( final int position )
            throws DBException,
                   RecordStoreFullException
    {
        // Local variables to improve speed on CDC
        final int[]     recordIds   = this.recordIds;
        final int[]     filtered    = this.filtered;

        if( recordIds == null )
        {
            return -1;
        }

        int recordId = -1;
        if( filtered != null )
        {
            if( position >= 0 && position < filteredCount )
            {
                recordId = filtered[position];
            }
        }
        else if( position >= 0 && position < recordIds.length )
        {
            recordId = recordIds[ position ];
        }
        return recordId;
    }

    /**
     * Packs the current filtered results making them the base results, so
     * the rowset can be refined by other filters. The operation can't be undone
     * as the original results are lost.
     */
    public void packFilter()
    {
        if( filtered != null )
        {
            if( filteredCount > 0 )
            {
                recordIds = filtered;
            }
            else
            {
                recordIds = null;
            }
            clearFilter();
        }
    }

    /**
     * Check if the row setProperty is filtered.
     *
     * @return true if it's filtered
     */
    public boolean isFiltered()
    {
        return filtered != null;
    }

    /**
     * Move the pointer to BOF.
     */
    public void beforeFirst()
    {
        index = -1;
    }

    /**
     * Move the pointer to EOF.
     */
    public void afterLast()
    {
        if( filtered == null )
        {
            if( recordIds != null )
            {
                index = recordIds.length;
            }
        }
        else
        {
            index = filteredCount;
        }
    }

    /**
     * Get the current position of the rowset pointer, from 0 to size - 1.
     * @return position
     */
    public int getPosition()
    {
        return index;
    }

    /**
     * Move the rowset pointer to the specified position.
     * @param newIndex new index
     */
    public void goTo( final int newIndex )
    {
        // Local variables to improve speed on CDC
        final int[] recordIds = this.recordIds;
        final int[] filtered = this.filtered;

        final int   index;
        if( newIndex < 0 )
        {
            index = -1;
        }
        else if( filtered == null )
        {
            if( recordIds == null )
            {
                index = -1;
            }
            else if( newIndex >= recordIds.length )
            {
                index = recordIds.length;
            }
            else
            {
                index = newIndex;
            }
        }
        else
        {
            if( newIndex >= filteredCount )
            {
                index = filteredCount;
            }
            else
            {
                index = newIndex;
            }
        }
        this.index = index;
    }

    /**
     * Release ScrollSet resources.
     */
    public void release()
    {
        recordIds   = null;
        filtered    = null;
        source      = null;
        keys        = null;
    }
}
