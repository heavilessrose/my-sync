package com.elondra.rp.binary;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ResourcePacker, packs and checks resource files
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

import bm.storage.Store;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.storage.InvalidRecordIDException;
import bm.db.index.Index;
import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import bm.core.zlib.ZOutputStream;
import bm.core.zlib.ZStream;

import java.util.Map;
import java.util.Iterator;
import java.io.File;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.elondra.rp.ResourceWriter;
import com.elondra.rp.PackException;
import com.elondra.rp.Properties;

/*
 * File Information
 *
 * Created on       : 16-ene-2008 21:06:52
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Writes consolidated properties in binary format.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class BinaryWriter
        implements ResourceWriter
{
    private String out;
    private int order;

    public String getOut()
    {
        return out;
    }

    public void setOut( final String out )
    {
        this.out = out;
    }

    public int getOrder()
    {
        return order;
    }

    public void setOrder( final int order )
    {
        this.order = order;
    }

    public void write( final Map consolidated )
            throws PackException
    {
        System.out.println( "Writing resources to index file: " + out + ".index" );
        final File file = new File( out + ".index" );
        final Store store = Store.get( out, 1 );
        final Index index = new Index( out, order, Index.KT_STRING, true, false );
        index.setRecordStore( store );
        OutputStream os = null;
        try
        {
            if( !file.getParentFile().exists() )
            {
                file.getParentFile().mkdirs();
            }
            os = new FileOutputStream( file );
            for( final Iterator i = consolidated.keySet().iterator(); i.hasNext(); )
            {
                final String language = (String) i.next();
                final Properties lang = (Properties) consolidated.get( language );
                System.out.println( "Processing language: " + language + ", " + lang.size() + " entries" );
                for( final Iterator j = lang.keySet().iterator(); j.hasNext(); )
                {
                    final String key = (String) j.next();
                    final String value = lang.getProperty( key );
                    index.insertObject( language + "#" + key, value );
                }
            }
            System.out.println( "Index size: " + store.getSize() + " bytes" );
            writeIndex( index, os );
        }
        catch( Exception e )
        {
            throw new PackException( e );
        }
        finally
        {
            if( os != null ) try{ os.close(); }catch( Exception e ){}
            System.out.println( "Compressed size: " + file.length() + " bytes" );
        }
    }

    private void writeIndex(
            final Index        index,
            final OutputStream os
    )
            throws IOException,
                   RSException,
                   SerializationException
    {
        System.out.println( "Writing index" );
        final ZOutputStream zos = new ZOutputStream(
                os,
                ZStream.Z_BEST_COMPRESSION
        );
        final SerializerOutputStream out = new SerializerOutputStream( zos );

        final int numRecords;
        try
        {
            index.getRecordStore().open();
        }
        catch( RecordStoreFullException e )
        {
            throw new RSException( 0, e );
        }
        final Store rs = index.getRecordStore();
        out.writeInt( index.getOrder() );
        out.writeByte( (byte) index.getType() );
        out.writeBoolean( index.isCaseSensitive() );
        numRecords = rs.getNumRecords();
        System.out.println( "Writing " + numRecords + " records" );
        out.writeInt( numRecords );
        for( int i = 1; i <= numRecords; i++ )
        {
            try
            {
                out.writeBlob( rs.getRecord( i ) );
            }
            catch( InvalidRecordIDException e )
            {
            }
        }
        out.flush();
        zos.flush();
        zos.close();
        System.out.println( "Done" );
    }

}
