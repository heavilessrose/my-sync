/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db.index;

import junit.framework.TestCase;
import junit.framework.TestSuite;

/*
 * File Information
 *
 * Created on       : 15-jul-2005 17:03:05
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision: 2 $
 */

/**
 * Test suite for IndexPkg
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 2 $
 */
public class IndexPkgTest
        extends TestCase
{
    public IndexPkgTest()
    {
        super();
    }

    public IndexPkgTest( final String s)
    {
        super(s);
    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new IndexPkgTest().getClass().getName()
        );

        try
        {
            suite.addTest( SortedIntArrayTest.suite() );
            suite.addTest( NodeKeyTest.suite() );
            suite.addTest( IndexTest.suite() );
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }

}
