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

import bm.core.ResourceManager;
import bm.core.SimpleCache;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.db.index.Index;
import bm.db.index.IndexInfo;
import bm.err.ErrorLog;
import bm.storage.*;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 07-dic-2006 12:25:44
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-10-19 12:25:27 +0200 (vie, 19 oct 2007) $
 * Revision         : $Revision: 16 $
 */

/**
 * A database table.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 16 $
 */
public class Table
    implements Store.Listener
{
    private static Log log = LogFactory.getLog( "Table" );

    // Parent database
    transient Database          parent;
    // Table information
    transient TableInfo         tableInfo;
    // Table status
    transient TableStatus       tableStatus;
    // Underlying recordstore
    transient Store     rs;
    // Foreign keys to other tables, keys are table names, values field names
    transient Hashtable         foreignKeys = new Hashtable();
    // Map of fields, the keys are the field names in lowercase, the values
    // are Integers corresponding to the mapping from name to position
    transient Hashtable         fieldMap;
    // Map of indexes, the keys are the indexes field expression in lower case,
    // the values are the Index objects
    transient Hashtable         indexMap;
    // Simple cache for storing fetched rows
    transient SimpleCache       cache = new SimpleCache();

    /**
     * Create a new local table.
     *
     * @param name table name
     */
    public Table( final String name )
    {
        tableInfo = new TableInfo();
        tableInfo.setName( name );
        tableInfo.setReadWriteType( TableInfo.WRITABLE );
        try
        {
            createIndex(
                    "si_" + name + "_id", 50,
                    "remote_id",
                    Index.KT_LONG, false
            );
        }
        catch( Exception e )
        {
            // No exception will raise since nothing will be done but adding
            // the index to the vector in the table info
            // as the table is new and has not been actually saved to disk
        }
    }

    /**
     * Create a new table.
     *
     * @param parent parent database
     * @param tableInfo table information
     * @param tableStatus table status object
     * @param fieldMap field mapping from name to position
     * @param foreignKeys map of foreign keys
     * @param indexMap index mapping from expressions to indexes
     */
    Table(
            final Database      parent,
            final TableInfo     tableInfo,
            final TableStatus   tableStatus,
            final Hashtable     fieldMap,
            final Hashtable     foreignKeys,
            final Hashtable     indexMap
    )
    {
        this.parent         = parent;
        this.tableInfo      = tableInfo;
        this.tableStatus    = tableStatus;
        this.fieldMap       = fieldMap;
        this.foreignKeys    = foreignKeys;
        this.indexMap       = indexMap;
    }

    /**
     * Add a new column specifying all the possible attributes.
     *
     * @param name column name
     * @param type column type (one of the FT_ constants at Constants)
     * @param length column length, only meaningful for String should be 0 for other types
     * @param nullable if the column can have null values
     * @param fkTable optional name of a table to which this column is a foreign key
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void addColumn(
            final String    name,
            final byte      type,
            final int       length,
            final boolean   nullable,
            final String    fkTable
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        tableInfo.addField( new FieldInfo( name, type, nullable, length, fkTable ) );
        if( fkTable != null )
        {
            foreignKeys.put( fkTable.toLowerCase(), name.toLowerCase() );
        }
        flushExisting();
    }

    /**
     * Add a new column (not for String columns).
     *
     * @param name column name
     * @param type column type (one of the FT_ constants at Constants)
     * @param nullable if the column can have null values
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void addColumn(
            final String    name,
            final byte      type,
            final boolean   nullable
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( type == Constants.FT_STRING )
        {
            throw new IllegalArgumentException( "String fields need a length" );
        }
        addColumn( name, type, 0, nullable, null );
    }

    /**
     * Add a new nullable column (not for String columns).
     *
     * @param name column name
     * @param type column type (one of the FT_ constants at Constants)
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void addColumn(
            final String    name,
            final byte      type
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( type == Constants.FT_STRING )
        {
            throw new IllegalArgumentException( "String fields need a length" );
        }
        addColumn( name, type, 0, true, null );
    }

    /**
     * Add a new nullable column.
     *
     * @param name column name
     * @param type column type (one of the FT_ constants at Constants)
     * @param length column length, only meaningful for String should be 0 for other types
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void addColumn(
            final String    name,
            final byte      type,
            final int       length
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        addColumn( name, type, length, true, null );
    }

    /**
     * Add a new nullable column with a foreign key (not for String columns)
     *
     * @param name column name
     * @param type column type (one of the FT_ constants at Constants)
     * @param fkTable optional name of a table to which this column is a foreign key
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void addColumn(
            final String    name,
            final byte      type,
            final String    fkTable
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( type == Constants.FT_STRING )
        {
            throw new IllegalArgumentException( "String fields need a length" );
        }
        addColumn( name, type, 0, true, fkTable );
    }

    /**
     * Create a new index.
     *
     * @param name index name, must be unique in the database
     * @param order index btree order
     * @param fields collection of fields that create the index (ordered)
     * @param keyType key type, only meaningfull if fields has just one element
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final int       order,
            final String[]  fields,
            final byte      keyType,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( order, name, fields, keyType, caseSensitive ) );
    }

    /**
     * Create a new index with a default btree order (30).
     *
     * @param name index name, must be unique in the database
     * @param fields collection of fields that create the index (ordered)
     * @param keyType key type, only meaningfull if fields has just one element
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final String[]  fields,
            final byte      keyType,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( 30, name, fields, keyType, caseSensitive ) );
    }

    /**
     * Create a new text index for multiple fields using the default btree order
     * (30).
     *
     * @param name index name, must be unique in the database
     * @param fields collection of fields that create the index (ordered)
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final String[]  fields,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo(
                30, name, fields, Index.KT_STRING, caseSensitive
        ) );
    }

    /**
     * Create a new text index.
     *
     * @param name index name, must be unique in the database
     * @param order index btree order
     * @param fields collection of fields that create the index (ordered)
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final int       order,
            final String[]  fields,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo(
                order, name, fields, Index.KT_STRING, caseSensitive
        ) );
    }

    /**
     * Create a case insensitive text index with a default btree order (30).
     *
     * @param name index name, must be unique in the database
     * @param fields collection of fields that create the index (ordered)
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex( final String name, final String[] fields )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( 30, name, fields, Index.KT_STRING, false ) );
    }

    /**
     * Create a new case insensitive text index.
     *
     * @param name index name, must be unique in the database
     * @param order index btree order
     * @param fields collection of fields that create the index (ordered)
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final int       order,
            final String[]  fields
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( order, name, fields, Index.KT_STRING, false ) );
    }

    /**
     * Create a new single field index with the default btree order (30).
     *
     * @param name index name, must be unique in the database
     * @param field field to be indexed
     * @param keyType key type, only meaningfull if fields has just one element
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final String    field,
            final byte      keyType,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( 30, name, field, keyType, caseSensitive ) );
    }

    /**
     * Create a new single field index.
     *
     * @param name index name, must be unique in the database
     * @param order btree index order
     * @param field field to be indexed
     * @param keyType key type, only meaningfull if fields has just one element
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex(
            final String    name,
            final int       order,
            final String    field,
            final byte      keyType,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( new IndexInfo( order, name, field, keyType, caseSensitive ) );
    }

    /**
     * Create an index through an IndexInfo structure.
     *
     * @param indexInfo index infor
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex( final IndexInfo indexInfo )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        tableInfo.addIndex( indexInfo );
        flushExisting();
        checkBuildNewIndex( indexInfo );
    }

    private void checkBuildNewIndex( final IndexInfo indexInfo )
            throws DBException, SerializationException,
                   RecordStoreFullException, RSException
    {
        if( tableInfo.getRecordId() != 0 )
        {
            final Index index = new Index(
                    indexInfo.getName(),
                    indexInfo.getOrder(),
                    indexInfo.getKeyType(),
                    indexInfo.isCaseSensitive()
            );
            indexMap.put( indexInfo.getFieldExpression().toLowerCase(), index );
            final RowSet set = findAll();
            open();
            try
            {
                //noinspection MethodCallInLoopCondition
                while( set.next() )
                {
                    final Row row = set.getCurrent();
                    index.insert(
                            buildIndexValue( row, indexInfo.getFieldNames() ),
                            row.getRecordId().intValue()
                    );
                }
            }
            finally
            {
                close();
            }
        }
    }

    private void flushExisting()
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        if( tableInfo.getRecordId() != 0 )
        {
            tableInfo.flushVectors();
            parent.updateTableInfo( tableInfo );
        }
    }

    /**
     * Create a full text index on a collection of fields.
     *
     * @param name index name, must be unique in the database
     * @param fields collection of fields that create the index (ordered)
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createFullTextIndex(
            final String    name,
            final String[]  fields,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( name, fields, Index.KT_FULL_TEXT, caseSensitive );
    }

    /**
     * Create a full text index on a single field.
     *
     * @param name index name, must be unique in the database
     * @param field field to be indexed
     * @param caseSensitive for text indexes, if the index is case sensitive
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createFullTextIndex(
            final String    name,
            final String    field,
            final boolean   caseSensitive
    )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( name, field, Index.KT_FULL_TEXT, caseSensitive );
    }

    /**
     * Create a case insensitive full text index on a collection of fields.
     *
     * @param name index name, must be unique in the database
     * @param fields collection of fields that create the index (ordered)
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createFullTextIndex( final String name, final String[] fields )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( name, fields, Index.KT_FULL_TEXT, false );
    }

    /**
     * Create a case insensitive full text index on a collection of fields.
     *
     * @param name index name, must be unique in the database
     * @param field field to be indexed
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createFullTextIndex( final String name, final String field )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( name, field, Index.KT_FULL_TEXT, false );
    }

    /**
     * Create a long index over a single field.
     *
     * @param name index name
     * @param field field to be indexed
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws SerializationException on object serialization errors
     */
    public void createIndex( final String name, final String field )
            throws DBException,
                   RecordStoreFullException,
                   RSException,
                   SerializationException
    {
        createIndex( name, field, Index.KT_LONG, false );
    }

    /**
     * Create a foreign key.
     *
     * @param field field that is a reference to another table, must be of Long type
     * @param table referenced table
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public void createForeignKey( final String field, final String table )
            throws DBException,
                   RecordStoreFullException,
                   RSException
    {
        final String tableName = table.toLowerCase();
        if( !foreignKeys.containsKey( tableName ) )
        {
            final FieldInfo fi = getFieldInfo( field );
            if( fi != null )
            {
                fi.setFkTable( tableName );
                foreignKeys.put( tableName, field.toLowerCase() );
                parent.updateTableInfo( tableInfo );
            }
            else
            {
                throw new DBException( 0, "No such field" );
            }
        }
    }

    /**
     * Get the table status.
     *
     * @return table status
     */
    public TableStatus getTableStatus()
    {
        return tableStatus;
    }

    /**
     * Get table name.
     *
     * @return table name.
     */
    public String getName()
    {
        return tableInfo.getName();
    }

    /**
     * Get parent database.
     * @return database
     */
    public Database getParent()
    {
        return parent;
    }

    /**
     * Unconditionaly closes the underlying record store.
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors
     */
    public synchronized void shutdown()
            throws RecordStoreFullException,
                   RSException
    {
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        log.debug( "shutdown table: " + tableInfo.getName() );
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            log.debug( "shutdown index: " + index.getName() );
            index.shutdown();
        }
        if( rs != null )
        {
            rs.shutdown();
        }
    }

    /**
     * Get a row given it's record id (recordstore identifier).
     *
     * @param recordId record id
     * @return row
     * @throws DBException on errors
     * @throws RSException on rs errors
     * @throws RecordStoreFullException if no space left
     */
    public synchronized Row fetch( final int recordId )
            throws DBException,
                   RSException, RecordStoreFullException
    {
        final SimpleCache cache = this.cache;

        Row row = (Row) cache.get( Integer.toString( recordId ) );
        if( row != null )
        {
            // Sanitiy check, if for any reason the cache returns an invalid object
            if(
                    row.getRecordId() != null &&
                    row.getRecordId().intValue() == recordId
            )
            {
                return row.cloneRow();
            }
            else
            {
                cache.clear();
            }
        }
        row = getRowFromStore( recordId, false );
        cache.add( Integer.toString( recordId ), row );
        return row.cloneRow();
    }

    Index getIndex( final int index )
    {
        return (Index) indexMap.get(
                tableInfo.getIndexInfo()[index].getFieldExpression()
        );
    }

    /**
     *
     * @param recordId record id to get
     * @param fetchDeleted if true it will not throw an Exception if the row
     * has deleted status
     * @return Row
     * @throws DBException on errors
     * @throws RSException on recordstore errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws RecordStoreFullException if no space left
     */
    Row getRowFromStore(
            final int recordId,
            final boolean fetchDeleted
    )
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        final Log log = Table.log;
        try
        {
            open();
            final byte[] data;
            try
            {
                 data = rs.getRecord( recordId );
            }
            catch( InvalidRecordIDException e )
            {
                log.error( "Row does not exist: " + recordId );
                ErrorLog.addError(
                        "Table",
                        "getRowFromStore",
                        new Object[] { new Integer( recordId ) },
                        tableInfo.getName(),
                        e
                );
                throw new RecordNotFoundException(
                         Constants.ERR_TBL_GET_ROW_FROM_STORE,
                         "table.getRowFromStore(): " +
                         Integer.toString( recordId )
                );
            }
            if( !fetchDeleted && data[Row.STATUS_BYTE] == Row.STATUS_DELETED )
            {
                log.warn( "Deleted row: " + recordId );
                ErrorLog.addError(
                        "Table",
                        "getRowFromStore",
                        new Object[] { new Integer( recordId ) },
                        tableInfo.getName() + ": status deleted",
                        null
                );
                throw new RecordNotFoundException(
                        Constants.ERR_TBL_GET_ROW_FROM_STORE,
                        "table.getRowFromStore(): " +
                        Integer.toString( recordId )
                );
            }
            final Row row = new Row( this );
            row.setRecordId( recordId );
            row.deserialize( new SerializerInputStream( new ByteArrayInputStream(
                    data
            ) ) );
            return row;
        }
        catch( SerializationException e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Table",
                    "getRowFromStore",
                    new Object[] { new Integer( recordId ) },
                    tableInfo.getName(),
                    e
            );
            throw new RecordNotFoundException(
                    Constants.ERR_TBL_GET_ROW_FROM_STORE,
                    "table.getRowFromStore()",
                    e
            );
        }
        finally
        {
            close();
        }
    }

    /**
     * Update the given row on the sotre.
     *
     * @param row row to update
     * @throws DBException on errors
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws InvalidRecordIDException if row is not found
     * @throws RecordStoreFullException if no space left on record store
     */
    synchronized void store_update( final Row row )
            throws DBException,
                   InvalidRecordIDException,
                   RecordStoreFullException
    {
        if( row == null )
        {
            log.error( "store_update: row is null" );
            return;
        }
        if( row.getRecordId() == null )
        {
            log.warn( "call to store_update with null id" );
            // The exception guarantees that it will be correctly added
            throw new InvalidRecordIDException( Constants.ERR_TBL_STORE_UPDATE );
        }
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            open();
            row.serialize( out );
            final byte[] data = baos.toByteArray();
            rs.setRecord(
                    row.getRecordId().intValue(),
                    data,
                    0,
                    data.length
            );
            row.dirty = false;
            cache.add( row.getRecordId().toString(), row );
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Table",
                    "store_update",
                    new Object[] { row },
                    tableInfo.getName(),
                    e
            );
            throw new DBException( Constants.ERR_TBL_STORE_UPDATE, e );
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
     * Add the row to the store.
     *
     * @param row row to add
     * @param generateId if the row id should be generated
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space left on record store
     */
    synchronized void store_add( final Row row, final boolean generateId )
            throws DBException,
                   RecordStoreFullException
    {
        try
        {
            open();
            int mustBeRecordId = 0;
            long baseId = 0;
            if( generateId )
            {
                mustBeRecordId = rs.getNextRecordId();
                baseId = LoginManager.getDeviceNumber() * -100000;
                row.setId( baseId - mustBeRecordId );
            }

            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );
            row.serialize( out );
            final byte[] data = baos.toByteArray();
            final int recordId = rs.addRecord( data, 0, data.length );
            row.setRecordId( new Integer( recordId ) );
            if( generateId && recordId != mustBeRecordId )
            {
                // This should not happen, but we need to check it anyway
                row.setId( baseId - recordId );
                store_update( row );
            }
            row.dirty = false;
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Table",
                    "store_add",
                    new Object[] { row, new Boolean( generateId ) },
                    tableInfo.getName(),
                    e
            );
            throw new DBException(
                    Constants.ERR_TBL_STORE_ADD,
                    "table.store_add()",
                    e
            );
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
     * Unconditionaly closes and drops the underlying record stores.<br/>
     * This method only drops the files associated to the table, but not the
     * table definition from the database structure. To do so use the dropTable
     * method of the parent Database object (which calls this method before).
     *
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors
     */
    public synchronized void drop()
            throws RecordStoreFullException,
                   RSException
    {
        final TableInfo tableInfo = this.tableInfo;
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        final Log log = Table.log;
        log.debug( "shutdown table: " + tableInfo.getName() );
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            log.debug( "shutdown index: " + index.getName() );
//            index.shutdown();
            index.drop();
        }
        if( rs != null )
        {
            try
            {
                rs.drop();
            }
            catch( RSException e )
            {
                log.error( e );
                ErrorLog.addError(
                        "Table",
                        "drop",
                        null,
                        "Error droping recordstore",
                        e
                );
            }
        }
        else
        {
            Store.safeDeleteRecordStore( tableInfo.getName() );
        }
    }

    /**
     * Open the underlying record store.<br/>If it's already open increments
     * the open count.
     *
     * @throws DBException on errors
     * @throws RSException on rs errors
     * @throws RecordStoreFullException if no space left
     */
    public synchronized void open()
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        Store rs = this.rs;
        if( rs == null )
        {
            byte multiplexerFactor;
            final int rowSize = tableInfo.getMaxRowSize();
            if( rowSize == 0 )
            {
                multiplexerFactor = 1;
            }
            else if( rowSize > 0 )
            {
                int approx = bm.storage.Constants.MAX_MULTIPLEXED_ROW_SIZE /
                             ( rowSize * Constants.MULTIPLEXER_SECURITY_FACTOR );
                if( approx > bm.storage.Constants.MAX_MULTIPLEXED_ROWS )
                {
                    approx = bm.storage.Constants.MAX_MULTIPLEXED_ROWS;
                }
                multiplexerFactor = (byte) approx;
            }
            else
            {
                int approx = bm.storage.Constants.MAX_MULTIPLEXED_ROW_SIZE /
                             ( rowSize * -1 * Constants.MULTIPLEXER_SECURITY_FACTOR );
                if( approx > Constants.MAX_MULTIPLEXED_BLOB_ROWS )
                {
                    approx = Constants.MAX_MULTIPLEXED_BLOB_ROWS;
                }
                multiplexerFactor = (byte) approx;
            }
            if( multiplexerFactor <= 0 )
            {
                multiplexerFactor = 1;
            }
            rs = Store.get( tableInfo.getName(), multiplexerFactor );
            this.rs = rs;
        }
        rs.open();
    }

    /**
     * Open the store for the table and also the stores of all the table indexes.
     *
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public synchronized void openTree()
            throws RecordStoreFullException,
                   RSException,
                   DBException
    {
        open();
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            index.open();
        }
    }

    /**
     * Close the store for this table and for the table indexes.
     *
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     */
    public synchronized void closeTree()
            throws RecordStoreFullException,
                   RSException,
                   DBException
    {
        close();
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            index.close();
        }
    }

    /**
     * Get table information.
     *
     * @return table information
     */
    public TableInfo getTableInfo()
    {
        return tableInfo;
    }

    /**
     * Close the underlying record store.<br/>Takes into account nested calls
     * to open, calling close on the recordstore when the open count reaches 0.
     *
     * @throws RSException on errors
     */
    public synchronized void close()
            throws RSException
    {
        if( rs != null )
        {
            rs.close();
        }
    }

    void updateIndexes( final Row rowNew, final Row rowOld )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        // Local variable improves performance on CDC
        final TableInfo tableInfo   = this.tableInfo;
        final Hashtable indexMap    = this.indexMap;
        try
        {
            final int indexCount = tableInfo.getIndexInfo().length;
            final int oldRecordId = rowOld != null ? rowOld.getRecordId().intValue() : -1;
            final int newRecordId = rowNew != null ? rowNew.getRecordId().intValue() : -1;
            if( rowOld != null && rowNew != null )
            {
                // Check changed indexes
                for( int i = 0; i < indexCount; i++ )
                {
                    boolean changed = false;
                    final String fieldNames[] = tableInfo.getIndexInfo()[i].getFieldNames();
                    final int length = fieldNames.length;
                    for( int j = 0; j < length && !changed; j++ )
                    {
                        final Object newValue = rowNew.getField( fieldNames[j] );
                        final Object oldValue = rowOld.getField( fieldNames[j] );
                        changed =
                                (newValue != null || oldValue != null ) &&
                                (
                                        (newValue != null && oldValue == null) ||
                                        (newValue == null && oldValue != null) ||
                                        !newValue.equals( oldValue )
                                );
                    }
                    if( changed )
                    {
                        final Index index = (Index) indexMap.get(
                                tableInfo.getIndexInfo()[i].getFieldExpression()
                        );
                        index.delete( buildIndexValue( rowOld, fieldNames ), oldRecordId );
                        index.insert( buildIndexValue( rowNew, fieldNames ), newRecordId );
                    }
                }
            }
            else if( rowNew != null )
            {
                // Add row to all indexes
                for( int i = 0; i < indexCount; i++ )
                {
                    final Index index = (Index) indexMap.get(
                            tableInfo.getIndexInfo()[i].getFieldExpression()
                    );
                    if( index != null )
                    {
                        index.insert(
                                buildIndexValue(
                                        rowNew,
                                        tableInfo.getIndexInfo()[i].getFieldNames()
                                ),
                                newRecordId
                        );
                    }
                }
            }
            else
            {
                // Remove from all indexes
                for( int i = 0; i < indexCount; i++ )
                {
                    final Index index = (Index) indexMap.get(
                            tableInfo.getIndexInfo()[i].getFieldExpression()
                    );
                    if( index != null )
                    {
                        index.delete(
                                buildIndexValue(
                                        rowOld,
                                        tableInfo.getIndexInfo()[i].getFieldNames()
                                ),
                                oldRecordId
                        );
                    }
                }
            }
        }
        catch( InvalidFieldException e )
        {
            throw new DBException(
                    Constants.ERR_TBL_UPDATE_INDEXES,
                    "table.updateIndexes()",
                    e
            );
        }
    }

    private Object buildIndexValue(
            final Row       row,
            final String[]  fieldNames
    )
    {
        final int length = fieldNames.length;
        if( length == 1 )
        {
            return row.getField( fieldNames[0] );
        }
        else
        {
            final StringBuffer value = new StringBuffer();
            for( int i = 0; i < length; i++ )
            {
                if( i > 0 )
                {
                    value.append( "+" );
                }
                value.append( row.getString( fieldNames[i] ) );
            }
            return value.toString();
        }
    }

    /**
     * Physically remove a row from the recordset, updating the associated
     * indexes.
     * @param recordId id of the row in the recordset
     * @throws DBException on errors
     * @throws SerializationException on io errors
     * @throws RSException on rs errors
     * @throws RecordStoreFullException if no space left on record store
     */
    void dropRow( final int recordId )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException
    {
        try
        {
            open();
            try
            {
                final Row row = fetch( recordId );
                updateIndexes( null, row );
                rs.deleteRecord( recordId );
                cache.remove( row.getRecordId().toString() );
                tableStatus.incModificationCount();
            }
            catch( RecordNotFoundException e )
            {
                log.warn( e );
            }
        }
        catch( InvalidRecordIDException e )
        {
            log.warn( e );
            // Record might have already been deleted
        }
        finally
        {
            close();
        }
    }

    /**
     * Find rows using an exact match on an index.<br/>
     * If the index is a compound index, the field name is then a field
     * expression that can be build concatenating the field names involved with
     * a plus sign as separator between them.
     *
     * @param fieldName column/field name or expression on the row. Must be indexed.
     * @param value exact value to find, can be Long or String
     * @return RowSet with results
     * @throws IndexNotFoundException if the field is not indexed
     * @throws SerializationException on io errors
     * @throws RecordStoreFullException if no space left on record store
     */
    public RowSet find( final String fieldName, final Object value )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        final int[] result = findRaw( fieldName, value );
        return new RowSet( this, result, fieldName, value, RowSet.SM_EXACT );
    }

    /**
     * Find row record id's using an exact match on an index.<br/>
     * If the index is a compound index, the field name is then a field
     * expression that can be build concatenating the field names involved with
     * a plus sign as separator between them.
     *
     * @param fieldName column/field name on the row. Must be indexed.
     * @param value exact value to find, can be Long or String
     * @return array of record ids (int) or null if no matching results
     * @throws IndexNotFoundException if the field is not indexed
     * @throws RecordStoreFullException if no space left on record store
     */
    public int[] findRaw( final String fieldName, final Object value )
            throws RecordStoreFullException,
                   DBException
    {
        final Index index = (Index) indexMap.get( fieldName.toLowerCase() );
        if( index == null )
        {
            throw new IndexNotFoundException(
                    Constants.ERR_TBL_FIND_RAW,
                    "table.findRaw(): " + fieldName
            );
        }
        return (int[]) index.find( value );
    }

    /**
     * Create a new empty row.<br/>
     * No changes are made until a call to addRow with this obeject is made.
     *
     * @return a new row
     */
    public Row createRow()
    {
        return new Row( this );
    }

    /**
     * Save a row. If the row recordId is null the row is added, otherwise it's
     * updated.
     *
     * @param row row to save
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public synchronized void save( final Row row )
            throws DBException,
                   RecordStoreFullException,
                   SerializationException,
                   RSException
    {
        final TableStatus tableStatus = this.tableStatus;
        final TableInfo tableInfo = this.tableInfo;
        if( tableInfo.isReadOnly() ) //  || tableStatus.isLocked()
        {
            throw new DBException( Constants.ERR_TBL_SAVE_ROW, tableInfo.getName() );
        }
        if( row == null )
        {
            throw new IllegalArgumentException( "table.save(): Row is null" );
        }
        if( row.isSent() )
        {
            ErrorLog.addError(
                    "Table",
                    "save",
                    new Object[] { row },
                    tableInfo.getName() + ": " + ResourceManager.getResource( "db.table.cantModifyNotConfirmed" ),
                    null
            );
            throw new DBException( Constants.ERR_TBL_SAVE_ROW, ResourceManager.getResource(
                    "db.table.cantModifyNotConfirmed"
            ) );
        }
        try
        {
            open();
            final boolean wasLocked = tableStatus.isLocked();
            tableStatus.setLocked( true );
            final Row rowOld;
            if( row.getRecordId() == null )
            {
                // new
                rowOld = null;
                row.setStatus( Row.STATUS_NEW );
                store_add( row, true );
            }
            else
            {
                // update
                rowOld = row.getRecordId() != null ?
                         getRowFromStore(row.getRecordId().intValue(), false ) :
                         null;
                if( row.getStatus() != Row.STATUS_NEW )
                {
                    row.setStatus( Row.STATUS_MODIFIED );
                }
                try
                {
                    store_update( row );
                }
                catch( InvalidRecordIDException e )
                {
                    log.error( e );
                    // Sholud never happen, as this exception will raise first
                    // when getting the current contents at rowOld
                }
                tableStatus.incModificationCount();
            }
            tableStatus.setChanged( true );
            updateIndexes( row, rowOld );
            tableStatus.setLocked( wasLocked );
            cache.add( row.getRecordId().toString(), row );
        }
        finally
        {
            close();
        }
    }

    /**
     * Find rows using an fuzzy match on an index.<br/>
     * This method uses a semi-full text search algorithm on string indexes,
     * so that it will search for all words given as the begining of any of
     * the words in the index.<br/>
     * If the index is a compound index, the field name is then a field
     * expression that can be build concatenating the field names involved with
     * a plus sign as separator between them.
     *
     * @param fieldName column/field name on the row. Must be indexed.
     * @param value exact value to find, can be Long or String
     * @return RowSet with results
     * @throws DBException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public RowSet findFuzzy( final String fieldName, final String value )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        final int[] result = findRawFuzzy( fieldName, value );
        return new RowSet( this, result, fieldName, value, RowSet.SM_STARTS_WITH );
    }

    /**
     * Find rows using an fuzzy match on an index.<br/>
     * This method uses a semi-full text search algorithm on string indexes,
     * so that it will search for all words given as the begining of any of
     * the words in the index.<br/>
     * If the index is a compound index, the field name is then a field
     * expression that can be build concatenating the field names involved with
     * a plus sign as separator between them.
     *
     * @param fieldName column/field name on the row. Must be indexed.
     * @param value exact value to find, can be Long or String
     * @return array of record ids (int) or null if no matching results
     * @throws DBException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public int[] findRawFuzzy( final String fieldName, final String value )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        final Index index = (Index) indexMap.get( fieldName.toLowerCase() );
        if( index == null )
        {
            throw new IndexNotFoundException(
                    Constants.ERR_TBL_FIND_RAW,
                    "table.findRawFuzzy(): " + fieldName
            );
        }
        return (int[]) index.findFuzzy( value );
    }

    /**
     * Remove a row from the store. It really marks it as deleted until purged.
     *
     * @param row row to remove
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws SerializationException on errors
     * @throws RecordStoreFullException on errors
     */
    public void remove( final Row row )
            throws DBException,
                   RecordStoreFullException,
                   SerializationException,
                   RSException
    {
        final TableInfo tableInfo = this.tableInfo;
        final TableStatus tableStatus = this.tableStatus;
        try
        {
            if( row.getRecordId() == null )
            {
                // Not stored, nothing to do
                return;
            }
            if( tableInfo.isReadOnly() ) // || tableStatus.isLocked()
            {
                throw new DBException( Constants.ERR_TBL_REMOVE_ROW, tableInfo.getName() );
            }
            if( row.isSent() ) // ShouldDo: it could be allowed
            {
                ErrorLog.addError(
                        "Table",
                        "remove",
                        new Object[] { row },
                        tableInfo.getName() + ": " + ResourceManager.getResource( "db.table.cantModifyNotConfirmed" ),
                        null
                );
                throw new DBException( Constants.ERR_TBL_REMOVE_ROW, ResourceManager.getResource(
                        "db.table.cantModifyNotConfirmed"
                ) );
            }
            if( row.getParent() != this )
            {
                final DBException dbException =
                        new DBException( Constants.ERR_TBL_REMOVE_ROW, "Incorrect parent table" );
                ErrorLog.addError(
                        "Table",
                        "remove",
                        new Object[] { row },
                        tableInfo.getName(),
                        dbException
                );
                throw dbException;
            }
            open();
            final boolean wasLocked = tableStatus.isLocked();
            tableStatus.setLocked( true );
            final byte status = row.getStatus();
            if(
                    status == Row.STATUS_NEW ||
                    status == Row.STATUS_ERROR ||
                    status == Row.STATUS_ERROR_DELETED
            )
            {
                // Simply remove
                rs.deleteRecord( row.getRecordId().intValue() );
            }
            else
            {
                row.setStatus( Row.STATUS_DELETED );
                store_update( row );
            }
            updateIndexes( null, row );
            tableStatus.setChanged( true );
            tableStatus.setLocked( wasLocked );
            cache.remove( row.getRecordId().toString() );
        }
        catch( InvalidRecordIDException e )
        {
            log.error( e );
            ErrorLog.addError(
                    "Table",
                    "remove",
                    new Object[] { row },
                    tableInfo.getName(),
                    e
            );
            throw new RecordNotFoundException( Constants.ERR_TBL_REMOVE_ROW, "table.remove( row ): " + row.getRecordId().toString(), e );
        }
        finally
        {
            close();
        }
    }

    /**
     * Get all the rows in the table.
     *
     * @return RowSet with all the rows
     * @throws DBException on errors
     * @noinspection MethodCallInLoopCondition
     * @throws SerializationException on serialization errors
     * @throws RSException on storage errors
     * @throws RecordStoreFullException if no space left
     */
    public RowSet findAll()
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException
    {
        StoreEnumeration se = null;
        try
        {
            final ProgressEvent event = new ProgressEvent();
            event.setAnimate( true );
            event.setCurrentValue( 3 );
            event.setMaxValue( new Integer( -1 ) );
            final String searching = ResourceManager.getResource( "global.Searching" );
            event.setMessage( "" );
            event.setPhase( searching );
            event.setSource( this );
            event.dispatch();

            open();
            final Index index = (Index) indexMap.get( Constants.REMOTE_ID );
            int[] recordIds = null;
            // If there's an index on remote id use the index buildSortedList
            // feature, it's much faster than a RecordEnumeration
            boolean failure = true;
            if( index != null )
            {
                try
                {
                    recordIds = index.buildSortedList();
                    failure = false;
                }
                catch( Exception e )
                {
                    // If this operation fails, fall back to the enumeration
                    log.error( e );
                }
            }
            if( failure )
            {
                se = rs.enumerateRecords();
                final int count = se.numRecords();
                event.setAnimate( false );
                event.setCurrentValue( 0 );
                event.setMaxValue( new Integer( count ) );
                event.dispatch();
                recordIds = new int[ count ];
                for( int i = 0; se.hasNext(); i++ )
                {
                    recordIds[i] = se.nextId();
                    if( i % 10 == 0 )
                    {
                        event.setCurrentValue( i );
                        event.dispatch();
                    }
                }
            }
            return new RowSet( this, recordIds, null, null, RowSet.SM_EXACT );
        }
        catch( InvalidRecordIDException e )
        {
            ErrorLog.addError(
                    "Table",
                    "findAll",
                    null,
                    tableInfo.getName(),
                    e
            );
            throw new DBException( Constants.ERR_TBL_FIND_ALL, "table.findAll()", e );
        }
        finally
        {
            if( se != null )
            {
                se.destroy();
            }
            close();
        }
    }

    /**
     * Get the size (in bytes) of the table data
     * @return size in bytes, -1 on error
     */
    public long getSize()
    {
        try
        {
            open();
            return rs.getSize();
        }
        catch( Exception e )
        {
            log.error( e );
            return -1;
        }
        finally
        {
            try
            {
                close();
            }
            catch( Exception e )
            {
                log.error( e );
            }
        }
    }

    /**
     * Get the total size of the table, in bytes, including the size of the
     * indexes associated to this table.
     * @return size in bytes, -1 on error
     */
    public long getTotalSize()
    {
        final long thisSize = getSize();
        if( thisSize == -1 )
        {
            return -1;
        }
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        int totalSize = 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            final int size = index.getSize();
            if( size == -1 )
            {
                return -1;
            }
            totalSize += size;
        }
        return totalSize + thisSize;
    }

//    public RecordStoreStats getRecordStoreStats()
//    {
//        return rs.getStats();
//    }

    public long getCacheHits()
    {
        return cache.getHits();
    }

    public long getCacheMisses()
    {
        return cache.getMisses();
    }

    public long getAverageFindTime()
    {
        long total = 0;
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            total += index.getAverageFindTime();
        }
        return total > 0 && count > 0 ? total / count : 0;
    }

    /**
     * Remove the underlying record store.
     *
     * @throws DBException on errors
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     */
    public synchronized void remove()
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        log.debug( "dropTable: " + tableInfo.getName() );
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            log.debug( "drop index: " + index.getName() );
            index.drop();
        }
        if( rs != null )
        {
            rs.drop();
        }
    }

    /**
     * Check if the table has a field with a given name.
     *
     * @param fieldName field name, case insensitive
     * @return true if it has such a field, false otherwise
     */
    public boolean hasField( final String fieldName )
    {
        return fieldMap.containsKey( fieldName.toLowerCase() );
    }

    public void rsOpen()
    {
    }

    public void rsClose()
    {
        cache = null;
    }

    /**
     * Get the name of the field in this table that is a foreign key to the
     * specified table name.
     * @param tableName table name
     * @return field name or null if there is no foreign key defined to that
     * table
     */
    public String getForeignKeyField( final String tableName )
    {
        return (String) foreignKeys.get( tableName.toLowerCase() );
    }

    /**
     * Get field information.
     *
     * @param index field index
     * @return field information or null if index is not right.
     */
    public FieldInfo getFieldInfo( final int index )
    {
        final int length = tableInfo.getFieldInfo().length;
        if( index >= 0 && index < length )
        {
            return tableInfo.getFieldInfo()[index];
        }
        else
        {
            return null;
        }
    }

    /**
     * Get field information.
     *
     * @param name field name
     * @return field info or null if not found
     */
    public FieldInfo getFieldInfo( final String name )
    {
        final Integer index = (Integer) fieldMap.get( name.toLowerCase() );
        if( index != null )
        {
            return getFieldInfo( index.intValue() );
        }
        else
        {
            return null;
        }
    }

    void rebuildIndexes( final Hashtable damaged )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException,
                   RSException
    {
        if( damaged == null || damaged.size() == 0 )
        {
            return;
        }
        final RowSet set = findAll();
        final IndexInfo[] indexInfo = tableInfo.getIndexInfo();
        final int count = indexInfo != null ? indexInfo.length : 0;
        log.debug( "shutdown table: " + tableInfo.getName() );
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            final Index index = (Index) indexMap.get(
                    indexInfo[i].getFieldExpression().toLowerCase()
            );
            log.debug( "shutdown index: " + index.getName() );
            index.shutdown();
            index.drop();
        }
        open();
        try
        {
            //noinspection MethodCallInLoopCondition
            while( set.next() )
            {
                //noinspection MethodCallInLoopCondition
                for( Enumeration i = damaged.keys(); i.hasMoreElements(); )
                {
                    final String[] fieldNames = (String[]) i.nextElement();
                    final Index index = (Index) damaged.get( fieldNames );
                    final Row row = set.getCurrent();
                    index.insert(
                            buildIndexValue( row, fieldNames ),
                            row.getRecordId().intValue()
                    );
                }
            }
        }
        finally
        {
            close();
        }
    }

    /**
     * Actually remove all the rows marked as removed.
     *
     * @throws DBException on database errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on storage errors
     * @throws InvalidRecordIDException on errors
     */
    public void pack()
            throws RecordStoreFullException,
                   RSException,
                   InvalidRecordIDException, DBException
    {
        final ProgressEvent event = new ProgressEvent();
        event.setAnimate( true );
        event.setMessage( "" );
        event.setPhase( ResourceManager.getResource( "global.packing" ) );
        event.setSource( this );
        event.dispatch();

        StoreEnumeration se = null;
        try
        {
            open();
            se = rs.enumerateRecords();
            final int count = se.numRecords();
            event.setAnimate( false );
            event.setCurrentValue( 0 );
            event.setMaxValue( new Integer( count ) );
            event.dispatch();
            for( int i = 0; se.hasNext(); i++ )
            {
                final int recordId = se.nextId();
                final byte[] data = rs.getRecord( recordId );
                if( data != null && data[Row.STATUS_BYTE] == Row.STATUS_DELETED )
                {
                    rs.deleteRecord( recordId );
                }
                if( i % 10 == 0 )
                {
                    event.setCurrentValue( i );
                    event.dispatch();
                }
            }
        }
        finally
        {
            if( se != null) { se.destroy(); }
            close();
        }
    }
}
