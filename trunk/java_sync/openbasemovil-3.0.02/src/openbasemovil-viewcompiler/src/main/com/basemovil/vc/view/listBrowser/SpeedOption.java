package com.basemovil.vc.view.listBrowser;
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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;
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
 * A speed option.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SpeedOption
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

    public void setCode( final int code )
    {
        this.code = code;
    }

    public String getText()
    {
        return text;
    }

    public void setText( final String text )
    {
        this.text = text;
    }

    public String getIconSet()
    {
        return iconSet;
    }

    public void setIconSet( final String iconSet )
    {
        this.iconSet = iconSet;
    }

    public Integer getIcon()
    {
        return icon;
    }

    public void setIcon( final Integer icon )
    {
        this.icon = icon;
    }

    public String getHelp()
    {
        return help;
    }

    public void setHelp( final String help )
    {
        this.help = help;
    }

    public String getOnlyIf()
    {
        return onlyIf;
    }

    public void setOnlyIf( final String onlyIf )
    {
        this.onlyIf = onlyIf;
    }

    public String getUnless()
    {
        return unless;
    }

    public void setUnless( final String unless )
    {
        this.unless = unless;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeInt( code );
            out.writeNullableString( ViewCompiler.escape( text ) );
            out.writeNullableString( iconSet );
            out.writeInt( icon );
            out.writeNullableString( ViewCompiler.escape( help ) );
            out.writeNullableString( onlyIf );
            out.writeNullableString( unless );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
