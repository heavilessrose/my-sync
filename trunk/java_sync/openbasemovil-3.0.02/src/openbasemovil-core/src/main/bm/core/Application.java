/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.core;
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

import java.util.Timer;

/*
 * File Information
 *
 * Created on       : 12-dic-2005 17:31:05
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Entry point for ApplicationManagers and other Application wide settings
 * and things that can vary from one platform to another.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public final class Application
{
    private static ApplicationManager   manager;
    private static CommandFactory       commandFactory;
    private static Timer                timer = new Timer();

    public static ApplicationManager getManager()
    {
        return manager;
    }

    public static synchronized void setManager( final ApplicationManager manager )
    {
        Application.manager = manager;
    }

    public static CommandFactory getCommandFactory()
    {
        return commandFactory;
    }

    public static synchronized void setCommandFactory(
            final CommandFactory commandFactory
    )
    {
        Application.commandFactory = commandFactory;
    }

    public static Timer getTimer()
    {
        return timer;
    }
}
