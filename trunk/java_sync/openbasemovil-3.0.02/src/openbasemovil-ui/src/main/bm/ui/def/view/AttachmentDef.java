package bm.ui.def.view;
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
 * Created on       : 17-oct-2007 18:19:49
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * View attachment.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class AttachmentDef
{
    private String name;
    private String type;
    private String value;

    public String getName()
    {
        return name;
    }

    public String getType()
    {
        return type;
    }

    public String getValue()
    {
        return value;
    }

    public void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        name = in.readString();
        type = in.readString();
        value = in.readString();
    }
}
