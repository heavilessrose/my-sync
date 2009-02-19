/*
 * Created on 08-Mar-2006
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
package dict.search;

import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Vector;

import dict.util.DataAccess;


/**
 * Contains the index to help finding in which dictionary file can a word be
 * found
 * 
 * @author Georgi R. Georgiev
 */
public class DictIndex {
    private String [] lowStartList;
    private String [] highStartList;
    private String [] fileNameList;
    // @PMD:REVIEWED:SingularField: by Administrator on 10/03/06 01:02
    private int currentIndex;
    
    /**
     * Stores the content of the index.dat file Contains meta information about
     * the translation files and which words are in which files
     */
    public DictIndex() {
        super();
        try {
            InputStreamReader isr = DataAccess.getRessourceStream("/index.dat");

            Vector lowChars = new Vector();
            Vector highChars = new Vector();
            Vector fileNames = new Vector();

            String line;
            while ((line = DataAccess.readLine(isr)) != null) {
                int index = line.indexOf(':');
                String lowString = line.substring(0, index);
                line = line.substring(index + 1);
                index = line.indexOf('-');
                String highString = line.substring(0, index);
                String fileName = line.substring(index + 1);
                lowChars.addElement(lowString);
                highChars.addElement(highString);
                fileNames.addElement(fileName);
            }
            int indexSize = lowChars.size();
            this.lowStartList = new String[indexSize];
            this.highStartList = new String[indexSize];
            this.fileNameList = new String[indexSize];
            
            lowChars.copyInto(this.lowStartList);
            highChars.copyInto(this.highStartList);
            fileNames.copyInto(this.fileNameList);
        }
        catch (IOException e) {
            e.printStackTrace();// should never happen
        }
    }
    
    /**
     * Return the file name where a given word should be searched for
     * @param word in small caps
     * @return file name as String or null, when not found
     */
    public String getFileName(final String word) {
        if (withinWords(word, this.lowStartList[this.currentIndex], this.highStartList[this.currentIndex]))
            return this.fileNameList[this.currentIndex];
        
        for (int i = 0; i < this.lowStartList.length; i++) {
            if (withinWords(word, this.lowStartList[i], this.highStartList[i])) {
                this.currentIndex = i;
                return this.fileNameList[i];
            }
        }
        return null;
    }
    
    private static boolean withinWords(final String word, final String lowWord, final String highWord) {
        int maxIndex = Math.min(word.length(), lowWord.length());
        boolean withinIndex = true;
        for (int j = 0; j < maxIndex; j++) {
            if (word.charAt(j) < lowWord.charAt(j)) {
                withinIndex = false;
                break;
            }
        }
        if (!withinIndex)
            return false;
        maxIndex = Math.min(word.length(), highWord.length());
        for (int j=0; j < maxIndex; j++)    {
            if (word.charAt(j) > highWord.charAt(j))    {
                return false;
            }
        }
        return true;
    }
    
}
