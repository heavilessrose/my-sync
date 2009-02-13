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

/*
 * File Information
 *
 * Created on       : 03-ago-2005 11:31:38
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision: 2 $
 */

/**
 * Simple way to filter records, but more efficiently than RecordFilters.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 2 $
 */
public interface RowFilter
{
    /**
     * Check if a row matches the filter criteria.
     *
     * @param row the row
     * @return true if so
     * @throws DBException on errors
     */
    boolean matches( Row row )
            throws DBException;
}
