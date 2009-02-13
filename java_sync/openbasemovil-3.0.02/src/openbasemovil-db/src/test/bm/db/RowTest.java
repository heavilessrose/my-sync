/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db;

import junit.framework.TestCase;
import junit.framework.TestSuite;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Date;
import java.util.Hashtable;
import java.util.Vector;

import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;

/*
 * File Information
 *
 * Created on       : 15-jul-2005 17:10:43
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-04-26 00:53:43 +0200 (jue, 26 abr 2007) $
 * Revision         : $Revision: 4 $
 */

/**
 * Test suite for Row
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 4 $
 */
public class RowTest
        extends TestCase
{
    private static final FieldInfo[]    testFields  = new FieldInfo[8];
    private static final Hashtable      fieldMap    = new Hashtable( 8 );
    private static Table table;
    static
    {
        final Vector fields = new Vector( 8 );
        fields.addElement(
                new FieldInfo( "tString", Constants.FT_STRING, true, 30 )
        );
        fields.addElement(
                new FieldInfo( "tBoolean", Constants.FT_BOOLEAN, false, 0 )
        );
        fields.addElement(
                new FieldInfo( "tDate", Constants.FT_DATE, true, 0 )
        );
        fields.addElement(
                new FieldInfo( "tFixedPoint", Constants.FT_FIXED_POINT, true, 0 )
        );
        fields.addElement(
                new FieldInfo( "tInt", Constants.FT_INT, true, 0 )
        );
        fields.addElement(
                new FieldInfo( "tLong", Constants.FT_LONG, true, 0 )
        );
        fields.addElement(
                new FieldInfo( "tShort", Constants.FT_SHORT, false, 0 )
        );
        fields.addElement(
                new FieldInfo( "tBlob", Constants.FT_BLOB, true, 0 )
        );
        fields.copyInto( testFields );
        fieldMap.put( "tstring", new Integer( 0 ) );
        fieldMap.put( "tboolean", new Integer( 1 ) );
        fieldMap.put( "tdate", new Integer( 2 ) );
        fieldMap.put( "tfixedpoint", new Integer( 3 ) );
        fieldMap.put( "tint", new Integer( 4 ) );
        fieldMap.put( "tlong", new Integer( 5 ) );
        fieldMap.put( "tshort", new Integer( 6 ) );
        fieldMap.put( "tblob", new Integer( 7 ) );
        final TableStatus tableStatus = new TableStatus( null );
        final TableInfo tableInfo = new TableInfo();
        tableInfo.setFieldInfo( testFields );
        tableInfo.setName( "test" );
        table = new Table( null, tableInfo, tableStatus, fieldMap, new Hashtable(1) );
        // ToDo: fix database
    }

    public RowTest()
    {
        super();
    }

    public RowTest( final String s)
    {
        super(s);
    }

    public void testGetSet()
    {
        final Row row = new Row( table );
        try
        {
            row.setField( 4, new Integer( 10 ) );
            final Date date = new Date();
            row.setField( "tDate", date );
            final byte[] data = "HelloWorld".getBytes();
            row.setField( "tBlob", data );

            assertNotNull( row.getField( "tInt" ) );
            assertTrue( row.getField( "tInt" ) instanceof Integer );
            assertEquals( 10, ((Integer) row.getField( "tInt" )).intValue() );

            assertNotNull( row.getField( 2 ) );
            assertTrue( row.getField( 2 ) instanceof Date );
            assertEquals( date, row.getField( 2 ) );

            assertNotNull( row.getField( "tBlob" ) );
            assertTrue( row.getField( "tBlob" ).getClass().isArray() );
            final byte[] testData = (byte[]) row.getField( "tBlob" );
            assertEquals( "HelloWorld", new String( testData ) );

            assertNull( row.getField( "tString" ) );
            assertNull( row.getField( "tBoolean" ) );
            assertNull( row.getField( "tFixedPoint" ) );
            assertNull( row.getField( "tLong" ) );
            assertNull( row.getField( "tShort" ) );
        }
        catch( DBException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }

        try
        {
            row.setField( "tBoolean", null );
        }
        catch( RequiredFieldCantBeNullException e )
        {
        }
        catch( DBException e )
        {
            fail( e.getMessage() );
        }

        try
        {
            row.setField( "tBlob", new Integer( 1 ) );
        }
        catch( InvalidDataTypeException e )
        {
        }
        catch( DBException e )
        {
            fail( e.getMessage() );
        }

        try
        {
            row.setField( "tBloK", "hello" );
        }
        catch( InvalidFieldException e )
        {
        }
        catch( DBException e )
        {
            fail( e.getMessage() );
        }
    }

    public void testSerializeDeserialize()
    {
        try
        {
            final Row row = new Row( table );

            row.setField( "tString", "Hola" );
            final Date date = new Date();
            row.setField( "tBoolean", new Boolean( true ) );
            row.setField( "tDate", date );
            row.setField( "tInt", new Integer( 10 ) );
            row.setField( "tLong", new Long( 15 ) );
            final byte[] data = "HelloWorld".getBytes();
            row.setField( "tBlob", data );

            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );

            // Serialization no longer throws RequiredFieldCantBeNullException
//            try
//            {
//                row.serialize( out );
//                fail( "SerializationException( RequiredFieldCantBeNullException ) was expected" );
//            }
//            catch( SerializationException e )
//            {
//                if( !(e.getSource() instanceof RequiredFieldCantBeNullException) )
//                {
//                    fail( e.getMessage() );
//                }
//            }

            baos.reset();
            row.setField( "tShort", new Short( (short) 40 ) );
            row.serialize( out );

            final Row row2 = new Row( table );

            ByteArrayInputStream bais = new ByteArrayInputStream(
                    baos.toByteArray()
            );
            SerializerInputStream in = new SerializerInputStream( bais );
            row2.deserialize( in );

            assertNotNull( row2.getField( "tString" ) );
            assertEquals( "Hola", row2.getField( "tString" ) );
            assertNotNull( row2.getField( "tBoolean" ) );
            assertEquals( new Boolean( true ), row2.getField( "tBoolean" ) );
            assertNotNull( row2.getField( "tDate" ) );
            assertEquals( date, row2.getField( "tDate" ) );
            assertNotNull( row2.getField( "tInt" ) );
            assertEquals( new Integer( 10 ), row2.getField( "tInt" ) );
            assertNotNull( row2.getField( "tLong" ) );
            assertEquals( new Long( 15 ), row2.getField( "tLong" ) );
            assertNotNull( row2.getField( "tShort" ) );
            assertEquals( new Short( (short) 40 ), row2.getField( "tShort" ) );
            assertNotNull( row2.getField( "tBlob" ) );
            assertTrue( row2.getField( "tBlob" ).getClass().isArray() );
            final byte[] testData = (byte[]) row2.getField( "tBlob" );
            assertEquals( "HelloWorld", new String( testData ) );

            row.setField( "tString", "012345678A012345678B012345678C0123" );
            baos.reset();
            row.serialize( out );

            bais = new ByteArrayInputStream(
                    baos.toByteArray()
            );
            in = new SerializerInputStream( bais );
            row2.deserialize( in );

            assertNotNull( row2.getField( "tString" ) );
            assertEquals( "012345678A012345678B012345678C", row2.getField( "tString" ) );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new RowTest().getClass().getName()
        );

        try
        {
            suite.addTest(new RowTest("testGetSet"));
            suite.addTest(new RowTest("testSerializeDeserialize"));
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }

}
