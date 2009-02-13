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

import bm.core.event.Event;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/*
 * File Information
 *
 * Created on       : 19-jul-2006 11:47:11
 * Created by       : Javier
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Microvision Flic driver.<br/>
 *
 * @author <a href="mailto:javier.chacon@elondra.com">Javier Chacón</a>
 * @version $Revision$
 */
public class FlicReader
        extends BluetoothDriver
{
    public static final int     ET_BARCODE  = 8889;
    public static final Integer BARCODE     = new Integer( ET_BARCODE );
    public static final String  BARCODE_KEY = "barCode";

    private OutputStream        out;
    private StreamConnection    conn;
    private String              lastSerialNumber = "";
    private Event               event = new Event( ET_BARCODE );

    private static final byte ACK = 0x06;

//    private static Log log = LogFactory.getLog( "FlicReader" );

    public FlicReader( final String port )
            throws IOException
    {
        super( "FlicReader", port );
        openConn();
        configFlic();
        disconnect();
    }

    protected void read()
            throws IOException
    {
        String barCode = readCode();
        if( barCode != null )
        {
            event.attach( BARCODE_KEY, barCode );
            event.dispatch();
        }
    }

    public void connect()
    {
        try
        {
            openConn();
            out.write( new byte[] {0x44, 0x0D });
        }
        catch( IOException e )
        {
        }
        disconnect();
    }

    private String readLine()
            throws IOException
    {
        final InputStream in = this.is;
        if( in != null )
        {
            final StringBuffer line = new StringBuffer();
            int c = in.read();
            if( c == ACK )
            {
                return null;
            }
            while( c != -1 )
            {
                if( c == '\r' )
                {
                    return line.toString();
                }
                else if( c >= ' ' )
                {
                    line.append( (char) c );
                }
                c = in.read();
            }
            return line.length() > 0 ? line.toString() : null;
        }
        else
        {
            return null;
        }
    }

    private void writeAck( final byte[] cmd )
            throws IOException
    {
        out.write( cmd );
        out.write( ACK );
    }

    private void configFlic()
            throws
            IOException
    {
//        conn = (StreamConnection) Connector.open( this.port );
//        out = conn.openOutputStream();
//        out.write( new byte[] {0x42, 0x4D, 0x31, 0x0D} );
        out.write( new byte[] {0x53, 0x30, 0x39, 0x30, 0x34, 0x0D} );
//        out.write( new byte[] {0x53, 0x30, 0x42, 0x30, 0x31, 0x0D} );
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod*/
    public String readCode()
            throws
            IOException
    {
        openConn();
        final String line = readLine();
        if( line != null )
        {
            final int codeSize = line.length() - 4;
            final String serialNumber = line.substring( codeSize );
            if ( !lastSerialNumber.equals( serialNumber ) )
            {
                lastSerialNumber = serialNumber;
                final String barCode = line.substring( 0, codeSize );
                writeAck( lastSerialNumber.getBytes( "US-ASCII" ) );
                disconnect();
                return barCode;
            }
            else
            {
                writeAck( lastSerialNumber.getBytes( "US-ASCII" ) );
                disconnect();
                return readCode();
            }
        }
        else
        {
//            shutdown();
            return null;
        }
    }

//    public void shutdown()
//    {
//        try
//        {
//            openConn();
//            out.write( new byte[] {0x54, 0x35, 0x0D} );
//        }
//        catch( IOException e )
//        {
//        }
//        finally
//        {
//            disconnect();
//        }
//   }

    public void disconnect()
    {
        if( out != null ) try{ out.close(); }catch( Exception e ){}
        if( is != null ) try{ is.close(); }catch( Exception e ){}
        if( conn != null ) try{ conn.close(); }catch( Exception e ){}
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod*/
    private void openConn()
            throws
            IOException
    {
        conn = (StreamConnection) Connector.open( this.port );
        is = conn.openInputStream();
        out = conn.openOutputStream();
    }
}
