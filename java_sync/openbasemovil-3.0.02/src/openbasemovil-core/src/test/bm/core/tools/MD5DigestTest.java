package bm.core.tools;
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
 * Created on       : 13-mar-2007 18:25:43
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * TestSuite for MD5Digest.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class MD5DigestTest
        extends TestCase
{
    public MD5DigestTest()
    {
        super();
    }

    public MD5DigestTest( String string )
    {
        super( string );
    }

    public void testByte()
    {
        final byte[] plain = new byte[ 100 ];
        for( int i = 0; i < 100; i++ )
        {
            plain[i] = (byte) i;
        }

        final MD5Digest digest = new MD5Digest();

        final byte[] out1 = new byte[16];
        digest.update( plain, 0, plain.length );
        digest.doFinal( out1, 0 );

        plain[0] = 100;

        final byte[] out2 = new byte[16];
        digest.update( plain, 0, plain.length );
        digest.doFinal( out2, 0 );

        plain[0] = 0;

        final byte[] out3 = new byte[16];
        digest.update( plain, 0, plain.length );
        digest.doFinal( out3, 0 );

        assertFalse( checkEqual( out1, out2 ) );
        assertTrue( checkEqual( out1, out3 ) );
    }

    private boolean checkEqual( final byte[] b1, final byte[] b2 )
    {
        for( int i = 0; i < b1.length; i++ )
        {
            if( b1[i] != b2[i] )
            {
                return false;
            }
        }
        return true;
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                MD5DigestTest.class.getName()
        );

        try
        {
            suite.addTest( new MD5DigestTest( "testByte" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }

        return suite;
    }
}
