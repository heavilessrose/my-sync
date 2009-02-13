/*
 * Copyright (c) 2005 Your Corporation. All Rights Reserved.
 */
package bm.core;
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
/**
 * Base J2ME Exception.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class J2MEException
    extends Exception
{
    /**
     * Exception that was the source/cause of this one.
     */
    protected Throwable	source;
    /**
     * Error code used for localization of error messages.<br/>
     * The localized resource will be named &quot;exception.<i>errorCode</i>&quot;
     */
    protected String    errorCode = "ERR_GENERIC";
    /**
     * The error number signals a single point in the software where the
     * exception was thrown.
     */
    protected int       errorNumber;

    public J2MEException( final int errorNumber )
    {
        super();
        this.errorNumber = errorNumber;
    }

    public J2MEException( final int errorNumber, final String string )
    {
        super(string);
        this.errorNumber = errorNumber;
    }

    public J2MEException( final int errorNumber, final Throwable source )
    {
        super( source.getMessage() );
        this.source = source;
        this.errorNumber = errorNumber;
    }

    public J2MEException(
            final int       errorNumber,
            final Throwable source,
            final String    message
    )
    {
        super( message );
        this.source = source;
        this.errorNumber = errorNumber;
    }

    public Throwable getSource()
    {
        return source;
    }

    /**
     * Get the exception message.
     *
     * @return message
     */
    public String getMessage()
    {
        // Local variable to improve speed on CDC
        final Throwable source = this.source;
        if( super.getMessage() != null && super.getMessage().trim().length() > 0 )
        {
            return super.getMessage();
        }
        else if( source != null )
        {
            if( source.getMessage() != null && source.getMessage().trim().length() > 0 )
            {
                return source.getMessage();
            }
            else
            {
                return source.getClass().getName();
            }
        }
        else
        {
            return null;
        }
    }

    /**
     * String representation.
     *
     * @return representation
     */
    public String toString()
    {
        final StringBuffer buffer = new StringBuffer();
        buffer.append( ResourceManager.getResource(
                "exception." + errorCode
        ) )
                .append( "[" )
                .append( errorNumber )
                .append( "] " )
                .append( ": " )
                .append( getMessage() );
        return buffer.toString();
    }

    /**
     * Get a trace of the string.<br/>
     * The method can build a trace while the source exception is a J2MEException
     *
     * @return trace
     */
    public String getTrace()
    {
        final StringBuffer buffer = new StringBuffer();
        buffer.append( toString() );
        // Local variable to improve speed on CDC
        final Throwable source = this.source;
        if( source != null && source instanceof J2MEException )
        {
            buffer.append( "\n" )
                    .append( ((J2MEException) source).getTrace() );
        }
        return buffer.toString();
    }

    /**
     * Get the error code for this exception.
     *
     * @return error code
     */
    public String getErrorCode()
    {
        return errorCode;
    }

    public int getErrorNumber()
    {
        if( source != null && source instanceof J2MEException )
        {
            return ((J2MEException) source).getErrorNumber();
        }
        else
        {
            return errorNumber;
        }
    }
}
