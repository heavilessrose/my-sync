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

import java.io.IOException;

/**
 * Dummy comm port that writes debug information to the console.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DebugCommPort
    extends AbstractCommPort
{
    public void open()
            throws IOException
    {
        System.out.println( "[DEBUG-CommPort] open" );
    }

    public void close()
    {
        System.out.println( "[DEBUG-CommPort] close" );
    }

    public void write( final byte c )
            throws IOException
    {
        System.out.println( "[DEBUG-CommPort] write( 0x" + Integer.toString( c, 16 ) + "): " + toChar( c )  );
    }

    private char toChar( final byte c )
    {
        return c >= 0x20 ? (char) c : ' ';
    }

    public int write( final byte[] data, final int offset, final int length )
            throws IOException
    {
        System.out.println( "[DEBUG-CommPort] write: " );
        print( data, offset, length );
        return data != null ? length : 0;
    }

    private void print( final byte[] data, final int offset, final int length )
    {
        if( data != null )
        {
            int count = 0;
            StringBuffer bytes = new StringBuffer();
            StringBuffer chars = new StringBuffer();
            for( int i = offset; i < (offset + length); i++ )
            {
                bytes.append( Integer.toString( data[i], 16 ) ).append( " " );
                chars.append( data[i] > 32 ? (char) data[i] : ' ' );
                count++;
                if( count > 30 )
                {
                    System.out.println( bytes.toString() + " | " + chars.toString() );
                    count = 0;
                    bytes = new StringBuffer();
                    chars = new StringBuffer();
                }
            }
            if( count > 0 )
            {
                System.out.println( bytes.toString() + " | " + chars.toString() );
            }
        }
    }

    public int write( final byte[] data )
            throws IOException
    {
        return data != null ? write( data, 0, data.length ) : 0;
    }

    public boolean setConfiguration()
    {
        System.out.println( "[DEBUG-CommPort] setConfiguration" );
        return true;
    }
}
