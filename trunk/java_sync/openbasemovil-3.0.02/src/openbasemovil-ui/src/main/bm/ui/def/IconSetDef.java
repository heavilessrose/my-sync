package bm.ui.def;
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

import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;

import java.io.ByteArrayInputStream;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:11:18
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Icon set definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class IconSetDef
{
    private String  source;
    private int     width;
    private int     height;

    public String getSource()
    {
        return source;
    }

    public int getWidth()
    {
        return width;
    }

    public int getHeight()
    {
        return height;
    }

    public void read( final byte[] data )
            throws SerializationException
    {
        final ByteArrayInputStream bais = new ByteArrayInputStream( data );
        final SerializerInputStream in = new SerializerInputStream( bais );

        in.readByte(); // skip version
        source = in.readString();
        width = in.readInt();
        height = in.readInt();
    }
}
