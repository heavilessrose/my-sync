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

import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Image;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 21-ago-2007 23:16:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A toolbar with buttons.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Toolbar
{
    private int     firstIndex;
    private Font    font;
    private Vector  buttons = new Vector( 10 );
    private int     color;
    private boolean selected;
    private int     selectedIndex;
    private int     maxButtonHeight;
    private int     backgroundColor = 0xF0F0F0;

    private int lastIndex;

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

    public int getBackgroundColor()
    {
        return backgroundColor;
    }

    public void setBackgroundColor( final int backgroundColor )
    {
        this.backgroundColor = backgroundColor;
    }

    public boolean isSelected()
    {
        return selected;
    }

    public void setSelected( final boolean selected )
    {
        this.selected = selected;
    }

    public void addButton( final ToolButton button )
    {
        if( button.image == null )
        {
            final Font font = this.font;
            final int width = font.stringWidth( button.text ) + 2;
            final int height = font.getHeight() + 2;
            button.image = Image.createImage( width, height );
            final Graphics g = button.image.getGraphics();
            g.setColor( 0xD0D0D0 );
            g.fillRect( 0, 0, width, height );
            g.setColor( color );
            g.drawString(
                    button.text,
                    1,
                    1 + font.getBaselinePosition(),
                    Graphics.LEFT | Graphics.BASELINE
            );
            if( button.help == null )
            {
                button.help = button.text;
            }
        }
        maxButtonHeight = Math.max( maxButtonHeight, button.image.getHeight() );
        buttons.addElement( button );
    }

    public int getHeight()
    {
        return maxButtonHeight + 4;
    }

    public String getHelp()
    {
        if( buttons.size() > 0 )
        {
            return ((ToolButton) buttons.elementAt( selectedIndex )).help;
        }
        else
        {
            return "";
        }
    }

    public void move( final int offset )
    {
        selectedIndex += offset;
        if( offset > 0 && selectedIndex > lastIndex )
        {
            firstIndex++;
            if( firstIndex > buttons.size() )
            {
                firstIndex = 0;
            }
        }
        else if( offset < 0 && selectedIndex < firstIndex )
        {
            firstIndex--;
            if( firstIndex < 0 )
            {
                firstIndex = buttons.size() - 1;
            }
        }
        if( selectedIndex >= buttons.size() )
        {
            selectedIndex = 0;
        }
        else if( selectedIndex < 0 )
        {
            selectedIndex = buttons.size() - 1;
        }
    }

    public void paint(
            final Graphics  g,
            final int       x0,
            final int       y0,
            final int       width,
            final int       height
    )
    {
        int y = y0 + 1;
        int x = x0 + 1;
        final int maxX = width - 2;
        final int count = buttons.size();
        int index = firstIndex;

        g.setColor( backgroundColor );
        g.fillRect( x0, y0, width, height );
        for( int i = 0; i < count && x < maxX; i++ )
        {
            final ToolButton button = (ToolButton) buttons.elementAt( index );
            final boolean isSelected = selected && i == selectedIndex;
            if( isSelected )
            {
                g.setColor( 0 );
                g.drawRect(
                        x,
                        y,
                        button.image.getWidth() + 2,
                        button.image.getHeight() + 2
                );
            }
            g.drawImage(
                    button.image,
                    x + 1,
                    y + 1,
                    Graphics.LEFT | Graphics.TOP
            );
            x += button.image.getWidth() + 2;
            lastIndex = index;
            index++;
            if( index > count )
            {
                index = 0;
            }
        }
        y = y0 + maxButtonHeight + 3;
        g.drawLine( x0 + 1, y, width - 2, y );
    }
}
