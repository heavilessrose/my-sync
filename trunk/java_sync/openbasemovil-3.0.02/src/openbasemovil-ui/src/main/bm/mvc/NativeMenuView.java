package bm.mvc;
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
import bm.core.ResourceManager;
import bm.core.mvc.AbstractController;
import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.util.Util;
import bm.ui.def.view.menu.MenuDef;
import bm.ui.def.view.menu.MenuItemDef;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.Sprite;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 21-ago-2007 1:42:14
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Basic menu view.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class NativeMenuView
        extends AbstractView
        implements MenuView
{
    static Hashtable images        = new Hashtable( 10 );

    protected Hashtable   map = new Hashtable( 10 );
    protected int         viewId;
    protected String      helpId;

    public static NativeMenuView create(
            final Controller    controller,
            final MenuDef       menuDef,
            final String        name
    )
    {
        final String title = ResourceManager.getResource( menuDef.getTitle() );
        final NativeMenuView menu = new NativeMenuView(
                controller,
                title,
                menuDef.getId()
        );
        final Vector items = menuDef.getItems();
        final int count = items.size();
        final boolean hasImageMap = menuDef.getImageMap() != null;
        if( hasImageMap )
        {
            loadImages(
                    name,
                    menuDef.getImageMap(),
                    menuDef.getImageMapBig(),
                    count
            );
        }
        for( int i = 0; i < count; i++ )
        {
            final MenuItemDef item = (MenuItemDef) items.elementAt( i );
            boolean active = true;
            final String fix = name + "." + i;
            if( item.getOnlyIf() != null )
            {
                active = ViewFactory.flags.contains( item.getOnlyIf() );
            }
            else if( item.getUnless() != null )
            {
                active = !ViewFactory.flags.contains( item.getUnless() );
            }
            if( active )
            {
                final String text = ResourceManager.getResource(
                        item.getTitle()
                );
                if( hasImageMap )
                {
                    menu.addOption(
                            text,
                            (Image) images.get( fix ),
                            item.getAction()
                    );
                }
                else
                {
                    menu.addOption( text, item.getAction() );
                }
            }
        }
        menu.helpId = menuDef.getHelpId();
        return menu;
    }

    public String getHelpId()
    {
        return helpId;
    }

    static void loadImages(
            final String    name,
            final String    imageMap,
            final String    imageMapBig,
            final int       count
    )
    {
        final Display display = Application.getManager().getDisplay();
        final int bestSize = display.getBestImageWidth(
                Display.LIST_ELEMENT
        );
        int size;
        if( bestSize > 0 )
        {
            size = bestSize < 24 ? 12 : 24;
        }
        else
        {
            size = display.getCurrent().getWidth() > 250 ? 24 : 12;
        }
        String imageSource =
                size == 24 && imageMapBig != null ?
                    imageMapBig :
                    imageMap;
        final Image all = Util.loadImage( display.getClass(), imageSource );
        if( all != null )
        {
            int x = 0;
            for( int i = 0; i < count; i++ )
            {
                final Image image = Image.createImage(
                        all,
                        x,
                        0,
                        size,
                        size,
                        Sprite.TRANS_NONE
                );
                x += size;
                images.put( name + "." + i, image );
            }
        }
    }

    protected NativeMenuView(
            final Controller controller,
            final String        title,
            final int           viewId
    )
    {
        super( controller );
        this.viewId = viewId;
        displayable = new List( title, List.IMPLICIT );
        displayable.setCommandListener( this );
    }

    public int getId()
    {
        return viewId;
    }

    public void setId( final int viewId )
    {
        this.viewId = viewId;
    }

    public void commandAction( final Command command,
                               final Displayable displayable
    )
    {
        if( !handleAction( command ) )
        {
            final ControllerEvent event = AbstractController.buildEvent(
                    this,
                    command,
                    (Choice) displayable
            );
            if( event.getCode() == ControllerEvent.LIST_ITEM )
            {
                final Integer code = (Integer)
                        map.get( new Integer( event.getIndex() ) );
                if( code != null )
                {
                    event.setCode( code.intValue() );
                }
            }
            controller.handle( event );
        }
    }

    protected int addOption( final String text, final int eventCode )
    {
        return addOption( text, null, eventCode );
    }

    protected int addOption(
            final String    text,
            final Image     image,
            final int       eventCode
    )
    {
        final int index = ( (List) displayable ).append( text, image );
        map.put( new Integer( index ), new Integer( eventCode ) );
        return index;
    }

    protected void setFont( final Font font )
    {
        final List list = (List) displayable;
        final int count = list.size();
        for( int i = 0; i < count; i++ )
        {
            list.setFont( i, font );
        }
    }

}
