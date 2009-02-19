/*
 * Created on 09-Nov-2005
 * 
 * Author: Georgi R. Georgiev
 * Copyright: Georgi R. Georgiev
 *  * *     This file is part of MobiDict

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

import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Vector;

/**
 * Contains all the menu strings in the application, that can be translated
 * @author Georgi R. Georgiev
 */
// @PMD:REVIEWED:TooManyFields: by Administrator on 05/03/06 04:13
public class Strings {
    
    private static String[] langList;

    //not defined as constants, because the menu language can be change dynamically
    public static String ABOUT;
    public static String AUTHOR;
    public static String MAIL;
    public static String BACK;
    public static String EXIT;
    public static String TYPE_WORD;
    public static String SEARCH;
    public static String SETTINGS;
    public static String SAVE;
    public static String RESULT_COUNT;
    public static String MENU_LANG;
    public static String VERSION;
    public static String NO_MATCH_FOR;
    public static String ENGLISH_GERMAN;
    public static String COPYRIGHT;
    public static String LICENSE;
    public static String ONLINE_SEARCH;
    public static String ENGLISH_TO_GERMAN;
    public static String GERMAN_TO_ENGLISH;
    public static String OPEN_CONN;
    public static String SEND_DATA;
    public static String READ_DATA;
    public static String TRANSLATION;
    public static String CONN_ERROR;
    public static String ERROR;
    public static String ONLINE_SEARCH_DISCL;
    public static String PROJ_TUTOR;
    public static String PROJECT;
    public static String USE_ENGL_INPUT;
    public static String SEARCHING_FOR;
    
    /**
     * Reads all the menu strings from the resource file
     * Should be called after the initMenuLang() because it uses that information
     * @throws IOException
     */
    public static void readMenu() throws IOException {
        final int index = AppSettings.getSettings().getCurrentLangIndex();
        
        final InputStreamReader isr = DataAccess.getRessourceStream('/' + langList[index] + "-Menu.txt"); //"/strings.txt");
        
        ABOUT = DataAccess.readString(isr);
        AUTHOR = DataAccess.readString(isr);
        MAIL = DataAccess.readString(isr);
        BACK = DataAccess.readString(isr);
        EXIT = DataAccess.readString(isr);
        TYPE_WORD = DataAccess.readString(isr);
        SEARCH = DataAccess.readString(isr);
        SETTINGS = DataAccess.readString(isr);
        SAVE = DataAccess.readString(isr);
        RESULT_COUNT = DataAccess.readString(isr);
        MENU_LANG = DataAccess.readString(isr);
        VERSION = DataAccess.readString(isr);
        NO_MATCH_FOR = DataAccess.readString(isr);
        ENGLISH_GERMAN = DataAccess.readString(isr);
        COPYRIGHT = DataAccess.readString(isr);
        LICENSE = DataAccess.readString(isr);
        ONLINE_SEARCH = DataAccess.readString(isr);
        ENGLISH_TO_GERMAN = DataAccess.readString(isr);
        GERMAN_TO_ENGLISH = DataAccess.readString(isr);
        OPEN_CONN = DataAccess.readString(isr);
        SEND_DATA = DataAccess.readString(isr);
        READ_DATA = DataAccess.readString(isr);
        TRANSLATION = DataAccess.readString(isr);
        CONN_ERROR = DataAccess.readString(isr);
        ERROR = DataAccess.readString(isr);
        ONLINE_SEARCH_DISCL = DataAccess.readString(isr);
        PROJ_TUTOR = DataAccess.readString(isr);
        PROJECT = DataAccess.readString(isr);
        USE_ENGL_INPUT = DataAccess.readString(isr);
        SEARCHING_FOR = DataAccess.readString(isr);
    }

    /**
     * Reads the menu languages available to the application from lang.txt
     * @throws IOException indicate that the lang format is broken
     */
    public static void initLangList() throws IOException   {
        final InputStreamReader isr = DataAccess.getRessourceStream("/lang.txt");
        
        final Vector stringList = new Vector();
        String lang;
        while ( (lang = DataAccess.readString(isr)) != null)  {
            //at least one language assumed
            stringList.addElement(lang);
        }
        langList = new String[stringList.size()];
        stringList.copyInto(langList);
    }
    
    /**
     * Get the list with all languages supported by the menu
     * @return string array with the language names
     */
    public static String[] getMenuLanguages()   {
        return langList;
    }
    
    /**
     * Converts first the string to lower cases
     * Then replaces the "ae", "oe", "ue", "ss" with 
     * @param source
     * @return String with the chars replaced, lower cased
     */
    public static String replaceUmlauts(String source) {
        final StringBuffer buffer = new StringBuffer(source.toLowerCase());
        for (int i = 0; i < buffer.length() - 1; i++) {
            final char currChar = buffer.charAt(i);
            final char nextChar = buffer.charAt(i + 1);
            if (currChar == 'a' && nextChar == 'e') {
                buffer.setCharAt(i, ' ');
                buffer.deleteCharAt(i + 1);
            }
            else
                if (currChar == 'o' && nextChar == 'e') {
                    buffer.setCharAt(i, ' ');
                    buffer.deleteCharAt(i + 1);
                }
                else
                    if (currChar == 'u' && nextChar == 'e') {
                        buffer.setCharAt(i, ' ');
                        buffer.deleteCharAt(i + 1);
                    }
                    else
                        if (currChar == 's' && nextChar == 's') {
                            buffer.setCharAt(i, ' ');
                            buffer.deleteCharAt(i + 1);
                        }

        }
        return buffer.toString();
    }
    
}
