package com.basemovil.vc.view.form;
/* -----------------------------------------------------------------------------
    OpenBaseMovil View Compiler, generates the binary form of views from
    an XML file.
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

import com.basemovil.vc.view.ViewChild;
import com.basemovil.vc.ViewCompilerException;

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
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
public class Item
    extends ViewChild
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

    protected int       type;
    protected String    name;
    protected Integer   layout;

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public Integer getLayout()
    {
        return layout;
    }

    public void setLayout( final Integer layout )
    {
        this.layout = layout;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeInt( type );
            out.writeString( name );
            out.writeInt( layout );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
