package bm.util;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Util Library, miscellaneous tools
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

import bm.core.res.ResourceProvider;
import bm.core.ResourceManager;
import bm.core.PropertiesReader;
import bm.core.SimpleCache;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.event.ProgressEvent;
import bm.db.index.Index;
import bm.db.DBException;
import bm.storage.RecordStoreFullException;
import bm.storage.RSException;

import java.io.InputStream;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 16-oct-2007 18:13:38
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A resource provider based on an index.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class IndexResourceProvider
    implements ResourceProvider
{
    private static final Log log = LogFactory.getLog( "IndexResourceProvider" );

    private String      locale;
    private Index       index;
    private String[]    languages;
    private SimpleCache cache = new SimpleCache( 20 );

    public IndexResourceProvider(
            final String    indexName,
            final int       indexOrder,
            final String[]  languages
    )
    {
        index = new Index( indexName, indexOrder, Index.KT_STRING, true, false );
        index.setSendProgressEvents( false );
        this.languages = languages;
        setLocale( ResourceManager.getLocale() );
    }

    /**
     * Set the active locale for resource look up.
     *
     * @param locale active locale
     */
    public void setLocale( String locale )
    {
        cache.clear();
        if( locale == null )
        {
            this.locale = "en";
            return;
        }
        final int localeCount = languages.length;
        String loc = null;
        for( int i = 0; i < localeCount; i++ )
        {
            if( languages[i].equals( locale ) )
            {
                loc = locale;
                break;
            }
        }
        if( loc == null && locale.indexOf( '_' ) > 0 )
        {
            final String aux = locale.substring( 0, locale.indexOf( '_' ) );
            for( int i = 0; i < localeCount; i++ )
            {
                if( languages[i].equals( aux ) )
                {
                    loc = aux;
                    break;
                }
            }
        }
        if( loc != null )
        {
            this.locale = loc;
        }
        else
        {
            this.locale = "en";
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
        try
        {
            final String target = locale + "#" + key;
            Object value = cache.get( target );
            if( value == null )
            {
                value = index.find( target );
                if( value != null )
                {
                    cache.add( target, value );
                }
            }
            return value != null ? value.toString() : null;
        }
        catch( Exception e )
        {
            return null;
        }
    }

    /**
     * Open the index, so some sequential accesses will go faster.
     */
    public void open()
    {
        try
        {
            index.open();
        }
        catch( RecordStoreFullException e )
        {
            log.error( e );
        }
        catch( RSException e )
        {
            log.error( e );
        }
    }

    /**
     * Close the index, when open has been called before.
     */
    public void close()
    {
        index.close();
    }

    /**
     * Rebuilds the resource index, removing the previous one if any.
     *
     * @param bundleNames array with names of the resource bundles
     * @param langs array with languages to use for the index
     * @throws IOException if a bundle can't be found or an error reading a bundle
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors at the storage system
     * @throws DBException on errors building the index
     */
    public void buildIndex(
            final String[]  bundleNames,
            final String[]  langs
    )
            throws IOException,
                   RecordStoreFullException,
                   RSException,
                   DBException
    {
        final Index index = this.index;
        final int bundleCount = bundleNames.length;
        final int langCount = langs.length;
        final ProgressEvent pe = new ProgressEvent( this );
        pe.setAnimate( true );
        pe.setCancellable( false );
        pe.dispatch();
        index.drop();
        index.open();
        InputStream is = null;
        try
        {
            for( int i = 0; i < bundleCount; i++ )
            {
                for( int j = 0; j < langCount; j++ )
                {
                    final String fileName = "/" + bundleNames[i] + "." +
                                            langs[j] + ".properties";
                    is = getClass().getResourceAsStream( fileName );
                    if( is == null )
                    {
                        throw new IOException( "Not found: " + fileName );
                    }
                    else
                    {
                        final PropertiesReader r = new PropertiesReader( is );
                        String[] pair = r.next();
                        while( pair != null )
                        {
                            log.debug( "pair: " + pair[0] + " = " + pair[1] );
                            index.insertObject( pair[0], pair[1] );
                            pair = r.next();
                        }
                        is.close();
                        System.gc();
                    }
                }
            }
        }
        finally
        {
            if( is != null ) try{ is.close(); }catch( Exception e ){}
            index.close();
        }
    }
}
