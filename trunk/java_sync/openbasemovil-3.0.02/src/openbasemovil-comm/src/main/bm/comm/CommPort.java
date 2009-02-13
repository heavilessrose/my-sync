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
 * Proxy interface for COM Port usage.
 */
public interface CommPort
{
    int PARITY_NONE     = 0;
    int PARITY_ODD      = 1;
    int PARITY_EVEN     = 2;
    int PARITY_MARK     = 3;
    int PARITY_SPACE    = 4;

    int STOPBITS_ONE            = 0;
    int STOPBITS_ONE_AND_HALF   = 1;
    int STOPBITS_TWO            = 2;

    String getName();

    void setName( String name );

    int getBaudRate();

    void setBaudRate( int baudRate );

    boolean isBinary();

    void setBinary( boolean binary );

    byte getParity();

    void setParity( byte parity );

    boolean isXonXoff();

    void setXonXoff( boolean xonXoff );

    byte getStopBits();

    void setStopBits( byte stopBits );

    void open()
            throws IOException;

    void close();

    void write( byte c )
            throws IOException;

    int write( byte[] data, int offset, int length )
            throws IOException;

    int write( byte[] data )
            throws IOException;

    boolean setConfiguration();
}
