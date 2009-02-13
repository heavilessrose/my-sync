/*
 * Copyright (c) 2006 Elondra S.L. All Rights Reserved.
 */
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
import bm.core.event.Event;
import bm.core.event.EventListener;
import bm.core.event.ProgressEvent;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.Command;

/*
 * File Information
 *
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Abstract controller implementation.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public abstract class AbstractController
	implements 	Controller
{
    protected ControllerTask    lastTask;

    private Log log = LogFactory.getLog( "AbstractController" );

    /**
     * Handle an event.
     *
     * @param event event to handle
     */
    public void handle( final ControllerEvent event)
    {
        synchronized( this )
        {
            final ControllerTask task = new ControllerTask( this, event, lastTask );
            lastTask = task;
            task.start();
        }
    }

    /**
     * Utility method that hides a possibly hanging progress view trying to
     * show the previous view or the main view if not.
     */
    public static void hideProgressView()
    {
        final View currentView = AbstractView.getCurrent();
        View target = Application.getManager().getMainView();
        if( currentView != null )
        {
            if(
                    currentView instanceof EventListener &&
                    Event.isRegistered(
                            ProgressEvent.PROGRESS,
                            (EventListener) currentView
                    )
            )
            {
                if( currentView.getCaller() != null )
                {
                    target = currentView.getCaller();
                }
            }
            else
            {
                target = currentView;
            }
        }
        target.show();
    }

    /**
     * Short cut that goes back to a view caller and then releases the view
     * resources.
     *
     * @param view source view
     */
    protected void goBack( final View view )
    {
        if( view.getCaller() != null )
        {
            view.getCaller().show();
            view.release();
        }
    }

    /**
     * Default empty implementation.
     *
     * @param view view released
     */
    public void onRelease( final View view )
    {
    }

    /**
     * Process an event. Subclasesses must implement this method to handle
     * events.
     *
     * @param event event to handle
     */
    protected abstract void process( ControllerEvent event );


    // ------------------------------------------------------------------------
    // Utility methods

    /**
     * Perform command action.
     *
     * @param parent parent view
     * @param command command
     */
    public static void commandAction(
            final View          parent,
            final Command       command
    )
    {
        final ControllerEvent event = buildEvent( parent, command, null );
        parent.getController().handle( event );
    }

    /**
     * Perform command action.
     *
     * @param parent parent view
     * @param command command
     * @param choice choice
     */
    public static void commandAction(
            final View          parent,
            final Command       command,
            final Choice        choice
    )
    {
        final ControllerEvent event = buildEvent( parent, command, choice );
        parent.getController().handle( event );
    }

    /**
     * Just create an event, without handling it.<br/>
     * Handles default controller actions and choice selection.
     *
     * @param parent parent view
     * @param command command
     * @return event
     */
    public static ControllerEvent buildEvent(
            final View      parent,
            final Command   command
    )
    {
        return buildEvent( parent, command, null );
    }

    /**
     * Just create an event, without handling it.<br/>
     * Handles default controller actions and choice selection.
     *
     * @param parent parent view
     * @param command command
     * @param choice choice
     * @return event
     */
    public static ControllerEvent buildEvent(
            final View      parent,
            final Command   command,
            final Choice    choice
    )
    {
        final ControllerEvent event = new ControllerEvent( parent );
        switch( command.getCommandType() )
        {
            case Command.BACK:
                event.setCode( ControllerEvent.BACK );
                break;

            case Command.CANCEL:
                event.setCode( ControllerEvent.CANCEL );
                break;

            case Command.EXIT:
                event.setCode( ControllerEvent.EXIT );
                break;

            case Command.HELP:
                event.setCode( ControllerEvent.HELP );
                break;

            case Command.OK:
                event.setCode( ControllerEvent.ACCEPT );
                break;

            case Command.STOP:
                event.setCode( ControllerEvent.STOP );
                break;

            default:
                switch( command.getPriority() )
                {
                    case ControllerEvent.HOME:
                        event.setCode( ControllerEvent.HOME );
                        break;

                    default:
                        if( choice != null )
                        {
                            event.setCode( ControllerEvent.LIST_ITEM );
                            event.setIndex( choice.getSelectedIndex() );
                        }
                }
                break;
        }
        return event;
    }
}
