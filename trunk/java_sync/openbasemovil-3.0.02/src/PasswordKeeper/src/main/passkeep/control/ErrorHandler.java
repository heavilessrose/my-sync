package passkeep.control;
/* -----------------------------------------------------------------------------
    AppTemplate, an application for building defect management built with BaseMovil
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

import bm.err.ErrorLog;
import bm.mvc.ViewFactory;
/*
 * File Information
 *
 * Created on       : 27-abr-2007 0:09:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Custom ErrorHandler.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ErrorHandler
    extends bm.core.ErrorHandler
{
    /**
     * Do the actual error handling. Subclasses should override this method.
     *
     * @param source error source
     * @param e      error
     */
    protected void handle( final Object source, final Exception e )
    {
        ErrorLog.addError(
                "ErrorHandler",
                "handle",
                new Object[] { source },
                null,
                e
        );
        ((MainController) ViewFactory.getController( "main" ))
                .handleError( source, e, null );
    }

    /**
     * Do the actual error handling. Subclasses should override this method.
     *
     * @param source  error source
     * @param message additional message
     * @param e       error
     */
    protected void handle(
            final Object source,
            final String message,
            final Exception e
    )
    {
        ErrorLog.addError(
                "ErrorHandler",
                "handle",
                new Object[] { source },
                message,
                e
        );
        ((MainController) ViewFactory.getController( "main" ))
                .handleError( source, e, message );
    }
}
