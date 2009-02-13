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

import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import bm.core.event.ProgressEvent;
import bm.core.ResourceManager;
import bm.core.zlib.ZOutputStream;
import bm.core.zlib.ZInputStream;
import bm.core.tools.DeviceInfo;
import bm.err.ErrorLog;

import javax.microedition.rms.*;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 26-abr-2007 0:51:53
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A Store is similar to a RecordStore, but it can handle space more efficently
 * using multiplexing.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Store
{
    public static final String CLASS_NAME   = "Store";

    static final Log log = LogFactory.getLog( CLASS_NAME );

    static Boolean useCompression;
    static boolean burstMode;

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    public static boolean useCompression()
    {
        if( useCompression == null )
        {
            final long rsSizeLimit = DeviceInfo.getDeviceInfo().getRsSizeLimit();
            useCompression = new Boolean( rsSizeLimit > 0 && rsSizeLimit < 1000000 );
        }
        return useCompression.booleanValue();
    }

    private static Hashtable map = new Hashtable( 10 );

    Record          currentRecord;
    RecordStore     rs;

    private StoreInfo       info;
    private int             openCount;
    private Listener        listener;
    private RecordLocator   locator;

    public Store( final StoreInfo info )
    {
        this.info = info;
        locator = new RecordLocator( info.multiplexer );
    }

    /**
     * Check if a given store exists.
     *
     * @param name store name
     * @return true if so
     */
    public static boolean exists( final String name )
    {
        return StoreInfo.get( name ) != null;
    }

    /**
     * Get a store. Two calls with the same name are guaranteed to get the
     * same object.<br/>
     * If the store does not really exists, a new store representation is
     * returned but nothing will be actually created until something is
     * written to the Store.
     *
     * @param name store name
     * @param multiplexer if not yet created, the multiplexer to use, at least 1
     * @return store
     */
    public static Store get( final String name, final int multiplexer )
    {
        //log.debug( "get( " + name + ", " + multiplexer + ")" );
        final Hashtable map = Store.map;
        if( map.containsKey( name ) )
        {
            return (Store) map.get( name );
        }
        else
        {
            StoreInfo info = StoreInfo.get( name );
            if( info == null )
            {
                info = new StoreInfo( name, multiplexer );
            }
            final Store store = new Store( info );
            map.put( name, store );
            return store;
        }
    }

    /**
     * Check if a RecordStore exists.
     * @param name record store name
     * @return true if exists, false if not
     */
    public static boolean recordStoreExists( final String name )
    {
        final String[] names = RecordStore.listRecordStores();
        if( names != null )
        {
            final int count = names.length;
            for( int i = 0; i < count; i++ )
            {
                if( names[i].equals( name ) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    public static synchronized void safeDeleteRecordStore( final String name )
            throws RSException,
                   RecordStoreFullException
    {
        safeCloseRecordStore( name );
        try
        {
            RecordStore.deleteRecordStore( name );
        }
        catch( RecordStoreNotFoundException e )
        {
        }
        catch( RecordStoreException e )
        {
            throw new RSException( Constants.ERR_STR_DELETE, e );
        }
        map.remove( name );
        final StoreInfo info = StoreInfo.get( name );
        if( info != null && info.recordId != null )
        {
            info.delete();
        }
        log.debug( "record store deleted: " + name );
    }

    public static synchronized void safeCloseRecordStore( final String name )
            throws RSException,
                   RecordStoreFullException
    {
        try
        {
            final Log log = Store.log;
            if( map.containsKey( name ) )
            {
                Store store = (Store) map.get( name );
                store.flush();
                store.openCount = 0;
                store.rs = null;
            }
            //noinspection RecordStoreOpenedButNotSafelyClosed
            final RecordStore rs = RecordStore.openRecordStore( name, false );
            while( true )
            {
                try
                {
                    rs.closeRecordStore();
                }
                catch( RecordStoreNotOpenException e )
                {
                    break;
                }
                catch ( RecordStoreException e )
                {
                    log.error( e );
                    throw new RSException( Constants.ERR_STR_CLOSE, e );
                }
            }
        }
        catch( RecordStoreNotFoundException e )
        {
            //log.debug( "RecordStore did not exist" );
        }
        catch( RecordStoreException e )
        {
            log.error( e );
            throw new RSException( Constants.ERR_STR_CLOSE, e );
        }
    }

    /**
     * Close all open recordstores.
     */
    public static synchronized void smartShutdown()
    {
        final Hashtable map = Store.map;
        final int count = map.size();
        if( count > 0 )
        {
            final ProgressEvent pe = new ProgressEvent();
            pe.setAnimate( false );
            pe.setMessage( "" );
            pe.setPhase( "" );
            pe.setTitle( ResourceManager.getResource( "db.shutdown.Title" ) );
            pe.setMaxValue( new Integer( count ) );
            pe.setCurrentValue( 0 );
            pe.dispatch();
            int index = 1;
            //noinspection MethodCallInLoopCondition
            for( Enumeration i = map.elements(); i.hasMoreElements(); )
            {
                pe.setMessage( index + "/" + count );
                index++;
                pe.increment();
                pe.dispatch();
                final Store store = (Store) i.nextElement();
                try
                {
                    store.shutdown();
                }
                catch( Exception e )
                {
                    log.error( e );
                }
            }
            pe.increment();
            pe.dispatch();
            map.clear();
        }
    }

    /**
     * Ensure close all open recordstores.
     */
    public static synchronized void shutdownAll()
    {
        final ProgressEvent pe = new ProgressEvent();
        pe.setAnimate( true );
        pe.setMessage( "" );
        pe.setPhase( "" );
        pe.setTitle( ResourceManager.getResource( "db.shutdown.Title" ) );
        pe.dispatch();

        final String[] names = RecordStore.listRecordStores();
        if( names != null )
        {
            final int count = names.length;
            pe.setMaxValue( new Integer( count ) );
            pe.setCurrentValue( 0 );
            pe.setAnimate( false );
            pe.dispatch();
            for( int i = 0; i < count; i++ )
            {
                pe.setMessage( (i+1) + "/" + count );
                pe.increment();
                pe.dispatch();
                try
                {
                    safeCloseRecordStore( names[i] );
                }
                catch( RSException e )
                {
                    log.error( e );
                }
                catch( RecordStoreFullException e )
                {
                    log.error( e );
                }
            }
            pe.increment();
            pe.dispatch();
        }
    }

    public Listener getListener()
    {
        return listener;
    }

    public synchronized void setListener( final Listener listener )
    {
        this.listener = listener;
    }

    /**
     * Check if this store has been marked as damaged.
     * @return if so
     */
    public boolean isDamaged()
    {
        return info.damaged;
    }

    /**
     * Set the damaged flag for this store.
     * @param damaged damaged
     * @throws RSException on errors writing to storage
     */
    public synchronized void setDamaged( final boolean damaged )
            throws RSException
    {
        info.damaged = damaged;
        StoreInfo.flush();
    }

    /**
     * Add a new record.
     *
     * @param data data to add
     * @return record id
     * @throws bm.storage.RSException on fs errors
     * @throws RecordStoreFullException if no more space available on record store
     */
    public synchronized int addRecord( final byte[] data )
        throws RSException,
               RecordStoreFullException
    {
        return addRecord( data, 0, data.length );
    }

    /**
     * Add a new record.
     *
     * @param data data to add
     * @param offset offset
     * @param length length
     * @return record id
     * @throws bm.storage.RSException on fs errors
     * @throws RecordStoreFullException if no more space available on record store
     */
    public synchronized int addRecord(
            final byte[]    data,
            final int       offset,
            final int       length
    )
        throws RSException,
               RecordStoreFullException
    {
        final StoreInfo info = this.info;
        final Log log = Store.log;
//        log.debug( "addRecord( " + (data != null) + ", " + offset + ", " + length + " )" );
        final int assignedRecordId = info.nextId;
        final RecordLocator locator = this.locator;
        try
        {
            open();
//            log.debug( "addRecord, nextId: " + assignedRecordId );
            locator.translate( assignedRecordId );
            makeCurrent( locator.recordId, true );
            currentRecord.set( locator.offset, data, offset, length );
            if( !burstMode )
            {
                flush();
            }
            info.addRecord( length );
            return assignedRecordId;
        }
        catch( RecordStoreFullException e )
        {
            throw e;
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    CLASS_NAME,
                    "addRecord",
                    new Object[] { data, new Integer( offset ), new Integer( length ) },
                    info.name,
                    e
            );
            throw new RSException( Constants.ERR_STR_ADD_RECORD, e );
        }
        finally
        {
            close();
        }
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    private void makeCurrent( final int recordId, final boolean adding )
            throws SerializationException,
                   InvalidRecordIDException,
                   RSException,
                   RecordStoreFullException
    {
        if( currentRecord == null || currentRecord.getRecordId() != recordId )
        {
//            log.debug( "change current from " + (currentRecord != null ? currentRecord.getRecordId() : 0) + " to " + recordId );
            if( currentRecord != null && currentRecord.getRecordId() > 0 )
            {
                flush();
            }
            try
            {
                loadRecord( recordId, adding );
            }
            catch( InvalidRecordIDException e )
            {
                if( !adding )
                {
                    ErrorLog.addError(
                            CLASS_NAME,
                            "makeCurrent",
                            new Object[] { new Integer( recordId ),
                                           new Boolean( adding )
                            },
                            "Invalid record",
                            e
                    );
                    throw e;
                }
                else
                {
                    currentRecord = new Record( recordId, info.multiplexer );
                }
            }
        }
        if( currentRecord == null )
        {
            currentRecord = new Record( recordId, info.multiplexer );
        }
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    private void loadRecord( final int recordId, final boolean adding )
            throws SerializationException,
                   InvalidRecordIDException,
                   RSException
    {
        try
        {
            //log.debug( "loadRecord( " + recordId + " )" );
            final byte[] data = rs.getRecord( recordId );
            final ByteArrayInputStream bais = new ByteArrayInputStream( data );
            final SerializerInputStream in = new SerializerInputStream(
                    useCompression() ? (InputStream) new ZInputStream( bais ) : bais
            );
            if( currentRecord == null )
            {
                currentRecord = new Record( recordId, info.multiplexer );
            }
            else
            {
                currentRecord.setRecordId( recordId );
            }
            currentRecord.deserialize( in );
            //log.debug( "currentRecord: " + currentRecord.toString() );
        }
        catch( javax.microedition.rms.InvalidRecordIDException e )
        {
            currentRecord = null; // Clean up in case it fails
            if( !adding )
            {
                ErrorLog.addError(
                    CLASS_NAME,
                    "loadRecord",
                    new Object[] { new Integer( recordId ) },
                        "mapped record id not found (Store: " + info.name + ", mpx: " + info.multiplexer + ")",
                        e
                );
                throw new InvalidRecordIDException( Constants.ERR_STR_LOAD_RECORD, e );
            }
        }
        catch( SerializationException e )
        {
            currentRecord = null; // Clean up in case it fails
            throw e;
        }
        catch( RecordStoreException e )
        {
            currentRecord = null; // Clean up in case it fails
            throw new RSException( Constants.ERR_STR_LOAD_RECORD, e );
        }
    }

    /**
     * Flush the contents of the store and make any pending change persistent.
     *
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void flush()
            throws RSException,
                   RecordStoreFullException
    {
        if( currentRecord != null && currentRecord.isDirty() )
        {
//            log.debug( "flushing record: " + currentRecord );
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            SerializerOutputStream out;
            ZOutputStream zos = null;
            if( useCompression() )
            {
                log.debug( "Using compression" );
                zos = new ZOutputStream(
                        baos,
                        DeviceInfo.getDeviceInfo().getOptimalCompression()
                );
                out = new SerializerOutputStream( zos );
            }
            else
            {
                out = new SerializerOutputStream( baos );
            }
            try
            {
                open();
                currentRecord.serialize( out );
                out.flush();
                if( useCompression() )
                {
                    //noinspection ConstantConditions
                    zos.flush();
                    zos.close();
                }
                final byte[] data = baos.toByteArray();
                if( currentRecord.getRecordId() > 0 && currentRecord.getRecordId() < rs.getNextRecordID() )
                {
                    //log.debug( "flush: exsiting record " + currentRecord );
                    rs.setRecord( currentRecord.getRecordId(), data, 0, data.length );
                }
                else
                {
                    //log.debug( "flush: new record " + currentRecord );
                    currentRecord.setRecordId(
                            rs.addRecord( data, 0, data.length )
                    );
                }
                currentRecord.setDirty( false );
            }
            catch( javax.microedition.rms.RecordStoreFullException e )
            {
                throw new RecordStoreFullException( Constants.ERR_STR_FLUSH, e );
            }
            catch( Exception e )
            {
                log.error( e );
                ErrorLog.addError(
                        CLASS_NAME,
                        "flush",
                        new Object[] { currentRecord },
                        info.name,
                        e
                );
                throw new RSException( Constants.ERR_STR_FLUSH, e );
            }
            finally
            {
                close();
            }
        }
    }

    /**
     * Close the underlying record store.<br/>Keeps track of nested calls so
     * record store is only closed when no previous calls to open remain, so
     * it can be safely called from any method.
     *
     * @throws RSException on rs errors
     */
    public synchronized void close()
            throws RSException
    {
        final RecordStore rs = this.rs;
        int openCount = this.openCount;
        if( openCount > 0 )
        {
            openCount--;
            if( openCount < 0 )
            {
                openCount = 0;
            }
            if( openCount == 0 )
            {
                try
                {
                    if( rs != null )
                    {
                        flush();
                        rs.closeRecordStore();
                    }
                }
                catch( RecordStoreNotOpenException e )
                {
                }
                catch( Exception e )
                {
                    ErrorLog.addError(
                            CLASS_NAME,
                            "close",
                            null,
                            info.name,
                            e
                    );
                    throw new RSException( Constants.ERR_STR_CLOSE, e );
                }
                finally
                {
                    this.rs = null;
                    if( listener != null )
                    {
                        listener.rsClose();
                    }
                }
            }
            this.openCount = openCount;
        }
    }

    /**
     * Open the underlying record store, creating it if necessary.<br/>
     * Keeps track of nested calls so record store is only opened one,
     * so it can be safely called from any method.
     *
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     */
    public synchronized void open()
            throws RSException,
                   RecordStoreFullException
    {
        open( true );
    }

    /**
     * Open the underlying record store.<br/>
     * Keeps track of nested calls so record store is only opened one,
     * so it can be safely called from any method.
     *
     * @param createIfNecessary create underlyin record store if necessary
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void open( final boolean createIfNecessary )
            throws RSException,
                   RecordStoreFullException
    {
        if( rs == null )
        {
            openCount = 0;
        }
        if( openCount == 0 )
        {
            try
            {
                //noinspection RecordStoreOpenedButNotSafelyClosed
                rs = RecordStore.openRecordStore( info.name, createIfNecessary );
            }
            catch( javax.microedition.rms.RecordStoreFullException e )
            {
                throw new RecordStoreFullException( Constants.ERR_STR_OPEN, e );
            }
            catch( RecordStoreException e )
            {
                log.error( e );
                throw new RSException( Constants.ERR_STR_OPEN, e );
            }
        }
        openCount++;
    }

    /**
     * Get store name.
     *
     * @return store name
     */
    public String getName()
    {
        return info.name;
    }

    public synchronized void shutdown()
            throws RecordStoreFullException,
                   RSException
    {
        flush();
        final RecordStore rs = this.rs;
        if( rs != null )
        {
            while( true )
            {
                try
                {
                    rs.closeRecordStore();
                }
                catch( RecordStoreNotOpenException e )
                {
                    break;
                }
                catch ( RecordStoreException e )
                {
                    log.error( e );
                }
            }
            openCount = 0;
            this.rs = null;
            if( listener != null )
            {
                listener.rsClose();
            }
        }
    }

    /**
     * Remove this store and underlying data.
     *
     * @throws RSException on record store exceptions
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     * */
    public synchronized void drop()
            throws RSException,
                   RecordStoreFullException
    {
        final String name = info.name;
        final short multiplexer = info.multiplexer;
        shutdown();
        safeDeleteRecordStore( info.name );
        if( info != null )
        {
            info.delete();
            info = new StoreInfo( name, multiplexer );
        }
    }

    public long getLastModified()
    {
        return info.lastModified;
    }

    public int getNextRecordId()
    {
        return info.nextId;
    }

    /**
     * Delete a record.
     *
     * @param recordId record id
     * @throws RSException on record store exceptions
     * @throws InvalidRecordIDException if the record does not exists
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void deleteRecord( final int recordId )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
//        final Log log = Store.log;
        try
        {
            //log.debug( "deleteRecord( " + recordId + " )" );
            open();
            final Store.RecordLocator locator = this.locator;
            locator.translate( recordId );
            makeCurrent( locator.recordId, false );
            if( !currentRecord.isNull( locator.offset ) )
            {
                final int length = currentRecord.getSize( locator.offset );
                currentRecord.delete( locator.offset );
                flush();
                if( currentRecord.size == 0 && currentRecord.getRecordId() < rs.getNextRecordID() - 1 )
                {
                    //log.debug( "removing empty Record: " + currentRecord );
                    rs.deleteRecord( currentRecord.getRecordId() );
                    this.currentRecord = null;
                }
                info.deleteRecord( length );
            }
            else
            {
                ErrorLog.addError(
                        CLASS_NAME,
                        "deleteRecord",
                        new Object[] { new Integer( recordId ) },
                        "Invalid record in valid chunk",
                        null
                );
                throw new InvalidRecordIDException( Constants.ERR_STR_DELETE_RECORD, Integer.toString( recordId ) );
            }
        }
        catch( javax.microedition.rms.InvalidRecordIDException e )
        {
            ErrorLog.addError(
                    CLASS_NAME,
                    "deleteRecord",
                    new Object[] { new Integer( recordId ) },
                    "Invalid record (@rs.deleteRecord): " + locator.toString(),
                    e
            );
            throw new InvalidRecordIDException( Constants.ERR_STR_DELETE_RECORD, e );
        }
        catch( RecordStoreException e )
        {
            ErrorLog.addError(
                    CLASS_NAME,
                    "deleteRecord",
                    new Object[] { new Integer( recordId ) },
                    "RecordStoreException (@rs.deleteRecord): " + locator.toString(),
                    e
            );
            throw new RSException( Constants.ERR_STR_DELETE_RECORD, e );
        }
        catch( SerializationException e )
        {
            ErrorLog.addError(
                    CLASS_NAME,
                    "deleteRecord",
                    new Object[] { new Integer( recordId ) },
                    "SerializationException (@rs.deleteRecord): " + locator.toString(),
                    e
            );
            throw new RSException( Constants.ERR_STR_DELETE_RECORD, e );
        }
        finally
        {
            close();
        }
    }

    public synchronized byte[] getRecord( final int recordId )
            throws RSException,
                   InvalidRecordIDException
    {
        final Log log = Store.log;
        try
        {
            //log.debug( "getRecord( " + recordId + " )" );
            open();
            final RecordLocator locator = this.locator;
            locator.translate( recordId );
            makeCurrent( locator.recordId, false );
            return currentRecord.get( locator.offset );
        }
        catch( InvalidRecordIDException e )
        {
            log.error( locator.toString(), e );
            throw e;
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    CLASS_NAME,
                    "getRecord",
                    new Object[] { new Integer( recordId ) },
                    info.name,
                    e
            );
            throw new RSException( Constants.ERR_STR_DELETE_RECORD, e );
        }
        finally
        {
            close();
        }
    }

    public synchronized int getRecord(
            final int       recordId,
            final byte[]    buffer,
            final int       offset
    )
            throws RSException,
                   InvalidRecordIDException
    {
        final byte[] data = getRecord( recordId );
        System.arraycopy( data, 0, buffer, offset, data.length );
        return data.length;
    }

    public synchronized int getRecordSize( final int recordId )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        final Log log = Store.log;
        try
        {
            //log.debug( "getRecordSize( " + recordId + " )" );
            open();
            final RecordLocator locator = this.locator;
            locator.translate( recordId );
            makeCurrent( locator.recordId, false );
            if( currentRecord.isNull( locator.offset ) )
            {
                throw new InvalidRecordIDException( Constants.ERR_STR_GET_RECORD_SIZE, Integer.toString( recordId ) );
            }
            else
            {
                return currentRecord.getSize( locator.offset );
            }
        }
        catch( SerializationException e )
        {
            log.error( e );
            throw new RSException( Constants.ERR_STR_GET_RECORD_SIZE, Integer.toString( recordId ) );
        }
        finally
        {
            close();
        }
    }

    public int getNumRecords()
    {
        return info.count;
    }

    public StoreEnumeration enumerateRecords()
            throws RSException,
                   RecordStoreFullException
    {
        try
        {
            open();
            return new StoreEnumeration(
                    this,
                    rs.enumerateRecords( null, null, false )
            );
        }
        catch( RecordStoreFullException e )
        {
            log.error( e );
            throw e;
        }
        catch( RecordStoreNotOpenException e )
        {
            log.error( e );
            throw new RSException( Constants.ERR_STR_ENUMERATE, e );
        }
    }

    public int getMultiplexer()
    {
        return info.multiplexer;
    }

    synchronized Vector mapValidRecordIds( final int recordId )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        try
        {
            open();
            makeCurrent( recordId, false );
            int id;
            if( info.multiplexer > 1 )
            {
                id = ((recordId - 1) * info.multiplexer) + 1;
            }
            else
            {
                id = recordId;
            }
            return currentRecord.getValidIds( id );
        }
        catch( SerializationException e )
        {
            log.error( e );
            throw new RSException( Constants.ERR_STR_GET_VALID_OFFSETS, e );
        }
        finally
        {
            close();
        }
    }

    public static boolean isBurstMode()
    {
        return burstMode;
    }

    public synchronized static void setBurstMode( final boolean burstMode )
            throws RecordStoreFullException,
                   RSException
    {
        if( Store.burstMode != burstMode )
        {
            Store.burstMode = burstMode;
            if( !burstMode )
            {
                StoreInfo.flush();
            }
        }
    }

    // ToDo: metodos pendientes
    // getFreeDisk

    public long getSize()
    {
        return info.size;
    }

    /**
     * Packs used space by copying records to a new record store, droping current
     * and copying back to this one. This method is needed to solve bugs on
     * some phones that miss space on setRecord or do not recover space on
     * deleteRecord mehtod calls.
     * @throws RSException on rs errors
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void packCopy()
            throws RSException,
                   RecordStoreFullException
    {
        ProgressEvent pe = new ProgressEvent();
        pe.setAnimate( true );
        pe.setTitle( ResourceManager.getResource( "rs.packing" ) );
        pe.setPhase( ResourceManager.getResource( "rs.packing" ) );
        pe.setSource( this );
        pe.dispatch();

        shutdown();
        open();
        RecordStore pack = null;
        try
        {
            final byte[] empty = new byte[1];
            pack = RecordStore.openRecordStore( info.name + "_bak", true );
            final int count = rs.getNextRecordID();
            pe.setAnimate( false );
            pe.setMaxValue( new Integer( (count - 1) * 2 ) );
            pe.setCurrentValue( 0 );
            pe.dispatch();
            for( int i = 1; i < count; i++ )
            {
                try
                {
                    final byte[] data = rs.getRecord( i );
                    pack.addRecord( data, 0, data.length );
                }
                catch( javax.microedition.rms.InvalidRecordIDException e )
                {
                    pack.addRecord( empty, 0, 1 );
                    pack.deleteRecord( i );
                }
                pe.increment();
                pe.dispatch();
            }
            drop();
            open();
            for( int i = 1; i < count; i++ )
            {
                try
                {
                    final byte[] data = pack.getRecord( i );
                    rs.addRecord( data, 0, data.length );
                }
                catch( javax.microedition.rms.InvalidRecordIDException e )
                {
                    rs.addRecord( empty, 0, 1 );
                    rs.deleteRecord( i );
                }
                pe.increment();
                pe.dispatch();
            }
        }
        catch( RecordStoreException e )
        {
            log.error( e );
            ErrorLog.addError(
                    CLASS_NAME,
                    "packCopy",
                    null,
                    info.name,
                    e
            );
            throw new RSException( Constants.ERR_STR_PACK_COPY, e );
        }
        finally
        {
            if( pack != null )
            {
                try{ pack.closeRecordStore(); }catch( Exception e ){}
                try{ RecordStore.deleteRecordStore( info.name + "_bak" ); }catch( Exception e ){}
            }
        }
    }

    public void setRecord( final int recordId, final byte[] data )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        setRecord( recordId, data, 0, data.length );
    }

    /**
     * Set the content of a record.
     *
     * @param recordId record id
     * @param data byte array with data
     * @param offset offset into data array
     * @param length length of data
     * @throws RSException on record store exception
     * @throws InvalidRecordIDException if the record does not exists
     * @throws RecordStoreFullException if no space left
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void setRecord(
            final int       recordId,
            final byte[]    data,
            final int       offset,
            final int       length
    )
            throws RSException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        try
        {
            //log.debug( "setRecord( " + recordId + ", " + (data != null) + ", " + offset + ", " + length + " )" );
            open();
            final Store.RecordLocator locator = this.locator;
            locator.translate( recordId );
            makeCurrent( locator.recordId, false );
            if( !currentRecord.isNull( locator.offset ) )
            {
                final int currentSize = currentRecord.getSize( locator.offset );
                currentRecord.set( locator.offset, data, offset, length );
                if( !burstMode )
                {
                    flush();
                }
                info.updateRecord( length - currentSize );
            }
            else
            {
                ErrorLog.addError(
                        CLASS_NAME,
                        "setRecord",
                        new Object[] { new Integer( recordId ), data },
                        "Invalid record in valid chunk",
                        null
                );
                throw new InvalidRecordIDException( Constants.ERR_STR_SET_RECORD, Integer.toString( recordId ) );
            }
        }
        catch( SerializationException e )
        {
            log.error( e );
            ErrorLog.addError(
                    CLASS_NAME,
                    "setRecord",
                    new Object[] { new Integer( recordId ), data, new Integer( offset ), new Integer( length ) },
                    info.name,
                    e
            );
            throw new RSException( Constants.ERR_STR_DELETE_RECORD, e );
        }
        finally
        {
            close();
        }
    }

    /**
     * Stops the Store system, so it's safe to make a backup.
     */
    public static void stopStoreSystem()
    {
        try
        {
            StoreInfo.flush();
        }
        catch( RSException e )
        {
            log.error( e );
            ErrorLog.addError( "Store", "stopStoreSystem", null, null, e );
        }
    }

    /**
     * Start again the store system, after a call to stopStoreSystem.<br/>
     * There's no need to call this method unless you call stopStoreSystem first.
     */
    public static void startStoreSystem()
    {
        StoreInfo.load();
    }

    private class RecordLocator
    {
        int multiplexer;
        int recordId;
        int offset;

        public RecordLocator( final int multiplexer )
        {
            this.multiplexer = multiplexer;
        }

        public void translate( final int id )
        {
            final int multiplexer = this.multiplexer;
            if( multiplexer > 1 )
            {
                final int zeroBasedId = ( id - 1 );
                recordId = ( zeroBasedId / multiplexer ) + 1;
                offset = zeroBasedId % multiplexer;
                //log.debug( "translation( " + multiplexer + "): " + recordId + ", " + offset );
            }
            else
            {
                recordId = id;
                offset = 0;
            }
        }

        public String toString()
        {
            return "RecordLocator{" +
                   "multiplexer=" + multiplexer +
                   ", recordId=" + recordId +
                   ", offset=" + offset +
                   '}';
        }
    }

    public interface Listener
    {
        /**
         * Called by the Store when the underlying record store is really
         * opened and listener should perform a set up.
         */
        void rsOpen();

        /**
         * Called by the Store when the underlying record store is really
         * closed and listener should perform clean up.
         */
        void rsClose();
    }

}
