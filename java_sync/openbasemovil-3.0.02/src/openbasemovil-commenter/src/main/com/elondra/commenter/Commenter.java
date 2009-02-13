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

import java.io.*;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 29-may-2006 10:15:21
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Simple line code commenter.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Commenter
{
    private static String ignore = ".svn|.cvs";

    public static void main( final String[] args )
    {
        (new Commenter()).Main( args );
    }

    private void Main( final String[] args )
    {
        if( args.length < 3 )
        {
            System.err.println( "Usage: commenter <in folder> <out folder> <pattern, ...>" );
            System.exit( 1 );
        }

        try
        {
            execute( args );
        }
        catch( Exception e )
        {
            e.printStackTrace();
        }
    }

    void execute( final String[] args )
            throws Exception
    {
        final File input = new File( args[0] );
        final File output = new File( args[1] );
        if( !input.exists() || !input.isDirectory() )
        {
            throw new Exception( args[0] + " is not a valid directory" );
        }

        if( output.exists() && !output.isDirectory() )
        {
            throw new Exception( args[1] + " is not a valid directory" );
        }
        if( !output.exists() )
        {
            output.mkdirs();
        }

        final String[] patterns = new String[ args.length - 2 ];
        System.arraycopy( args, 2, patterns, 0, patterns.length );

        run( input, output, patterns );
    }

    void execute(
            final String    in,
            final String    out,
            final Vector    patternList
    )
            throws Exception
    {
        final File input = new File( in );
        final File output = new File( out );
        if( !input.exists() || !input.isDirectory() )
        {
            throw new Exception( in + " is not a valid directory" );
        }

        if( output.exists() && !output.isDirectory() )
        {
            throw new Exception( out + " is not a valid directory" );
        }
        if( !output.exists() )
        {
            output.mkdirs();
        }

        final String[] patterns = new String[ patternList.size() ];
        for( int i = 0; i < patternList.size(); i++ )
        {
            patterns[i] = (String) patternList.get( i );
        }

        run( input, output, patterns );
    }

    private void run(
            final File      input,
            final File      output,
            final String[]  patterns
    )
            throws IOException
    {
        parse( input, output, patterns, true );
    }

    private void parse(
            final File      input,
            final File      output,
            final String[]  patterns,
            final boolean   first
    )
            throws IOException
    {
        if( ignore.contains( input.getName().toLowerCase() ) )
        {
            return;
        }

        if( input.isDirectory() )
        {
            final File newOutput;
            if( !first )
            {
                newOutput = new File( output, input.getName() );
                newOutput.mkdirs();
            }
            else
            {
                newOutput = output;
            }

            final File[] children = input.listFiles();
            if( children != null )
            {
                for( int i = 0; i < children.length; i++ )
                {
                    parse(
                            children[i],
                            newOutput,
                            patterns,
                            false
                    );
                }
            }
        }
        else
        {
            process(
                    input,
                    new File( output, input.getName() ),
                    patterns
            );
        }
    }

    private void process(
            final File input,
            final File output,
            final String[] patterns
    )
            throws IOException
    {
        FileReader r = null;
        FileWriter w = null;

        try
        {
            r = new FileReader( input );
            w = new FileWriter( output );
            final BufferedReader in = new BufferedReader( r );
            final BufferedWriter out = new BufferedWriter( w );
            String line = in.readLine();
            while( line != null )
            {
                if( checkPatterns( line, patterns ) )
                {
                    out.write( "// [Commenter]" + line );
                }
                else
                {
                    out.write( line );
                }
                out.newLine();
                line = in.readLine();
            }
            out.flush();
            out.close();
            in.close();
        }
        finally
        {
            try{ if( r != null ) r.close(); }catch( Exception e ){}
            try{ if( w != null ) w.close(); }catch( Exception e ){}
        }

    }

    private boolean checkPatterns( final String line, final String[] patterns )
    {
        if( line.indexOf( "[Commenter-Don't]" ) >= 0 )
        {
            return false;
        }
        final int size = patterns.length;
        for( int i = 0; i < size; i++ )
        {
            if( line.indexOf( patterns[i] ) >= 0 )
            {
                return true;
            }
        }
        return false;
    }
}
