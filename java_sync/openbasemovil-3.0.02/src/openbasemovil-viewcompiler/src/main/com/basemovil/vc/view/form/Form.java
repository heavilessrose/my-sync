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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.view.View;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:17:37
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Form bean.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Form
    extends View
{
    private boolean     readOnly        = true;
    private Boolean     saveOnAccept;
    private Boolean     defaultButtons;
    private Vector      items           = new Vector(10);
    private Vector      itemNames       = new Vector(10);

    public Form()
    {
        viewType = "form";
    }

    public boolean isReadOnly()
    {
        return readOnly;
    }

    public void setReadOnly( final boolean readOnly )
    {
        this.readOnly = readOnly;
    }

    public Boolean getSaveOnAccept()
    {
        return saveOnAccept;
    }

    public void setSaveOnAccept( final Boolean saveOnAccept )
    {
        this.saveOnAccept = saveOnAccept;
    }

    public Boolean getDefaultButtons()
    {
        return defaultButtons;
    }

    public void setDefaultButtons( final Boolean defaultButtons )
    {
        this.defaultButtons = defaultButtons;
    }

    public void add( final Item item )
            throws Exception
    {
        if( itemNames.contains( item ) )
        {
            throw new Exception( "Duplicate item: " + item.getName() + " @ " + name );
        }
        else
        {
            item.setParent( this );
            items.add( item );
            itemNames.add( item.getName() );
        }
    }

    protected void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeBoolean( readOnly );
            out.writeBoolean( saveOnAccept );
            out.writeBoolean( defaultButtons );
            final int count = items.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final Item item = (Item) items.get( i );
                item.store( out );
            }
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
