/*
 * Created on 19-Oct-2005
 * 
 * Author: Georgi R. Georgiev
 * Copyright: Georgi R. Georgiev
 * *     This file is part of MobiDict

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

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;

import dict.search.OnlineSearchTask;
import dict.util.Global;
import dict.util.Strings;

/**
 * 
 * @author Georgi R. Georgiev
 */
public class AboutForm extends Form implements CommandListener {
    
    private static final String NAME_PART = ": Georgi R. Georgiev\n";
    
    /**
     * Add the strings and commands
     *
     */
    public AboutForm() {
        super(Strings.ABOUT);

        append(Global.appName + ' ' + Strings.VERSION + ": "
                + Global.appVersion + '\n');
        append(Strings.ENGLISH_GERMAN + '\n');
        append(Strings.AUTHOR + NAME_PART);
        append(Strings.COPYRIGHT + NAME_PART);
        append(Strings.MAIL + ": geo.georgi@gmail.com\n\n");
        append(Strings.LICENSE + '\n');
        append("University Kaiserslautern\n");
        append("Research Group Integrated Communication Systems (ICSY)\n");
        append("www.icsy.de\n");
        append(Strings.PROJ_TUTOR + ": Jochen Müller\n\n");

        append(Strings.ONLINE_SEARCH_DISCL + OnlineSearchTask.TRANSL_URL + '\n');
        

        addCommand(new Command(Strings.BACK, Command.BACK, 1));

        setCommandListener(this);
    }

    /**
     * Go back to the search form
     */
    public void commandAction(final Command c, final Displayable d) {
        DisplayHandler.showSearchForm(false);
    }
    

}
