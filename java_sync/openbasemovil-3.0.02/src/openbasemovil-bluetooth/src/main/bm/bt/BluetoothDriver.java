package bm.bt;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Util Library, miscellaneous tools
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

import bm.core.ControlledTask;

import java.io.IOException;
import java.io.InputStream;
/*
 * File Information
 *
 * Created on       : 22-ago-2007 12:29:20
 * Created by       : Javier
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A ControlledTask for background BluetoothDriver operation.
 *
 * @author <a href="mailto:javier.chacon@elondra.com">Javier Chacon</a>
 * @version $Revision$
 */
public abstract class BluetoothDriver
        extends ControlledTask
{
    protected InputStream is;
    protected String port;

    public BluetoothDriver( final String name, final String port )
    {
        super( name, true );
        this.port = port;
    }

    protected void task()
    {
        connect();
        while( !cancel )
        {
            try
            {
                while( !cancel )
                {
                    read();
                }
            }
            catch( IOException e )
            {
                readCancelled();
                if( !cancel )
                {
                    connect();
                    if( is == null )
                    {
                        try
                        {
                            Thread.sleep( 10000 );
                        }
                        catch( InterruptedException e1 )
                        {
                        }
                    }
                }
            }
        }
        disconnect();
    }

    protected abstract void read()
            throws IOException;

    protected abstract void disconnect();

    protected abstract void connect();

    protected void readCancelled()
    {
    }

    public void die()
    {
        if( is != null ) try{ is.close(); } catch( Exception e ){}
    }
}
