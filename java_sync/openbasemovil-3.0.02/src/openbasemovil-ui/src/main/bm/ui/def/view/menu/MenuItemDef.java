package bm.ui.def.view.menu;
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

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:02:39
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Menu item definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuItemDef
{
    private String  title;
    private int     action;
    private String  onlyIf;
    private String  unless;
    private Integer iconIndex;
    private Integer selectedIconIndex;
    private String  help;
    private String  view;

    public String getTitle()
    {
        return title;
    }

    public int getAction()
    {
        return action;
    }

    public String getOnlyIf()
    {
        return onlyIf;
    }

    public String getUnless()
    {
        return unless;
    }

    public Integer getIconIndex()
    {
        return iconIndex;
    }

    public Integer getSelectedIconIndex()
    {
        return selectedIconIndex;
    }

    public String getHelp()
    {
        return help;
    }

    public String getView()
    {
        return view;
    }

    public void read( final SerializerInputStream in )
            throws SerializationException
    {
        final byte version = in.readByte();
        title = in.readString();
        action = in.readInt();
        onlyIf = in.readNullableString();
        unless = in.readNullableString();
        iconIndex = in.readNullableInt();
        selectedIconIndex = in.readNullableInt();
        help = in.readNullableString();
        if( version > 1 )
        {
            view = in.readNullableString();
        }
    }
}
