package bm.mvc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
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

import bm.db.Row;
import bm.db.DBException;
import bm.storage.RSException;

import bm.storage.RecordStoreFullException;
import bm.core.mvc.View;
/*
 * File Information
 *
 * Created on       : 20-mar-2007 3:40:07
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Used from data bound views to resolve fields not found in the binded row.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface FieldResolver
{
    /**
     * Resolve the field name for a given row.
     *
     * @param view view requesting resolution
     * @param fieldName field name
     * @param source the row @return value or null if not known
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space left
     * @throws bm.storage.RSException on errors
     * @return resolved object, null if not found
     */
    Object resolve( final View view, String fieldName, Row source )
            throws DBException,
                   RecordStoreFullException,
                   RSException;
}
