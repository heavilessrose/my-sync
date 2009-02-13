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

import java.util.*;
import java.io.*;
/*
 * File Information
 *
 * Created on       : 16-ene-2008 21:18:41
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Full fledged resource packer, with both text and binary options.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class AbstractPacker
{
    protected String    out;
    protected List      resourceBundles = new Vector( 10 );

    public String getOut()
    {
        return out;
    }

    public void setOut( final String out )
    {
        this.out = out;
    }

    public void configure( final String files )
    {
        final StringBuffer buffer = new StringBuffer();
        final char[] chars = files.toCharArray();
        final int size = chars.length;
        for( int i = 0; i < size; i++ )
        {
            switch( chars[i] )
            {
                case '\t':
                case '\n':
                case '\r':
                    break;

                case ',':
                    if( buffer.toString().trim().length() > 0 )
                    {
                        addConfigured( new ResourceFile( buffer.toString().trim() ) );
                        buffer.delete( 0, buffer.length() );
                    }
                    break;

                default:
                    buffer.append( chars[i] );
            }
        }
        if( buffer.toString().trim().length() > 0 )
        {
            addConfigured( new ResourceFile( buffer.toString().trim() ) );
        }
    }

    public void addConfigured( ResourceFile file )
    {
        resourceBundles.add( file );
    }

    public final void execute()
            throws PackException
    {
        if( out == null )
        {
            throw new PackException( "out property must be set" );
        }
        if( resourceBundles.size() == 0 )
        {
            throw new PackException( "At least one resource file must be specified" );
        }
        final Map consolidated = merge();
        checkErrors( consolidated );
        write( consolidated );
    }

    protected abstract void write( final Map consolidated )
        throws PackException;

    protected abstract void configure( final String[] args );

    protected void Main( final String[] args )
    {
        if( args.length < 4 )
        {
            System.out.println( "usage: packer outpath mode resource_file,..." );
        }
        else
        {
            try
            {
                configure( args );
                for( int i = 2; i < args.length; i++ )
                {
                    final ResourceFile rf = new ResourceFile();
                    rf.setFile( args[i] );
                    addConfigured( rf );
                }
                execute();
            }
            catch( PackException e )
            {
                e.printStackTrace();
            }
        }
    }

    protected void checkErrors( final Map consolidated )
            throws PackException
    {
        System.out.println( "Checking for errors" );
        final Map errors = new HashMap( 10 );
        for( final Iterator i = consolidated.keySet().iterator(); i.hasNext(); )
        {
            final String language = (String) i.next();
            final Properties lang = (Properties) consolidated.get( language );
            for( final Iterator j = lang.keySet().iterator(); j.hasNext(); )
            {
                final Object key = j.next();
                for( final Iterator k = consolidated.keySet().iterator(); k.hasNext(); )
                {
                    final String language2 = (String) k.next();
                    if( !language2.equals( language ) )
                    {
                        final Properties lang2 = (Properties)
                                consolidated.get( language2 );
                        if( !lang2.containsKey( key ) )
                        {
                            StringBuffer list = (StringBuffer)
                                    errors.get( key.toString() );
                            if( list == null )
                            {
                                list = new StringBuffer();
                            }
                            else
                            {
                                list.append( "," );
                            }
                            list.append( language2 );
                            errors.put( key.toString(), list );
                        }
                    }
                }
            }
        }
        if( errors.size() > 0 )
        {
            showErrors( errors );
            throw new PackException( "Unbalanced resources" );
        }
    }

    protected Map merge()
            throws PackException
    {
        System.out.println( "Merging files" );
        final Map consolidated = new HashMap( 10 );
        final Properties aux = new Properties();
        for( final Iterator i = resourceBundles.iterator(); i.hasNext(); )
        {
            final ResourceFile resourceFile = (ResourceFile) i.next();
            final File base = new File( resourceFile.getFile() );
            final File dir = base.getParentFile();
            final String baseName = base.getName();
            if( !dir.exists() )
            {
                throw new PackException( "Base directory does not exist: " + base.getAbsolutePath() );
            }
            final File[] files = dir.listFiles(
                    new FilenameFilter()
                    {
                        public boolean accept( File dir, String name )
                        {
                            return name.startsWith( baseName + "." )
                                    && name.endsWith( ".properties" );
                        }
                    }
            );
            if( files == null || files.length == 0 )
            {
                throw new PackException( "No files matching: " + resourceFile.getFile() );
            }
            for( int j = 0; j < files.length; j++ )
            {
                final File target = files[j];
                final StringTokenizer stk = new StringTokenizer( target.getName(), "." );
                stk.nextToken();
                final String language = stk.nextToken();

                Properties lang = (Properties) consolidated.get( language );
                if( lang == null )
                {
                    lang = new Properties();
                }
                aux.clear();
                InputStream is = null;
                try
                {
                    System.out.println( "Loading " + target );
                    is = new FileInputStream( target );
                    aux.load( is, resourceFile.getEncoding() );
                }
                catch( IOException e )
                {
                    throw new PackException( e );
                }
                finally
                {
                    if( is != null ) try{ is.close(); }catch( Exception e ){}
                }
                for( final Iterator k = aux.keySet().iterator(); k.hasNext(); )
                {
                    final Object name = k.next();
                    if( !lang.containsKey( name ) )
                    {
                        lang.put( name, aux.get( name ) );
                    }
                }
                consolidated.put( language, lang );
            }
        }
        System.out.println( "Resources consolidated" );
        return consolidated;
    }

    private void showErrors( final Map errors )
    {
        for( final Iterator i = errors.keySet().iterator(); i.hasNext(); )
        {
            final String key = (String) i.next();
            System.err.println(
                    "Missing resource '" + key + "' in language(s): " +
                    errors.get( key )
            );
        }
    }
}
