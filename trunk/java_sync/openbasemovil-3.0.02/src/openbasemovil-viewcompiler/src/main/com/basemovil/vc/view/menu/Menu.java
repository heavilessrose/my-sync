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

import com.basemovil.vc.view.View;
import com.basemovil.vc.ViewCompilerException;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;

import java.util.List;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:38:01
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A menu.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Menu
    extends View
{
    public static final byte    ENHANCED    = 0;
    public static final byte    NATIVE      = 1;

    public static final byte    LIST        = 0;
    public static final byte    GRID        = 1;

    private String  type;
    private String  helpId;
    private String  imageMap;
    private String  imageMapBig;
    private String  mode;
    private String  iconSet;
    private Integer fontFace;
    private Integer fontStyle;
    private Integer fontSize;
    private Integer color;
    private Integer selectedColor;
    private Integer background;
    private Integer selectedBackground;
    private Integer helpLines;
    private List    items = new Vector( 10 );

    public Menu()
    {
        viewType = "menu";
    }

    public String getType()
    {
        return type;
    }

    public void setType( final String type )
    {
        this.type = type;
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

    public String getMode()
    {
        return mode;
    }

    public void setMode( final String mode )
    {
        this.mode = mode;
    }

    public String getIconSet()
    {
        return iconSet;
    }

    public void setIconSet( final String iconSet )
    {
        this.iconSet = iconSet;
    }

    public Integer getFontFace()
    {
        return fontFace;
    }

    public void setFontFace( final Integer fontFace )
    {
        this.fontFace = fontFace;
    }

    public Integer getFontStyle()
    {
        return fontStyle;
    }

    public void setFontStyle( final Integer fontStyle )
    {
        this.fontStyle = fontStyle;
    }

    public Integer getFontSize()
    {
        return fontSize;
    }

    public void setFontSize( final Integer fontSize )
    {
        this.fontSize = fontSize;
    }

    public Integer getColor()
    {
        return color;
    }

    public void setColor( final Integer color )
    {
        this.color = color;
    }

    public Integer getSelectedColor()
    {
        return selectedColor;
    }

    public void setSelectedColor( final Integer selectedColor )
    {
        this.selectedColor = selectedColor;
    }

    public Integer getBackground()
    {
        return background;
    }

    public void setBackground( final Integer background )
    {
        this.background = background;
    }

    public Integer getSelectedBackground()
    {
        return selectedBackground;
    }

    public void setSelectedBackground( final Integer selectedBackground )
    {
        this.selectedBackground = selectedBackground;
    }

    public Integer getHelpLines()
    {
        return helpLines;
    }

    public void setHelpLines( final Integer helpLines )
    {
        this.helpLines = helpLines;
    }

    public void add( final MenuItem item )
    {
        items.add( item );
    }

    protected void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeByte( type == null || !type.equals( "native" ) ? ENHANCED : NATIVE );
            out.writeNullableString( helpId );
            out.writeNullableString( imageMap );
            out.writeNullableString( imageMapBig );
            out.writeByte( mode == null || !mode.equals( "grid" ) ? LIST : GRID );
            out.writeNullableString( iconSet );
            out.writeInt( fontFace );
            out.writeInt( fontStyle );
            out.writeInt( fontSize );
            out.writeInt( color );
            out.writeInt( selectedColor );
            out.writeInt( background );
            out.writeInt( selectedBackground );
            out.writeInt( helpLines );
            final int count = items.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final MenuItem item = (MenuItem) items.get( i );
                item.store( out );
            }
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
