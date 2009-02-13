package bm.core.res;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
 * Created on       : 15-feb-2007 23:32:56
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * A ResourceProvider provides access to a resource store.
*
* @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
* @version $Revision: 6 $
*/
public interface ResourceProvider
{
    /**
     * Set the active locale for resource look up.
     * @param locale active locale
     */
    void setLocale( String locale );

    /**
     * Get a resource given it's key.
     * @param key resource key
     * @return resource if found, null if not found.
     */
    String getResource( String key );

    /**
     * Open any underlying resource so subsequent calls to getResource go faster.
     */
    void open();

    /**
     * Close any underlying resource previously open.
     */
    void close();
}
