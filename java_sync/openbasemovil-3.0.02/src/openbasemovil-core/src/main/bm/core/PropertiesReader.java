package bm.core;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 16-oct-2007 18:28:17
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Reads properties from a stream.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class PropertiesReader
{
    private InputStreamReader r;
    private StringBuffer line = new StringBuffer();

    /**
     * Create a new PropertiesReader using the default encoding, UTF-8.
     *
     * @param is input stream
     */
    public PropertiesReader( final InputStream is )
    {
        try
        {
            r = new InputStreamReader( is, CoreConstants.DEFAULT_ENCODING );
        }
        catch( UnsupportedEncodingException e )
        {
        }
    }

    /**
     * Create a new PropertiesReader using the given encoding.
     *
     * @param is input stream
     * @param encoding encoding
     * @throws UnsupportedEncodingException if the encoding is no supported
     */
    public PropertiesReader( final InputStream is, final String encoding )
            throws UnsupportedEncodingException
    {
        r = new InputStreamReader( is, encoding );
    }

    /**
     * Read the next key-value pair from the file.
     *
     * @return an array with two elements, being the first one the key and the
     * second one the value, or null if no more keys available.
     * @throws IOException on errors
     */
    public String[] next()
            throws IOException
    {
        String line = readLine();
        while( line != null )
        {
            if( line.trim().length() == 0 || line.charAt( 0 ) == '#' )
            {
                line = readLine();
            }
            else
            {
                final String[] retval = processLine( line );
                if( retval == null )
                {
                    line = readLine();
                }
                else
                {
                    return retval;
                }
            }
        }
        return null;
    }

    private String readLine()
            throws IOException
    {
        int c = r.read();
        if( c == -1 )
        {
            return null;
        }
        boolean done = false;
        boolean escape = false;
        while( c != -1 && !done )
        {
            switch( c )
            {
                case '\\':
                    if( !escape )
                    {
                        escape = true;
                    }
                    else
                    {
                        line.append( '\\' );
                        escape = false;
                    }
                    break;

                case '\r':
                    break;

                case '\n':
                    if( escape )
                    {
                        escape = false;
                    }
                    else
                    {
                        done = true;
                    }
                    break;

                case 'n':
                    if( escape )
                    {
                        escape = false;
                        line.append( '\n' );
                    }
                    else
                    {
                        line.append( 'n' );
                    }
                    break;

                case 't':
                    if( escape )
                    {
                        escape = false;
                        line.append( '\t' );
                    }
                    else
                    {
                        line.append( 't' );
                    }
                    break;

                default:
                    line.append( (char) c );
                    escape = false;
                    break;
            }
            if( !done )
            {
                c = r.read();
            }
        }
        final String val = line.toString();
        line.delete( 0, line.length() );
        return val;
    }

    private String[] processLine( final String line )
    {
        final int count = line.length();
        final StringBuffer key = new StringBuffer();
        char c = line.charAt( 0 );
        int i = 0;
        while( c != '=' && i < count )
        {
            key.append( c );
            i++;
            if( i < count )
            {
                c = line.charAt( i );
            }
        }
        final String trimmedKey = key.toString().trim();
        if( trimmedKey.length() > 0 && i < count )
        {
            return new String[] { trimmedKey, line.substring( ++i ) };
        }
        else
        {
            return null;
        }
    }
}
