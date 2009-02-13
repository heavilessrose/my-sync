package bm.storage;

import junit.framework.TestCase;
import junit.framework.TestSuite;
import bm.core.ErrorHandler;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;

import javax.microedition.rms.RecordStore;
import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 12:54:06
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Test suite for StoreInfo.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StoreInfoTest
    extends TestCase
{
    static
    {
        ErrorHandler.setHandler( new UnitTestErrorHandler() );
    }

    public StoreInfoTest()
    {
        super();
    }

    public StoreInfoTest( String string )
    {
        super( string );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite( StoreInfoTest.class.getName() );
        try
        {
            suite.addTest( new StoreInfoTest( "testInitialLoad" ) );
            suite.addTest( new StoreInfoTest( "testBasicSerialization" ) );
            suite.addTest( new StoreInfoTest( "testSave" ) );
            suite.addTest( new StoreInfoTest( "testLoad" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        return suite;
    }

    public void testLoad()
    {
        StoreInfo.load();
        assertNotNull( StoreInfo.cache );
        assertEquals( 1, StoreInfo.cache.size() );
        final StoreInfo info = (StoreInfo) StoreInfo.cache.get( "test" );
        assertNotNull( info );
        assertEquals( "test", info.name );
    }

    public void testInitialLoad()
    {
        StoreInfo.get( "" ); // Trigger static initializer

        assertNotNull( StoreInfo.cache );
        assertEquals( 0, StoreInfo.cache.size() );
    }

    public void testSave()
    {
        final StoreInfo info = new StoreInfo( "test" );
        assertNull( info.recordId );

        try
        {
            info.save();
            assertNotNull( info.recordId );
            RecordStore rs = RecordStore.openRecordStore( StoreInfo.RS_NAME, false );
            assertEquals( 1, rs.getNumRecords() );
            final byte[] data = rs.getRecord( info.recordId.intValue() );
            assertNotNull( data );
            assertEquals( 33, data.length );
            final ByteArrayInputStream bais = new ByteArrayInputStream( data );
            final SerializerInputStream in = new SerializerInputStream( bais );
            final StoreInfo info2 = new StoreInfo( "test2" );
            info2.deserialize( in );
            assertTrue( info.equals( info2 ) );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testBasicSerialization()
    {
        try
        {
            final StoreInfo info = new StoreInfo( "test" );
            testSerialization( info );

            info.count++;
            info.nextId = 4;
            testSerialization( info );
        }
        catch( SerializationException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void testSerialization( final StoreInfo info )
            throws SerializationException
    {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final SerializerOutputStream out = new SerializerOutputStream( baos );
        info.serialize( out );

        final StoreInfo info2 = new StoreInfo( "test2" );
        final ByteArrayInputStream bais = new ByteArrayInputStream( baos.toByteArray() );
        final SerializerInputStream in = new SerializerInputStream( bais );
        info2.deserialize( in );

        assertTrue( info.equals( info2 ) );
    }
}
