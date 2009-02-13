package bm.vm;

/* -----------------------------------------------------------------------------
    bmScript Scripting language for Mobile Devices
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

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 07-oct-2007 1:24:23
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Constants for vm package.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public final class Constants
{
    public static final int ERR_SCL_CLASS_NOT_FOUND     = -20001;

    public static final int ERR_MET_CLASS_NOT_FOUND     = -20011;

    public static final int ERR_EXPR_CLASS_NOT_FOUND    = -20021;

    public static final int ERR_BLOCK_CREATE_COMMAND    = -20031;

    public static final int ERR_EXPR_CREATE_COMMAND     = -20041;

    /** @noinspection MismatchedQueryAndUpdateOfCollection*/
    public static final Vector EMPTY = new Vector( 0 );
}
