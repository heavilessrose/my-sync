/*
 * Copyright (c) 2006 Elondra S.L. All Rights Reserved.
 */
package bm.core.tools;
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
import bm.core.ResourceManager;
import bm.core.res.PropertyResourceProvider;

import java.util.Calendar;

/*
 * File Information
 *
 * Created on       : 03-mar-2006 23:41:03
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Test suite for LocalFormatTools
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class LocalFormatToolsTest
        extends TestCase
{
    private static Calendar cal;
    static
    {
        cal = Calendar.getInstance();
        cal.set( Calendar.YEAR, 2006 );
        cal.set( Calendar.MONTH, Calendar.MARCH );
        cal.set( Calendar.DAY_OF_MONTH, 3 );
        cal.set( Calendar.HOUR_OF_DAY, 9 );
        cal.set( Calendar.MINUTE, 2 );
        cal.set( Calendar.SECOND, 4 );
    }

    public LocalFormatToolsTest()
    {
        super();
    }

    public LocalFormatToolsTest( final String s )
    {
        super( s );
    }

    public void test()
    {
        assertEquals( "3", LocalFormatTools.format( cal.getTime(), "d" ) );
        assertEquals( "03", LocalFormatTools.format( cal.getTime(), "dd" ) );
        assertEquals( "vie", LocalFormatTools.format( cal.getTime(), "ddd" ) );
        assertEquals( "Viernes", LocalFormatTools.format( cal.getTime(), "dddd" ) );

        assertEquals( "3", LocalFormatTools.format( cal.getTime(), "M" ) );
        assertEquals( "03", LocalFormatTools.format( cal.getTime(), "MM" ) );
        assertEquals( "mar", LocalFormatTools.format( cal.getTime(), "MMM" ) );
        assertEquals( "Marzo", LocalFormatTools.format( cal.getTime(), "MMMM" ) );

        assertEquals( "06", LocalFormatTools.format( cal.getTime(), "yy" ) );
        assertEquals( "2006", LocalFormatTools.format( cal.getTime(), "yyyy" ) );

        assertEquals( "9", LocalFormatTools.format( cal.getTime(), "h" ) );
        assertEquals( "09", LocalFormatTools.format( cal.getTime(), "hh" ) );

        assertEquals( "2", LocalFormatTools.format( cal.getTime(), "m" ) );
        assertEquals( "02", LocalFormatTools.format( cal.getTime(), "mm" ) );

        assertEquals( "4", LocalFormatTools.format( cal.getTime(), "s" ) );
        assertEquals( "04", LocalFormatTools.format( cal.getTime(), "ss" ) );

        assertEquals(
                "Viernes, 3 de Marzo de 2006",
                LocalFormatTools.format( cal.getTime(), "dddd, d 'de' MMMM 'de' yyyy" )
        );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                LocalFormatToolsTest.class.getName()
        );

        try
        {
            ResourceManager.setActiveLocale( "es_ES" );
            ResourceManager.addProvider( new PropertyResourceProvider( "j2me.core" ) );
            suite.addTest( new LocalFormatToolsTest( "test" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }

        return suite;
    }

}
