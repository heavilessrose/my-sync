package bm.core.io;

import bm.core.Properties;
import bm.core.math.FixedPoint;
import bm.core.tools.DeviceInfo;

import java.util.Hashtable;
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
 * Created on       : 20-nov-2007 17:55:48
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * The SerializableFactory is place where different libraries or programs can
 * register their serializable classes. This serves as a replacement for
 * Class.forName that works when you use an obfuscator, since the class will
 * hold the class name as a String that won't change, and a reference to the
 * class that will change accordingly to the obfuscator job.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SerializableFactory
{
    private static Hashtable map = new Hashtable( 10 );
    static
    {
        // Register Serializable classes known to this project

        map.put( "bm.core.Properties", new Properties().getClass() );
        map.put( "bm.core.math.FixedPoint", new FixedPoint().getClass() );
        map.put( "bm.core.tools.DeviceInfo", new DeviceInfo().getClass() );
    }

    /**
     * Register a serializable class.
     *
     * @param className must match the value returned by the getName method of
     * the Serializable class
     * @param clazz the class itself
     */
    public static void register( final String className, final Class clazz )
    {
        if( !map.containsKey( className ) )
        {
            map.put( className, clazz );
        }
    }

    /**
     * Get a class.
     *
     * @param className class name
     * @return class or null if not found
     */
    public static Class lookup( final String className )
    {
        return (Class) map.get( className );
    }
}
