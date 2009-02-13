package bm.storage;

import junit.framework.TestCase;
import junit.framework.TestSuite;

import javax.microedition.rms.RecordStore;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.io.FileInputStream;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 12:54:19
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Test case for Store.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StoreTest
    extends TestCase
{
    public StoreTest()
    {
        super();
    }

    public StoreTest( String string )
    {
        super( string );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite( StoreTest.class.getName() );
        try
        {
            suite.addTest( new StoreTest( "testInitialGetSet" ) );
            suite.addTest( new StoreTest( "testInitialSet" ) );
            suite.addTest( new StoreTest( "testInitialGetRecordSize" ) );

            suite.addTest( new StoreTest( "testStore1" ) );
            suite.addTest( new StoreTest( "testStoreBurst1" ) );

            suite.addTest( new StoreTest( "testStore2" ) );
            suite.addTest( new StoreTest( "testStoreBurst2" ) );

            suite.addTest( new StoreTest( "testMassive" ) );

//            suite.addTest( new StoreTest( "testStore10" ) );
//            suite.addTest( new StoreTest( "testStoreBurst10" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
        return suite;
    }

    /*
drop
enumerateRecords
packCopy
shutdown

     */

    public void testMassive()
    {
        testMassive( 1 );
        testMassive( 2 );
        testMassive( 3 );
        testMassive( 15 );
        testMassive( 40 );
        testMassive( 99 );
    }

    private void testMassive( final int multiplexer )
    {
        try
        {
            final byte[] dummy = createTestData( 100, (byte) 10 );
            final Store store = Store.get( "massive" + multiplexer, multiplexer );
            final StoreTestLoad load = new StoreTestLoad();
//            load.load( getClass().getResourceAsStream( "/testload.bin" ) );
            load.load( new FileInputStream( "/home/narciso/BaseMovil/level1/basemovil-storage-standard/testload-fail1.bin" ) );
            final List operations = load.getOperations();
            final Map sizes = new HashMap( operations.size() );
            final Map contents = new HashMap( operations.size() );
            for( final Iterator i = operations.iterator(); i.hasNext(); )
            {
                final StoreTestLoad.Operation op = ( StoreTestLoad.Operation )
                        i.next();
                switch( op.code )
                {
                    case StoreTestLoad.OP_ADD:
                        addRecord( op, store, sizes, contents );
                        break;

                    case StoreTestLoad.OP_MODIFY:
                    {
                        modifyRecord( sizes, op, store, contents, dummy );
                    }
                    break;

                    case StoreTestLoad.OP_DELETE:
                        removeRecord( sizes, contents, op, store );
                        break;
                }
            }

            System.out.println( "massive[ " + multiplexer + " ].live = " + sizes.size() );

            assertEquals( sizes.size(), store.getNumRecords() );

            final StoreEnumeration se = store.enumerateRecords();
            assertEquals( sizes.size(), se.numRecords() );
            while( se.hasNext() )
            {
                int recordId = se.nextId();
                assertTrue( sizes.containsKey( new Integer( recordId - 1 ) ) );
                final byte[] data = store.getRecord( recordId );
                assertNotNull( data );
                RecordTest.byteArrayEquals(
                        createTestData(
                                ((Integer) sizes.get( new Integer( recordId - 1 ) ) ).intValue(),
                                ((Integer) contents.get( new Integer( recordId - 1 ) ) ).byteValue()
                        ),
                        data
                );
            }

            se.destroy();
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void removeRecord(
            final Map sizes, final Map contents, final StoreTestLoad.Operation op,
            final Store store
    ) throws
      RSException,
      InvalidRecordIDException,
      bm.storage.RecordStoreFullException,
      bm.storage.InvalidRecordIDException
    {
        final Integer key = new Integer( op.recordId  );
        if( sizes.containsKey( key ) )
        {
            store.deleteRecord( op.recordId );
            sizes.remove( key );
            contents.remove( key );
        }
        else
        {
            try
            {
                store.deleteRecord( op.recordId );
                fail( "Deleted already deleted record" );
            }
            catch( Exception e )
            {
            }
        }
    }

    private void modifyRecord(
            final Map sizes, final StoreTestLoad.Operation op,
            final Store store,
            final Map contents,
            final byte[] dummy
    ) throws RSException, InvalidRecordIDException, RecordStoreFullException
    {
        final Integer key = new Integer( op.recordId );
        if( sizes.containsKey( key ) )
        {
            final byte[] data = createTestData( op.size, op.content );
            store.setRecord( op.recordId, data );
            sizes.put( key, new Integer( op.size ) );
            contents.put( key, new Integer( op.content ) );
        }
        else
        {
            try
            {
                store.setRecord( op.recordId, dummy );
                fail( "Set deleted record" );
            }
            catch( Exception e )
            {
            }
        }
    }

    private void addRecord(
            final StoreTestLoad.Operation op, final Store store,
            final Map sizes,
            final Map contents
    ) throws RSException, RecordStoreFullException
    {
        final byte[] data = createTestData( op.size, op.content );
        final int recordId = store.addRecord( data );
        final Integer key = new Integer( recordId );
        sizes.put( key, new Integer( op.size ) );
        contents.put( key, new Integer( op.content ) );
    }

    public void testStoreBurst2()
    {
        try
        {
            final Store store = Store.get( "test2burst", 2 );

            store.setBurstMode( true );
            store.open( true );

            // Add 10 rows
            store.addRecord( createTestData( 200, (byte) 1 ) );
            assertEquals( 2, store.getNextRecordId() );
            assertEquals( 1, store.getNumRecords() );
            assertEquals( 0, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 2 ) );
            assertEquals( 3, store.getNextRecordId() );
            assertEquals( 2, store.getNumRecords() );
            assertEquals( 0, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 3 ) );
            assertEquals( 4, store.getNextRecordId() );
            assertEquals( 3, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 4 ) );
            assertEquals( 5, store.getNextRecordId() );
            assertEquals( 4, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 5 ) );
            assertEquals( 6, store.getNextRecordId() );
            assertEquals( 5, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 6 ) );
            assertEquals( 7, store.getNextRecordId() );
            assertEquals( 6, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 7 ) );
            assertEquals( 8, store.getNextRecordId() );
            assertEquals( 7, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 8 ) );
            assertEquals( 9, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 9 ) );
            assertEquals( 10, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 10 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            store.setBurstMode( false );
            assertEquals( 5, store.rs.getNumRecords() );

            RecordTest.byteArrayEquals(
                    createTestData( 200, (byte) 1 ),
                    store.getRecord( 1 )
            );

            store.close();
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testStore2()
    {
        try
        {
            final Store store = Store.get( "test2", 2 );

            store.open( true );

            // Add 10 rows
            store.addRecord( createTestData( 200, (byte) 1 ) );
            assertEquals( 2, store.getNextRecordId() );
            assertEquals( 1, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 2 ) );
            assertEquals( 3, store.getNextRecordId() );
            assertEquals( 2, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 3 ) );
            assertEquals( 4, store.getNextRecordId() );
            assertEquals( 3, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 4 ) );
            assertEquals( 5, store.getNextRecordId() );
            assertEquals( 4, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 5 ) );
            assertEquals( 6, store.getNextRecordId() );
            assertEquals( 5, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 6 ) );
            assertEquals( 7, store.getNextRecordId() );
            assertEquals( 6, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 7 ) );
            assertEquals( 8, store.getNextRecordId() );
            assertEquals( 7, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 8 ) );
            assertEquals( 9, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 9 ) );
            assertEquals( 10, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 10 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            // Modify 2 records
            store.setRecord( 2, createTestData( 150, (byte) 2 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            store.setRecord( 4, createTestData( 91, (byte) 4 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );

            // Delete 2 records
            store.deleteRecord( 3 );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );
            store.deleteRecord( 9 );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );

            assertEquals( 200, store.getRecordSize( 1 ) );
            assertEquals( 91, store.getRecordSize( 4 ) );
            assertEquals( 150, store.getRecordSize( 2 ) );
            assertEquals( 100, store.getRecordSize( 10 ) );

            RecordTest.byteArrayEquals(
                    createTestData( 200, (byte) 1 ),
                    store.getRecord( 1 )
            );
            RecordTest.byteArrayEquals(
                    createTestData( 150, (byte) 2 ),
                    store.getRecord( 2 )
            );
            RecordTest.byteArrayEquals(
                    createTestData( 91, (byte) 4 ),
                    store.getRecord( 4 )
            );

            store.close();
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testStoreBurst1()
    {
        try
        {
            final Store store = Store.get( "test1burst", 1 );

            store.setBurstMode( true );
            store.open( true );

            // Add 10 rows
            store.addRecord( createTestData( 200, (byte) 1 ) );
            assertEquals( 2, store.getNextRecordId() );
            assertEquals( 1, store.getNumRecords() );
            assertEquals( 0, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 2 ) );
            assertEquals( 3, store.getNextRecordId() );
            assertEquals( 2, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 3 ) );
            assertEquals( 4, store.getNextRecordId() );
            assertEquals( 3, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 4 ) );
            assertEquals( 5, store.getNextRecordId() );
            assertEquals( 4, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 5 ) );
            assertEquals( 6, store.getNextRecordId() );
            assertEquals( 5, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 6 ) );
            assertEquals( 7, store.getNextRecordId() );
            assertEquals( 6, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 6, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 7 ) );
            assertEquals( 8, store.getNextRecordId() );
            assertEquals( 7, store.getNumRecords() );
            assertEquals( 6, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 7, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 8 ) );
            assertEquals( 9, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 7, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 8, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 9 ) );
            assertEquals( 10, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 8, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 9, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 10 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            assertEquals( 9, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertTrue( store.currentRecord.dirty );
            assertEquals( 10, store.currentRecord.getRecordId() );

            store.setBurstMode( false );
            assertEquals( 10, store.rs.getNumRecords() );

            RecordTest.byteArrayEquals(
                    createTestData( 200, (byte) 1 ),
                    store.getRecord( 1 )
            );

            store.close();
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testInitialGetRecordSize()
    {
        final Store store = Store.get( "testInitialGetRS", 1 );

        try
        {
            final byte[] data = createTestData( 10, (byte) 1 );
            RecordStore rs = RecordStore.openRecordStore( "testInitialGetRS", true );
            rs.addRecord( data, 0, data.length );
            rs.closeRecordStore();

            store.open( true );
            try
            {
                store.getRecordSize( 1 );
                fail( "Should complain about invalid record" );
            }
            catch( RSException e )
            {
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testInitialGetSet()
    {
        final Store store = Store.get( "testInitialGetSet", 1 );

        try
        {
            store.open( true );

            try
            {
                store.getRecord( 1 );
            }
            catch( InvalidRecordIDException e )
            {
            }

            try
            {
                store.setRecord( 1, createTestData( 10, (byte) 1 ) );
                fail( "Set on invalid record" );
            }
            catch( InvalidRecordIDException e )
            {
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testInitialSet()
    {
        final Store store = Store.get( "testInitialSet", 1 );

        try
        {
            store.open( true );

            try
            {
                store.setRecord( 1, createTestData( 10, (byte) 1 ) );
                fail( "Set on invalid record" );
            }
            catch( InvalidRecordIDException e )
            {
            }

            store.addRecord( createTestData( 200, (byte) 1 ) );
            assertEquals( 2, store.getNextRecordId() );
            assertEquals( 1, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testStore1()
    {
        try
        {
            final Store store = Store.get( "test1", 1 );

            store.open( true );

            // Add 10 rows
            store.addRecord( createTestData( 200, (byte) 1 ) );
            assertEquals( 2, store.getNextRecordId() );
            assertEquals( 1, store.getNumRecords() );
            assertEquals( 1, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 1, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 2 ) );
            assertEquals( 3, store.getNextRecordId() );
            assertEquals( 2, store.getNumRecords() );
            assertEquals( 2, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 2, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 3 ) );
            assertEquals( 4, store.getNextRecordId() );
            assertEquals( 3, store.getNumRecords() );
            assertEquals( 3, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 3, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 4 ) );
            assertEquals( 5, store.getNextRecordId() );
            assertEquals( 4, store.getNumRecords() );
            assertEquals( 4, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 4, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 5 ) );
            assertEquals( 6, store.getNextRecordId() );
            assertEquals( 5, store.getNumRecords() );
            assertEquals( 5, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 5, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 6 ) );
            assertEquals( 7, store.getNextRecordId() );
            assertEquals( 6, store.getNumRecords() );
            assertEquals( 6, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 6, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 7 ) );
            assertEquals( 8, store.getNextRecordId() );
            assertEquals( 7, store.getNumRecords() );
            assertEquals( 7, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 7, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 8 ) );
            assertEquals( 9, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 8, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 8, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 9 ) );
            assertEquals( 10, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 9, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 9, store.currentRecord.getRecordId() );

            store.addRecord( createTestData( 100, (byte) 10 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            assertEquals( 10, store.rs.getNumRecords() );
            assertNotNull( store.currentRecord );
            assertFalse( store.currentRecord.dirty );
            assertEquals( 10, store.currentRecord.getRecordId() );

            // Modify 2 records
            store.setRecord( 2, createTestData( 150, (byte) 2 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );
            store.setRecord( 4, createTestData( 91, (byte) 4 ) );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 10, store.getNumRecords() );

            // Delete 2 records
            store.deleteRecord( 3 );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 9, store.getNumRecords() );
            assertEquals( 9, store.rs.getNumRecords() );
            store.deleteRecord( 9 );
            assertEquals( 11, store.getNextRecordId() );
            assertEquals( 8, store.getNumRecords() );
            assertEquals( 8, store.rs.getNumRecords() );

            assertEquals( 200, store.getRecordSize( 1 ) );
            assertEquals( 91, store.getRecordSize( 4 ) );
            assertEquals( 150, store.getRecordSize( 2 ) );
            assertEquals( 100, store.getRecordSize( 10 ) );

            RecordTest.byteArrayEquals(
                    createTestData( 200, (byte) 1 ),
                    store.getRecord( 1 )
            );
            RecordTest.byteArrayEquals(
                    createTestData( 150, (byte) 2 ),
                    store.getRecord( 2 )
            );
            RecordTest.byteArrayEquals(
                    createTestData( 91, (byte) 4 ),
                    store.getRecord( 4 )
            );

            store.close();
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private byte[] createTestData( final int length, final byte fill )
    {
        final byte[] set = new byte[ length ];
        for( int i = 0; i < length; i++ )
        {
            set[i] = fill;
        }
        return set;
    }
}
