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

/**
 * Abstract class for CommPorts.<br/>
 * Default configuration for ports:<br>
 * <ul>
 *  <li>baud rate: 19200</li>
 *  <li>binary: true</li>
 *  <li>parity: PARITY_NONE</li>
 *  <li>XON/XOFF: false</li>
 *  <li>stop bits: STOPBITS_ONE</li>
 * </ul>
 */
public abstract class AbstractCommPort
        implements CommPort
{
    protected String  name;
    protected int     baudRate    = 19200;
    protected boolean binary      = true;
    protected byte    parity      = PARITY_NONE;
    protected boolean xonXoff;
    protected byte    stopBits    = STOPBITS_ONE;

    public String getName()
    {
        return name;
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public int getBaudRate()
    {
        return baudRate;
    }

    public void setBaudRate( final int baudRate )
    {
        this.baudRate = baudRate;
    }

    public boolean isBinary()
    {
        return binary;
    }

    public void setBinary( final boolean binary )
    {
        this.binary = binary;
    }

    public byte getParity()
    {
        return parity;
    }

    public void setParity( final byte parity )
    {
        this.parity = parity;
    }

    public boolean isXonXoff()
    {
        return xonXoff;
    }

    public void setXonXoff( final boolean xonXoff )
    {
        this.xonXoff = xonXoff;
    }

    public byte getStopBits()
    {
        return stopBits;
    }

    public void setStopBits( final byte stopBits )
    {
        this.stopBits = stopBits;
    }
}
