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
import bm.core.Properties;
import bm.core.ResourceManager;
import bm.core.log.LogFactory;
import bm.core.log.Log;

import java.io.IOException;
import java.io.InputStream;
/*
 * File Information
 *
 * Created on       : 15-feb-2007 23:37:45
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * ResourceProvider that gets resources from properties files.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class PropertyResourceProvider
    implements ResourceProvider
{
    private static final Log log = LogFactory.getLog( "PropertyResourceProvider" );

    private Properties  bundle;
    private String      name;

    public PropertyResourceProvider( final String resourceBundle )
    {
        try
        {
            name = resourceBundle;
            bundle = findBundle( name );
        }
        catch( IOException e )
        {
            log.error( "loading resource bundle: " + resourceBundle, e );
        }
    }

    /**
     * Set the active locale for resource look up.
     *
     * @param locale active locale
     */
    public void setLocale( String locale )
    {
        try
        {
            bundle = findBundle( name );
        }
        catch( IOException e )
        {
            log.error( "changing locale [" + locale + "] : " + name, e );
        }
    }

    /**
     * Get a resource given it's key.
     *
     * @param key resource key
     * @return resource if found, null if not found.
     */
    public String getResource( String key )
    {
        return bundle.getProperty( key );
    }

    private Properties findBundle( final String name )
            throws IOException
    {
        InputStream is = ResourceManager.findLocalizedStream(
                this.getClass(),
                name,
                ".properties",
                true
        );
        if( is == null )
        {
            is = ResourceManager.findLocalizedStream(
                    this.getClass(),
                    name,
                    ".txt",
                    true
            );
        }
        if( is != null )
        {
            return new Properties( is );
        }
        else
        {
            return null;
        }
    }

    /**
     * Open any underlying resource so subsequent calls to getResource go faster.
     */
    public void open()
    {
    }

    /**
     * Close any underlying resource previously open.
     */
    public void close()
    {
    }
}
