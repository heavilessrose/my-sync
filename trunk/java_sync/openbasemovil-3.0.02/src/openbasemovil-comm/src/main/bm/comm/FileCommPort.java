/*
 * Copyright (c) 2006 Your Corporation. All Rights Reserved.
 */
package bm.comm;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Comm Library, an abstraction layer over COM ports
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

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * Dummy communication port useful for debugging. Just echoes the printable
 * characters to a file (and new lines). Requires CDC or Java SE.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class FileCommPort
    extends AbstractCommPort
{
    FileWriter fw;
    PrintWriter out;

    public void open()
            throws IOException
    {
        fw = new FileWriter( name );
        out = new PrintWriter( fw );
    }

    public void close()
    {
        try { out.flush(); } catch( Exception e ){}
        try { out.close(); } catch( Exception e ){}
        try { fw.flush(); } catch( Exception e ){}
        try { fw.close(); } catch( Exception e ){}
    }

    public void write( final byte c )
            throws IOException
    {
        if( c >= 32 )
        {
            out.print( (char) c );
        }
        else if( c == '\n' )
        {
            out.print( '\n' );
        }
    }

    public int write( final byte[] data, final int offset, final int length )
            throws IOException
    {
        if( data != null )
        {
            for( int i = 0; i < length; i++ )
            {
                write( data[ offset + i ] );
            }
            return length;
        }
        else
        {
            return 0;
        }
    }

    public int write( final byte[] data )
            throws IOException
    {
        return data != null ? write( data, 0, data.length ) : 0;
    }

    public boolean setConfiguration()
    {
        return false;
    }
}
