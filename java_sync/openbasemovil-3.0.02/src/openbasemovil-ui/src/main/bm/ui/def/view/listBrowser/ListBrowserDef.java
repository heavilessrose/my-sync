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
import bm.ui.def.view.ViewDef;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:08:55
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * List browser definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ListBrowserDef
        extends ViewDef
{
    public static final byte    ENHANCED    = 0;
    public static final byte    NATIVE      = 1;

    private Byte    type;
    private String  separator;
    private Integer pageSize;
    private Boolean wrap;
    private Integer fontFace;
    private Integer fontStyle;
    private Integer fontSize;
    private Integer color;
    private Integer selectedColor;
    private Integer background;
    private Integer selectedBackground;
    private Integer maxLines;

    private Vector fields = new Vector( 10 );
    private Vector speedOptions = new Vector( 10 );

    public Byte getType()
    {
        return type;
    }

    public String getSeparator()
    {
        return separator;
    }

    public Integer getPageSize()
    {
        return pageSize;
    }

    public Integer getFontFace()
    {
        return fontFace;
    }

    public Integer getFontStyle()
    {
        return fontStyle;
    }

    public Integer getFontSize()
    {
        return fontSize;
    }

    public Integer getColor()
    {
        return color;
    }

    public Integer getSelectedColor()
    {
        return selectedColor;
    }

    public Integer getBackground()
    {
        return background;
    }

    public Integer getSelectedBackground()
    {
        return selectedBackground;
    }

    public Integer getMaxLines()
    {
        return maxLines;
    }

    public Boolean getWrap()
    {
        return wrap;
    }

    public Vector getFields()
    {
        return fields;
    }

    public Vector getSpeedOptions()
    {
        return speedOptions;
    }

    protected void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        type = in.readNullableByte();
        separator = in.readNullableString();
        pageSize = in.readNullableInt();
        wrap = in.readNullableBoolean();
        fontFace = in.readNullableInt();
        fontStyle = in.readNullableInt();
        fontSize = in.readNullableInt();
        color = in.readNullableInt();
        selectedColor = in.readNullableInt();
        background = in.readNullableInt();
        selectedBackground = in.readNullableInt();
        maxLines = in.readNullableInt();

        fields.removeAllElements();
        int count = in.readInt();
        for( int i = 0; i < count; i++ )
        {
            final FieldDef field = new FieldDef();
            field.read( in );
            fields.addElement( field );
        }

        speedOptions.removeAllElements();
        count = in.readInt();
        for( int i = 0; i < count; i++ )
        {
            final SpeedOptionDef option = new SpeedOptionDef();
            option.read( in );
            speedOptions.addElement( option );
        }
    }
}
