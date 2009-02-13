package bm.core.io;
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

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Date;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 29-mar-2007 17:14:04
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * TestSuite for Serializer Streams.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SerializerStreamTest
        extends TestCase
{
    public SerializerStreamTest()
    {
    }

    public SerializerStreamTest( String string )
    {
        super( string );
    }

    public static TestSuite suite()
    {
        final TestSuite suite = new TestSuite(
                SerializerStreamTest.class.getName()
        );

        try
        {
            suite.addTest( new SerializerStreamTest( "testAll" ) );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }

        return suite;
    }

    public void testAll()
    {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final DataOutputStream dos = new DataOutputStream( baos );
        final SerializerOutputStream out = new SerializerOutputStream( dos );

        try
        {
            out.writeInt( 10 );
            out.writeInt( new Integer( 11 ) );
            out.writeInt( null );
            out.writeByte( (byte) 10 );
            out.writeByte( new Byte( (byte) 11 ) );
            out.writeByte( null );
            out.writeBoolean( true );
            out.writeBoolean( new Boolean( false ) );
            out.writeBoolean( null );
            out.writeShort( (short) 10 );
            out.writeShort( new Short( (short) 11 ) );
            out.writeShort( null );
            out.writeLong( 10 );
            out.writeLong( new Long( 11 ) );
            out.writeLong( null );
            final Date date = new Date();
            out.writeNullableDate( date );
            out.writeNullableDate( null );
            out.writeDate( date );
            out.writeNullableString( null );
            out.writeNullableString( "Hello" );
            out.writeNullableString( "Hello", 4 );
            out.writeString( "Hello dolly" );
            out.writeString( "Hello dolly", 5 );
            final byte[] data = new byte[5];
            for( int i = 0; i < 5; i++ )
            {
                data[i] = (byte) i;
            }
            out.writeNullableBlob( data );
            out.writeBlob( data );
            out.writeNullableBlob( null );

            final Object[] array = new Object[14];
            final Hashtable map = new Hashtable( 2 );
            map.put( "K1", new Byte( (byte) 100 ) );
            map.put( date, new Short( (short) 10 ) );
            final Vector list = new Vector( 0 );
            final Vector list2 = new Vector( 1 );
            list2.addElement( "Este" );

            array[0] = new DummySerializable( "Hola" );
            array[1] = "Adios";
            array[2] = new Long( 10 );
            array[3] = new Byte( (byte) 1 );
            array[4] = new Boolean( false );
            array[5] = new Integer( 202 );
            array[6] = new Short( (short) 15 );
            array[7] = map;
            array[8] = list;
            array[9] = data;
            array[10] = null;
            array[11] = date;
            array[12] = list2;
            array[13] = new Hashtable(1);

            out.writeObject( array );
            out.writeNullableObject( array );
            out.writeNullableObject( null );

            out.flush();

            final byte[] stream = baos.toByteArray();

            final ByteArrayInputStream bais = new ByteArrayInputStream( stream );
            final DataInputStream dis = new DataInputStream( bais );
            final SerializerInputStream in = new SerializerInputStream( dis );

            assertEquals( 10, in.readInt() );
            Integer i = in.readNullableInt();
            assertNotNull( i );
            assertEquals( 11, i.intValue() );
            i = in.readNullableInt();
            assertNull( i );

            assertEquals( (byte) 10, in.readByte() );
            Byte b = in.readNullableByte();
            assertNotNull( b );
            assertEquals( (byte) 11, b.byteValue() );
            b = in.readNullableByte();
            assertNull( b );

            assertTrue( in.readBoolean() );
            Boolean bo = in.readNullableBoolean();
            assertNotNull( bo );
            assertFalse( bo.booleanValue() );
            bo = in.readNullableBoolean();
            assertNull( bo );

            assertEquals( (short) 10, in.readShort() );
            Short sh = in.readNullableShort();
            assertNotNull( sh );
            assertEquals( (short) 11, sh.shortValue() );
            sh = in.readNullableShort();
            assertNull( sh );

            assertEquals( (long) 10, in.readLong() );
            Long l = in.readNullableLong();
            assertNotNull( l );
            assertEquals( (long) 11, l.longValue() );
            l = in.readNullableLong();
            assertNull( l );

            Date date2 = in.readNullableDate();
            assertNotNull( date2 );
            assertEquals( date, date2 );
            date2 = in.readNullableDate();
            assertNull( date2 );
            date2 = in.readDate();
            assertNotNull( date2 );
            assertEquals( date, date2 );

            String s2 = in.readNullableString();
            assertNull( s2 );
            s2 = in.readNullableString();
            assertNotNull( s2 );
            assertEquals( "Hello", s2 );
            s2 = in.readNullableString();
            assertNotNull( s2 );
            assertEquals( "Hell", s2 );
            s2 = in.readString();
            assertNotNull( s2 );
            assertEquals( "Hello dolly", s2 );
            s2 = in.readString();
            assertNotNull( s2 );
            assertEquals( "Hello", s2 );

            byte[] data2 = in.readNullableBlob();
            assertNotNull( data2 );
            assertEquals( 5, data2.length );
            for( int j = 0; j < 5; j++ )
            {
                assertEquals( data[j], data2[j] );
            }

            data2 = in.readBlob();
            assertNotNull( data2 );
            assertEquals( 5, data2.length );
            for( int j = 0; j < 5; j++ )
            {
                assertEquals( data[j], data2[j] );
            }

            data2 = in.readNullableBlob();
            assertNull( data2 );

            Object o = in.readObject();
            assertNotNull( o );
            assertTrue( o.getClass().isArray() );
            Object[] array2 = (Object[]) o;
            checkArray( array, array2 );

            o = in.readNullableObject();
            assertNotNull( o );
            assertTrue( o.getClass().isArray() );
            array2 = (Object[]) o;
            checkArray( array, array2 );

            o = in.readNullableObject();
            assertNull( o );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
    }

    private void checkArray( final Object[] array, final Object[] array2 )
    {
        if( array == null && array2 == null )
        {
        }
        else if( array == null || array2 == null )
        {
            fail( "One is null" );
        }
        else
        {
            assertEquals( array.length, array2.length );
            for( int i = 0; i < array.length; i++ )
            {
                if( !(array[i] instanceof byte[]) )
                {
                    assertEquals( array[i], array2[i] );
                }
                else
                {
                    assertTrue( array2[i] instanceof byte[] );
                    final byte[] a1 = (byte[]) array[i];
                    final byte[] a2 = (byte[]) array2[i];
                    assertEquals( a1.length, a2.length );
                    for( int j = 0; j < a1.length; j++ )
                    {
                        assertEquals( a1[j], a2[j] );
                    }
                }
            }
        }
    }

}
