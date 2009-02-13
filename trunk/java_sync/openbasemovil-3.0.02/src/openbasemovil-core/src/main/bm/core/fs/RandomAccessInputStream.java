package bm.core.fs;
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
import bm.core.CoreConstants;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.InvalidRecordIDException;
import java.io.InputStream;
import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
/*
 * File Information
 *
 * Created on       : 25-nov-2006 19:33:35
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Random access file over fixed cluster size SequentialFileOutputStream.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class RandomAccessInputStream
    extends InputStream
{
    private static Log log = LogFactory.getLog( "RandomAccessInputStream" );

    private RecordStore rs;
    private long        size;
    private int         clusterSize;
    private int         currentRecordId;
    private long        currentOffsset;
    private byte[]      chunk;
    private int         pos;
    private boolean     eof;

    /**
     * Create a new RandomAccessInputStream over a given RecordStore.<br/>
     * The records store must have been created with a SequentialFileOutputStream.
     *
     * @param rs record store
     * @throws FSException on errors
     */
    public RandomAccessInputStream( final RecordStore rs )
            throws FSException
    {
        this.rs = rs;
        init();
    }

    private void init()
            throws FSException
    {
        readHeader();
        currentRecordId = 2;
        currentOffsset = 0;
        if( size > 0 )
        {
            try
            {
                chunk = rs.getRecord( currentRecordId );
            }
            catch( InvalidRecordIDException e )
            {
                chunk = null;
                eof = true;
            }
            catch( RecordStoreException e )
            {
                log.error( e );
                throw new FSException( CoreConstants.ERR_RAIS_INIT, e );
            }
        }
    }

    /**
     * Create a new RandomAccessInputStream over a RecordStore with the given
     * name.<br/>
     * The records store must have been created with a SequentialFileOutputStream.
     *
     * @param name record store name
     * @throws FSException on errors
     */
    public RandomAccessInputStream( final String name )
            throws FSException
    {
        try
        {
            rs = RecordStore.openRecordStore( name, false );
            init();
        }
        catch( RecordStoreException e )
        {
            log.error( e );
            throw new FSException( CoreConstants.ERR_RAIS_CONSTRUCTOR, e );
        }
    }

    /**
     * Get total file size.
     *
     * @return file size.
     */
    public long getSize()
    {
        return size;
    }

    /**
     * Get the cluster size used to build the unerlying file.
     *
     * @return cluster size
     */
    public int getClusterSize()
    {
        return clusterSize;
    }

    /**
     * Check if EOF has been reached.
     *
     * @return true if so
     */
    public boolean isEof()
    {
        return eof;
    }

    /**
     * Get the current read offset.
     *
     * @return offset
     */
    public long getCurrentOffsset()
    {
        return currentOffsset;
    }

    private void readHeader()
            throws FSException
    {
        try
        {
            final byte[] data = rs.getRecord( 1 );
            final ByteArrayInputStream bais = new ByteArrayInputStream( data );
            final DataInputStream in = new DataInputStream( bais );

            final int magic = in.readInt();
            if( magic != SequentialFileOutputStream.MAGIC_NUMBER )
            {
                throw new RecordStoreException( "Invalid magic number" );
            }
            final byte version = in.readByte();
            clusterSize = in.readInt();
            size = in.readLong();
            eof = size == 0;
        }
        catch( Exception e )
        {
            log.error( e );
            throw new FSException( CoreConstants.ERR_RAIS_READ_HEADER, e );
        }
    }

    /**
     * Move the current read offset to the given one.
     *
     * @param offset offset, if the offset is invalid the position is moved to
     * EOF.
     * @throws FSException on errors
     */
    public void seek( final long offset )
            throws FSException
    {
        try
        {
            if( offset >= 0 && offset < size )
            {
                final int recordId = (int) (offset / clusterSize) + 2;
                final int recordOffset = (int) (offset % clusterSize);
                if( recordId != currentRecordId )
                {
                    chunk = rs.getRecord( recordId );
                    currentRecordId = recordId;
                }
                pos = recordOffset;
                eof = false;
                currentOffsset = offset;
            }
            else
            {
                eof = true;
            }
        }
        catch( RecordStoreException e )
        {
            log.error( e );
            throw new FSException( CoreConstants.ERR_RAIS_SEEK, e );
        }
    }

    /**
     * Read a single byte from the stream.
     *
     * @return byte, -1 if eof
     * @throws IOException on errors
     */
    public int read()
            throws IOException
    {
        if( !eof )
        {
            final int c = chunk[pos++];
            if( pos >= chunk.length )
            {
                try
                {
                    currentRecordId++;
                    if( currentRecordId == rs.getNextRecordID() )
                    {
                        eof = true;
                    }
                    else
                    {
                        chunk = rs.getRecord( currentRecordId );
                        pos = 0;
                    }
                }
                catch( RecordStoreException e )
                {
                    throw new IOException( e.getMessage() );
                }
            }
            currentOffsset++;
            return c & 0xFF;
        }
        else
        {
            return -1;
        }
    }

    public int available()
            throws IOException
    {
        return (int) (size - currentOffsset);
    }
}
