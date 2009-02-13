package bm.ui;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
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

import bm.util.Util;

import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.game.Sprite;
import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 13-ago-2007 13:57:16
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Stores sets of icons in merged images.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class IconStore
{
    private static Hashtable images = new Hashtable( 10 );
    private static Hashtable sets = new Hashtable( 10 );

    private static IconStore dummy = new IconStore();

    private IconStore(){}

    public static synchronized void addIconSet(
            final String    source,
            final String    name,
            final int       width,
            final int       height
    )
    {
        if( !sets.containsKey( name ) )
        {
            final Image all = Util.loadImage( dummy.getClass(), source );
            if( all != null )
            {
                final int count = all.getWidth() / width;
                int x = 0;
                for( int i = 0; i < count; i++ )
                {
                    final Image image = Image.createImage(
                            all,
                            x,
                            0,
                            width,
                            height,
                            Sprite.TRANS_NONE
                    );
                    x += width;
                    images.put( name + "." + i, image );
                }
                sets.put( name, new Integer( count ) );
            }
        }
    }

    public static boolean hasIconSet( final String name )
    {
        return  name != null && sets.containsKey( name );
    }

    public static synchronized void clear()
    {
        sets.clear();
        images.clear();
    }

    public static synchronized void removeIconSet( final String name )
    {
        final Integer count = (Integer) sets.get( name );
        if( count != null )
        {
            sets.remove( name );
            final int size = count.intValue();
            for( int i = 0; i < size; i++ )
            {
                images.remove( name + "." + i );
            }
        }
    }

    public static Image getImage( final String name, final int index )
    {
        return (Image) images.get( name + "." + index );
    }

    public static Image getImage( final String key )
    {
        return (Image) images.get( key );
    }
}
