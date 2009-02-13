package bm.core.tools;
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
import bm.core.tools.StringTokenizer;
/*
 * File Information
 *
 * Created on       : 15-ago-2006 23:52:24
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Test Suite for StringTokenizer.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StringTokenizerTest
    extends TestCase
{
    public StringTokenizerTest()
    {
        super();
    }

    public StringTokenizerTest( String string )
    {
        super( string );
    }

    public void test()
    {
        final StringTokenizer stk = new StringTokenizer(
                "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47",
                ","
        );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "GPGGA", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "123519", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "4807.038", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "N", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "01131.000", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "E", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "1", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "08", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "0.9", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "545.4", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "M", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "46.9", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "M", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "", stk.nextToken() );
        assertTrue( stk.hasMoreTokens() );
        assertEquals( "*47", stk.nextToken() );
        assertFalse( stk.hasMoreTokens() );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                StringTokenizerTest.class.getName()
        );
        try
        {
            suite.addTest( new StringTokenizerTest( "test" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        return suite;
    }
}
