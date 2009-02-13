package com.elondra.commenter;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Commenter, a simple preprocessor to comment out things
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

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 16-ene-2008 16:57:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Ant Task for the commenter tool.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CommenterTask
    extends Task
{
    private Commenter commenter = new Commenter();

    private String    in;
    private String    out;
    private String    patterns;

    public String getIn()
    {
        return in;
    }

    public void setIn( final String in )
    {
        this.in = in;
    }

    public String getOut()
    {
        return out;
    }

    public void setOut( final String out )
    {
        this.out = out;
    }

    public String getPatterns()
    {
        return patterns;
    }

    public void setPatterns( final String patterns )
    {
        this.patterns = patterns;
    }

    public void execute()
            throws BuildException
    {
        try
        {
            final Vector v = new Vector( 10 );
            final StringBuffer buffer = new StringBuffer();
            final char[] chars = patterns.toCharArray();
            final int size = chars.length;
            boolean escape = false;
            for( int i = 0; i < size; i++ )
            {
                switch( chars[i] )
                {
                    case '\\':
                        if( escape )
                        {
                            buffer.append( '\\' );
                            escape = false;
                        }
                        else
                        {
                            escape = true;
                        }
                        break;

                    case ',':
                        if( escape )
                        {
                            buffer.append( ',' );
                            escape = false;
                        }
                        else
                        {
                            if( buffer.length() > 0 )
                            {
                                v.add( buffer.toString() );
                                buffer.delete( 0, buffer.length() );
                            }
                        }
                        break;

                    default:
                        if( escape )
                        {
                            escape = false;
                        }
                        buffer.append( chars[i] );
                }
            }
            if( buffer.length() > 0 )
            {
                v.add( buffer.toString() );
            }
            commenter.execute( in, out, v );
        }
        catch( Exception e )
        {
            throw new BuildException( e );
        }
    }
}
