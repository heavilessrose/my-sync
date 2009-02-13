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

/*
 * File Information
 *
 * Created on       : 15-ago-2006 16:59:19
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Simple GPS data reader, uses NMEA 0183 protocol.<br/>
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class GpsReader
        extends BluetoothDriver
{
    public static final int     ET_GPS  = 8888;
    public static final Integer GPS     = new Integer( ET_GPS );
    public static final String  GPS_KEY = "gpsInfo";

    private StreamConnection    conn;
    private InputStream         is;
    private GpsInfo             gpsInfo = new GpsInfo();
    private StringBuffer        command = new StringBuffer();
    private Event               event = new Event( ET_GPS );

    public GpsReader( final String url )
    {
        super( "GpsReader", url );
        event.attach( GPS_KEY, gpsInfo );
    }

    public GpsInfo getGpsInfo()
    {
        return gpsInfo;
    }

    protected void disconnect()
    {
        if( is != null || conn != null )
        {
            gpsInfo.setStatus( GpsInfo.STATUS_DISCONNECTING );
            event.dispatch();
            try{ if( is != null ) { is.close(); } } catch( Exception e ){}
            try{ if( conn != null ) { conn.close(); } } catch( Exception e ){}
            gpsInfo.setStatus( GpsInfo.STATUS_DISCONNECTED );
            event.dispatch();
        }
    }

    protected void connect()
    {
        disconnect();
        gpsInfo.setStatus( GpsInfo.STATUS_CONNECTING );
        event.dispatch();
        try
        {
            conn = (StreamConnection) Connector.open( port );
            is = conn.openInputStream();
            gpsInfo.setStatus( GpsInfo.STATUS_CONNECTED );
            event.dispatch();
        }
        catch( IOException e )
        {
            gpsInfo.setStatus( GpsInfo.STATUS_DISCONNECTED);
            event.dispatch();
        }
    }

    protected void read()
            throws IOException
    {
        int c = is.read();
        switch( c )
        {
            case '\r':
                //noinspection ResultOfMethodCallIgnored
                is.read(); // skip \n
                // continue to $ case ->

            case '$':
                if( command.length() > 0 )
                {
                    parseCommand( command.toString() );
                }
                command.delete( 0, command.length() );
                break;

            default:
                command.append( (char) c );
                break;
        }
    }

    protected void readCancelled()
    {
        command.delete( 0, command.length() );
    }

    private void parseCommand( final String command )
    {
        if( command.startsWith( "GPGGA" ) )
        {
            if( gpsInfo.parseGGA( command ) )
            {
                event.dispatch();
            }
        }
    }
}
