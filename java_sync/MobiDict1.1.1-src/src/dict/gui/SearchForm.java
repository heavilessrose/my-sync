/*
 * Created on 12-Jun-2005
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

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.ItemStateListener;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;

import dict.MobiDict;
import dict.search.Callback;
import dict.search.SearchThread;
import dict.util.AppSettings;
import dict.util.Global;
import dict.util.Strings;

/**
 * Search form contains the main user functionality
 * @author Georgi R. Georgiev
 */
public class SearchForm extends Form implements ItemStateListener, CommandListener, Callback	{
    
    private final TextField searchField;
    private final StringItem resultItem;
    
    private final StringItem statusItem;
    
    // @PMD:REVIEWED:SingularField: by Administrator on 18/02/06 00:52
    private String prevSearchWord = "";
    
    private final Command cmdSettings = new Command(Strings.SETTINGS, Command.ITEM, 2);
    private final Command onlineSearch = new Command(Strings.ONLINE_SEARCH, Command.ITEM, 1);

    /**
     * Create the search form
     * Add invisible items to the form 
     */
    public SearchForm() {
        super(Global.appName);

        this.searchField = new TextField(Strings.TYPE_WORD, "",
                Global.TEXT_FIELD_CHARS, TextField.ANY);
        append(this.searchField);
        
        this.statusItem = new StringItem("", "");
        append(this.statusItem);
        
        this.resultItem = new StringItem("", "");
        append(this.resultItem);
        
        addCommand(new Command(Strings.ABOUT, Command.HELP, 1));
        addCommand(new Command(Strings.EXIT, Command.EXIT, 1));
        addCommand(this.cmdSettings);
        addCommand(this.onlineSearch);
        
        setItemStateListener(this);
        setCommandListener(this);
    }

    /**
     * Only searchField can have itemStateChanged notification
     * Send a signal to the search worker thread
     * @param item
     */
    public void itemStateChanged(final Item item) {
        String searchWord = this.searchField.getString();
        if (AppSettings.getSettings().useEnglishInput())
            searchWord = Strings.replaceUmlauts(searchWord);
        
        if (!this.prevSearchWord.startsWith(searchWord)) {
            this.statusItem.setText(Strings.SEARCHING_FOR + ' ' + searchWord
                    + '\n');

            SearchThread.getSearchThread().searchFor(searchWord,
                    this.resultItem, this.statusItem);
        }
        this.prevSearchWord = searchWord;
    }

    /**
     * Handle menu commands
     */
    public void commandAction(final Command c, final Displayable d) {
        switch (c.getCommandType()) {
        case Command.HELP:
            final AboutForm aboutForm = new AboutForm();
            DisplayHandler.setCurrent(aboutForm);
            break;
        case Command.EXIT:
            MobiDict.getInstance().notifyDestroyed();
            break;
        case Command.ITEM:
            if (c == this.cmdSettings) {
                final SettingsForm settingsForm = new SettingsForm();
                DisplayHandler.setCurrent(settingsForm);
            }
            else if (c == this.onlineSearch) {
                DisplayHandler.setCurrent(new OnlineSearchForm(this.searchField
                            .getString()));
            }
        default:
        }        
        
    }

    public void reportAlert(Alert alert) {
        DisplayHandler.setCurrent(alert);
        
    }

    public void reportEndTask() {
        //Do nothing, search task is always present
    }

}
