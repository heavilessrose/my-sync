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

import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 11-may-2006 17:31:08
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Simple cache for use within a host class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SimpleCache
{
    private static long wideHits;
    private static long wideMisses;

    private Hashtable   cache;
    private Vector      stamps;
    private int         maxSize;
    private long        hits;
    private long        misses;

    public static long getWideHits()
    {
        return wideHits;
    }

    public static long getWideMisses()
    {
        return wideMisses;
    }

    public SimpleCache()
    {
        this( CoreConstants.SIMPLE_CACHE_DEFAULT_SIZE );
    }

    public SimpleCache( final int size )
    {
        this.maxSize = size;
        cache = new Hashtable( size );
        stamps = new Vector( size );
    }

    public int getMaxSize()
    {
        return maxSize;
    }

    public void setMaxSize( final int maxSize )
    {
        this.maxSize = maxSize;
    }

    public int getSize()
    {
        return cache.size();
    }

    public void add( final Object key, final Object object )
    {
        final Hashtable cache = this.cache;

        if( !cache.containsKey( key ) )
        {
            if( cache.size() == maxSize )
            {
                discard();
            }
            cache.put( key, object );
            stamps.insertElementAt( key, 0 );
        }
        else
        {
            cache.put( key, object );
            touch( key );
        }
    }

    public void remove( final Object key )
    {
        if( stamps.contains( key ) )
        {
            stamps.removeElement( key );
            cache.remove( key );
        }
    }

    public void clear()
    {
        cache.clear();
        stamps.removeAllElements();
        hits = misses = 0;
    }

    public synchronized Object get( final Object key )
    {
        final Object o = cache.get( key );
        if( o != null )
        {
            hits++;
            wideHits++;
            touch( key );
        }
        else
        {
            misses++;
            wideMisses++;
        }
        return o;
    }

    public long getHits()
    {
        return hits;
    }

    public long getMisses()
    {
        return misses;
    }

    private void discard()
    {
        final Object key = stamps.lastElement();
        stamps.removeElement( key );
        cache.remove( key );
    }

    private void touch( final Object key )
    {
        stamps.removeElement( key );
        stamps.insertElementAt( key, 0 );
    }
}
