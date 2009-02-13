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

import junit.framework.TestCase;
import junit.framework.TestSuite;
/*
 * File Information
 *
 * Created on       : 16-ago-2006 9:53:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Test Suite for GpsInfo.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class GpsInfoTest
    extends TestCase
{
    private static GpsInfo gpsInfo;

    public GpsInfoTest()
    {
        super();
    }

    public GpsInfoTest( String string )
    {
        super( string );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                GpsInfoTest.class.getName()
        );
        try
        {
            suite.addTest( new GpsInfoTest( "testParseGGA" ) );
            suite.addTest( new GpsInfoTest( "testConversions" ) );
        }
        catch( Exception e )
        {
            Assert.fail( e.getMessage() );
        }
        return suite;
    }

    public void testConversions()
    {
        assertEquals( "48.117", gpsInfo.getGISLatitude().toString( '.', null ) );
        assertEquals( "11.517", gpsInfo.getGISLongitude().toString( '.', null ) );
        assertEquals( "N 48º 7' 2''", gpsInfo.getLatitudeString() );
        assertEquals( "E 11º 31' 0''", gpsInfo.getLongitudeString() );
        gpsInfo.setLongitude( 'W' );
        assertEquals( "-11.517", gpsInfo.getGISLongitude().toString( '.', null ) );
        assertEquals( "W 11º 31' 0''", gpsInfo.getLongitudeString() );
    }

    public void testParseGGA()
    {
        gpsInfo = new GpsInfo();

        Assert.assertTrue( gpsInfo.parseGGA(
                "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"
        ) );
        assertEquals( (byte) 48, gpsInfo.getLatitudeDegree() );
        assertEquals( "7.038", gpsInfo.getLatitudeMinutes().toString( '.', null ) );
        assertEquals( 'N', gpsInfo.getLatitude() );
        assertEquals( (byte) 11, gpsInfo.getLongitudeDegree() );
        assertEquals( "31.000", gpsInfo.getLongitudeMinutes().toString( '.', null ) );
        assertEquals( 'E', gpsInfo.getLongitude() );
        assertEquals( 1, gpsInfo.getQuality() );
        assertEquals( 8, gpsInfo.getSatellites() );
        assertEquals( "545.4", gpsInfo.getAltitude().toString( '.', null ) );
    }
}
