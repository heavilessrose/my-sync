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
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:25:25
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Speed Option definition.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SpeedOptionDef
{
    private int     code;
    private String  text;
    private String  iconSet;
    private Integer icon;
    private String  help;
    private String  onlyIf;
    private String  unless;

    public int getCode()
    {
        return code;
    }

    public String getText()
    {
        return text;
    }

    public String getIconSet()
    {
        return iconSet;
    }

    public Integer getIcon()
    {
        return icon;
    }

    public String getHelp()
    {
        return help;
    }

    public String getOnlyIf()
    {
        return onlyIf;
    }

    public String getUnless()
    {
        return unless;
    }

    public void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        code = in.readInt();
        text = in.readNullableString();
        iconSet = in.readNullableString();
        icon = in.readNullableInt();
        help = in.readNullableString();
        onlyIf = in.readNullableString();
        unless = in.readNullableString();
    }
}
