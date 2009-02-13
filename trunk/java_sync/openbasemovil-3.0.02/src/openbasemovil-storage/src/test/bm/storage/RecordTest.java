package bm.storage;

import junit.framework.TestCase;
import junit.framework.TestSuite;

import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 12:53:58
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Test suite for Record.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class RecordTest
        extends TestCase
{
    public RecordTest()
    {
        super();
    }

    public RecordTest( String string )
    {
        super( string );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite( RecordTest.class.getName() );
        try
        {
            suite.addTest( new RecordTest( "testBasicSerialization" ) );
            suite.addTest( new RecordTest( "testSetRecord" ) );
            suite.addTest( new RecordTest( "testDelete" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        return suite;
    }

    public void testBasicSerialization()
    {
        final Record record = new Record( 1, 10 );
        assertEquals( 10, record.multiplexer );
        assertNotNull( record.items );
        assertEquals( 10, record.items.length );
        assertFalse( record.dirty );
        assertEquals( 0, record.size );
        for( int i = 0; i < 10; i++ )
        {
            assertNull( record.items[i] );
        }

        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final SerializerOutputStream out = new SerializerOutputStream( baos );

        try
        {
            record.serialize( out );

            final byte[] data = baos.toByteArray();
            assertNotNull( data );
            assertEquals( 19, data.length );
            ByteArrayInputStream bais = new ByteArrayInputStream( data );
            SerializerInputStream in = new SerializerInputStream( bais );

            final Record record2 = new Record( 1, 20 );
            record2.deserialize( in );
            assertEquals( 10, record2.multiplexer );
            assertNotNull( record2.items );
            assertEquals( 10, record2.items.length );
            assertFalse( record2.dirty );
            assertEquals( 0, record2.size );
            for( int i = 0; i < 10; i++ )
            {
                assertNull( record2.items[i] );
            }
        }
        catch( SerializationException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testSetRecord()
    {
        final Record record = new Record( 1, 10 );

        final byte[] set = createTestData( 40 );

        try
        {
            record.set( -1, set, 0, set.length );
            fail( "Should not be able to set at index -1" );
        }
        catch( RSException e )
        {
        }

        try
        {
            record.set( 11, set, 0, set.length );
            fail( "Should not be able to set at index 11" );
        }
        catch( RSException e )
        {
        }

        try
        {
            // Set the first item
            record.set( 0, set, 0, set.length );
            assertNotNull( record.items[0] );
            byteArrayEquals( set, record.items[0] );
            assertTrue( record.dirty );
            assertEquals( 1, record.size );
            testSerialize( record );

            // Reset the first item
            record.set( 0, set, 1, set.length - 2 );
            assertNotNull( record.items[0] );
            final byte[] aux = new byte[ set.length - 2 ];
            System.arraycopy( set, 1, aux, 0, aux.length );
            byteArrayEquals( aux, record.items[0]);
            assertTrue( record.dirty );
            assertEquals( 1, record.size );
            testSerialize( record );

            // Set the 4th item
            record.set( 3, set, 0, set.length );
            assertNotNull( record.items[3] );
            byteArrayEquals( set, record.items[3] );
            assertTrue( record.dirty );
            assertEquals( 2, record.size );
            testSerialize( record );
        }
        catch( RSException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private byte[] createTestData( final int length )
    {
        final byte[] set = new byte[ length ];
        for( int i = 0; i < length; i++ )
        {
            set[i] = (byte) (i % 128);
        }
        return set;
    }

    public void testDelete()
    {
        final byte[] set = createTestData( 1017 );

        try
        {
            final Record record = new Record( 1, 10 );
            for( int i = 0; i < record.multiplexer; i++ )
            {
                record.set( i, set, 0, set.length );
            }

            try
            {
                record.delete( -1 );
                fail( "Should not be able to delete index -1" );
            }
            catch( RSException e )
            {
            }

            try
            {
                record.delete( record.multiplexer + 1 );
                fail( "Should not be able to delete index " + (record.multiplexer + 1) );
            }
            catch( RSException e )
            {
            }

            record.delete( 8 );
            assertEquals( 9, record.size );
            assertNull( record.items[8] );
            assertTrue( record.dirty );
            testSerialize( record );

            record.delete( 3 );
            assertEquals( 8, record.size );
            assertNull( record.items[3] );
            assertNull( record.items[8] );
            assertTrue( record.dirty );
            testSerialize( record );

            int size = 8;
            for( int i = 0; i < record.multiplexer; i++ )
            {
                if( i != 3 && i != 8 )
                {
                    record.delete( i );
                    size--;
                    assertEquals( size, record.size );
                    assertNull( record.items[i] );
                    assertTrue( record.dirty );
                    testSerialize( record );
                }
            }
        }
        catch( RSException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void testSerialize( final Record record )
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            record.serialize( out );

            final byte[] data = baos.toByteArray();
            final ByteArrayInputStream bais = new ByteArrayInputStream( data );
            final SerializerInputStream in = new SerializerInputStream( bais );
            final Record record2 = new Record( 1, record.multiplexer );
            record2.deserialize( in );
            assertEquals( record, record2 );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void assertEquals( final Record r1, final Record r2 )
    {
        assertEquals( r1.multiplexer, r2.multiplexer );
        assertEquals( r1.size, r2.size );
        for( int i = 0; i < r1.multiplexer; i++ )
        {
            if( r1.items[i] == null && r2.items[i] != null )
            {
                fail( "Expected null item at index " + i );
            }
            else if( r1.items[i] != null && r2.items[i] == null )
            {
                fail( "Expected non-null item at index " + i );
            }
            else if( r1.items[i] != null )
            {
                byteArrayEquals( r1.items[i], r2.items[i] );
            }
        }
    }

    static void byteArrayEquals( final byte[] a1, final byte[] a2 )
    {
        if( a1 != null && a2 != null )
        {
            if( a1.length != a2.length )
            {
                fail( "Expected length: " + a1.length + ", actual length: " + a2.length );
            }
            else
            {
                for( int i = 0; i < a1.length; i++ )
                {
                    if( a1[i] != a2[i] )
                    {
                        fail( "Expected " + a1[i] + " but was " + a2[i] );
                    }
                }
            }
        }
        else if( a1 != null || a2 != null )
        {
            fail( "One array is null and the other is not" );
        }
    }
}
