package bm.core.fs;
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

import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 21-nov-2006 9:09:22
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * File System test.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class FSTest
        extends TestCase
{
    public FSTest()
    {
        super();
    }

    public FSTest( String string )
    {
        super( string );
    }

    public void testNonEmpty()
    {
        // Write to file
        SequentialFileOutputStream os = new SequentialFileOutputStream( "test" );
        try
        {
            os.open();
            for( int i = 0; i < 1050; i++ )
            {
                for( int b = -10; b < 10; b++ )
                {
                    os.write( b );
                }
            }
            os.flush();
            os.close();
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        finally
        {
            try{ os.close(); }catch( Exception e ){}
        }

        RandomAccessInputStream sf = null;
        try
        {
            sf = new RandomAccessInputStream( "test" );
            for( int i = 0; i < 1050; i++ )
            {
                for( int b = -10; b < 10; b++ )
                {
                    int c = sf.read();
                    assertEquals( b, c );
                }
            }
            try
            {
                if( sf.read() != -1 )
                {
                    fail( "Should not read more" );
                }
            }
            catch( IOException e )
            {
            }
            sf.seek( 0 );
            for( int b = -10; b < 10; b++ )
            {
                int c = sf.read();
                assertEquals( b, c );
            }
            sf.seek( 20 );
            for( int i = 0; i < 1049; i++ )
            {
                for( int b = -10; b < 10; b++ )
                {
                    int c = sf.read();
                    assertEquals( b, c );
                }
            }
            sf.seek( 20 );
            sf.seek( 20980 );
            for( int b = -10; b < 10; b++ )
            {
                int c = sf.read();
                assertEquals( b, c );
            }
            sf.close();
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        finally
        {
            if( sf != null ) try{ sf.close(); }catch( Exception e ){}
        }
    }

    public void testEmpty()
    {
        // Empty file
        SequentialFileOutputStream os = new SequentialFileOutputStream( "test2" );
        try
        {
            os.open();
            os.flush();
            os.close();
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        finally
        {
            try{ os.close(); }catch( Exception e ){}
        }

        RandomAccessInputStream sf = null;
        try
        {
            sf = new RandomAccessInputStream( "test2" );
            assertTrue( sf.isEof() );
            if( sf.read() != -1 )
            {
                fail( "Should not read" );
            }
            sf.close();
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        finally
        {
            if( sf != null ) try{ sf.close(); }catch( Exception e ){}
        }
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                FSTest.class.getName()
        );

        try
        {
            suite.addTest( new FSTest( "testEmpty" ) );
            suite.addTest( new FSTest( "testNonEmpty" ) );
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }
}
