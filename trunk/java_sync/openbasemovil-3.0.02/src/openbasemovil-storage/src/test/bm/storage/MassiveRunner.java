package bm.storage;

import java.util.List;
import java.util.Iterator;
import java.util.Map;
import java.util.HashMap;
import java.io.FileInputStream;
/*
 * File Information
 *
 * Created on       : 01-may-2007 13:02:07
 * Created by       : narciso
 * Last modified by : \$Author$
 * Last modified on : \$Date$
 * Revision         : \$Revision$
 */

/**
 * ToDo: add class description.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version \$Revision$
 */
public class MassiveRunner
{
    public static void main( final String[] args )
    {
        (new MassiveRunner()).Main( args );
    }

    private void Main( final String[] args )
    {
        try
        {
            final String file = args[0];
            int passes;
            if( args.length > 1 )
            {
                try
                {
                    passes = Integer.parseInt( args[1] );
                }
                catch( NumberFormatException e )
                {
                    passes = 20;
                }
            }
            else
            {
                passes = 20;
            }
            for( int i = 0; i < passes; i++ )
            {
                // 1. Create persistent test
                if( args.length < 3 )
                {
                    StoreTestLoad.main( args );
                }

                // 2. Run test
                runTest( file, 1 );
                runTest( file, 2 );
                runTest( file, 3 );
                runTest( file, 25 );
                runTest( file, 99 );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
        }
    }

    private void runTest( final String file, final int multiplexer )
    {
        try
        {
            final byte[] dummy = createTestData( 100, (byte) 10 );
            final String storeName = "massive" + multiplexer;
            Store.safeDeleteRecordStore( storeName );
            final Store store = Store.get( storeName, multiplexer );
            final StoreTestLoad load = new StoreTestLoad();
            load.load( new FileInputStream( file ) );
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
                    {
                        final byte[] data = createTestData( op.size, op.content );
                        final int recordId = store.addRecord( data );
                        sizes.put( new Integer( recordId - 1 ), new Integer( op.size ) );
                        contents.put( new Integer( recordId - 1 ), new Integer( op.content ) );
                    }
                        break;

                    case StoreTestLoad.OP_MODIFY:
                        if( sizes.containsKey( new Integer( op.recordId - 1 ) ) )
                        {
                            final byte[] data = createTestData( op.size, op.content );
                            store.setRecord( op.recordId, data );
                            sizes.put( new Integer( op.recordId - 1 ), new Integer( op.size ) );
                            contents.put( new Integer( op.recordId - 1 ), new Integer( op.content ) );
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
                        break;

                    case StoreTestLoad.OP_DELETE:
                        if( sizes.containsKey( new Integer( op.recordId - 1 ) ) )
                        {
                            store.deleteRecord( op.recordId );
                            sizes.remove( new Integer( op.recordId - 1 ) );
                            contents.remove( new Integer( op.recordId - 1 ) );
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
                assertTrue( sizes.containsKey( new Integer( recordId - 1 ) )  );
                final byte[] data = store.getRecord( recordId );
                assertNotNull( data );
                byteArrayEquals(
                        createTestData(
                                ((Integer) sizes.get( new Integer( recordId - 1 ) )).intValue(),
                                ((Integer) contents.get( new Integer( recordId - 1 ) )).byteValue()
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

    private void assertTrue( final boolean b )
    {
        if( !b )
        {
            fail( "Expected true but found false" );
        }
    }

    private void assertNotNull( final byte[] data )
    {
        if( data == null )
        {
            fail( "Expected not null" );
        }
    }

//    private void assertFalse( final boolean b )
//    {
//        if( b )
//        {
//            fail( "Expected false but found true" );
//        }
//    }

    private void assertEquals( final int i, final int i1 )
    {
        if( i != i1 )
        {
            fail( "Expected " + i + " but found " + i1 );
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

    private void fail( final String message )
    {
        System.err.println( message );
        System.exit( 1 );
    }

    private void byteArrayEquals( final byte[] a1, final byte[] a2 )
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
