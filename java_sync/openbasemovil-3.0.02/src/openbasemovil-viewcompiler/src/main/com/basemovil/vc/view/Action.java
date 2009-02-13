package com.basemovil.vc.view;
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
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;

import java.util.HashMap;
import java.util.Map;
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
public class Action
        extends ViewChild
{
    private static Map types = new HashMap( 7 );
    static
    {
        types.put( "screen",    new Integer( 1 ) );
        types.put( "back",      new Integer( 2 ) );
        types.put( "cancel",    new Integer( 3 ) );
        types.put( "ok",        new Integer( 4 ) );
        types.put( "help",      new Integer( 5 ) );
        types.put( "stop",      new Integer( 6 ) );
        types.put( "exit",      new Integer( 7 ) );
    }

    protected String    name;
    protected int       code;
    protected String    label;
    protected int       order = -1;
    protected boolean   active = true;
    protected String    onlyIf;
    protected String    unless;
    protected String    forItem;
    protected String    type;
    protected Boolean   defaultAction;

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public int getCode()
    {
        return code;
    }

    public void setCode( final int code )
    {
        this.code = code;
    }

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label;
    }

    public int getOrder()
    {
        return order;
    }

    public void setOrder( final int order )
    {
        this.order = order;
    }

    public boolean isActive()
    {
        return active;
    }

    public void setActive( final boolean active )
    {
        this.active = active;
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

    public String getFor()
    {
        return forItem;
    }

    public void setFor( final String forItem )
    {
        this.forItem = forItem;
    }

    public String getType()
    {
        return type;
    }

    public void setType( final String type )
    {
        this.type = type;
    }

    public Boolean getDefault()
    {
        return defaultAction;
    }

    public void setDefault( final Boolean defaultAction )
    {
        this.defaultAction = defaultAction;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeString( name );
            out.writeInt( code );
            out.writeString( ViewCompiler.escape( label ) );
            out.writeInt( order );
            out.writeBoolean( active );
            out.writeNullableString( onlyIf );
            out.writeNullableString( unless );
            out.writeNullableString( forItem );
            final Integer t = type != null ? (Integer) types.get( type ) : null;
            out.writeInt( t );
            out.writeBoolean( defaultAction );
        }
        catch( Exception e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
