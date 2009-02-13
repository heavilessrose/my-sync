package bm.db;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
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

import bm.core.J2MEException;
import bm.core.ResourceManager;
import bm.core.ControlledTask;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.tools.Tools;
import bm.db.index.Index;
import bm.db.index.IndexInfo;
import bm.err.ErrorLog;
import bm.storage.*;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 07-dic-2006 11:10:28
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-09-11 14:25:53 +0200 (mar, 11 sep 2007) $
 * Revision         : $Revision: 14 $
 */

/**
 * The database keeps information about tables, and serves as an access point
 * for them.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 14 $
 */
public class Database
{
    public static final short ENGINE_VERSION = 3;

    static Log log = LogFactory.getLog( "Database" );

    /**
     * Collection of table objects, indexed by name.
     */
    private Hashtable tables        = new Hashtable( 10 );
    /**
     * Collection of TableInfo objects, indexed by name.
     */
    private Hashtable tableInfo     = new Hashtable( 10 );
    /**
     * Collection of TableStatus objects, indexed by bname.
     */
    private Hashtable tableStatus   = new Hashtable( 10 );
    /**
     * List of TableInfo objects, in the right order to send updates to the server.
     */
    private Vector    tableInfoList = new Vector( 10 );
    /**
     * Underlying record store.
     */
    private Store rs;

    /**
     * Database header.
     */
    private DatabaseHeader    header;

    private String  name;
    private boolean started;

    private static Hashtable databases = new Hashtable( 1 );

    /**
     * Connect to a given database.<br/>
     * Reuses already connected databases, so it's safe to call twice on this
     * method with the same argument, as it will return the same object.
     *
     * @param source sync source
     * @return Database object
     */
    public static Database connect( final String source )
    {
        Database db = (Database) databases.get( source );
        if( db == null )
        {
            db = new Database();
            db.name = source;
            db.rs = Store.get( source + "_db", (byte) 1 );
            db.loadHeader();
            databases.put( source, db );
        }
        return db;
    }

    /**
     * Create a new database.
     *
     * @param name database name
     * @return the database
     * @throws DBException if already exists
     * @throws RSException on storage errors
     * @throws RecordStoreFullException if no space left
     */
    public static Database create( final String name )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        Database db = (Database) databases.get( name );
        if( db == null )
        {
            db = new Database();
            db.name = name;
            db.rs = Store.get( name + "_db", (byte) 1 );
            db.header = new DatabaseHeader();
            db.saveHeader();
            databases.put( name, db );
        }
        else
        {
            throw new DBException( 0, "Existing database" );
        }
        return db;
    }

    /**
     * Get database name.
     *
     * @return database name
     */
    public String getName()
    {
        return name;
    }

    /**
     * Get a table information object given the table name.
     *
     * @param name table name
     * @return TableInfo or null if not found
     */
    public Table getTable( final String name )
    {
        return (Table) tables.get( name.toLowerCase() );
    }

    /**
     * Open the underlying record store.<br/>If it's already open increments
     * the open count.
     *
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     */
    public synchronized void open()
            throws RSException,
                   RecordStoreFullException
    {
        rs.open();
    }

    /**
     * Close the underlying record store.<br/>Takes into account nested calls
     * to open, calling close on the recordstore when the open count reaches 0.
     *
     * @throws RSException on errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public synchronized void close()
            throws RSException
    {
        rs.close();
    }

    /**
     * Shutdown the database.<br/>Must be called before program exit.
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors
     */
    public synchronized void shutdown()
            throws RecordStoreFullException,
                   RSException
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = tables.elements(); i.hasMoreElements(); )
        {
            final Table table = (Table) i.nextElement();
            table.shutdown();
        }
        if( rs != null )
        {
            rs.shutdown();
        }
    }

    /**
     * Calculate the total size of the database in bytes, including all the
     * tables and indexes.
     *
     * @return size in bytes
     */
    public long getTotalSize()
    {
        final int thisSize = (int) rs.getSize();
        if( thisSize == -1 )
        {
            return -1;
        }
        int totalSize = 0;
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = tables.elements(); i.hasMoreElements(); )
        {
            final Table table = (Table) i.nextElement();
            final long tableSize = table.getTotalSize();
            if( tableSize == -1 )
            {
                return -1;
            }
            totalSize += tableSize;
        }
        return totalSize + thisSize;
    }

    /**
     * Drop the database with all it's tables and indexes
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     */
    public void drop()
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        final Store rs = Store.get( name + "_db", (byte) 1 );
        rs.open( false );
        this.rs = rs;
        try
        {
            start();
        }
        catch( Exception e )
        {
            log.error( e );
        }
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = tableInfo.elements(); i.hasMoreElements(); )
        {
            final TableInfo tableInfo = (TableInfo) i.nextElement();
            final Table table = getTable( tableInfo.getName() );
            if( table != null )
            {
                table.drop();
            }
            else
            {
                final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
                if( indexInfo != null )
                {
                    final int indexes = indexInfo.length;
                    for( int j = 0; j < indexes; j++ )
                    {
                        if( indexInfo[j].getName() != null )
                        {
                            Store.safeDeleteRecordStore( indexInfo[j].getName() );
                        }
                    }
                }
                Store.safeDeleteRecordStore( tableInfo.getName() );
            }
        }
        rs.drop();
        Store.get( name + "_mix", (byte) 1 ).drop();
        databases.remove( name );
        header = new DatabaseHeader();
        log.info( "Dropped" );
    }

    /**
     * Check for locked tables and mark them as damaged.<br/>
     * A table is marked as locked while performing updates, so if an exception
     * rises the marked flag is uncleaned and thus the table might have suffered
     * damages.
     *
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void sanityCheck()
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = tableStatus.elements(); i.hasMoreElements(); )
        {
            final TableStatus ts = (TableStatus) i.nextElement();
            if( ts.isLocked() )
            {
                ts.setDamaged( true );
            }
        }
    }

    /**
     * Test if the database is damaged: any of the tables is damaged.
     *
     * @return if the database is damaged
     */
    public boolean isDamaged()
    {
        //noinspection MethodCallInLoopCondition
        for( Enumeration i = tableStatus.elements(); i.hasMoreElements(); )
        {
            final TableStatus ts = (TableStatus) i.nextElement();
            if( ts.isDamaged() )
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Start the database. Must be called once for each program execution before
     * any operation on database or tabe happens, with the exception of the
     * initialize method, which needs to be called once before the first start.
     *
     * @throws DBException on db errors
     * @throws SerializationException on errors
     * @throws bm.storage.RSException on errors
     * @throws bm.storage.RecordStoreFullException on errors
     */
    public void start()
            throws DBException,
                   RecordStoreFullException,
                   SerializationException,
                   RSException
    {
        if( !started )
        {
            try
            {
                open();

                loadStructure();

                final ProgressEvent event = new ProgressEvent();
                event.setSource( this );

                event.setCurrentValue( 1 );
                event.setMaxValue( new Integer( 2 ) );
                event.setPhase( ResourceManager.getResource(
                        "db.start.startSystem"
                ) );
                ProgressEvent.dispatch( event );

                event.setPhase( ResourceManager.getResource(
                        "db.start.Starting"
                ) );
                event.setMessage( event.getPhase() );
                ProgressEvent.dispatch( event );
                // Now build objects
                softStart( event );
                event.increment();
                ProgressEvent.dispatch( event );

                started = true;

                event.setCurrentValue( 2 );
                event.setMaxValue( new Integer( 2 ) );
                event.setPhase( ResourceManager.getResource(
                        "db.start.startSystem"
                ));
                event.setMessage( ResourceManager.getResource(
                        "global.Done"
                ) );
                ProgressEvent.dispatch( event );
                log.info( "Started" );
            }
            finally
            {
                close();
            }
        }
    }

    /**
     * Soft start of the database. It does not read any information from
     * storage, but relies on the information present on the tableInfo
     * structures. This method is designed for server side start of the database
     * where no real operations takes place, just structure checking.<br/>
     * This method MUST NOT be used from devices.
     *
     * @param event progress event, when called from the server it should be null
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws DBException on errors
     * @throws RSException on recordstore errors
     */
    public void softStart( final ProgressEvent event  )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        try
        {
            final int count = tableInfo.size();
            int current = 0;
            final String eventMessage = event != null ? event.getMessage() : null;
            if( event != null )
            {
                event.setAnimate( false );
                event.setMaxValue( new Integer( count ) );
                event.setCurrentValue( 0 );
            }
            // Now build objects
            //noinspection MethodCallInLoopCondition
            for( Enumeration enu = tableInfo.elements(); enu.hasMoreElements(); )
            {
                final TableInfo info = (TableInfo) enu.nextElement();
                startTable( info );
                if( event != null )
                {
                    current++;
                    event.setMessage( eventMessage + " " + current + "/" + count );
                    event.increment();
                    event.dispatch();
                }
            }
            log.info( "Started" );
        }
        finally
        {
            close();
        }
    }

    /**
     * Create a new table.
     *
     * @param table the table to create
     * @throws DBException on database errors
     * @throws SerializationException on object serialization errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void createTable( final Table table )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException
    {
        table.tableInfo.flushVectors();
        createTable( table.tableInfo );
        startTable( table );
    }

    /**
     * Create a new table.
     *
     * @param info table information
     * @throws SerializationException on errors serializing table info
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors adding to persistent storage
     * @throws DBException if the table already exists
     */
    public void createTable( final TableInfo info )
            throws SerializationException,
                   RecordStoreFullException,
                   RSException,
                   DBException
    {
        if( getTable( info.getName() ) != null )
        {
            throw new DBException( 0, "Table already exists" );
        }
        final ByteArrayOutputStream     baos = new ByteArrayOutputStream();
        final SerializerOutputStream    out = new SerializerOutputStream( baos );

        baos.reset();
        out.writeByte( Constants.DBRT_TABLE_INFO );
        info.serialize( out );
        byte[] data = baos.toByteArray();
        int recordId = rs.addRecord( data, 0, data.length );
        info.setRecordId( recordId );
        log.debug( "saved table info in " + data.length + " bytes" );

        final TableStatus status = new TableStatus( this );
        status.setTableName( info.getName() );
        baos.reset();
        out.writeByte( Constants.DBRT_TABLE_STATUS );
        status.serialize( out );
        data = baos.toByteArray();
        recordId = rs.addRecord( data, 0, data.length );
        status.setRecordId( recordId );
        log.debug( "saved table status in " + data.length + " bytes" );
        addTableStatus( status );
    }

    void startTable( final Table table )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        final Hashtable[] maps = buildTableFieldMap(
                table.tableInfo.getFieldInfo()
        );
        final Hashtable indexMap = buildTableIndexMap(
                table.tableInfo.getIndexInfo()
        );
        table.parent = this;
        table.fieldMap = maps[0];
        table.foreignKeys = maps[1];
        table.indexMap = indexMap;
        table.tableStatus = (TableStatus) tableStatus.get( table.getName() );
        // Sanity check, only if a full start is beign done
        if( table.getTableStatus() != null )
        {
            if( table.getTableStatus().isLocked() )
            {
                table.getTableStatus().setDamaged( true );
            }
        }
        tables.put( table.getName(), table );
    }

    private Hashtable[] buildTableFieldMap( final FieldInfo[] fieldInfo )
    {
        final int fieldCount = fieldInfo.length;
        final Hashtable fieldMap = new Hashtable( fieldCount );
        final Hashtable foreignKeys = new Hashtable( 1 );
        for( int i = 0; i < fieldCount; i++ )
        {
            final FieldInfo fi = fieldInfo[i];
            fieldMap.put( fi.getName(), new Integer( i ) );
            if( fi.getFkTable() != null )
            {
                foreignKeys.put( fi.getFkTable().toLowerCase(),  fi.getName()  );
            }
        }
        return new Hashtable[] { fieldMap, foreignKeys };
    }

    public Table startTable( final TableInfo info )
            throws RecordStoreFullException,
                   DBException,
                   RSException
    {
        final Hashtable[] maps = buildTableFieldMap( info.getFieldInfo()  );
        final Hashtable indexMap = buildTableIndexMap( info.getIndexInfo() );
        final Table table = new Table(
                this,
                info,
                (TableStatus) tableStatus.get( info.getName() ),
                maps[0],
                maps[1],
                indexMap
        );
        // Sanity check, only if a full start is beign done
        if( table.getTableStatus() != null )
        {
            if( table.getTableStatus().isLocked() )
            {
                table.getTableStatus().setDamaged( true );
            }
        }
        tables.put( info.getName(), table );
        return table;
    }

    private Hashtable buildTableIndexMap( final IndexInfo[] tableIndexInfo )
    {
        int indexCount;
        final Hashtable indexMap;
        if( tableIndexInfo != null )
        {
            indexCount = tableIndexInfo.length;
            indexMap = new Hashtable( indexCount );
            for( int i = 0; i < indexCount; i++ )
            {
                final IndexInfo idx = tableIndexInfo[i];
                final Index index = new Index(
                        idx.getName(),
                        idx.getOrder(),
                        idx.getKeyType(),
                        idx.isCaseSensitive()
                );
                indexMap.put( idx.getFieldExpression(), index );
            }
        }
        else
        {
            indexMap = new Hashtable(1);
        }
        return indexMap;
    }

    /**
     *
     * @noinspection FieldRepeatedlyAccessedInMethod,MethodCallInLoopCondition
     * @throws DBException on errors
     * @throws RSException on recordstore errors
     * @throws RecordStoreFullException if no space left
     */
    private void loadStructure()
            throws DBException,
                   RSException, RecordStoreFullException
    {
        // Local variable to improve speed on CDC
        final Hashtable tableInfo = this.tableInfo;
        final Store rs = this.rs;
        final StoreEnumeration re = rs.enumerateRecords();

        try
        {
            final int[] rids = DbTool.getSortedRecordIds( re );

            tableInfo.clear();
            tableStatus.clear();
            tableInfoList.removeAllElements();

            byte[] data;
            try
            {
                data = Tools.secureAlloc( Constants.START_SIZE );
            }
            catch( J2MEException e )
            {
                log.error( e );
                ErrorLog.addError(
                        "Database",
                        "loadStructure",
                        null,
                        null,
                        e
                );
                throw new DBException( Constants.ERR_DB_LOAD_STRUCTURE, e );
            }
            int recordId;
            int recordSize;
            byte rowType;
            //noinspection MethodCallInLoopCondition
            final ProgressEvent event = new ProgressEvent();
            event.setSource( this );
            event.setCurrentValue( 0 );
            event.setPhase( ResourceManager.getResource(
                    "db.start.startSystem"
            ) );
            event.setMaxValue( new Integer( 2 ) );
            ProgressEvent.dispatch( event );

            final int length = rids.length;

            event.setCurrentValue( 0 );
            event.setPhase( ResourceManager.getResource(
                    "db.start.loadData"
            ) );
            event.setMaxValue( new Integer( length ) );
            ProgressEvent.dispatch( event );
            //noinspection MethodCallInLoopCondition
            for( int i = 0; i < length; i++ )
            {
                recordId = rids[i];
                // Skip header record
                if( recordId > 1 )
                {
                    recordSize = rs.getRecordSize( recordId );
                    if( recordSize > data.length )
                    {
                        try
                        {
                            data = Tools.secureAlloc(
                                    recordSize + Constants.GROWTH_FACTOR
                            );
                        }
                        catch( J2MEException e )
                        {
                            log.error( e );
                            ErrorLog.addError(
                                    "Database",
                                    "loadStructure",
                                    null,
                                    null,
                                    e
                            );
                            throw new DBException(
                                    Constants.ERR_DB_LOAD_STRUCTURE,
                                    e
                            );
                        }
                    }
                    rs.getRecord( recordId, data, 0 );
                    ByteArrayInputStream bais = new ByteArrayInputStream( data, 0, recordSize );
                    SerializerInputStream in = new SerializerInputStream( bais );
                    rowType = in.readByte();
                    switch( rowType )
                    {
                        case Constants.DBRT_TABLE_INFO:
                            final TableInfo info = new TableInfo();
                            info.deserialize( in );
                            info.setRecordId( recordId );
                            tableInfo.put( info.getName(), info );
                            tableInfoList.addElement( info );
                            break;

                        case Constants.DBRT_TABLE_STATUS:
                            final TableStatus status = new TableStatus( this );
                            status.deserialize( in );
                            status.setRecordId( recordId );
                            tableStatus.put( status.getTableName(), status );
                            break;
                    }
                    event.increment();
                    ProgressEvent.dispatch( event );
                }
            }
            event.increment();
            ProgressEvent.dispatch( event );
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Database",
                    "loadStructure",
                    null,
                    null,
                    e
            );
            throw new DBException( Constants.ERR_DB_LOAD_STRUCTURE, e );
        }
    }

    private void loadHeader()
    {
        header = new DatabaseHeader();
        try
        {
            open();
            final byte[] data = rs.getRecord( 1 );
            header.deserialize(
                    new SerializerInputStream(
                            new ByteArrayInputStream( data )
                    )
            );
        }
        catch( InvalidRecordIDException e )
        {
        }
        catch( Exception e )
        {
            log.error( e );
        }
        finally
        {
            try
            {
                close();
            }
            catch( RSException e )
            {
                log.error( e );
            }
        }
    }

    /**
     * Update the status of a table.
     *
     * @param status TableStatus to update
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    void updateTableStatus( final TableStatus status )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream dos = new SerializerOutputStream( baos );
            dos.writeByte( Constants.DBRT_TABLE_STATUS );
            status.serialize( dos );
            open();
            final byte[] data = baos.toByteArray();
            rs.setRecord( status.getRecordId(), data, 0, data.length );
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Database",
                    "updateTableStatus",
                    new Object[] { status },
                    null,
                    e
            );
            throw new DBException(
                    Constants.ERR_DB_UPDATE_STATUS,
                    "db.updateTableStatus()",
                    e
            );
        }
        finally
        {
            close();
        }
    }

    /**
     * Update the status of a table.
     *
     * @param info TableStatus to update
     * @throws DBException on errors
     * @throws RecordStoreFullException if no more space is left on record store
     * @throws RSException on recordstore errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    void updateTableInfo( final TableInfo info )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream dos = new SerializerOutputStream( baos );
            dos.writeByte( Constants.DBRT_TABLE_INFO );
            info.serialize( dos );
            open();
            final byte[] data = baos.toByteArray();
            rs.setRecord( info.getRecordId(), data, 0, data.length );
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Database",
                    "updateTableInfo",
                    new Object[] {info},
                    null,
                    e
            );
            throw new DBException(
                    Constants.ERR_DB_UPDATE_STATUS,
                    "db.updateTableInfo()",
                    e
            );
        }
        finally
        {
            close();
        }
    }

    synchronized void saveHeader()
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        open();
        final Log log = Database.log;
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            header.serialize( out );
            final byte[] data = baos.toByteArray();
            final Store rs = this.rs;
            if( rs.getNumRecords() == 0 )
            {
                final int recordId = rs.addRecord( data, 0, data.length );
                if( recordId != 1 )
                {
                    shutdown();
                    Store.safeDeleteRecordStore( name + "_db" );
                    saveHeader();
                }
            }
            else
            {
                rs.setRecord( 1, data, 0, data.length );
            }
        }
        catch( Exception e )
        {
            ErrorLog.addError(
                    "Database",
                    "saveHeader",
                    null,
                    null,
                    e
            );
            log.error( e );
            throw new DBException( Constants.ERR_DB_SAVE_DBHEADER, e );
        }
        finally
        {
            close();
        }
    }

    /**
     * Change the database major version number.
     *
     * @param major version number
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void setVersionMajor( final short major )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        header.setVersionMajor( major );
        saveHeader();
    }

    /**
     * Change the database minor version number.
     *
     * @param minor version number
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void setVersionMinor( final short minor )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        header.setVersionMinor( minor );
        saveHeader();
    }

    void resetHeader()
            throws RSException,
                   DBException,
                   RecordStoreFullException
    {
        header = new DatabaseHeader();
        saveHeader();
    }

    public Vector getTableInfoList()
    {
        return tableInfoList;
    }

    /**
     * Get database major version number.
     *
     * @return version major
     */
    public short getVersionMajor()
    {
        return header.getVersionMajor();
    }

    /**
     * Get database minor version number.
     *
     * @return version minor
     */
    public short getVersionMinor()
    {
        return header.getVersionMinor();
    }

    public TableInfo getTableInfo( final String name )
    {
        return (TableInfo) tableInfo.get( name );
    }

    /**
     * Get the table status information for a given table.
     *
     * @param name table name
     * @return table status
     */
    public TableStatus getTableStatus( final String name )
    {
        return (TableStatus) tableStatus.get( name );
    }

    void addTableInfo( final TableInfo info )
    {
        tableInfo.put( info.getName(), info );
    }

    void addTableStatus( final TableStatus status )
    {
        tableStatus.put( status.getTableName(), status );
    }

    /**
     * Enumerate all tables in the database.
     *
     * @return enumeration of tables
     */
    public Enumeration tables()
    {
        return tables.elements();
    }

    /**
     * Actually remove all marked as removed rows from all the tables in the
     * database.
     *
     * @throws DBException on database errors
     * @throws SerializationException on object serialization errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws InvalidRecordIDException on errors
     */
    public void pack()
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException,
                   InvalidRecordIDException
    {
        final ProgressEvent pe = new ProgressEvent( this );
        final int size = tableInfoList.size();
        pe.setAnimate( false );
        pe.setCancellable( true );
        pe.setMaxValue( new Integer( size ) );
        pe.setPhase( ResourceManager.getResource( "db.packing" ) );
        for( int i = 0; i < size && !ControlledTask.isCancelled(); i++ )
        {
            final TableInfo tableInfo =
                    (TableInfo) tableInfoList.elementAt( i );
            pe.setMessage( ResourceManager.getResource(
                    "tableNames." + tableInfo.getName()
            ) );
            pe.dispatch();
            final Table table = (Table) tables.get( tableInfo.getName() );
            table.pack();
            pe.increment();
            pe.dispatch();
        }
    }

    /**
     * Repair the databae if it is damaged.<br/>
     * Currently does not repair the tables, but the indexes that are marked
     * as damaged. Tables are not usually damaged, but indexes are more complex
     * and can be damaged more easily if the phone runs out of battery or the
     * like.
     *
     * @throws DBException on database errors
     * @throws SerializationException on object serialization errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void repair()
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException
    {
        final ProgressEvent pe = new ProgressEvent( this );
        final int size = tableInfoList.size();
        pe.setAnimate( false );
        pe.setCancellable( true );
        pe.setMaxValue( new Integer( size ) );
        pe.setPhase( ResourceManager.getResource( "db.repairing" ) );
        final Hashtable damaged = new Hashtable( 10 );
        for( int i = 0; i < size && !ControlledTask.isCancelled(); i++ )
        {
            final TableInfo tableInfo =
                    (TableInfo) tableInfoList.elementAt( i );
            pe.setMessage( ResourceManager.getResource(
                    "tableNames." + tableInfo.getName()
            ) );
            pe.dispatch();
            final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
            if( indexInfo != null && indexInfo.length > 0 )
            {
                damaged.clear();
                final Table table = (Table) tables.get( tableInfo.getName() );
                final int indexCount = indexInfo.length;
                for( int j = 0; j < indexCount; j++ )
                {
                    final Index index = table.getIndex( j );
                    if( index.isDamaged() )
                    {
                        damaged.put( indexInfo[j].getFieldNames(), index );
                    }
                }
                if( damaged.size() > 0 )
                {
                    table.rebuildIndexes( damaged );
                }
            }
            pe.increment();
            pe.dispatch();
        }
    }

    void stopTable( final TableInfo info )
    {
        final String tableName = info.getName();
        tables.remove( tableName );
        tableInfo.remove( tableName );
        tableStatus.remove( tableName );
        tableInfoList.removeElement( info );
    }

    /**
     * Drop a table from the database, not just the files of the table but
     * the table definition from the database structure.
     *
     * @param table table to drop
     * @throws RSException on storage errors
     * @throws RecordStoreFullException if no space left
     */
    public void dropTable( final Table table )
            throws RecordStoreFullException,
                   RSException
    {
        table.drop();
        stopTable( table.getTableInfo() );

        // Phisical drop
        try
        {
            open();
            rs.deleteRecord( table.getTableStatus().getRecordId() );
            rs.deleteRecord( table.getTableInfo().getRecordId() );
        }
        catch( InvalidRecordIDException e )
        {
            log.error( e );
        }
        finally
        {
            try{ close(); }catch( Exception e ){}
        }
    }
}
