package bm.comm;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Comm Library, an abstraction layer over COM ports
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

import java.io.IOException;

/**
 * Native adapter library for using comm ports under Windows CE/Mobile.<br/>
 * Default configuration for ports:<br>
 * <ul>
 *  <li>name: COM1:</li>
 *  <li>baud rate: 19200</li>
 *  <li>binary: true</li>
 *  <li>parity: PARITY_NONE</li>
 *  <li>XON/XOFF: false</li>
 *  <li>stop bits: STOPBITS_ONE</li>
 * </ul>
 */
public class WindowsCommPort
        extends AbstractCommPort
{
    private static final int BUFFER_SIZE = 16 * 1024;

    static
    {
        System.loadLibrary( "CommJava" );
        System.out.println( "Loaded library CommJava" );
    }

    private long    descriptor  = -1;
    private byte[]  buffer = new byte[ BUFFER_SIZE ];
    private int     count = 0;

    public WindowsCommPort()
    {
        name = "COM1:";
    }

    /**
     * Create a comm port for the given por name.<br/>
     * The port name must take the form <code>COM<i>X</i>:</code>, where X
     * is substituted by a number, for example: <code>COM1:</code>.
     * @param name com port name
     */
    public WindowsCommPort( final String name)
    {
        this.name = name;
    }

    /**
     * Create a comm port for the given por name.<br/>
     * The port name must take the form <code>COM<i>X</i>:</code>, where X
     * is substituted by a number, for example: <code>COM1:</code>.
     * @param name com port name
     * @param baudRate the baud rate
     */
    public WindowsCommPort( final String name, final int baudRate )
    {
        this.name       = name;
        this.baudRate   = baudRate;
    }

    /**
     * Open the com port.
     * @throws IOException if can't be opened
     */
    public void open()
            throws IOException
    {
        if( descriptor == -1 )
        {
            descriptor = open( name );
            if( descriptor == -1 )
            {
                throw new IOException( "Can't open port: " + name );
            }
            setConfiguration();
        }
    }

    /**
     * Close the com port.
     */
    public void close()
    {
        if( descriptor != -1 )
        {
            try
            {
                flush();
            }
            catch( IOException e )
            {
            }
            close( descriptor );
            descriptor = -1;
        }
    }

    /**
     * Write a single character to the com port. Does nothing if port not open.
     * @param c character to write
     * @throws IOException on errors
     */
    public void write( final byte c )
            throws IOException
    {
        if( descriptor != -1 )
        {
            if( count >= BUFFER_SIZE )
            {
                flush();
            }
            buffer[count++] = c;
//            if( !write( descriptor, c ) )
//            {
//                throw new IOException();
//            }
        }
    }

    /**
     * Write a set of bytes to the com port.
     * @param data data to write
     * @param offset offset
     * @param length length
     * @return number of bytes written, 0 if the port is closed
     * @throws IOException on errors
     */
    public int write( final byte[] data, final int offset, final int length )
            throws IOException
    {
        if( data != null )
        {
            int count = 0;
            int i = offset;
            final int maxLength = data.length;
            while( count < length && i < maxLength )
            {
                write( data[i] );
                i++;
                count++;
            }
            return count;
        }
        else
        {
            return 0;
        }
    }

    /**
     * Write a set of bytes to the com port.
     * @param data data to write
     * @return number of bytes written, 0 if the port is closed
     * @throws IOException on errors
     */
    public int write( final byte[] data )
            throws IOException
    {
        if( data != null )
        {
            return write( data, 0, data.length );
        }
        else
        {
            return 0;
        }
    }

    /**
     * Send the configuration settings to the com port.
     * @return true if configuration was successful
     */
    public boolean setConfiguration()
    {
        if( descriptor != -1 )
        {
            return configure(
                    descriptor,
                    baudRate,
                    binary,
                    parity,
                    xonXoff,
                    stopBits
            ) == 1;
        }
        else
        {
            return false;
        }
    }

    private void flush()
        throws IOException
    {
        if( buffer != null )
        {
            for( int i = 0; i < count; i++ )
            {
                if( !write( descriptor, buffer[i] ) )
                {
                    throw new IOException();
                }
            }
            count = 0;
        }
    }

    private native int configure(
            final long      handle,
            final int       baudRate,
            final boolean   binary,
            final byte      parity,
            final boolean   xonXoff,
            final byte      stopBits
    );

    private native long open( final String name );

    private native void close( final long handle );

    private native boolean write( final long handle, final byte c );
}
