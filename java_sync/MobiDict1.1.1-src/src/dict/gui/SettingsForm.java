/*
 * Created on 10-Jan-2006
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

import java.io.IOException;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;

import dict.util.AppSettings;
import dict.util.Strings;

/**
 * Settings form
 * @author Georgi R. Georgiev
 */
public class SettingsForm extends Form implements CommandListener {
    
    private final TextField resultsCountField;
    private final Command cmdSave, cmdBack;
    private final ChoiceGroup langChoice;
    private final ChoiceGroup englInputChoice;

    /**
     * Init the settings
     *
     */
    public SettingsForm() {
        super(Strings.SETTINGS);
        
        AppSettings settings = AppSettings.getSettings();
        int resultCount = settings.getMaxResultCount();
        
        this.resultsCountField = new TextField(Strings.RESULT_COUNT,
                resultCount + "", 2, TextField.NUMERIC);
        append(this.resultsCountField);
        
        append(" \n");
        
        this.langChoice = new ChoiceGroup(Strings.MENU_LANG, Choice.EXCLUSIVE,
                Strings.getMenuLanguages(), null);
        this.langChoice.setSelectedIndex(settings.getCurrentLangIndex(), true);
        append(this.langChoice);
        
        this.englInputChoice = new ChoiceGroup("", Choice.MULTIPLE);
        this.englInputChoice.append(Strings.USE_ENGL_INPUT, null);
        this.englInputChoice.setSelectedIndex(0, settings.useEnglishInput());
        append(this.englInputChoice);
        
        this.cmdSave = new Command(Strings.SAVE, Command.OK, 1);
        this.cmdBack = new Command(Strings.BACK, Command.BACK, 1);
        
        addCommand(this.cmdSave);
        addCommand(this.cmdBack);
        
        setCommandListener(this);
    }

    /**
     * Save the settings Use showSearchForm(true) to reload the search form when
     * the settings are changed
     */
    public void commandAction(Command c, Displayable d) {
        if (c == this.cmdSave) {
            final String resultCountString = this.resultsCountField.getString();
            
            if (resultCountString.length() > 0) {
                final byte resultCount = Byte.parseByte(resultCountString);
                final byte langIndex = (byte) this.langChoice.getSelectedIndex();
                //System.out.println("lang index=" +langIndex);
                
                final AppSettings settings = AppSettings.getSettings();
                final int oldLangIndex = settings.getCurrentLangIndex();
                final boolean useEnglishInput = this.englInputChoice
                        .getSelectedFlags(new boolean[1]) == 1;
                settings.setSettings(resultCount, langIndex, useEnglishInput);
                
                //new language set
                if (langIndex != oldLangIndex)    {
                    //System.out.println("new lang");
                    try {
                        Strings.readMenu();
                    }
                    catch (IOException e) {
                        final Alert alert = new Alert("Error", e.toString(), null, AlertType.ERROR);
                        DisplayHandler.setCurrent(alert);
                    }
                }
                DisplayHandler.showSearchForm(true);
            }
        }
        else if (c == this.cmdBack) {
            DisplayHandler.showSearchForm(false);
        }
    }

}
