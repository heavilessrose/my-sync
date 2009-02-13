package com.basemovil.vc.view.menu;
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
 * Created on       : 17-oct-2007 23:02:39
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A menu item.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuItem
{
    private String  title;
    private int     action;
    private String  onlyIf;
    private String  unless;
    private Integer iconIndex;
    private Integer selectedIconIndex;
    private String  help;
    private String  view;

    public String getTitle()
    {
        return title;
    }

    public void setTitle( final String title )
    {
        this.title = title;
    }

    public int getAction()
    {
        return action;
    }

    public void setAction( final int action )
    {
        this.action = action;
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

    public Integer getIconIndex()
    {
        return iconIndex;
    }

    public void setIconIndex( final Integer iconIndex )
    {
        this.iconIndex = iconIndex;
    }

    public Integer getSelectedIconIndex()
    {
        return selectedIconIndex;
    }

    public void setSelectedIconIndex( final Integer selectedIconIndex )
    {
        this.selectedIconIndex = selectedIconIndex;
    }

    public String getHelp()
    {
        return help;
    }

    public void setHelp( final String help )
    {
        this.help = help;
    }

    public String getView()
    {
        return view;
    }

    public void setView( final String view )
    {
        this.view = view;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 2 );
            out.writeString( ViewCompiler.escape( title ) );
            out.writeInt( action );
            out.writeNullableString( onlyIf );
            out.writeNullableString( unless );
            out.writeInt( iconIndex );
            out.writeInt( selectedIconIndex );
            out.writeNullableString( ViewCompiler.escape( help ) );
            out.writeNullableString( ViewCompiler.escape( view ) );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
