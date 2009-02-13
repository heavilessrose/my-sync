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

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:57:40
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A text item.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ChoiceDef
        extends ItemDef
{
    public static final int EXCLUSIVE   = 1;
    public static final int MULTIPLE    = 2;
    public static final int POPUP       = 4;

    protected String    label;
    protected String    labelExtra;
    protected String    bind;
    protected Boolean   wrap;
    protected int       choiceType;
    protected Vector    options = new Vector( 10 );

    public String getLabel()
    {
        return label;
    }

    public String getLabelExtra()
    {
        return labelExtra;
    }

    public String getBind()
    {
        return bind;
    }

    public boolean isWrap()
    {
        return wrap == null || wrap.booleanValue();
    }

    public int getChoiceType()
    {
        return choiceType;
    }

    public Vector getOptions()
    {
        return options;
    }

    protected void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        label = in.readNullableString();
        labelExtra = in.readNullableString();
        wrap = in.readNullableBoolean();
        bind = in.readNullableString();
        choiceType = in.readInt();

        options.removeAllElements();
        final int count = in.readInt();
        for( int i = 0; i < count; i++ )
        {
            final Option option = new Option();
            option.read( in );
            options.addElement( option );
        }
    }
}
