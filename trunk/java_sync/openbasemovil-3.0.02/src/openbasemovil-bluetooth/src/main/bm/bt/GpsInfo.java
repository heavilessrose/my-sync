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

import bm.core.tools.StringTokenizer;
import bm.core.math.FixedPoint;

import java.util.Date;
/*
 * File Information
 *
 * Created on       : 15-ago-2006 17:01:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * GPS information structure.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class GpsInfo
{
    public static final int Q_INVALID       = 0;
    public static final int Q_GPS_FIX       = 1;
    public static final int Q_DGPS_FIX      = 2;
    public static final int Q_PPS_FIX       = 3;
    public static final int Q_RTK           = 4;
    public static final int Q_FLOAT_RTK     = 5;
    public static final int Q_ESTIMATED     = 6;
    public static final int Q_MANUAL        = 7;
    public static final int Q_SIMULATION    = 8;

    public static final byte STATUS_DISCONNECTED     = 0;
    public static final byte STATUS_CONNECTING       = 1;
    public static final byte STATUS_CONNECTED        = 2;
    public static final byte STATUS_DISCONNECTING    = 3;

    private byte        latitudeDegree;
    private FixedPoint  latitudeMinutes = FixedPoint.ZERO;
    private char        latitude;
    private byte        longitudeDegree;
    private FixedPoint  longitudeMinutes = FixedPoint.ZERO;
    private char        longitude;
    private short       quality;
    private short       satellites;
    private FixedPoint  altitude = FixedPoint.ZERO;
    private byte        status;
    private Date        timestamp = new Date( 0 );

    public FixedPoint getGISLatitude()
    {
        return latitudeMinutes
                .setPrecision( 6 )
                .div( latitude == 'N' ? 60 : -60 )
                .add( latitudeDegree * (latitude == 'N' ? 1 : -1) );
    }

    public FixedPoint getGISLongitude()
    {
        return longitudeMinutes
                .setPrecision( 6 )
                .div( longitude == 'W' ? -60 : 60 )
                .add( longitudeDegree * (longitude == 'W' ? -1 : 1) );
    }

    public String getLatitudeString()
    {
        final int minutes = (int) latitudeMinutes.trunc();
        final int seconds = (int) latitudeMinutes.sub( minutes ).mult( 60 ).setPrecision( 0 ).trunc();
        return new StringBuffer()
                .append( latitude ).append( ' ' )
                .append( latitudeDegree ).append( "º " )
                .append( minutes ).append( "' " )
                .append( seconds ).append( "''")
                .toString();
    }

    public String getLongitudeString()
    {
        final int minutes = (int) longitudeMinutes.trunc();
        final int seconds = (int) longitudeMinutes.sub( minutes ).mult( 60 ).setPrecision( 0 ).trunc();
        return new StringBuffer()
                .append( longitude ).append( ' ' )
                .append( longitudeDegree ).append( "º " )
                .append( minutes ).append( "' " )
                .append( seconds ).append( "''")
                .toString();
    }

    public byte getStatus()
    {
        return status;
    }

    public void setStatus( final byte status )
    {
        this.status = status;
    }

    public byte getLatitudeDegree()
    {
        return latitudeDegree;
    }

    public synchronized void setLatitudeDegree( final byte latitudeDegree )
    {
        this.latitudeDegree = latitudeDegree;
    }

    public FixedPoint getLatitudeMinutes()
    {
        return latitudeMinutes;
    }

    public synchronized void setLatitudeMinutes( final FixedPoint latitudeMinutes )
    {
        this.latitudeMinutes = latitudeMinutes;
    }

    public char getLatitude()
    {
        return latitude;
    }

    public synchronized void setLatitude( final char latitude )
    {
        this.latitude = latitude;
    }

    public byte getLongitudeDegree()
    {
        return longitudeDegree;
    }

    public synchronized void setLongitudeDegree( final byte longitudeDegree )
    {
        this.longitudeDegree = longitudeDegree;
    }

    public FixedPoint getLongitudeMinutes()
    {
        return longitudeMinutes;
    }

    public synchronized void setLongitudeMinutes( final FixedPoint longitudeMinutes )
    {
        this.longitudeMinutes = longitudeMinutes;
    }

    public char getLongitude()
    {
        return longitude;
    }

    public synchronized void setLongitude( final char longitude )
    {
        this.longitude = longitude;
    }

    public short getQuality()
    {
        return quality;
    }

    public synchronized void setQuality( final short quality )
    {
        this.quality = quality;
    }

    public short getSatellites()
    {
        return satellites;
    }

    public synchronized void setSatellites( final short satellites )
    {
        this.satellites = satellites;
    }

    public FixedPoint getAltitude()
    {
        return altitude;
    }

    public synchronized void setAltitude( final FixedPoint altitude )
    {
        this.altitude = altitude;
    }

    public Date getTimestamp()
    {
        return timestamp;
    }

    public synchronized void setTimestamp( final Date timestamp )
    {
        this.timestamp = timestamp;
    }

    public boolean isFresh( final long maxTime )
    {
        return System.currentTimeMillis() - timestamp.getTime() < maxTime;
    }

    public synchronized boolean parseGGA( final String command )
    {
        /*
            GGA - essential fix data which provide 3D location and accuracy data.

            $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

            Where:
                 GGA          Global Positioning System Fix Data
                 123519       Fix taken at 12:35:19 UTC
                 4807.038,N   Latitude 48 deg 07.038' N
                 01131.000,E  Longitude 11 deg 31.000' E
                 1            Fix quality: 0 = invalid
                                           1 = GPS fix (SPS)
                                           2 = DGPS fix
                                           3 = PPS fix
                               4 = Real Time Kinematic
                               5 = Float RTK
                                           6 = estimated (dead reckoning) (2.3 feature)
                               7 = Manual input mode
                               8 = Simulation mode
                 08           Number of satellites being tracked
                 0.9          Horizontal dilution of position
                 545.4,M      Altitude, Meters, above mean sea level
                 46.9,M       Height of geoid (mean sea level) above WGS84
                                  ellipsoid
                 (empty field) time in seconds since last DGPS update
                 (empty field) DGPS station ID number
                 *47          the checksum data, always begins with
        */
        try
        {
            final byte oldLatitudeDegree = latitudeDegree;
            final FixedPoint oldLatitudeMinutes = latitudeMinutes.clone();
            final char oldLatitude = latitude;
            final byte oldLongitudeDegree = longitudeDegree;
            final FixedPoint oldLongitudeMinutes = longitudeMinutes.clone();
            final char oldLongitude = longitude;
            final short oldQuality = quality;
            final short oldSatellites = satellites;
            final FixedPoint oldAltitude = altitude.clone();

            final StringTokenizer stk = new StringTokenizer( command, "," );
            stk.nextToken(); // Skip GPGGA
            stk.nextToken(); // Skip time
            parseGpsLatitude( stk.nextToken() );
            latitude = stk.nextToken().charAt( 0 );
            parseGpsLongitude( stk.nextToken() );
            longitude = stk.nextToken().charAt( 0 );
            quality = Short.parseShort( stk.nextToken() );
            satellites = Short.parseShort( stk.nextToken() );
            stk.nextToken(); // Skip dilution
            altitude = FixedPoint.parse( stk.nextToken(), '.' );

            final boolean changed =
                    oldLatitude != latitude ||
                    oldLatitudeDegree != latitudeDegree ||
                    !oldLatitudeMinutes.equals( latitudeMinutes ) ||
                    oldLongitude != longitude ||
                    oldLongitudeDegree != longitudeDegree ||
                    !oldLongitudeMinutes.equals( longitudeMinutes ) ||
                    oldQuality != quality ||
                    oldSatellites != satellites ||
                    !oldAltitude.equals( altitude );
            if( changed )
            {
                timestamp.setTime( System.currentTimeMillis() );
            }
            return changed;
        }
        catch( Throwable e )
        {
            return false;
        }
    }

    private void parseGpsLatitude( final String latitude )
    {
        latitudeDegree = Byte.parseByte( latitude.substring( 0, 2 ) );
        latitudeMinutes = FixedPoint.parse( latitude.substring( 2 ), '.' );
    }

    private void parseGpsLongitude( final String longitude )
    {
        longitudeDegree = Byte.parseByte( longitude.substring( 0, 3 ) );
        longitudeMinutes = FixedPoint.parse( longitude.substring( 3 ), '.' );
    }
}
