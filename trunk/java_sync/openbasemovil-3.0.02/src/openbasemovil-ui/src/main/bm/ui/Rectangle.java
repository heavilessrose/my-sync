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
/*
 * File Information
 *
 * Created on       : 21-ago-2007 0:35:51
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A rectangular zone.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Rectangle
{
    private int x;
    private int y;
    private int width;
    private int height;

    public Rectangle(
            final int x,
            final int y,
            final int width,
            final int height
    )
    {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public int getX()
    {
        return x;
    }

    public void setX( final int x )
    {
        this.x = x;
    }

    public int getY()
    {
        return y;
    }

    public void setY( final int y )
    {
        this.y = y;
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

    public boolean isIn( final int x, final int y )
    {
        return this.x <= x && (this.x + width) > x &&
               this.y <= y && (this.y + height) > y;
    }

    public void fill( final Graphics g )
    {
        g.fillRect( x, y, width, height );
    }

    public void setClip( final Graphics g )
    {
        g.setClip( x, y, width, height );
    }
}
