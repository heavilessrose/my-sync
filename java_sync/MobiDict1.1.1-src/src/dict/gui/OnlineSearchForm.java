/*
 * Created on 09-Mar-2006
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

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.ItemStateListener;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.Ticker;

import dict.search.Callback;
import dict.search.OnlineSearchTask;
import dict.util.AppSettings;
import dict.util.Global;
import dict.util.Strings;

/**
 * Form for online search
 * @author Georgi R. Georgiev
 */
public class OnlineSearchForm extends Form implements CommandListener, ItemStateListener, Callback {
    
    private final TextField searchField;
    private final StringItem resultField;
    private final ChoiceGroup langChoice;
    private static Ticker refreshTicker;
    private static OnlineSearchTask searchTask;

    /**
     * Creates new online search form
     * @param word string to be shown in the search field
     */
    public OnlineSearchForm(String word) {
        super(Global.appName);
        
        addCommand(new Command(Strings.BACK, Command.BACK, 1));
        addCommand(new Command(Strings.TRANSLATION, Command.OK, 1));
        
        this.searchField = new TextField(Strings.ONLINE_SEARCH, word,
                Global.TEXT_FIELD_CHARS, TextField.ANY);
        append(this.searchField);
        
        this.resultField = new StringItem("", "");
        append(this.resultField);
        
        this.langChoice = new ChoiceGroup("", Choice.EXCLUSIVE);
        this.langChoice.append(Strings.ENGLISH_TO_GERMAN, null);
        this.langChoice.append(Strings.GERMAN_TO_ENGLISH, null);
        this.langChoice.setSelectedIndex(AppSettings.getSettings()
                .getOnlineTransl(), true);
        append(this.langChoice);
        
        setCommandListener(this);
        setItemStateListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getCommandType() == Command.OK) {
            refreshTicker = new Ticker(Strings.SEARCH);
            final int lang = this.langChoice.getSelectedIndex();
            // no need to replace umlauts, google service does the job
            searchTask = new OnlineSearchTask(this.searchField.getString(),
                    refreshTicker, this.resultField, lang, this);
            setTicker(refreshTicker);
            final Thread t = new Thread(searchTask);
            t.start();
        }
        else {// c == Command.BACK
            if (searchTask != null) {
                searchTask.deactivate();// just in case he is not done
            }
            DisplayHandler.showSearchForm(false);
        }
    }

    /**
     * Save the language translation way.
     * That way the user sees his last translation choice when he is
     * back to the form
     * @param item
     */
    public void itemStateChanged(Item item) {
        AppSettings.getSettings().setOnlineTransl(
                (byte) this.langChoice.getSelectedIndex());
    }

    public void reportAlert(Alert alert) {
        DisplayHandler.setCurrent(alert);
    }

    public void reportEndTask() {
        setTicker(null);
    }

}
