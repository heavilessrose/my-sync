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

import com.basemovil.vc.TopLevelElement;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;

import java.util.Vector;
import java.io.ByteArrayOutputStream;

import bm.db.index.Index;
import bm.core.io.SerializerOutputStream;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:18:34
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Base view class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class View
    extends TopLevelElement
{
    protected String    viewType;
    protected int       id;
    protected String    name;
    protected String    title;
    protected String    controller;
    protected Vector    actionNames = new Vector( 10 );
    protected Vector    actions     = new Vector( 10 );
    protected Vector    attachments = new Vector( 10 );

    public int getId()
    {
        return id;
    }

    public void setId( final int id )
    {
        this.id = id;
    }

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public String getTitle()
    {
        return title;
    }

    public void setTitle( final String title )
    {
        this.title = title;
    }

    public String getController()
    {
        return controller;
    }

    public void setController( final String controller )
    {
        this.controller = controller;
    }

    public void add( final Action action )
            throws Exception
    {
        if( actionNames.contains( action.getName() ) )
        {
            throw new Exception( "Duplicate action: " + action.getName() + " @ " + name );
        }
        else
        {
            if( action.getOrder() == -1 )
            {
                action.setOrder( actions.size() + 1 );
            }
            action.setParent( this );
            actions.add( action );
            actionNames.add( action.getName() );
        }
    }

    public void add( final Attachment attachment )
    {
        attachment.setParent( this );
        attachments.add( attachment );
    }

    public final void store( final Index index )
            throws ViewCompilerException
    {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final SerializerOutputStream out = new SerializerOutputStream( baos );

        try
        {
            out.writeByte( (byte) 1 );
            out.writeInt( id );
            out.writeString( ViewCompiler.escape( title ) );
            out.writeNullableString( controller );

            int count = actions.size();
            out.writeInt( count );
            if( count > 0 )
            {
                for( int i = 0; i < count; i++ )
                {
                    final Action action = (Action) actions.get( i );
                    action.store( out );
                }
            }
            count = attachments.size();
            out.writeInt( count );
            if( count > 0 )
            {
                for( int i = 0; i < count; i++ )
                {
                    final Attachment attachment = (Attachment) attachments.get( i );
                    attachment.store( out );
                }
            }

            store( out );

            out.flush();
            out.close();
            index.insertObject( viewType + "." + name, baos.toByteArray() );
        }
        catch( Exception e )
        {
            throw new ViewCompilerException( e );
        }
    }

    protected abstract void store( final SerializerOutputStream out )
        throws ViewCompilerException;
}
