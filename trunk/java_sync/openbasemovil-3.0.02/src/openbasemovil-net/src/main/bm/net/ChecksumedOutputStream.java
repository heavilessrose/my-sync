package bm.net;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Network Library, provides an abstraction layer for simplified
    network access over different channels.
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

import java.io.OutputStream;
import java.io.IOException;

import bm.core.tools.Adler32;
/*
 * File Information
 *
 * Created on       : 26-nov-2006 12:11:12
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * This output stream performs an Adler32 checksum over it's contents, and also
 * calculates the size in bytes of the data written.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class ChecksumedOutputStream
    extends OutputStream
{
    private OutputStream    out;
    private Adler32         crc = new Adler32();
    private long            size;

    public ChecksumedOutputStream( final OutputStream out )
    {
        this.out = out;
    }

    public long getCRC()
    {
        return crc.getValue();
    }

    public long getSize()
    {
        return size;
    }

    public void write( int i )
            throws IOException
    {
        crc.update( i );
        out.write( i );
        size++;
    }

    public void write( byte[] bytes )
            throws IOException
    {
        crc.update( bytes );
        out.write( bytes );
        size += bytes.length;
    }

    public void write( byte[] bytes, int offset, int length )
            throws IOException
    {
        crc.update( bytes, offset, length );
        out.write( bytes, offset, length );
        size += length;
    }

    public void close()
            throws IOException
    {
        out.close();
    }
}
