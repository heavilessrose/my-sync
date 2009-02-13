package com.elondra.rp;
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

import java.util.Hashtable;
import java.util.Enumeration;
import java.io.*;

/*
 * File Information
 *
 * Created on       : 12-mar-2007 23:05:58
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Properties implementation fully compatible with the mobile one.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Properties
    extends Hashtable
    implements Serializable
{
    private String encoding = "UTF-8";

    /**
     * Void constructor.
     */
    public Properties()
    {
    }

    /**
     * Save the contents to a stream.
     * @param os output stream
     * @throws IOException on errors
     */
    public void save( final OutputStream os )
            throws IOException
    {
        final OutputStreamWriter out = new OutputStreamWriter(
                os,
                encoding
        );
        for( Enumeration i = keys(); i.hasMoreElements(); )
        {
            final String key = (String) i.nextElement();
            final String value = (String) get( key );
            out.write( key );
            out.write( "=" );
            out.write( escape( value ) );
            out.write( "\n" );
        }
        out.flush();
    }

    private String escape( final String value )
    {
        final StringBuffer buffer = new StringBuffer();
        final int count = value.length();
        for( int i = 0; i < count; i++ )
        {
            char c = value.charAt( i );
            switch( c )
            {
                case '\\':
                    buffer.append( "\\\\" );
                    break;

                case '\n':
                    buffer.append( "\\n" );
                    break;

                case '\r':
                    buffer.append( "\\r" );
                    break;

                case '\t':
                    buffer.append( "\\t" );

                default:
                    buffer.append( c );
            }
        }
        return buffer.toString();
    }

    /**
     * Create a properties object loading it from the given stream.
     * @param is input
     * @throws IOException on errors
     */
    public Properties( final InputStream is )
            throws IOException
    {
        load( is, encoding );
    }

    /**
     * Create a properties object loading it from the given stream with the
     * given encoding.
     * @param is input
     * @param encoding encoding
     * @throws IOException on errors
     */
    public Properties( final InputStream is, final String encoding )
            throws IOException
    {
        load( is, encoding );
    }

    /**
     * Load the properties from the given file, does not delete existing
     * properties.
     * @param is input
     * @throws IOException on errors
     * @see #load(java.io.InputStream, String)
     */
    public void load( final InputStream is )
            throws IOException
    {
        load( is, encoding );
    }

    /**
     * Load the properties from the given file with the given encoding, does not
     * delete the existing properties.<br/>
     * This method handles comments (lines starting with #), and escaping through the
     * \ character. If a backslash is followed by a new line character, the new
     * line is ignored, if it's followed by a n or a t the respective control
     * character is inserted (new line or tab).<br/>
     * Carriage return characeters are alaways ignored (\r).<br/>
     * Invalid lines (whithout an equal sign or starting whit an equal sign) are
     * ignored.<br/>
     * Empty properties are setProperty to an empty string, not to null.
     * @param is input
     * @param encoding encoding
     * @throws IOException on errors
     */
    public synchronized void load( final InputStream is, final String encoding )
            throws IOException
    {
        final InputStreamReader r = new InputStreamReader(
                is,
                encoding
        );
        StringBuffer line = new StringBuffer();
        int c = r.read();
        boolean escape = false;
        while( c != -1 )
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
                        processLine( line.toString() );
                        line = new StringBuffer();
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
            c = r.read();
        }
        if( line.toString().length() > 0 )
        {
            processLine( line.toString() );
        }
    }

    public String getProperty( final String key )
    {
        return (String) get( key );
    }

    /**
     * Put all elements from another properties object, eventually overriding
     * the ones at this object.
     * @param props properties to add
     */
    public void putAll( final Properties props )
    {
        for( final Enumeration i = props.keys(); i.hasMoreElements(); )
        {
            final String key = (String) i.nextElement();
            put( key, props.getProperty( key ) );
        }
    }

    private void processLine( final String line )
    {
        if( line == null || line.trim().length() == 0 )
        {
            return;
        }
        if( line.charAt( 0 ) == '#' )
        {
            return;
        }
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
            put( trimmedKey, line.substring( ++i ) );
        }
    }
}
