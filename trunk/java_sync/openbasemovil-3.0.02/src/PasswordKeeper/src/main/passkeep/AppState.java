/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package passkeep;
/* -----------------------------------------------------------------------------
    AppTemplate, an application for building defect management built with BaseMovil
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

import bm.db.DBException;
import bm.util.ApplicationState;

/*
 * File Information
 *
 * Created on       : 29-jul-2005 13:03:47
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2005/11/30 10:21:30 $
 * Revision         : $Revision: 1.2 $
 */

/**
 * Collection of objects used between different views.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 1.2 $
 */
public final class AppState
    extends ApplicationState
{
    public static void set( final Object key, final Object value )
            throws DBException
    {
        map.setElement( key, value );
    }

    public static Object get( final Object key )
            throws DBException
    {
        return map.getElement( key );
    }

    private static int screenWidth, screenHeight;
    private static String password;

    public static int getScreenHeight()
    {
        return screenHeight;
    }

    public static void setScreenHeight( final int screenHeight )
    {
        AppState.screenHeight = screenHeight;
    }

    public static int getScreenWidth()
    {
        return screenWidth;
    }

    public static void setScreenWidth( final int screenWidth )
    {
        AppState.screenWidth = screenWidth;
    }

    public static String getPassword()
    {
        return password;
    }

    public static void setPassword( final String password )
    {
        AppState.password = password;
    }

    public static void clearState()
            throws DBException
    {
        clearTransient();
    }
}
