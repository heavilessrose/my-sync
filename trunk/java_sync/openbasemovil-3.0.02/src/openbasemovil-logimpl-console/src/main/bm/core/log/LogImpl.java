/*
 * Copyright (c) 2005 Your Corporation. All Rights Reserved.
 */
package bm.core.log;
/* -----------------------------------------------------------------------------
    OpenBaseMovil LogImpl-Console Library, console implementation of logging
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
 * Default log implementation, debug version.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 1.2 $
 */
public class LogImpl
    implements Log,
               LogConfigurator
{
    private static final String INFO    = "[INFO ]";
    private static final String WARN    = "[WARN ]";
    private static final String DEBUG   = "[DEBUG]";
    private static final String ERROR   = "[ERROR]";
    private static final String FATAL   = "[FATAL]";

    private String shortName;

    /**
     * Set the name (class or package) that this instance represents.
     *
     * @param name instance name
     */
    public void setName( final String name )
    {
        shortName = name;
    }

    /**
     * Log an information message.
     *
     * @param message message to log
     */
    public void info( final String message)
    {
        doLog( INFO, message, null );
    }

    /**
     * Log a debug message.
     *
     * @param message message to log
     */
    public void debug( final String message)
    {
        doLog( DEBUG, message, null );
    }

    /**
     * Log a debug message.
     *
     * @param message message to log
     * @param e exception to show
     */
    public void debug( final String message, final Throwable e)
    {
        doLog( DEBUG, message, e );
    }

    /**
     * Log a warning message.
     *
     * @param message message to log
     */
    public void warn( final String message)
    {
        doLog( WARN, message, null );
    }

    /**
     * Log a warning message.
     *
     * @param message message to log
     * @param e exception to show
     */
    public void warn( final String message, final Throwable e)
    {
        doLog( WARN, message, e );
    }

    /**
     * Log an error message.
     *
     * @param message message to log
     */
    public void error( final String message)
    {
        doLog( ERROR, message, null );
    }

    /**
     * Log an error message.
     *
     * @param message message to log
     * @param e exception to show
     */
    public void error( final String message, final Throwable e)
    {
        doLog( ERROR, message, e );
    }

    /**
     * Log a debug message.
     *
     * @param e throwable
     */
    public void debug( final Throwable e )
    {
        doLog( DEBUG, "", e );
    }

    /**
     * Log a warning message.
     *
     * @param e throwable
     */
    public void warn( final Throwable e )
    {
        doLog( WARN, "", e );
    }

    /**
     * Log an error message.
     *
     * @param e throwable
     */
    public void error( final Throwable e )
    {
        doLog( ERROR, "", e );
    }

    /**
     * Log a fatal error message.
     *
     * @param message message to log
     */
    public void fatal( final String message )
    {
        doLog( FATAL, message, null );
    }

    /**
     * Log a fatal error message.
     *
     * @param message message to log
     * @param e throwable associated
     */
    public void fatal( final String message, final Throwable e )
    {
        doLog( FATAL, message, e );
    }

    /**
     * Log a fatal error message.
     *
     * @param e throwable
     */
    public void fatal( final Throwable e )
    {
        doLog( FATAL, "", e );
    }

    public String getContent()
    {
        return null;
    }

    public void clear()
    {
    }

    private void doLog(
            final String    level,
            final String    message,
            final Throwable e
    )
    {
        final StringBuffer msg = new StringBuffer();
        msg.append( "[" ).append( shortName ).append( "] " )
                .append( level ).append( " " )
                .append( message );
        System.out.println( msg.toString() );
        if( e != null )
        {
            e.printStackTrace();
        }
    }
}
