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
 * Created on       : 05-jul-2005 18:22:28
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-04-26 00:53:43 +0200 (jue, 26 abr 2007) $
 * Revision         : $Revision: 4 $
 */

/**
 * Test suite for SortedIntArray
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 4 $
 * @noinspection FieldRepeatedlyAccessedInMethod,MethodCallInLoopCondition,ArrayLengthInLoopCondition
 */
public class SortedIntArrayTest
        extends TestCase
{
    private static SortedIntArray array = new SortedIntArray( 4 );

    public SortedIntArrayTest()
    {
        super();
    }

    public SortedIntArrayTest( final String s)
    {
        super(s);
    }

    public void testInsert()
    {
        // 2, 8, 4, 12, 1 ==> 2, 4, 8, 12
        assertEquals( 0, array.insert( 2 ) );
        assertEquals( 1, array.insert( 8 ) );
        assertEquals( 1, array.insert( 4 ) );
        assertEquals( 3, array.insert( 12 ) );
        assertEquals( -1, array.insert( 1 ) );
        assertEquals( 4, array.size() );
        assertEquals( 2, array.getElementAt( 0 ) );
        assertEquals( 4, array.getElementAt( 1 ) );
        assertEquals( 8, array.getElementAt( 2 ) );
        assertEquals( 12, array.getElementAt( 3 ) );
    }

    public void testInsertGrow()
    {
        array.setGrowthFactor( 16 );
        // 1 ==> 1, 2, 4, 8, 12
        assertEquals( 0, array.insert( 1 ) );
        assertEquals( 5, array.size() );
        assertEquals( 1, array.getElementAt( 0 ) );
        assertEquals( 2, array.getElementAt( 1 ) );
        assertEquals( 4, array.getElementAt( 2 ) );
        assertEquals( 8, array.getElementAt( 3 ) );
        assertEquals( 12, array.getElementAt( 4 ) );
    }

    public void testRemove()
    {
        array.remove( 8 );
        assertEquals( 4, array.size() );
        assertEquals( -1, array.findIndex( 8 ) );
        assertEquals( 12, array.getElementAt( 3 ) );
        assertEquals( 4, array.getElementAt( 2 ) );
    }

    public void testFindIndex()
    {
        assertEquals( 1, array.findIndex( 2 ) );
    }

    public void testFindInsertionPoint()
    {
        // current: 1, 2, 4, 12
        assertEquals( 0, array.findInsertionPoint( 0 ) );
        assertEquals( 2, array.findInsertionPoint( 3 ) );
        assertEquals( 4, array.findInsertionPoint( 20 ) );
        final SortedIntArray newArray = new SortedIntArray( 16, 16 );
        assertEquals( 0, newArray.findInsertionPoint( 10 ) );
        assertEquals( 0, newArray.insert( 10 ) );
        assertEquals( 0, newArray.findInsertionPoint( 5 ) );
        assertEquals( 1, newArray.findInsertionPoint( 11 ) );
    }

    public void testSerialize()
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            array.serialize( out );
            final byte[] data = baos.toByteArray();
            final SortedIntArray newArray = new SortedIntArray( 16, 16 );
            final ByteArrayInputStream bais = new ByteArrayInputStream( data );
            final SerializerInputStream in = new SerializerInputStream( bais );
            newArray.deserialize( in );
            assertEquals( array.size(), newArray.size() );
            for( int i = 0; i < array.size(); i++ )
            {
                assertEquals( array.getElementAt( i ), newArray.getElementAt( i ) );
            }
        }
        catch( SerializationException e )
        {
            fail( e.getMessage() );
        }
    }

    public void testToIntArray()
    {
        final int[] result = array.toIntArray();
        assertNotNull( result );
        assertEquals( 4, result.length );
        for( int i = 0; i < result.length; i++ )
        {
            assertEquals( array.getElementAt( i ), result[i] );
        }
        final SortedIntArray newArray = new SortedIntArray( 16 );
        assertNull( newArray.toIntArray() );
    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new SortedIntArrayTest().getClass().getName()
        );

        try
        {
            suite.addTest(new SortedIntArrayTest("testInsert"));
            suite.addTest(new SortedIntArrayTest("testInsertGrow"));
            suite.addTest(new SortedIntArrayTest("testRemove"));
            suite.addTest(new SortedIntArrayTest("testFindIndex"));
            suite.addTest(new SortedIntArrayTest("testFindInsertionPoint"));
            suite.addTest(new SortedIntArrayTest("testSerialize"));
            suite.addTest(new SortedIntArrayTest("testToIntArray"));
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }

}
