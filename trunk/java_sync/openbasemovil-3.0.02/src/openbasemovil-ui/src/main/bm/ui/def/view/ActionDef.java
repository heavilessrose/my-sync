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
 * Created on       : 17-oct-2007 18:19:44
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * View action.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ActionDef
{
    public static final int SCREEN  = 1;
    public static final int BACK    = 2;
    public static final int CANCEL  = 3;
    public static final int OK      = 4;
    public static final int HELP    = 5;
    public static final int STOP    = 6;
    public static final int EXIT    = 7;

    protected String    name;
    protected int       code;
    protected String    label;
    protected int       order = -1;
    protected boolean   active = true;
    protected String    onlyIf;
    protected String    unless;
    protected String    forItem;
    protected Integer   type;
    protected Boolean   defaultAction;

    public String getName()
    {
        return name;
    }

    public int getCode()
    {
        return code;
    }

    public String getLabel()
    {
        return label;
    }

    public int getOrder()
    {
        return order;
    }

    public boolean isActive()
    {
        return active;
    }

    public String getOnlyIf()
    {
        return onlyIf;
    }

    public String getUnless()
    {
        return unless;
    }

    public String getFor()
    {
        return forItem;
    }

    public Integer getType()
    {
        return type;
    }

    public Boolean getDefault()
    {
        return defaultAction;
    }

    public void read( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        name = in.readString();
        code = in.readInt();
        label = in.readString();
        order = in.readInt();
        active = in.readBoolean();
        onlyIf = in.readNullableString();
        unless = in.readNullableString();
        forItem = in.readNullableString();
        type = in.readNullableInt();
        defaultAction = in.readNullableBoolean();
    }
}
