package bm.db;

import junit.framework.TestCase;
import junit.framework.TestSuite;
import bm.core.ErrorHandler;
import bm.storage.Store;

/*
 * File Information
 *
 * Created on       : 28-jun-2007 17:33:00
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Persistent map test.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class PersistentMapTest
        extends TestCase
{
    public PersistentMapTest()
    {
    }

    public PersistentMapTest( String string )
    {
        super( string );
    }

    public void testNew()
    {
        final PersistentMap map1 = new PersistentMap( "map1" );
        assertTrue( Store.exists( "map1" ) );
        final PersistentMap map2 = new PersistentMap( "map1" );
    }

    public void testSet()
    {
        try
        {
            final PersistentMap map = new PersistentMap( "map1" );
            map.setElement( "sample", "key" );
        }
        catch( DBException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testGet()
    {
        final PersistentMap map = new PersistentMap( "map1" );
        try
        {
            assertNotNull( map.getElement( "sample" ) );
            assertEquals( "key", map.getElement( "sample" ) );
        }
        catch( DBException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testRemove()
    {
        final PersistentMap map = new PersistentMap( "map1" );
        try
        {
            map.setElement( "sample", null );
            assertNull( map.getElement( "sample" ) );
        }
        catch( DBException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new PersistentMapTest().getClass().getName()
        );

        try
        {
            suite.addTest(new PersistentMapTest("testNew"));
            suite.addTest(new PersistentMapTest("testSet"));
            suite.addTest(new PersistentMapTest("testGet"));
            suite.addTest(new PersistentMapTest("testRemove"));
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }
}
