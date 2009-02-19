/*
 * Created on 10-Jan-2006
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
package dict.util;

import javax.microedition.rms.RecordStore;

/**
 * Represents the settings in the application
 * @author Georgi R. Georgiev
 */
public class AppSettings {
    
    /**
     * Position of the "english->german" string in the ChoiceGroup
     * in the settings form
     */
    public static final int TRANSL_EN_DE = 0;
    /**
     * Position of the "german->english" string in the ChoiceGroup
     * in the settings form
     */
    public static final int TRANSL_DE_EN = 1;
    
    private static final int DEF_RESULT_COUNT = 5;
    private static final int SETTINGS_SIZE = 4;
    
    private static AppSettings settings;
    
    private byte resultCount;
    
    private byte currentLang;
    private byte onlineTransl;
    private boolean englishInput;//convert oe to ö, ae to ä etc.
    
    /**
     * Get the settings instance
     * @return single instance of the Settings object
     */
    public static AppSettings getSettings()    {
        if (settings == null)
            settings = new AppSettings();
        return settings;
    }
    
    /**
     * Return the maximal possible results shown to the user
     * @return greather than 0, default value MAX_DEF_VALUE
     */
    public int getMaxResultCount()  {
        return this.resultCount;
    }
    
    /**
     * Set the new maximum possible results from the search shown to the user.
     * Save the settings immediately in the record store.
     * 
     * @param count
     *            positive value expected
     * @param langIndex
     *            index with the current chosen menu language
     * @param enInput
     *            parameter to indicate usage of the enlish keyboard input
     * 
     */
    public void setSettings(final byte count, final byte langIndex,
            boolean enInput) {
        this.resultCount = count;
        this.currentLang = langIndex;
        this.englishInput = enInput;
        saveSettings();
    }
    
    /**
     * Save the app settings in record store
     *
     */
    private void saveSettings() {
        byte [] recordData = new byte[SETTINGS_SIZE];
        recordData[0] = this.resultCount;
        recordData[1] = this.currentLang;
        recordData[2] = this.onlineTransl;
        recordData[3] = (byte) (this.englishInput ? 1 : 0);
        try {
            final RecordStore settingsStore = RecordStore.openRecordStore("settings",
                    true);
            if (settingsStore.getNumRecords() > 0)  {
                settingsStore.setRecord(1, recordData, 0, recordData.length);
            }
            else    {
                settingsStore.addRecord(recordData, 0, recordData.length);
            }
            settingsStore.closeRecordStore();
        }
        //saved amount of data is small, so a exceptions are not expected and should be considered bug
        catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Create the internal settings instace. Reads the settings from the record
     * store. When no record store exists, takes the default values.
     * 
     */
    private AppSettings()  {
        super();
        try {
            RecordStore settingsStore = RecordStore.openRecordStore("settings",
                    true);
            if (settingsStore.getNumRecords() > 0)  {
                byte [] record = settingsStore.getRecord(1);
                try {
                    this.resultCount = record[0];
                    this.currentLang = record[1];
                    this.onlineTransl =record[2];
                    this.englishInput = record[3] >= 1;
                }
                //to catch version upgrades, where the new setting information is added
                catch (ArrayIndexOutOfBoundsException e)    {   }
            }
            else    {
                this.resultCount = DEF_RESULT_COUNT;
            }
            //System.out.println("readed result="+ this.resultCount);
            settingsStore.closeRecordStore();
            
        }
        catch (Exception e) {
            //exceptions are not expected and should be considered bug
            e.printStackTrace();
        }
    }
    
    /**
     * Get the current menu language
     * @return index to the menu language list
     */
    public int getCurrentLangIndex()    {
        return this.currentLang;
    }
    
    /**
     * The last used online search type
     * 
     * @return 0 when EN->DE translation was last user, or 1 when DE->EN
     *         translation was used
     */
    public byte getOnlineTransl() {
        return this.onlineTransl;
    }
    
    /**
     * Return if english input style should be used
     * @return return if english input style should be used
     */
    public boolean useEnglishInput()    {
        return this.englishInput;
    }
    
    /**
     * Set the last used searh type
     * 
     * @param transl
     *            0 when EN->DE translation was last user, or 1 when DE->EN
     *            translation was used
     */
    public void setOnlineTransl(final byte transl) {
        this.onlineTransl = transl;
        saveSettings();
    }

}
