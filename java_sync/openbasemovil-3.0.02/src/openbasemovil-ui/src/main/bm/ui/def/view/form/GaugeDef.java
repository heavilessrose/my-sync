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
 * Created on       : 17-oct-2007 21:52:22
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
public class GaugeDef
        extends ItemDef
{
    protected String    label;
    protected String    labelExtra;
    protected Boolean   readOnly;
    protected String    bind;
    protected Integer   maxValue;

    public String getLabel()
    {
        return label;
    }

    public String getLabelExtra()
    {
        return labelExtra;
    }

    public boolean isReadOnly()
    {
        return readOnly != null && readOnly.booleanValue();
    }

    public String getBind()
    {
        return bind;
    }

    public int getMaxValue()
    {
        return maxValue != null ? maxValue.intValue() : 100;
    }

    protected void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        label = in.readNullableString();
        labelExtra = in.readNullableString();
        readOnly = in.readNullableBoolean();
        bind = in.readNullableString();
        maxValue = in.readNullableInt();
    }
}
