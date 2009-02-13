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

import bm.core.ResourceManager;
import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.mvc.MenuView;
import bm.mvc.ViewFactory;
import bm.ui.def.view.menu.MenuDef;
import bm.ui.def.view.menu.MenuItemDef;

import javax.microedition.lcdui.Font;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 21-ago-2007 0:51:55
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
public class IconMenuView
        extends AbstractView
        implements MenuView,
                   Menu.Listener
{
    protected Hashtable   map = new Hashtable( 10 );
    protected int         viewId;
    protected String      helpId;

    public static IconMenuView create(
            final Controller    controller,
            final MenuDef       menuDef
    )
    {
        final String title = ResourceManager.getResource( menuDef.getTitle() );
        final int id = menuDef.getId();
        final IconMenuView menuView = new IconMenuView( controller, title, id );
        final Menu menu = (Menu) menuView.displayable;

        ViewFactory.loadIconStore( menuDef.getIconSet() );
        menu.setIconSet( menuDef.getIconSet() );
        menu.setMode( menuDef.getMode() == MenuDef.LIST ? Menu.LIST : Menu.GRID );
        if( menuDef.getBackground() != null )
        {
            menu.setBackground( menuDef.getBackground().intValue() );
        }
        if( menuDef.getSelectedBackground() != null )
        {
            menu.setSelectedBackground(
                    menuDef.getSelectedBackground().intValue()
            );
        }
        if( menuDef.getHelpLines() != null )
        {
            menu.setHelpLines( menuDef.getHelpLines().intValue() );
        }
        if( menuDef.getColor() != null )
        {
            menu.setColor( menuDef.getColor().intValue() );
        }
        if( menuDef.getSelectedColor() != null )
        {
            menu.setSelectedColor( menuDef.getSelectedColor().intValue() );
        }
        if(
                menuDef.getFontFace() != null ||
                menuDef.getFontStyle() != null ||
                menuDef.getFontSize() != null
        )
        {
            final Font font = Font.getFont(
                    menuDef.getFontFace() != null ?
                    menuDef.getFontFace().intValue() :
                    Font.FACE_PROPORTIONAL,
                    menuDef.getFontStyle() != null ?
                    menuDef.getFontStyle().intValue() :
                    Font.STYLE_PLAIN,
                    menuDef.getFontSize() != null ?
                    menuDef.getFontSize().intValue() :
                    Font.SIZE_SMALL
            );
            if( font != null )
            {
                menu.setFont( font );
            }
        }
        final Vector items = menuDef.getItems();
        final int count = items.size();
        int iconIndex = 0;
        for( int i = 0; i < count; i++ )
        {
            boolean active = true;
//            final String fix = name + "." + i;
            final MenuItemDef itemDef = (MenuItemDef) items.elementAt( i );
            if( itemDef.getOnlyIf() != null )
            {
                active = ViewFactory.hasFlag( itemDef.getOnlyIf() );
            }
            else if( itemDef.getUnless() != null )
            {
                active = !ViewFactory.hasFlag( itemDef.getUnless() );
            }
            final int icon = iconIndex++;
            final int selectedIcon = iconIndex++;
            if( active )
            {
                menu.addItem(
                        itemDef.getTitle(),
                        itemDef.getIconIndex() != null ?
                            itemDef.getIconIndex().intValue() :
                            icon,
                        itemDef.getSelectedIconIndex() != null ?
                            itemDef.getSelectedIconIndex().intValue() :
                            selectedIcon,
                        itemDef.getAction(),
                        itemDef.getHelp(),
                        itemDef.getView()
                );
            }
        }
        menuView.helpId = menuDef.getHelpId();
        return menuView;
    }

    public String getHelpId()
    {
        return helpId;
    }

    public void itemSelected( MenuItem item )
    {
        if( item.view == null )
        {
            controller.handle( new ControllerEvent( item.action, this ) );
        }
        else
        {
            final ControllerEvent event = new ControllerEvent( 11000, this );
            event.add( "view", item.view );
            controller.handle( event );
        }
    }

    protected IconMenuView(
            final Controller controller,
            final String        title,
            final int           viewId
    )
    {
        super( controller );
        this.viewId = viewId;
        final Menu menu = new Menu( this );
        menu.setTitle( title );
        menu.setCommandListener( this );
        displayable = menu;
    }

    public int getId()
    {
        return viewId;
    }

    public void setId( final int viewId )
    {
        this.viewId = viewId;
    }

    public void addItem(
            final String    label,
            final int       iconIndex,
            final int       selectedIconIndex,
            final int       action,
            final String    view
    )
    {
        ((Menu) displayable).addItem( label, iconIndex, selectedIconIndex, action, view );
    }

    public void addItem(
            final String    label,
            final int       iconIndex,
            final int       selectedIconIndex,
            final int       action,
            final String    help,
            final String    view
    )
    {
        ((Menu) displayable).addItem( label, iconIndex, selectedIconIndex, action, help, view );
    }

    public void setFont( final Font font )
    {
        ((Menu) displayable).setFont( font );
    }
}
