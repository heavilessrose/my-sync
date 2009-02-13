package bm.core.mvc;
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
import bm.core.ResourceManager;
import bm.core.CoreConstants;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 20-mar-2007 11:53:36
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Help provider that retrieves help from resources in the classpath.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class ResourceHelpProvider
    implements HelpProvider
{
    /**
     * Find help for a given id.
     *
     * @param helpId   help id
     * @return help or null if no help
     * @throws java.io.IOException on errors
     */
    public String getHelp( String helpId )
            throws IOException
    {
        InputStream is = null;
        InputStreamReader in = null;
        try
        {
            is = ResourceManager.findLocalizedStream(
                    this.getClass(),
                    helpId,
                    ".txt",
                    false
            );
            if( is == null )
            {
                is = ResourceManager.findLocalizedStream(
                        this.getClass(),
                        helpId,
                        ".txt",
                        true
                );
            }
            if( is != null )
            {
                in = new InputStreamReader( is, CoreConstants.DEFAULT_ENCODING );
                final StringBuffer buffer = new StringBuffer();
                int c = in.read();
                while( c != -1 )
                {
                    buffer.append( (char) c );
                    c = in.read();
                }
//                log.debug( "help text:" + buffer.toString() );
                return buffer.toString();
            }
            else
            {
                return null;
            }
        }
        finally
        {
            try{ if( in != null ) in.close(); }catch( Exception e ){}
            try{ if( is != null ) is.close(); }catch( Exception e ){}
        }
    }
}
