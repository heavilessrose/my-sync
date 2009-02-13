/*
 * Copyright (c) 2006 Elondra, S.L. All Rights Reserved.
 */
package bm.bt;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Bluetooth Library, Bluetooth utilties
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

import javax.microedition.io.StreamConnection;
import javax.microedition.io.Connector;
import java.io.IOException;
import java.io.OutputStream;

import bm.core.log.LogFactory;
import bm.core.log.Log;
import bm.comm.AbstractCommPort;

/*
 * File Information
 *
 * Created on       : 13-feb-2006 16:36:04
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Bluetooth comm port adapter.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class BluetoothCommPort
    extends AbstractCommPort
{
    private static Log log = LogFactory.getLog( "BluetoothCommPort" );

    private StreamConnection    conn;
    private OutputStream        out;

    /** @noinspection FieldRepeatedlyAccessedInMethod*/
    public void open()
            throws IOException
    {
        if( conn == null )
        {
            try
            {
                //noinspection ConnectionOpenedButNotSafelyClosed
                conn = (StreamConnection) Connector.open( name );
                if( conn != null )
                {
                    out = conn.openOutputStream();
                }
            }
            catch( IOException e )
            {
                log.error( e );
            }
        }
    }

    public void close()
    {
        try{ out.close(); }catch( Exception e ){}
        try{ conn.close(); }catch( Exception e ){}
        out = null;
        conn = null;
    }

    public void write( final byte b )
            throws IOException
    {
        if( out != null )
        {
            out.write( b );
        }
    }

    public int write( final byte[] bytes, final int offset, final int length )
            throws IOException
    {
        if( out != null && bytes != null )
        {
            out.write( bytes, offset, length );
            return length;
        }
        else
        {
            return 0;
        }
    }

    public int write( final byte[] bytes )
            throws IOException
    {
        if( bytes != null )
        {
            return write( bytes, 0, bytes.length );
        }
        else
        {
            return 0;
        }
    }

    public boolean setConfiguration()
    {
        return true;
    }
}
