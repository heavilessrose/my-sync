package com.basemovil.vc;
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

import bm.db.index.Index;
import bm.core.io.SerializerOutputStream;

import java.io.ByteArrayOutputStream;

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
 * An icon set.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class IconSet
    extends TopLevelElement
{
    private String  id;
    private String  source;
    private int     width;
    private int     height;

    public String getId()
    {
        return id;
    }

    public void setId( final String id )
    {
        this.id = id;
    }

    public String getSource()
    {
        return source;
    }

    public void setSource( final String source )
    {
        this.source = source;
    }

    public int getWidth()
    {
        return width;
    }

    public void setWidth( final int width )
    {
        this.width = width;
    }

    public int getHeight()
    {
        return height;
    }

    public void setHeight( final int height )
    {
        this.height = height;
    }

    /**
     * Store the binary element definition in the index.
     *
     * @param index index
     * @throws com.basemovil.vc.ViewCompilerException
     *          on errors
     */
    public void store( final Index index )
            throws ViewCompilerException
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );

            out.writeByte( (byte) 1 );
            out.writeString( source );
            out.writeInt( width );
            out.writeInt( height );
            out.flush();
            out.close();

            index.insertObject( "iconset." + id, baos.toByteArray() );
        }
        catch( Exception e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
