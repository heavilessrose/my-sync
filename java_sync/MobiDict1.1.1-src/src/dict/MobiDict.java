/*
 * Created on 12-Jun-2005
 * 
 * Author: Georgi R. Georgiev
 * Copyright: Georgi R. Georgiev
 *     This file is part of MobiDict

    MobiDict is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MobiDict is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */

package dict;

import java.io.IOException;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import dict.gui.DisplayHandler;
import dict.search.SearchThread;
import dict.util.Global;
import dict.util.Strings;



/**
 * Main class of the application
 * @author Georgi R. Georgiev
 */
public class MobiDict extends MIDlet    {
    
    // @PMD:REVIEWED:AssignmentToNonFinalStatic: by Administrator on 14/02/06 16:43
    private static MobiDict appInstance;
    
    private static boolean init;

    /**
     * Save the current app instance for easy access 
     */
    public MobiDict() {
        super();
        appInstance = this;
        Global.appName = appInstance.getAppProperty("MIDlet-Name");
        Global.appVersion = getAppProperty("MIDlet-Version");
    }

    /**
     * Save the display
     * Read the menu international strings
     */
    public void startApp() throws MIDletStateChangeException {
        SearchThread.startSearch();//start because the thread is stopped when the app is in pause state
        
        if (init)
            return;
        
        final Display display = Display.getDisplay(this);
        
        try {
            Strings.initLangList();
            
            Strings.readMenu();
            DisplayHandler.setDisplay(display);
            DisplayHandler.showSearchForm(true);
            init = true;
        }
        catch (IOException e)   {
            final Alert alert = new Alert("Error", e.toString(), null, AlertType.ERROR);
            display.setCurrent(alert);
        }
        
    }
    
    /**
     * Stop the search process when in background
     */
    public void pauseApp() {
        SearchThread.cancelSearch();
    }

    /**
     * Cancel the search thread to avoid resource bugs on some phones
     * @param unconditional
     */
    public void destroyApp(final boolean unconditional)
            throws MIDletStateChangeException {
        SearchThread.cancelSearch();
    }
    
    /**
     * Return the midlet main instance
     * @return instance of the midlet
     */
    public static MobiDict getInstance() {
        return appInstance;
    }
    
}
