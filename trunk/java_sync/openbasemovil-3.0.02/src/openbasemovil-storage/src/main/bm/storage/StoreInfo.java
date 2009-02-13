package bm.storage;

/* -----------------------------------------------------------------------------
    OpenBaseMovil Storage Library
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

import bm.core.ErrorHandler;
import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 1:32:07
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Keeps store information.
*
* @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
* @version $Revision$
*/
public class StoreInfo
        implements Serializable
{
    private static final Log log = LogFactory.getLog( "StoreInfo" );

    static final String RS_NAME = "sys_store_info";

    static Hashtable    cache;

    private static ByteArrayOutputStream    baos;
    private static SerializerOutputStream   out;

    static
    {
        load();
    }

    static synchronized void flush()
            throws RSException
    {
        RSException e = null;
        //noinspection MethodCallInLoopCondition
        for( final Enumeration i = cache.elements(); i.hasMoreElements(); )
        {
            final StoreInfo info = (StoreInfo) i.nextElement();
            if( info.dirty )
            {
                try
                {
                    info.save();
                }
                catch( SerializationException e1 )
                {
                    log.error( e1 );
                    e = new RSException( Constants.STI_FLUSH, e1 );
                }
            }
        }
        if( e != null )
        {
            throw e;
        }
    }

    static synchronized void load()
    {
        RecordStore rs = null;
        RecordEnumeration re = null;
        //noinspection CheckForOutOfMemoryOnLargeArrayAllocation
        byte[] buffer = new byte[ 256 ];
        try
        {
            rs = RecordStore.openRecordStore( RS_NAME, true );
            cache = new Hashtable( rs.getNumRecords() );
            re = rs.enumerateRecords( null, null, false );
            int recordId;
            int recordSize;
            //noinspection MethodCallInLoopCondition
            while( re.hasNextElement() )
            {
                recordId = re.nextRecordId();
                recordSize = rs.getRecordSize( recordId );
                if( buffer.length < recordSize )
                {
                    buffer = new byte[ recordSize + 64 ];
                }
                rs.getRecord( recordId, buffer, 0 );
                final StoreInfo info = new StoreInfo();
                info.deserialize( new SerializerInputStream(
                        new ByteArrayInputStream( buffer )
                ) );
                info.recordId = new Integer( recordId );
                cache.put( info.name, info );
            }
        }
        catch( RecordStoreException e )
        {
            log.error( e );
            ErrorHandler.handleError( null, e );
        }
        catch( SerializationException e )
        {
            log.error( e );
            ErrorHandler.handleError( null, e );
        }
        finally
        {
            if( re != null ) try{ re.destroy(); }catch( Exception e ){}
            if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
        }
    }

    public static StoreInfo get( final String name )
    {
        return (StoreInfo) cache.get( name );
    }

    public static synchronized void set( final StoreInfo info )
            throws SerializationException,
                   RSException
    {
        final StoreInfo current = (StoreInfo) cache.get( info.name );
        if( current != null )
        {
            info.recordId = current.recordId;
        }
        info.save();
        if( current != null )
        {
            cache.put( info.name, info );
        }
        flush();
    }

    String  name;
    int     count;
    long    size;
    int     nextId = 1;
    short   multiplexer = 1;
    long    lastModified;
    Integer recordId;
    boolean dirty;
    boolean damaged;

    /**
     * Only for backup restore.
     */
    public StoreInfo()
    {
    }

    public StoreInfo( final String name )
    {
        this.name = name;
        cache.put( name, this );
    }

    public StoreInfo( final String name, final short multiplexer )
    {
        this.name = name;
        this.multiplexer = multiplexer;
        cache.put( name, this );
    }

    public StoreInfo( final String name, final int multiplexer )
    {
        this.name = name;
        this.multiplexer = (short) multiplexer;
        cache.put( name, this );
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.storage.StoreInfo";
    }

    /**
     * Write object status to stream.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void serialize( final SerializerOutputStream out )
        throws SerializationException
    {
        out.writeByte( (byte) 2 ); // Version
        out.writeString( name );
        out.writeInt( count );
        out.writeLong( size );
        out.writeInt( nextId );
        out.writeShort( multiplexer );
        out.writeLong( lastModified );
        out.writeBoolean( damaged );
    }

    /**
     * Read object status from stream.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        final byte version = in.readByte();
        name = in.readString();
        count = in.readInt();
        size = in.readLong();
        nextId = in.readInt();
        multiplexer = in.readShort();
        lastModified = in.readLong();
        if( version > 1 )
        {
            damaged = in.readBoolean();
        }
        else
        {
            damaged = false;
        }
    }

    public void addRecord( final int length )
            throws SerializationException
    {
        count++;
        size += length;
        nextId++;
        lastModified = System.currentTimeMillis();
        dirty = true;
        if( !Store.burstMode )
        {
            save();
        }
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    public void save()
            throws SerializationException
    {
        RecordStore rs = null;
        try
        {
            rs = RecordStore.openRecordStore( RS_NAME, true );
            if( out != null )
            {
                baos.reset();
            }
            else
            {
                baos = new ByteArrayOutputStream();
                out = new SerializerOutputStream( baos );
            }
            serialize( out );
            final byte[] data = baos.toByteArray();
            if( recordId != null )
            {
                rs.setRecord( recordId.intValue(), data, 0, data.length );
            }
            else
            {
                recordId = new Integer( rs.addRecord( data, 0, data.length ) );
                cache.put( name, this );
            }
            dirty = false;
        }
        catch( RecordStoreException e )
        {
            log.error( e );
        }
        finally
        {
            if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
        }
    }

    public void delete()
    {
        if( recordId != null )
        {
            RecordStore rs = null;
            try
            {
                rs = RecordStore.openRecordStore( RS_NAME, true );
                rs.deleteRecord( recordId.intValue() );
            }
            catch( RecordStoreException e )
            {
                log.error( e );
            }
            finally
            {
                if( rs != null ) try{ rs.closeRecordStore(); }catch( Exception e ){}
            }
        }
        cache.remove( name );
    }

    public void deleteRecord( final int length )
            throws SerializationException
    {
        count--;
        lastModified = System.currentTimeMillis();
        size -= length;
        dirty = true;
        if( !Store.burstMode )
        {
            save();
        }
    }

    public void updateRecord( final int difference )
            throws SerializationException
    {
        size += difference;
        lastModified = System.currentTimeMillis();
        dirty = true;
        if( !Store.burstMode )
        {
            save();
        }
    }

    public boolean equals( final Object o )
    {
        if( this == o )
        {
            return true;
        }
        if( o == null || getClass() != o.getClass() )
        {
            return false;
        }

        final StoreInfo storeInfo = (StoreInfo) o;

        if( count != storeInfo.count )
        {
            return false;
        }
        if( lastModified != storeInfo.lastModified )
        {
            return false;
        }
        if( multiplexer != storeInfo.multiplexer )
        {
            return false;
        }
        if( nextId != storeInfo.nextId )
        {
            return false;
        }
        if( size != storeInfo.size )
        {
            return false;
        }
        //noinspection RedundantIfStatement
        if( !name.equals( storeInfo.name ) )
        {
            return false;
        }

        return true;
    }

    public int hashCode()
    {
        int result;
        result = name.hashCode();
        result = 31 * result + count;
        result = 31 * result + (int) ( size ^ ( size >>> 32 ) );
        result = 31 * result + nextId;
        result = 31 * result + (int) multiplexer;
        result = 31 * result + (int) ( lastModified ^ ( lastModified >>> 32 ) );
        return result;
    }
}
