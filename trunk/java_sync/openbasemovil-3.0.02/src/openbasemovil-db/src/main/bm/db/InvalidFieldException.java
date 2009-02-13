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
 * Created on       : 15-jul-2005 18:20:04
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/**
 * The field name or index does not exist on a row.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
public class InvalidFieldException
        extends RuntimeException
{
    public InvalidFieldException()
    {
        super();
    }

    public InvalidFieldException( final String message)
    {
        super(message);
    }
}
