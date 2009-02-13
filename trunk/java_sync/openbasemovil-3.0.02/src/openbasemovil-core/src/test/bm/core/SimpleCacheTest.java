package bm.core;
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

import junit.framework.TestCase;
import junit.framework.TestSuite;
/*
 * File Information
 *
 * Created on       : 11-may-2006 17:38:10
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Test for SimpleCache.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SimpleCacheTest
        extends TestCase
{
    public SimpleCacheTest()
    {
        super();
    }

    public SimpleCacheTest( final String s )
    {
        super( s );
    }

    public void test()
    {
        final SimpleCache sc = new SimpleCache( 10 );

        for( int i = 0; i < 10; i++ )
        {
            final String key = "k" + i;
            final String data = "o" + i;
            sc.add( key, data );
            assertEquals( i + 1, sc.getSize() );
            assertEquals( data, sc.get( key ) );
        }

        sc.add( "k11", "o11" );
        assertEquals( 10, sc.getSize() );
        assertNull( sc.get( "k0" ) );
        for( int i = 1; i < 10; i++ )
        {
            assertNotNull( sc.get( "k" + i ) );
        }
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                SimpleCacheTest.class.getName()
        );

        try
        {
            suite.addTest( new SimpleCacheTest( "test" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }

        return suite;
    }
}
