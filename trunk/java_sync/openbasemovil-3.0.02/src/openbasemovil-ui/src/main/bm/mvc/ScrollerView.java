/*
 * Copyright (c) 2006 Elondra S.L.. All Rights Reserved.
 */
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

import bm.storage.RecordStoreFullException;

import bm.db.DBException;
import bm.db.ScrollSet;
import bm.storage.RSException;

/*
 * File Information
 *
 * Created on       : 09-ene-2006 17:13:50
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A ScrollerView can scroll results through a ScrollSet.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface ScrollerView
{
    /**
     * Set the scroll set.
     * @param scrollSet scroll set
     * @throws RecordStoreFullException if rs is out of space
     * @throws DBException on db errors
     * @throws bm.storage.RSException on errors
     */
    void setScrollSet( ScrollSet scrollSet )
            throws DBException,
                   RecordStoreFullException,
                   RSException;

    /**
     * Get the scroll set.
     * @return scroll set
     */
    ScrollSet getScrollSet();
}
