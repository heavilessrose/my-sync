package com.elondra.rp;

import java.util.Map;
import java.util.Iterator;
import java.io.OutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
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

/*
 * File Information
 *
 * Created on       : 16-ene-2008 21:05:43
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Writes consolidated properties as text.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class TextWriter
    implements ResourceWriter
{
    private String out;
    private int order;

    public String getOut()
    {
        return out;
    }

    public void setOut( final String out )
    {
        this.out = out;
    }

    public int getOrder()
    {
        return order;
    }

    public void setOrder( final int order )
    {
        this.order = order;
    }

    public void write( final Map consolidated )
            throws PackException
    {
        for( final Iterator i = consolidated.keySet().iterator(); i.hasNext(); )
        {
            final String language = (String) i.next();
            final Properties lang = (Properties) consolidated.get( language );
            OutputStream os = null;
            try
            {
                final File file = new File( out + "." + language + ".properties" );
                if( !file.getParentFile().exists() )
                {
                    file.getParentFile().mkdirs();
                }
                os = new FileOutputStream( file );
                lang.save( os );
            }
            catch( IOException e )
            {
                throw new PackException( e );
            }
            finally
            {
                if( os != null ) try{ os.close(); }catch( Exception e ){}
            }
        }
    }
}
