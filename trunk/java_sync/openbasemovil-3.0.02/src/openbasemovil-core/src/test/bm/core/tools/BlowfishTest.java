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
 * Created on       : 04-ene-2007 14:31:26
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * TestSuite for Blowfish.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class BlowfishTest
        extends TestCase
{
    public BlowfishTest()
    {
        super();
    }

    public BlowfishTest( String string )
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

        final Blowfish cipher = new Blowfish();
        cipher.init( true, "MiClaveBonita".getBytes() );
        final byte[] encrypted = cipher.encrypt( plain );
        final byte[] plain2    = cipher.decryptBytes( encrypted );

        for( int i = 0; i < 100; i++ )
        {
            if( plain[i] != plain2[i] )
            {
                fail( "Bad encryption/decryption" );
            }
        }
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                LocalFormatToolsTest.class.getName()
        );

        try
        {
            suite.addTest( new BlowfishTest( "testByte" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }

        return suite;
    }
}
