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

import java.util.HashMap;
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
 * A field in a list browser.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Field
{
    public static final byte PLAIN  = 0;
    public static final byte DATE   = 1;
    public static final byte TIME   = 2;
    public static final byte MONEY  = 3;
    public static final byte STATIC = 4;

    private static final HashMap MODE_MAP = new HashMap( 5 );
    static
    {
        MODE_MAP.put( "plain", new Byte( PLAIN ) );
        MODE_MAP.put( "date", new Byte( DATE ) );
        MODE_MAP.put( "time", new Byte( TIME ) );
        MODE_MAP.put( "money", new Byte( MONEY ) );
        MODE_MAP.put( "static", new Byte( STATIC ) );
    }

    private String  name;
    private String  mode;
    private String  label;
    private String  trueLabel;
    private String  falseLabel;
    private String  resource;

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public String getMode()
    {
        return mode;
    }

    public void setMode( final String mode )
    {
        this.mode = mode;
    }

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label;
    }

    public String getTrueLabel()
    {
        return trueLabel;
    }

    public void setTrueLabel( final String trueLabel )
    {
        this.trueLabel = trueLabel;
    }

    public String getFalseLabel()
    {
        return falseLabel;
    }

    public void setFalseLabel( final String falseLabel )
    {
        this.falseLabel = falseLabel;
    }

    public String getResource()
    {
        return resource;
    }

    public void setResource( final String resource )
    {
        this.resource = resource;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeString( name );
            out.writeByte( mode != null ? (Byte) MODE_MAP.get( mode ) : null );
            out.writeNullableString( ViewCompiler.escape( label ) );
            out.writeNullableString( ViewCompiler.escape( trueLabel ) );
            out.writeNullableString( ViewCompiler.escape( falseLabel ) );
            out.writeNullableString( resource );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
