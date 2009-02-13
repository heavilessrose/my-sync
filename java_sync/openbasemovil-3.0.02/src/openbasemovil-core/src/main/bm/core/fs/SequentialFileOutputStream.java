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
import bm.core.log.LogFactory;
import bm.core.log.Log;
import bm.core.CoreConstants;

import javax.microedition.rms.*;
import java.io.IOException;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.OutputStream;
/*
 * File Information
 *
 * Created on       : 26-nov-2006 0:50:58
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * OutputStream to create a sequential file over a recordset, that can be then
 * accessed as a random access file with RandomAccessInputStream.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class SequentialFileOutputStream
    extends OutputStream
{
    private static Log log = LogFactory.getLog( "SequentialFileOutputStream" );

    private static final int DEFAULT_CHUNK_SIZE = 5 * 1024;
    public static final int MAGIC_NUMBER = "_CLUSTERED_FILE_".hashCode();

    private RecordStore rs;
    private int         clusterSize;
    private byte[]      current;
    private int         pos;
    private String      name;
    private long        size;
    private boolean     recordStoreFull;

    public SequentialFileOutputStream()
    {
        this( "_tmp_" + System.currentTimeMillis() );
    }

    public SequentialFileOutputStream( final int clusterSize )
    {
        this( "_tmp_" + System.currentTimeMillis(), clusterSize );
    }

    public SequentialFileOutputStream( final String name )
    {
        this( name, DEFAULT_CHUNK_SIZE );
    }

    public SequentialFileOutputStream(
            final String        name,
            final int           clusterSize
    )
    {
        this.name = name;
        this.clusterSize = clusterSize;
        current = new byte[ clusterSize ];
    }

    public String getName()
    {
        return name;
    }

    public int getClusterSize()
    {
        return clusterSize;
    }

    public boolean isRecordStoreFull()
    {
        return recordStoreFull;
    }

    public void open()
            throws RecordStoreFullException,
                   FSException
    {
        if( rs == null )
        {
            try
            {
                rs = RecordStore.openRecordStore( name, false );
            }
            catch ( RecordStoreFullException e )
            {
                recordStoreFull = true;
                throw e;
            }
            catch( RecordStoreException e )
            {
            }
        }
        try
        {
            close();
            rs = RecordStore.openRecordStore( name, true );
            writeHeader();
        }
        catch ( RecordStoreFullException e )
        {
            recordStoreFull = true;
            throw e;
        }
        catch( Exception e )
        {
            log.error( e );
            throw new FSException( CoreConstants.ERR_SFO_OPEN );
        }
    }

    public void close()
            throws IOException
    {
        doFlush();
        try
        {
            if( rs != null )
            {
                writeHeader();
                try
                {
                    while( true )
                    {
                        try
                        {
                            rs.closeRecordStore();
                        }
                        catch ( RecordStoreFullException e )
                        {
                            recordStoreFull = true;
                            throw e;
                        }
                        catch( RecordStoreNotOpenException e )
                        {
                            break;
                        }
                    }
                }
                finally
                {
                    rs = null;
                }
            }
        }
        catch ( RecordStoreFullException e )
        {
            recordStoreFull = true;
            throw new IOException( e.getMessage() );
        }
        catch( RecordStoreException e )
        {
            throw new IOException( e.getMessage() );
        }
    }

    private void doFlush()
            throws IOException
    {
        if( pos > 0 )
        {
            try
            {
                rs.addRecord( current, 0, pos );
                pos = 0;
            }
            catch ( RecordStoreFullException e )
            {
                recordStoreFull = true;
                throw new IOException( e.getMessage() );
            }
            catch( RecordStoreException e )
            {
                throw new IOException( e.getMessage() );
            }
        }
    }

    private void writeHeader()
            throws IOException,
                   RecordStoreFullException,
                   RecordStoreException
    {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final DataOutputStream out = new DataOutputStream( baos );

        out.writeInt( MAGIC_NUMBER ); // Magic number
        out.writeByte( 1 ); // Version
        out.writeInt( clusterSize );
        out.writeLong( size );

        out.flush();
        final byte[] data = baos.toByteArray();

        try
        {
            rs.setRecord( 1, data, 0, data.length );
        }
        catch( InvalidRecordIDException e )
        {
            rs.addRecord( data, 0, data.length );
        }
    }

    public void write( int i )
            throws IOException
    {
        size++;
        current[ pos++ ] = (byte) i;
        if( pos == clusterSize )
        {
            doFlush();
        }
    }

    public void flush()
            throws IOException
    {
    }
}
