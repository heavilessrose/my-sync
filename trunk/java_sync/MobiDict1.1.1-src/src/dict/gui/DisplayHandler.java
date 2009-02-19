/*
 * Created on 29-Mar-2006
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
package dict.gui;

import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Ticker;

import dict.search.SearchThread;

/**
 * @author Georgi R. Georgiev
 * This component handles all request to the Display object.
 */
public class DisplayHandler {
    
    private static Display display;
    private static SearchForm searchForm;
    
    /**
     * Init the handler with the display object. Should be called before methods
     * of the DisplayHandler are called
     * 
     * @param d
     */
    public static void setDisplay(Display d) {
        display = d;
    }
    
    /**
     * Set ticker with help text when the user is waiting for extern operation
     * @param ticker
     */
    public static void setTicker(final Ticker ticker) {
        final Displayable disp = display.getCurrent();
        if (disp instanceof Form)   {
            final Form form = (Form) disp;
            form.setTicker(ticker);
        }
    }
    
    /**
     * Shows the search form on the display.
     * @param createNew when true creates a new instance of the search form
     * when false reuses the old instance
     */
    public static void showSearchForm(final boolean createNew) {
        if (createNew)  {
            searchForm = new SearchForm();
            SearchThread.setCallback(searchForm);
        }
        
        display.setCurrent(searchForm);
    }
    
    /**
     * Convient way to access display.setCurrent
     * @param displ
     */
    public static void setCurrent(final Displayable displ)    {
        display.setCurrent(displ);
    }
}
