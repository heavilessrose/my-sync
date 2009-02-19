/*
 * Created on 12-Jun-2005
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
package dict.search;

import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Vector;

import dict.util.DataAccess;

/**
 * Contains the loaded dictionary.
 * Only the words, that begin with the first char
 * of the search word are in the memory.
 * Only one instance per application.
 * 
 * @author Georgi R. Georgiev
 */
public class Dictionary {
    
    private static final char TRANSL_DELIM = '|';
    private static final int BIN_ITER = 8;
    
    private static Dictionary dictInstance;
    
    //keys and values should always be the same length
    private String [] dictKeys = new String[0];
    private String [] dictValues = new String[0];
    
    private String currentFileName;
    private final DictIndex dictIndex;
    
    /**
     * Get the dictionary instance
     * @return return instance of the dictionary
     */
    public static Dictionary getDictionary() 	{
        if (dictInstance == null)
            dictInstance = new Dictionary();
        return dictInstance;
    }
    
    private Dictionary()    {
        super();
        this.dictIndex = new DictIndex();
    }
    
    /**
     * Load the dictionary with the given start char
     * @param fileName name of the dictionary, without path
     */
    private void loadDictionary(final String fileName)  {
        try {
            final InputStreamReader isr = DataAccess
                    .getRessourceStream('/' + fileName);

            String line = DataAccess.readLine(isr);//always start with word count
            final int wordCount = Integer.parseInt(line);
            this.dictKeys = new String[wordCount];
            this.dictValues = new String[wordCount];
            
            //while ( (line = Strings.readLine(isr)) != null) {
            for (int i=0; i < wordCount; i++)   {
                line = DataAccess.readLine(isr);
                final int translIndex = line.indexOf(TRANSL_DELIM);
                final String key = line.substring(0, translIndex);
                final String value = line.substring(translIndex + 1);
                
                this.dictKeys[i] = key;
                this.dictValues[i] = value;
                //keys.addElement(key);
                //values.addElement(value);
            }
            
            this.currentFileName = fileName;

            isr.close();
        }
        // @PMD:REVIEWED:EmptyCatchBlock: by Administrator on 19/02/06 19:36
        catch (IOException e) {
//ignore errors, 
//they may occur when dictChar is a special character (?*) or 
//the translating language may not match the typing language
        }
    }
    
    /**
     * Find a word in the dictionary. Use case-insensitive search. Will load a
     * new dictionary when needed. Linear search is used
     * 
     * @param word
     *            search word
     * @param resultCount
     *            max results that should be returned
     * @return Vector with Strings for the found words, even indexes are keys,
     *         odd indexes are values. The length of the vector is always even
     *         Null when no results are found
     */
    public Vector findWord(final String word, final int resultCount) 	{
        final String wordSmallCases = word.toLowerCase();
        
        final String fileName = this.dictIndex.getFileName(wordSmallCases);
        
        if (fileName == null)
            return null;
        
        if (!fileName.equals(this.currentFileName))
            loadDictionary(fileName);
                
        final int startIndex = binSearch(wordSmallCases);
        
        //System.out.println("start=" + startIndex);
        final Vector resultsVector = linearSearch(startIndex, wordSmallCases, resultCount);
        return resultsVector.size() > 0 ? resultsVector : null;
    }
    
    /**
     * Linear search through the database
     * 
     * @param startIndex
     *            from the start index
     * @param wordSmallCases
     *            searched word
     * @param maxResults
     *            maximal number of results
     * @return Vector with the results with size results*2 with key on even and
     *         translations on the odd indexes
     */
    private Vector linearSearch(int startIndex, final String wordSmallCases, final int maxResults)   {
        final Vector resultsVector = new Vector();
        final int wordLen = wordSmallCases.length();
        boolean exitFlag = false;
        for (int i=startIndex; i < this.dictKeys.length; i++)   {
            if (this.dictKeys[i].length() < wordLen)
                continue;
            
            boolean match = true;
            for (int j = 0; j < wordLen; j++) {
                final char wordChar = wordSmallCases.charAt(j);
                //if (wordChar != '?')
                final char c = Character.toLowerCase(this.dictKeys[i].charAt(j));
                if (c != wordChar) {
                    if (wordChar < c)   {
                        exitFlag = true;
                    }
                    match = false;
                    break;
                }
            }
            if (match)  {
                resultsVector.addElement(this.dictKeys[i]);
                resultsVector.addElement(this.dictValues[i]);
                if (resultsVector.size() >= 2*maxResults)
                    return resultsVector;
            }
            if (exitFlag)
                break;
        }
        return resultsVector;
    }
    
    /**
     * 
     * @param word String in small cases
     * @return index from where to start the search
     */
    private int binSearch(final String word) {
        int low = 0;
        int high = this.dictKeys.length - 1;
        
        int startIndex = low;
        int compar = 0;
        for (int i=0; i < BIN_ITER; i++)   {
            startIndex = (high + low) / 2;
            final String dictWord = this.dictKeys[startIndex].toLowerCase();
            compar = word.compareTo(dictWord);
            if (compar < 0)
                high -= (high - low) / 2;
            else if (compar > 0)
                low += (high - low) / 2;
            else break;//word found
            //System.out.println("low=" + low + " high=" + high);
        }
        if (compar <= 0)
            startIndex = low;
        return startIndex;
    }
    
}
