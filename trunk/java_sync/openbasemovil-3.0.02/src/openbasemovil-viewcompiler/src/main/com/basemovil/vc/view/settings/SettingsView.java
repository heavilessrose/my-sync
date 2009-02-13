package com.basemovil.vc.view.settings;
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

import com.basemovil.vc.view.View;
import com.basemovil.vc.ViewCompilerException;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;

import java.util.List;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:30:48
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Settings view bean.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SettingsView
    extends View
{
    private String  helpId;
    private String  imageMap;
    private String  imageMapBig;

    private List    items = new Vector( 10 );

    public SettingsView()
    {
        viewType = "settingsView";
    }

    public String getHelpId()
    {
        return helpId;
    }

    public void setHelpId( final String helpId )
    {
        this.helpId = helpId;
    }

    public String getImageMap()
    {
        return imageMap;
    }

    public void setImageMap( final String imageMap )
    {
        this.imageMap = imageMap;
    }

    public String getImageMapBig()
    {
        return imageMapBig;
    }

    public void setImageMapBig( final String imageMapBig )
    {
        this.imageMapBig = imageMapBig;
    }

    public void add( final Setting setting )
    {
        items.add( setting );
    }

    protected void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeNullableString( helpId );
            out.writeNullableString( imageMap );
            out.writeNullableString( imageMapBig );

            final int count = items.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final Setting setting = (Setting) items.get( i );
                setting.store( out );
            }
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
