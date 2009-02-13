/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db.index;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

/*
 * File Information
 *
 * Created on       : 07-jul-2005 11:09:41
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-10-19 12:25:27 +0200 (vie, 19 oct 2007) $
 * Revision         : $Revision: 16 $
 */

/**
 * Test suite for NodeKey
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 16 $
 */
public class NodeKeyTest
        extends TestCase
{
    public NodeKeyTest()
    {
        super();
    }

    public NodeKeyTest( final String s)
    {
        super(s);
    }

    public void testSerializeString()
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            final SortedIntArray array = new SortedIntArray( 1 );
            array.insert( 1 );
            final SortedIntArray array2 = new SortedIntArray( 1 );
            array.insert( 2 );
            final Index index = new Index( "sample", 2, Index.KT_STRING, true );
            final NodeKey key = new NodeKey( index, new IndexKey( "MyKey" ), array );
            key.serialize( out );
            final NodeKey key2 = new NodeKey( index, new IndexKey( "MyKey2" ), array2 );
            final ByteArrayInputStream bais = new ByteArrayInputStream( baos.toByteArray() );
            final SerializerInputStream in = new SerializerInputStream( bais );
            key2.deserialize( in );
            assertEquals( key.getKey(), key2.getKey() );
            final SortedIntArray key2Data = (SortedIntArray) key2.getData();
            assertNotNull( key2Data );
            assertEquals( 1, key2Data.size() );
            assertEquals(
                    ((SortedIntArray)key.getData()).getElementAt( 0 ),
                    key2Data.getElementAt( 0 )
            );
        }
        catch( SerializationException e )
        {
            fail( e.getMessage() );
        }
    }

    public void testSerializeLong()
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            final SortedIntArray array = new SortedIntArray( 1 );
            array.insert( 1 );
            final SortedIntArray array2 = new SortedIntArray( 1 );
            array.insert( 2 );
            final Index index = new Index( "sample", 2, Index.KT_LONG, true );
            final NodeKey key = new NodeKey( index, new IndexKey( new Long( 1 ) ), array );
            key.serialize( out );
            final NodeKey key2 = new NodeKey( index, new IndexKey( new Long( 2 ) ), array2 );
            final ByteArrayInputStream bais = new ByteArrayInputStream( baos.toByteArray() );
            final SerializerInputStream in = new SerializerInputStream( bais );
            key2.deserialize( in );
            assertEquals( key.getKey(), key2.getKey() );
            final SortedIntArray data = (SortedIntArray) key2.getData();
            assertNotNull( data );
            assertEquals( 1, data.size() );
            assertEquals( ((SortedIntArray)key.getData()).getElementAt( 0 ), data.getElementAt( 0 ) );
        }
        catch( SerializationException e )
        {
            fail( e.getMessage() );
        }
    }

    public void testCompareStringCaseSensitive()
    {
        final SortedIntArray array = new SortedIntArray( 1 );
        array.insert( 1 );
        final Index index = new Index( "sample", 2, Index.KT_STRING, true );
        final NodeKey key1 = new NodeKey( index, new IndexKey( "Value1" ), array );
        final NodeKey key2 = new NodeKey( index, new IndexKey( "value1" ), array );
        assertTrue( key1.compareTo( key2 ) < 0 );
        assertTrue( key2.compareTo( key1 ) > 0 );
        assertTrue( key1.compareTo( key1 ) == 0 );
        key2.setKey( new IndexKey( "Value1" ) );
        assertTrue( key1.compareTo( key2 ) == 0 );
        assertTrue( key2.compareTo( key1 ) == 0 );
    }

    public void testCompareStringNoCaseSensitive()
    {
        final SortedIntArray array = new SortedIntArray( 1 );
        array.insert( 1 );
        final Index index = new Index( "sample", 2, Index.KT_STRING, false );
        final NodeKey key1 = new NodeKey( index, new IndexKey( "value1" ), array );
        final NodeKey key2 = new NodeKey( index, new IndexKey( "Value2" ), array );
        assertTrue( key1.compareTo( key2 ) < 0 );
        assertTrue( key2.compareTo( key1 ) > 0 );
        assertTrue( key1.compareTo( key1 ) == 0 );
        key2.setKey( new IndexKey( "Value1" ) );
        assertTrue( key1.compareTo( key2 ) == 0 );
        assertTrue( key2.compareTo( key1 ) == 0 );
    }

    public void testCompareLong()
    {
        final SortedIntArray array = new SortedIntArray( 1 );
        array.insert( 1 );
        final Index index = new Index( "sample", 2, Index.KT_LONG, true );
        final NodeKey key1 = new NodeKey( index, new IndexKey( new Long( 1 ) ), array );
        final NodeKey key2 = new NodeKey( index, new IndexKey( new Long( 10 ) ), array );
        assertTrue( key1.compareTo( key2 ) < 0 );
        assertTrue( key2.compareTo( key1 ) > 0 );
        assertTrue( key1.compareTo( key1 ) == 0 );
        key2.setKey( new IndexKey( new Long( 1 ) ) );
        assertTrue( key1.compareTo( key2 ) == 0 );
        assertTrue( key2.compareTo( key1 ) == 0 );

    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new NodeKeyTest().getClass().getName()
        );

        try
        {
            suite.addTest(new NodeKeyTest("testSerializeString"));
            suite.addTest(new NodeKeyTest("testSerializeLong"));
            suite.addTest(new NodeKeyTest("testCompareStringCaseSensitive"));
            suite.addTest(new NodeKeyTest("testCompareStringNoCaseSensitive"));
            suite.addTest(new NodeKeyTest("testCompareLong"));
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }

}
