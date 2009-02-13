/*
 * Copyright (c) 2006 Elondra, S.L. All Rights Reserved.
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

import bm.storage.RSException;

import bm.storage.RecordStoreFullException;

/*
 * File Information
 *
 * Created on       : 09-ene-2006 17:16:51
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-05-15 03:11:14 +0200 (mar, 15 may 2007) $
 * Revision         : $Revision: 6 $
 */

/**
 * A ScrollSet reprsents a set of rows that can be scrolled up and down.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public interface ScrollSet
{
    /**
     * Move before the first record, if any.
     * @throws DBException on errors
     */
    void beforeFirst()
            throws DBException;

    /**
     * Move after the last record, if any.
     * @throws DBException on errors
     */
    void afterLast()
            throws DBException;
    /**
     * Move to the next row.
     * @return true if there's a next row, false otherwise
     * @throws DBException on errors
     */
    boolean next()
            throws DBException;

    /**
     * Move to the previous row.
     * @return true if there's a previous row, false otherwise
     * @throws DBException on errors
     */
    boolean previous()
            throws DBException;

    /**
     * Get the current row.
     * @return the current row or null if no current row
     * @throws bm.db.DBException on errors
     * @throws RSException on rs errors
     * @throws RecordStoreFullException if no more space is available on record store
     */
    Row getCurrent()
            throws DBException, RecordStoreFullException, RSException;

    /**
     * Release scroll setProperty resources.
     */
    void release();

    /**
     * Open the associated table, to improve sequential movement operations.
     * @throws bm.db.DBException on errors
     * @throws bm.storage.RSException on recordstore errors
     * @throws bm.storage.RecordStoreFullException if no space left
     */
    void openSource()
            throws RSException,
                   DBException, RecordStoreFullException;

    /**
     * Close the associated table.
     * @throws bm.db.DBException if no more space is left on record store
     * @throws bm.storage.RSException on recordstore errors
     */
    void closeSource()
            throws DBException,
                   RSException;

    /**
     * Get the current row.
     *
     * @return an int or -1 if there no rows or the pointer is at EOF or BOF
     * @throws bm.db.DBException on errors
     * @throws bm.storage.RecordStoreFullException if no more space is left on record store
     */
    int getCurrentRecordId()
            throws DBException,
                   RecordStoreFullException;

    /**
     * Get the row at a given position.
     *
     * @param position index of the row, from 0 to size() - 1
     * @return row or null if the position is invalid
     * @throws bm.db.DBException on errors
     * @throws bm.storage.RecordStoreFullException if no more space is left on record store
     * @throws bm.storage.RSException on recordstore errors
     */
    Row getRowAt( int position )
            throws DBException,
                   RecordStoreFullException,
                   RSException;

    /**
     * Get the recordId at a given position.
     *
     * @param position index of the row, from 0 to size() - 1
     * @return row or -1 if the position is invalid
     * @throws bm.db.DBException on errors
     * @throws bm.storage.RecordStoreFullException if no more space is left on record store
     */
    int getRecordIdAt( int position )
            throws DBException,
                   RecordStoreFullException;

    /**
     * Get the current position of the rowset pointer, from 0 to size - 1.
     * @return position
     */
    int getPosition();

    /**
     * Move the rowset pointer to the specified position.
     * @param newIndex new index
     */
    void goTo( int newIndex );

    /**
     * Get the size of the rowset. If it's filtered returns the size of the
     * filtered result, not the original size.
     *
     * @return rowset size
     */
    int size();
}
