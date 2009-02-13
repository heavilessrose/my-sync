package bm.midp;
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

import bm.core.Application;
import bm.ui.Menu;
import bm.ui.MenuItem;

import javax.microedition.lcdui.*;
/*
 * File Information
 *
 * Created on       : 07-ago-2007 13:19:28
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Provides a selection dialog.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SelectionDialog
    implements Menu.Listener
{
    int selected = -1;

    private Display     display;
    private Menu        menu;

    public SelectionDialog( final String title, final String[] options )
    {
        this( title, options, null );
    }

    public SelectionDialog(
            final String    title,
            final String[]  options,
            final String    ticker
    )
    {
        final Menu menu = new Menu( this );
        menu.setTitle( title );
        menu.setMode( Menu.LIST );
        final int size = options.length;
        for( int i = 0; i < size; i ++ )
        {
            menu.addItem( options[i], 0, 0, i, null );
        }
        if( ticker != null )
        {
            final Ticker t = new Ticker( ticker );
            menu.setTicker( t );
        }
        this.menu = menu;
        display = Application.getManager().getDisplay();
    }


    public void itemSelected( MenuItem item )
    {
        selected = item.getAction();
    }

    private class Waiter
        extends Thread
    {
        public void run()
        {
            while( selected == -1 )
            {
                try
                {
                    sleep( 500 );
                }
                catch( InterruptedException e )
                {
                }
            }
        }
    }

    public int select()
    {
        final Waiter waiter = new Waiter();
        final Display display = this.display;
        final Displayable current = display.getCurrent();
        display.setCurrent( menu );
        waiter.start();
        try
        {
            waiter.join();
        }
        catch( InterruptedException e )
        {
        }
        if( current != null )
        {
            display.setCurrent( current );
        }
        return selected;
    }
}
