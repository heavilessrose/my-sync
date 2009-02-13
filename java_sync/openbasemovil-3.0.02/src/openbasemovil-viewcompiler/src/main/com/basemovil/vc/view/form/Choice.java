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

import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;

import java.util.HashMap;
import java.util.List;
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
public class Choice
        extends Item
{
    public static final int EXCLUSIVE   = 1;
    public static final int MULTIPLE    = 2;
    public static final int POPUP       = 4;
    private static final HashMap typeMap = new HashMap( 3 );
    static
    {
        typeMap.put( "exclusive", new Integer( EXCLUSIVE ) );
        typeMap.put( "multiple", new Integer( MULTIPLE ) );
        typeMap.put( "popup", new Integer( POPUP ) );
    }

    protected String    label;
    protected String    labelExtra;
    protected String    bind;
    protected Boolean   wrap;
    protected String    choiceType;
    protected List      options = new Vector( 10 );

    public Choice()
    {
        type = CHOICE;
    }

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label;
    }

    public String getLabelExtra()
    {
        return labelExtra;
    }

    public void setLabelExtra( final String labelExtra )
    {
        this.labelExtra = labelExtra;
    }

    public String getBind()
    {
        return bind;
    }

    public void setBind( final String bind )
    {
        this.bind = bind;
    }

    public Boolean getWrap()
    {
        return wrap;
    }

    public void setWrap( final Boolean wrap )
    {
        this.wrap = wrap;
    }

    public String getChoiceType()
    {
        return choiceType;
    }

    public void setChoiceType( final String choiceType )
    {
        this.choiceType = choiceType;
    }

    public void add( final Option option )
    {
        options.add( option );
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        super.store( out );

        try
        {
            out.writeByte( (byte) 1 );
            out.writeNullableString( ViewCompiler.escape( label ) );
            out.writeNullableString( ViewCompiler.escape( labelExtra ) );
            out.writeBoolean( wrap );
            out.writeNullableString( bind );
            out.writeInt(
                    ((Integer) typeMap.get( choiceType )).intValue()
            );
            final int count = options.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final Option option = (Option) options.get( i );
                option.store( out );
            }
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
