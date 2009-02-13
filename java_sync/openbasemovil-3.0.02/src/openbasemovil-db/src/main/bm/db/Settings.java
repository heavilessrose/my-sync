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

import bm.core.log.Log;
import bm.core.log.LogFactory;

import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 23-may-2007 16:25:12
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The preferences database allows the application to store and retrieve
 * miscellaneous preference and configuration values.<br/>
 * The database can hold serializable objects which can be saved and loaded
 * directly from it. It uses a PersistentMap as the backend store.<br/>
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public final class Settings
{
    private static final String CLASS_NAME = "Settings";
    static final String STORE_NAME = "sys_preferences";

    private static Log log = LogFactory.getLog( CLASS_NAME );

    static PersistentMap map = new PersistentMap( STORE_NAME );

    private Settings()
    {
    }

    /**
     * Get an enumeration of all the keys stored in the database, included
     * the Preferences object.
     * @return enumeration of keys
     */
    public static Enumeration keys()
    {
        return map.getKeys();
    }

    /**
     * Reload settings from store.
     */
    public static synchronized void reload()
    {
        map = new PersistentMap( STORE_NAME );
    }

    // -------------------------------------------------------------------------
    // Property access
    // -------------------------------------------------------------------------

    /**
     * Get a property.
     *
     * @param key property key
     * @return property value or null if not found
     * @throws bm.db.DBException on errors reading from persistent storage
     */
    public static String getProperty( final String key )
            throws DBException
    {
        final Object value = map.getElement( key );
        return value != null ? value.toString() : null;
    }

    /**
     * Get a property as a long value.
     *
     * @param key property key
     * @param defValue default value if not found or not a number
     * @return long value of property or default value
     * @throws bm.db.DBException on errors reading from persistent storage
     */
    public static long getPropertyLong( final String key, final long defValue )
            throws DBException
    {
        final Object value = map.getElement( key );
        if( value == null )
        {
            return defValue;
        }
        else
        {
            if( value instanceof Long )
            {
                return ((Long) value).longValue();
            }
            else if( value instanceof Integer )
            {
                return ((Integer) value).intValue();
            }
            else if( value instanceof Short )
            {
                return ((Short) value).shortValue();
            }
            else if( value instanceof Byte )
            {
                return ((Byte) value).byteValue();
            }
            else
            {
                try
                {
                    return Long.parseLong( value.toString() );
                }
                catch( NumberFormatException e )
                {
                    log.error( e );
                    return defValue;
                }
            }
        }
    }

    /**
     * Set a property.
     * @param key property key
     * @param value property value
     * @throws bm.db.DBException on errors reading or writing to the record store.
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public static void setProperty(
            final String key,
            final String value
    )
            throws DBException
    {
        map.setElement( key, value );
    }

    public static Object get( final String key )
            throws DBException
    {
        return map.getElement( key );
    }

    public static PersistentMap getInstance()
    {
        return map;
    }
}
