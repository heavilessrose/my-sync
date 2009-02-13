package bm.ui.def.view.listBrowser;
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
import bm.core.math.FixedPoint;

import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.DateField;
import javax.microedition.lcdui.Gauge;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:17:47
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * List field definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class FieldDef
{
    public static final byte PLAIN  = 0;
    public static final byte DATE   = 1;
    public static final byte TIME   = 2;
    public static final byte MONEY  = 3;
    public static final byte STATIC = 4;

    private String  name;
    private Byte    mode;
    private String  label;
    private String  trueLabel;
    private String  falseLabel;
    private String  resource;

    public String getName()
    {
        return name;
    }

    public Byte getMode()
    {
        return mode;
    }

    public String getLabel()
    {
        return label;
    }

    public String getTrueLabel()
    {
        return trueLabel;
    }

    public String getFalseLabel()
    {
        return falseLabel;
    }

    public String getResource()
    {
        return resource;
    }

    public void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version

        name = in.readString();
        mode = in.readNullableByte();
        label = in.readNullableString();
        trueLabel = in.readNullableString();
        falseLabel = in.readNullableString();
        resource = in.readNullableString();
    }
}
