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
/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:09:36
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Button definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ButtonDef
        extends ItemDef
{
    public static final byte BUTTON_TYPE_BACK      = 0;
    public static final byte BUTTON_TYPE_CANCEL    = 1;
    public static final byte BUTTON_TYPE_EXIT      = 2;
    public static final byte BUTTON_TYPE_HELP      = 3;
    public static final byte BUTTON_TYPE_OK        = 4;
    public static final byte BUTTON_TYPE_STOP      = 5;
    public static final byte BUTTON_TYPE_HOME      = 6;
    public static final byte BUTTON_TYPE_CUSTOM    = 7;

    protected String    label;
    protected String    image;
    protected int       code;
    protected int       buttonType;

    public String getLabel()
    {
        return label;
    }

    public String getImage()
    {
        return image;
    }

    public int getCode()
    {
        return code;
    }

    public int getButtonType()
    {
        return buttonType;
    }

    protected void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version

        buttonType = in.readInt();
        code = in.readInt();
        label = in.readNullableString();
        image = in.readNullableString();
    }
}
