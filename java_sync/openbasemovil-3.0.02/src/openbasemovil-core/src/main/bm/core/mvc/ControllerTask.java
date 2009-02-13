package bm.core.mvc;
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
import bm.core.Application;
import bm.core.ControlledTask;
import bm.core.HelpAssisted;
import bm.core.log.Log;
import bm.core.log.LogFactory;
/*
 * File Information
 *
 * Created on       : 24-ago-2007 11:30:15
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * The task that actually performs the actions in response to the user
 * interaction.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
class ControllerTask
    extends ControlledTask
{
    private static final Log log = LogFactory.getLog( "ControllerTask" );

    protected AbstractController controller;
    protected ControllerEvent    event;
    protected ControllerTask     previous;

    protected ControllerTask(
            final AbstractController    controller,
            final ControllerEvent       event,
            final ControllerTask        previous
    )
    {
        super( "ControllerTask", false );
        this.controller = controller;
        this.event = event;
        this.previous = previous;
    }

    /**
     * If the maximum time for the thread to cancel is reached, die is called
     * to try to stop it. The subclass should do whatever is possible to
     * end its execution, for example, if it's blocked in an InputStream for
     * an external peripheral, it could try to close the stream so an exception
     * is trhown at the blocking read method.
     */
    public void die()
    {
    }

    /**
     * Perform the task.
     */
    protected void task()
    {
        if( previous != null && previous.isAlive() )
        {
            try
            {
                previous.join();
            }
            catch( InterruptedException e )
            {
                log.error( e );
            }
        }
        try
        {
            if(
                    event.getCode() == ControllerEvent.HELP &&
                    event.getSource() instanceof HelpAssisted
                    )
            {
                HelpView.showHelp( (HelpAssisted) event.getSource() );
            }
            else
            {
                controller.process( event );
            }
        }
        catch( Exception e )
        {
            log.error( e );
            Application.getManager().showError( e );
            AbstractController.hideProgressView();
        }
    }
}
