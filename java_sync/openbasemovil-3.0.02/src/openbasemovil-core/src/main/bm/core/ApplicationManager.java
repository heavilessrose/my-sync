/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
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

import bm.core.mvc.View;

import javax.microedition.lcdui.Display;

/*
 * File Information
 *
 * Created on       : 12-dic-2005 17:31:24
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * The Application Manager handles calls that are used through out the application
 * but wich implementation is dependent on the underlying system.<br/>
 * Normal implementations will be CLDC and CDC application managers.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface ApplicationManager
{
    /**
     * Exit the application.
     */
    void exitApplication();

    /**
     * Ask user if he/she want's to exit application and call exitApplication
     * if he/she answers yes, do nothing if not.
     */
    void mildExitApplication();

    /**
     * Get the main view for this program.
     *
     * @return main view
     */
    View getMainView();

    /**
     * Set the main view for this program.
     * @param mainView the new main view
     */
    void setMainView( View mainView );

    /**
     * If the application gets locked in a progress view there can be a method
     * for restoring the best view for the application in its current state.
     * @return the best view to show in the current application state.
     */
    View getBestView();

    /**
     * Get the display associated to this application.
     *
     * @return display
     */
    Display getDisplay();

    /**
     * Utility mehtod that shows a warning alert.
     *
     * @param message message to show
     */
    void showWarning( String message );

    /**
     * Utility mehtod that shows an information alert.
     *
     * @param message message to show
     */
    void showInfo( String message );

    /**
     * Utility mehtod that shows an error alert.
     *
     * @param message message to show
     */
    void showError( String message );

    /**
     * Utility mehtod that shows an error alert.
     *
     * @param message message to show
     * @param e exception to show
     */
    void showError( String message, Exception e );

    /**
     * Utility mehtod that shows an error alert.
     *
     * @param e exception to show
     */
    void showError( Exception e );

    /**
     * Utitliy method that displays a confirmation alert.<br/>
     * It shows a modal alert and waits until the user presses yes o no, then
     * the method returns.<br/>In fact, this is a short cut for Dialog class.
     *
     * @param title alert title
     * @param message alert message
     * @return true if the user pressed yes, false if the user pressed no
     */
    boolean confirm( String title, String message );

    /**
     * Display a modal dialog that allows the user to select one of the given
     * options.
     *
     * @param title dialog title
     * @param options options array
     * @return index of the option selected
     */
    int select( String title, String[] options );

    /**
     * Display a modal dialog that allows the user to select one of the given
     * options.
     *
     * @param title dialog title
     * @param options options array
     * @param ticker ticker text
     *
     * @return index of the option selected
     */
    int select(
            String    title,
            String[]  options,
            String    ticker
    );

    /**
     * Ask the user for a text input.
     *
     * @param title title for window
     * @return value entered by the user, null if the user cancelled edition
     */
    String prompt( String title );

    /**
     * Ask the user for a text input.
     *
     * @param title title for window
     * @param maxSize maximum size for text
     * @return value entered by the user, null if the user cancelled edition
     */
    String prompt( String title, int maxSize );

    /**
     * Ask the user for a text input.
     *
     * @param title title for window
     * @param maxSize maximum size for text
     * @param constraints as specified in MIDP TextField
     * @return value entered by the user, null if the user cancelled edition
     */
    String prompt( String title, int maxSize, int constraints );

    /**
     * Ask the user for a password.
     *
     * @param title title for window
     * @return value entered by the user, null if the user cancelled edition
     */
    String promptPassword( String title );

    /**
     * Ask the user for a password.
     *
     * @param title title for window
     * @param maxSize maximum size for text
     * @return value entered by the user, null if the user cancelled edition
     */
    String promptPassword( String title, int maxSize );

    /**
     * Get a configuration property.
     *
     * @param key key
     * @return value or null if not found
     */
    String getProperty( String key );

    /**
     * Get a configuration property.
     *
     * @param key key
     * @param defaultValue default value if not found
     * @return value or defaultValue if not found
     */
    String getProperty( String key, String defaultValue );

    /**
     * Get a configuration property as an Integer.
     *
     * @param key key
     * @return value or null if not found or invalid number
     */
    Integer getPropertyInt( String key );

    /**
     * Get a configuration property as an int.
     *
     * @param key key
     * @param defaultValue default value if null or invalid
     * @return value or defaultValue
     */
    int getPropertyInt( String key, int defaultValue );

    /**
     * Get a configuration property as Long.
     *
     * @param key key
     * @return vallue or null if not found or invalid number
     */
    Long getPropertyLong( String key );

    /**
     * Get a configuration property as a long.
     *
     * @param key key
     * @param defaultValue default value if not found or invalid number
     * @return value or defaultValue
     */
    long getPropertyLong( String key, long defaultValue );

    /**
     * Get a configuration property as Boolean.
     *
     * @param key key
     * @return vallue or null if not found
     */
    Boolean getPropertyBoolean( String key );

    /**
     * Get a configuration property as a boolean.
     *
     * @param key key
     * @param defaultValue default value if not found
     * @return value or defaultValue
     */
    boolean getPropertyBoolean( String key, boolean defaultValue );

    /**
     * Check if the application is running in a MIDP environment, and thus,
     * resources are more constrained than in CDC or standard Java.
     * @return true if is MIDP, false otherwise
     */
    boolean isMIDP();

    /**
     * Lookup an object given it's url.<br/>
     * This is useful for system dependent resources, such as printers,etc.<br/>
     * For example, to retrieve a configured default printer:<br/>
     * <code>Application.getManager().lookUp( "printer://default" );
     * @return requested object or null if not available.
     */
    Object lookUp( String url );
}
