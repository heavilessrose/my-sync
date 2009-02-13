package bm.ui.def.view.settings;
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
import bm.ui.def.view.ViewDef;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:30:48
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Settings view definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SettingsViewDef
        extends ViewDef
{
    private String  helpId;
    private String  imageMap;
    private String  imageMapBig;

    private Vector  items = new Vector( 10 );

    public String getHelpId()
    {
        return helpId;
    }

    public String getImageMap()
    {
        return imageMap;
    }

    public String getImageMapBig()
    {
        return imageMapBig;
    }

    public Vector getItems()
    {
        return items;
    }

    protected void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        helpId = in.readNullableString();
        imageMap = in.readNullableString();
        imageMapBig = in.readNullableString();

        items.removeAllElements();
        final int count = in.readInt();
        for( int i = 0; i < count; i++ )
        {
            final Setting setting = new Setting();
            setting.read( in );
            items.addElement( setting );
        }
    }
}
