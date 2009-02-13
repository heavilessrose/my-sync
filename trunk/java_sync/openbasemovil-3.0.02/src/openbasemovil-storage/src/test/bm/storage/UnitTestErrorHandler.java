package bm.storage;

import bm.core.ErrorHandler;
import junit.framework.TestCase;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 15:24:36
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Error handler to catch static exceptions and make the test fail on them.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class UnitTestErrorHandler
    extends ErrorHandler
{
    protected void handle( final Exception e )
    {
        e.printStackTrace();
        TestCase.fail( e.getMessage() );
    }

    protected void handle( final String message, final Exception e )
    {
        System.out.println( message );
        e.printStackTrace();
        TestCase.fail( e.getMessage() );
    }
}
