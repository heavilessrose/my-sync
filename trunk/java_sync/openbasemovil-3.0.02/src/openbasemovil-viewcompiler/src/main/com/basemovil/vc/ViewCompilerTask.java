package com.basemovil.vc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil View Compiler, generates the binary form of views from
    an XML file.
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

import org.apache.tools.ant.Task;
import org.apache.tools.ant.BuildException;

import java.io.File;
import java.util.Vector;
import java.util.Iterator;
/*
 * File Information
 *
 * Created on       : 18-oct-2007 0:01:04
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Ant Task for the View Compiler.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ViewCompilerTask
    extends Task
{
    private Integer order;
    private String  out;
    private String  files;

    public Integer getOrder()
    {
        return order;
    }

    public void setOrder( final Integer order )
    {
        this.order = order;
    }

    public String getOut()
    {
        return out;
    }

    public void setOut( final String out )
    {
        this.out = out;
    }

    public String getFiles()
    {
        return files;
    }

    public void setFiles( final String files )
    {
        this.files = files;
    }

    private Vector getFileList()
    {
        final Vector v = new Vector( 10 );
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
                        v.add( buffer.toString().trim() );
                        buffer.delete( 0, buffer.length() );
                    }
                    break;

                default:
                    buffer.append( chars[i] );
            }
        }
        if( buffer.toString().trim().length() > 0 )
        {
            v.add( buffer.toString().trim() );
        }
        return v;
    }

    public void execute()
            throws BuildException
    {
        try
        {
            System.out.println( "ViewCompiler v1.0.1" );
            final ViewCompiler compiler = new ViewCompiler();
            if( order != null )
            {
                compiler.setOrder( order.intValue() );
            }
            for( final Iterator i = getFileList().iterator(); i.hasNext(); )
            {
                compiler.parse( new File( (String) i.next() ) );
            }
            compiler.store( new File( out ) );
        }
        catch( Exception e )
        {
            throw new BuildException( e );
        }
    }
}
