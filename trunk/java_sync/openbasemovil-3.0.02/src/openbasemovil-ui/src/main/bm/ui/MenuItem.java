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

import javax.microedition.lcdui.Image;
/*
 * File Information
 *
 * Created on       : 28-ago-2007 17:56:21
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An item in a menu.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuItem
{
    String  label;
    Image   image;
    Image   selectedImage;
    int     action;
    String  help;
    String  iconSet;
    String  view;

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label;
    }

    public Image getImage()
    {
        return image;
    }

    public void setImage( final Image image )
    {
        this.image = image;
    }

    public Image getSelectedImage()
    {
        return selectedImage;
    }

    public void setSelectedImage( final Image selectedImage )
    {
        this.selectedImage = selectedImage;
    }

    public int getAction()
    {
        return action;
    }

    public void setAction( final int action )
    {
        this.action = action;
    }

    public String getHelp()
    {
        return help;
    }

    public void setHelp( final String help )
    {
        this.help = help;
    }

    public String getView()
    {
        return view;
    }

    public void setView( final String view )
    {
        this.view = view;
    }

    public MenuItem(
            final String label,
            final int iconIndex,
            final int selectedIconIndex,
            final int action,
            final String help,
            final String iconSet,
            final String view
    )
    {
        this.label = ResourceManager.getResource( label );
        this.help = help != null ? ResourceManager.getResource( help ) : "";
        image = IconStore.getImage( iconSet, iconIndex );
        selectedImage = IconStore.getImage( iconSet, selectedIconIndex );
        this.action = action;
        this.iconSet = iconSet;
        this.view = view;
    }
}
