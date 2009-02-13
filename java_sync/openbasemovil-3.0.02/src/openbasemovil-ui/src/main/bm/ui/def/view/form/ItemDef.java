package bm.ui.def.view.form;
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

import javax.microedition.lcdui.Item;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:31:38
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Base form item.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class ItemDef
{
    public static final int TEXT        = 0;
    public static final int DATE        = 1;
    public static final int TIME        = 2;
    public static final int TIMESTAMP   = 3;
    public static final int GAUGE       = 4;
    public static final int SPACER      = 5;
    public static final int IMAGE       = 6;
    public static final int CHOICE      = 7;
    public static final int MONEY       = 8;
    public static final int PERCENT     = 9;
    public static final int INTEGER     = 10;
    public static final int FIXED_POINT = 11;
    public static final int BUTTON      = 12;

    protected String    name;
    protected Integer   layout;
    protected int       itemType;

    public String getName()
    {
        return name;
    }

    public int getLayout()
    {
        return layout != null ? layout.intValue() : Item.LAYOUT_2;
    }

    public int getItemType()
    {
        return itemType;
    }

    protected abstract void read( final SerializerInputStream in )
        throws SerializationException;

    public static ItemDef create( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        final int type = in.readInt();
        final String name = in.readString();
        final Integer layout = in.readNullableInt();
        ItemDef item = null;
            switch( type )
        {
            case TEXT:
            case INTEGER:
                item = new TextDef();
                break;

            case DATE:
            case TIME:
            case TIMESTAMP:
                item = new DateDef();
                break;

            case GAUGE:
                item = new GaugeDef();
                break;

            case SPACER:
                item = new SpacerDef();
                break;

            case IMAGE:
                item = new ImageDef();
                break;

            case CHOICE:
                item = new ChoiceDef();
                break;

            case MONEY:
            case FIXED_POINT:
            case PERCENT:
                item = new FixedPointDef();
                break;

            case BUTTON:
                item = new ButtonDef();
                break;
        }
        if( item == null )
        {
            throw new SerializationException( 0, "Invalid item type" );
        }
        else
        {
            item.itemType = type;
            item.name = name;
            item.layout = layout;
            item.read( in );
            return item;
        }
    }
}
