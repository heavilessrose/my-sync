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
/*
 * File Information
 *
 * Created on       : 26-abr-2007 2:05:23
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * ErrorHandler handles errors for non-visual classes and static initializers,
 * so important errors can always be delivered to the user if needed and
 * in a comprehensive way.<br/>
 * The default implementation just calls the ApplicationManager showError
 * methods. If you want to provide your own implementation, just subclass
 * ErrorHandler, override the handle methods, and set the system wide
 * error handler to an instance of your class by calling setErrorHandler static
 * method.<br/>
 * If you set the handler to null, no handling will be done and errors will be
 * silently ignored.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class ErrorHandler
{
    private static ErrorHandler handler = new ErrorHandler();

    /**
     * Get the current system wide error handler.
     *
     * @return error handler
     */
    public static ErrorHandler getHandler()
    {
        return handler;
    }

    /**
     * Set the current system wide error handler.
     *
     * @param handler new handler, if null errors will be ignored
     */
    public static synchronized void setHandler( final ErrorHandler handler )
    {
        ErrorHandler.handler = handler;
    }

    /**
     * Handle an error.
     *
     * @param source error source
     * @param e error
     */
    public static void handleError( final Object source, final Exception e )
    {
        if( handler != null )
        {
            handler.handle( source, e );
        }
    }

    /**
     * Handle an error.
     *
     * @param source error source
     * @param message additional message
     * @param e error
     */
    public static void handleError(
            final Object    source,
            final String    message,
            final Exception e )
    {
        if( handler != null )
        {
            handler.handle( source, message, e );
        }
    }

    /**
     * Do the actual error handling. Subclasses should override this method.
     *
     * @param source error source
     * @param e error
     */
    protected void handle( final Object source, final Exception e )
    {
        Application.getManager().showError( e );
    }

    /**
     * Do the actual error handling. Subclasses should override this method.
     *
     * @param source error source
     * @param message additional message
     * @param e error
     */
    protected void handle(
            final Object    source,
            final String    message,
            final Exception e
    )
    {
        Application.getManager().showError( message, e );
    }
}
