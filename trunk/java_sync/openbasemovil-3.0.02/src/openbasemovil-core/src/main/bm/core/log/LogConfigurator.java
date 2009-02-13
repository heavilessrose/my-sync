/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;
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
/**
 * LogImpl must implement this interface in order to be able to configure the
 * instance, as the Log interface must remain clean for logging purpouses.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface LogConfigurator
{
    /**
     * Set the name (class or package) that this instance represents.
     *
     * @param name instance name
     */
    void setName( String name );
}
