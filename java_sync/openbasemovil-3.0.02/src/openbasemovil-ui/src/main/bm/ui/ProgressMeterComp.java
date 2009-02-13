package bm.ui;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
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

import bm.core.Application;
import bm.core.ErrorHandler;
import bm.util.Util;

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Gauge;
import javax.microedition.lcdui.game.Sprite;
import java.io.InputStream;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 15-may-2007 4:02:32
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A custom, thin progress meter that takes little space on screen.<br/>
 * This is a simple class is used from other visual classes, like ProgressMeter
 * or EnhancedSplashView.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ProgressMeterComp
{
    private static Image startSlice;
    private static Image endSlice;
    private static Image filledSlice;
    private static Image emptySlice;

    static
    {
        createSlices();
    }

    private int     maxValue;
    private int     value;
    private String  label = "";
    private boolean showLabel = true;
    private Font    font = Util.SMALL_FONT;
    private boolean labelAbove = true;
    private int     color;
    private int     lastValue;
    private int     blockSize = 40;
    private int     increment = 4;

    protected ProgressMeterComp( final String label )
    {
        this.label = label;
    }

    protected int getMinContentWidth()
    {
        return Application.getManager().getDisplay().getCurrent().getWidth();
    }

    protected int getMinContentHeight()
    {
        return startSlice.getHeight() + (showLabel ? font.getHeight() + 1 : 0);
    }

    protected int getPrefContentWidth( int i )
    {
        return Application.getManager().getDisplay().getCurrent().getWidth();
    }

    protected int getPrefContentHeight( int i )
    {
        return startSlice.getHeight() + (showLabel ? font.getHeight() + 1 : 0);
    }

    public String getLabel()
    {
        return label;
    }

    public void setLabel( final String label )
    {
        this.label = label != null ? label : "";
    }

    public boolean isShowLabel()
    {
        return showLabel;
    }

    public void setShowLabel( final boolean showLabel )
    {
        this.showLabel = showLabel;
    }

    public Font getFont()
    {
        return font;
    }

    public void setFont( final Font font )
    {
        this.font = font;
    }

    public int getColor()
    {
        return color;
    }

    public void setColor( final int color )
    {
        this.color = color;
    }

    private static void createSlices()
    {
        final ProgressMeterComp comp = new ProgressMeterComp( "" );
        final InputStream source = comp.getClass()
                .getResourceAsStream( "/progressBar.png" );
        if( source == null )
        {
            ErrorHandler.handleError( comp, "/progressBar.png not found", new Exception() );
        }
        else
        {
            try
            {
                final Image src = Image.createImage( source );
                final int height = src.getHeight();
                startSlice = Image.createImage(
                        src,
                        0,
                        0,
                        1,
                        height,
                        Sprite.TRANS_NONE
                );
                emptySlice = Image.createImage(
                        src,
                        1,
                        0,
                        1,
                        height,
                        Sprite.TRANS_NONE
                );
                filledSlice = Image.createImage(
                        src,
                        2,
                        0,
                        1,
                        height,
                        Sprite.TRANS_NONE
                );
                endSlice = Image.createImage(
                        src,
                        3,
                        0,
                        1,
                        height,
                        Sprite.TRANS_NONE
                );
            }
            catch( IOException e )
            {
                ErrorHandler.handleError( comp, e );
            }
        }
    }

    public int getMaxValue()
    {
        return maxValue;
    }

    public void setMaxValue( final int maxValue )
    {
        this.maxValue = maxValue;
    }

    public int getValue()
    {
        return value;
    }

    public void setValue( final int value )
    {
        if( maxValue != Gauge.INDEFINITE )
        {
            this.value = value;
        }
        else
        {
            lastValue += increment;
            if( lastValue < 0 )
            {
                lastValue = 0;
                increment *= -1;
            }
            else if( lastValue > (100 - blockSize) )
            {
                lastValue = (100 - blockSize);
                increment *= -1;
            }
        }
    }

    public boolean isLabelAbove()
    {
        return labelAbove;
    }

    public void setLabelAbove( final boolean labelAbove )
    {
        this.labelAbove = labelAbove;
    }

    public void paint( Graphics graphics, final int x0, final int y0, int w, int h )
    {
        final Image buffer = Image.createImage( w, h );
        final Graphics g = buffer.getGraphics();
        g.setColor( 0xFFFFFF );
        g.fillRect( 0, 0, w, h );

        if( showLabel )
        {
            if( labelAbove )
            {
                drawLabel( g, w, 0 );
                drawBar( g, w, font.getHeight() + 1 );
            }
            else
            {
                drawBar( g, w, 0 );
                drawLabel( g, w, font.getHeight() + 1 );
            }
        }
        else
        {
            drawBar( g, w, 0 );
        }

        graphics.drawImage( buffer, x0, y0, Graphics.TOP | Graphics.LEFT );
    }

    private void drawLabel( final Graphics g, final int width, final int y )
    {
        g.setFont( font );
        g.setColor( color );
        g.drawString( label, 0, y, Graphics.TOP | Graphics.LEFT );
    }

    private void drawBar( final Graphics g, final int width, final int y )
    {
        final int barWidth = width - 2;
        int x = 0;
        g.drawImage( startSlice, x++, y, Graphics.TOP | Graphics.LEFT );
        if( maxValue != Gauge.INDEFINITE )
        {
            final int filledWidth = maxValue != 0 ? (value * barWidth) / maxValue : 0;
            final int emptyWidth = barWidth - filledWidth;
            for( int i = 0; i < filledWidth; i++ )
            {
                g.drawImage( filledSlice, x++, y, Graphics.TOP | Graphics.LEFT );
            }
            for( int i = 0; i < emptyWidth; i++ )
            {
                g.drawImage( emptySlice, x++, y, Graphics.TOP | Graphics.LEFT );
            }
        }
        else
        {
            final int blockPos = lastValue * barWidth / 100;
            for( int i = 0; i < blockPos; i++ )
            {
                g.drawImage( emptySlice, x++, y, Graphics.TOP | Graphics.LEFT );
            }
            for( int i = 0; i < blockSize; i++ )
            {
                g.drawImage( filledSlice, x++, y, Graphics.TOP | Graphics.LEFT );
            }
            while( x < barWidth )
            {
                g.drawImage( emptySlice, x++, y, Graphics.TOP | Graphics.LEFT );
            }
        }
        g.drawImage( endSlice, x, y, Graphics.TOP | Graphics.LEFT );
    }
}
