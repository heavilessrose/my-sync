/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;
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
 * Log system interface.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface Log
{
    /**
     * Log a debug message.
     *
     * @param message message to log
     */
    void debug( String message );

    /**
     * Log a debug message.
     *
     * @param message message to log
     * @param e throwable associated
     */
    void debug( String message, Throwable e );

    /**
     * Log a debug message.
     *
     * @param e throwable
     */
    void debug( Throwable e );

    /**
     * Log an information message.
     *
     * @param message message to log
     */
    void info( String message );

    /**
     * Log a warning message.
     *
     * @param message message to log
     */
    void warn( final String message );

    /**
     * Log a warning message.
     *
     * @param message message to log
     * @param e throwable associated
     */
    void warn( String message, Throwable e );

    /**
     * Log a warning message.
     *
     * @param e throwable
     */
    void warn( Throwable e );

    /**
     * Log an error message.
     *
     * @param message message to log
     */
    void error( final String message );

    /**
     * Log an error message.
     *
     * @param message message to log
     * @param e throwable associated
     */
    void error( String message, Throwable e );

    /**
     * Log an error message.
     *
     * @param e throwable
     */
    void error( Throwable e );

    /**
     * Log a fatal error message.
     *
     * @param message message to log
     */
    void fatal( final String message );

    /**
     * Log a fatal error message.
     *
     * @param message message to log
     * @param e throwable associated
     */
    void fatal( String message, Throwable e );

    /**
     * Log a fatal error message.
     *
     * @param e throwable
     */
    void fatal( Throwable e );

    /**
     * Get log contents.
     *
     * @return content or null if not supported
     */
    public String getContent();

    /**
     * Clear log contents.
     */
    public void clear();
}
