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
import bm.core.math.FixedPoint;

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
 * View (see MVC Pattern).
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface View
{
    void setController( Controller controller );
    Controller getController();

    /**
     * Each view must have an id that identifies it.
     * @return view id
     */
    int getId();

    void show();
//    void show( View caller );

    View getCaller();
    void setCaller( View view );

    /**
     * The view is about to hide and we shuld free up resources used by it.
     */
    void release();

    /**
     * Fill in view data from a repository. Useful when you need to persist
     * application/view state.
     */
    void feed();

    /**
     * If the view supports restore. feed should not be called if the view
     * can not be restored.
     * @return if supports restore
     */
    boolean canBeRestored();

    /**
     * Add a command to the view. This method follows the IOC pattern, allowing
     * the container to configure the view by injecting commands into it.
     * @param command the command
     */
    void addCommand( Command command );

    /**
     * Allow removing of commands.
     * @param command command to remove
     */
    void removeCommand( Command command );

    /**
     * Set the view title.
     *
     * @param title title
     */
    void setTitle( String title );

    /**
     * Get the view title.
     * @return title or null if no displayable set
     */
    String getTitle();

    /**
     * Check if the view is showing.
     * @return true if it is
     */
    boolean isShown();

    /**
     * Add an action to the view.<br/>
     * An action is a command associated to the view and to a concrete action
     * code, so the view can directly create and dispatch a controller event.<br/>
     * Actions can be enabled or disabled, which actually means that the command
     * is added or removed from the underlying displayable object.
     *
     * @param action the action
     * @param active if initially active
     */
    void addAction( Action action, boolean active );

    /**
     * Remove an action from the view.
     *
     * @param action action
     */
    void removeAction( Action action );

    /**
     * Get an action.
     *
     * @param name action name
     * @return action or null if not found
     */
    Action getAction( String name );

    /**
     * Check if the action is currently active.
     *
     * @param action action
     * @return true if so
     */
    boolean isActive( Action action );

    /**
     * Activate an action.
     *
     * @param action the action
     */
    void activate( Action action );

    /**
     * Activate an action.
     *
     * @param name action name
     */
    void activate( String name );

    /**
     * Deactivate an action.
     *
     * @param action action
     */
    void deactivate( Action action );

    /**
     * Deactivate an action.
     *
     * @param name action name
     */
    void deactivate( String name );

    /**
     * Attach an temporary obeject to the view. Useful for state management.<br/>
     * If the value is null, this is equivalent to detach( key ).
     *
     * @param key object key
     * @param value object
     */
    void attach( Object key, Object value );

    /**
     * Detach an object.
     *
     * @param key object key
     */
    void detach( Object key );

    /**
     * Detach all attached objects.
     */
    void detachAll();

    /**
     * Find an attached object, either to this view or to one of the previous
     * views in the call chain.
     *
     * @param key object key
     * @return object or null if not attached
     */
    Object findAttachment( Object key );

    /**
     * Get an attached object.
     *
     * @param key object key
     * @return object or null if not attached
     */
    Object getAttachment( Object key );

    /**
     * Get an attachment as a boolean value.
     * @param key key
     * @return boolean value, or false if it's null or not a Boolean
     */
    boolean getBooleanAttachment( Object key );

    /**
     * Get an attachment as a String value.
     * @param key key
     * @return String value, or null if not found
     */
    String getStringAttachment( Object key );

    /**
     * Get an attachment as a FixedPoint value.
     * @param key key
     * @return FixedPoint value, or null if not found or not a FixedPoint
     */
    FixedPoint getFixedPointAttachment( Object key );

    /**
     * Get an attachment as an int value.
     * @param key key
     * @return int value, or -1 if it's null or not an Integer, Long, Short or Byte
     */
    int getIntAttachment( Object key );

    /**
     * Get an attachment as a long value.
     * @param key key
     * @return long value, or -1 if it's null or not a Long, Integer, Short or Byte
     */
    long getLongAttachment( Object key );

    /**
     * Utility method that goes to a view setting it's caller to this view.<br/>
     * Improves code clarity.
     * @param view view
     */
    void go( View view );
}
