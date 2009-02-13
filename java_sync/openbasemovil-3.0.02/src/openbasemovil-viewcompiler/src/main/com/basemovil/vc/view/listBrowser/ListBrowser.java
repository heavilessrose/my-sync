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

import com.basemovil.vc.view.View;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;

import java.util.List;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:08:55
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A list browser.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ListBrowser
    extends View
{
    public static final byte    ENHANCED    = 0;
    public static final byte    NATIVE      = 1;

    private String  type;
    private String  separator;
    private Integer pageSize;
    private Boolean wrap;
    private Integer fontFace;
    private Integer fontStyle;
    private Integer fontSize;
    private Integer color;
    private Integer selectedColor;
    private Integer background;
    private Integer selectedBackground;
    private Integer maxLines;

    private List fields = new Vector( 10 );
    private List speedOptions = new Vector( 10 );

    public ListBrowser()
    {
        viewType = "listBrowser";
    }

    public String getType()
    {
        return type;
    }

    public void setType( final String type )
    {
        this.type = type;
    }

    public String getSeparator()
    {
        return separator;
    }

    public void setSeparator( final String separator )
    {
        this.separator = separator;
    }

    public Integer getPageSize()
    {
        return pageSize;
    }

    public void setPageSize( final Integer pageSize )
    {
        this.pageSize = pageSize;
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

    public Integer getMaxLines()
    {
        return maxLines;
    }

    public void setMaxLines( final Integer maxLines )
    {
        this.maxLines = maxLines;
    }

    public Boolean getWrap()
    {
        return wrap;
    }

    public void setWrap( final Boolean wrap )
    {
        this.wrap = wrap;
    }

    public void add( final Field field )
    {
        fields.add( field );
    }

    public void add( final SpeedOption option )
    {
        speedOptions.add( option );
    }

    protected void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        try
        {
            out.writeByte( (byte) 1 );
            out.writeByte(
                    type == null ?
                        null :
                        new Byte( !type.equals( "native" ) ? ENHANCED : NATIVE )
            );
            out.writeNullableString( ViewCompiler.escape( separator ) );
            out.writeInt( pageSize );
            out.writeBoolean( wrap );
            out.writeInt( fontFace );
            out.writeInt( fontStyle );
            out.writeInt( fontSize );
            out.writeInt( color );
            out.writeInt( selectedColor );
            out.writeInt( background );
            out.writeInt( selectedBackground );
            out.writeInt( maxLines );

            int count = fields.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final Field field = (Field) fields.get( i );
                field.store( out );
            }

            count = speedOptions.size();
            out.writeInt( count );
            for( int i = 0; i < count; i++ )
            {
                final SpeedOption option = (SpeedOption) speedOptions.get( i );
                option.store( out );
            }
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
