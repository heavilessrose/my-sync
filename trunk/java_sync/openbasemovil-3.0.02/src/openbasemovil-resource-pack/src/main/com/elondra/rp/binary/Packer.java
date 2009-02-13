package com.elondra.rp.binary;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ResourcePacker, packs and checks resource files
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

import com.elondra.rp.AbstractPacker;
import com.elondra.rp.PackException;
import com.elondra.rp.TextWriter;
import com.elondra.rp.ResourceWriter;

import java.util.*;

/*
 * File Information
 *
 * Created on       : 16-oct-2007 23:22:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Standalone class that really does the packaging.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Packer
        extends AbstractPacker
{
    protected String    mode;
    protected int       order;

    public int getOrder()
    {
        return order;
    }

    public void setOrder( final int order )
    {
        this.order = order;
    }

    public String getMode()
    {
        return mode;
    }

    public void setMode( final String mode )
    {
        this.mode = mode;
    }

    public static void main( String[] args )
    {
        (new Packer()).Main( args );
    }

    protected void configure( final String[] args )
    {
        setOut( args[0] );
        setMode( args[1] );
    }

    protected void write( final Map consolidated )
            throws PackException
    {
        if( mode == null || !mode.equalsIgnoreCase( "binary" ) )
        {
            final TextWriter writer = new TextWriter();
            writer.setOut( out );
            writer.write( consolidated );
        }
        else
        {
            final ResourceWriter writer = new BinaryWriter();
            writer.setOut( out );
            writer.setOrder( order );
            writer.write( consolidated );
        }
    }
}
