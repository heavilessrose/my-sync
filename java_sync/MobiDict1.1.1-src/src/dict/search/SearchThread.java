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

import java.util.Vector;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.StringItem;

import dict.util.AppSettings;
import dict.util.Strings;

/**
 * Worker thread that implements the actual searching
 * @author Georgi R. Georgiev
 */
public class SearchThread extends Thread {
    
    private static SearchThread workThread;
    
    private String searchWord;
    private StringItem refreshItem;
    private StringItem searchStatusItem;
    private static Callback callback;
    private static boolean running;
    
    /**
     * Use max one instance of the thread, returned by this function
     * @return the instance of the worker thread
     */
    public static SearchThread getSearchThread()	{
        return workThread;
    }
    
    private SearchThread()	{
        super();
        this.searchWord = "";
    }
    
    /**
     * Set the callback for the search thread
     * @param cb
     */
    public static void setCallback(Callback cb)  {
        callback = cb;
    }
    
    /**
     * Find the translations from the dictionary and update the screen form.
     * Otherwise sleep
     */
    public void run() {
        String word = "";
        try {
            while (running) {
                if (this.searchWord.equals(word)) {//the search word already found
                    waitToInterrupt();
                }
                else    {//new search word set from the gui
                
                    word = this.searchWord;
                    final Dictionary dictionary = Dictionary.getDictionary();

                    final int maxResults = AppSettings.getSettings()
                            .getMaxResultCount();
                    final Vector result = dictionary.findWord(word, maxResults);

                    //if new word typed from the user in the meantime
                    if (!word.equals(this.searchWord)) {
                        continue;//go to the normal search
                    }
                    
                    if (result == null) {
                        this.searchStatusItem.setText(Strings.NO_MATCH_FOR
                                + ' ' + word + '\n');
                    }
                    else    //at least one result
                        updateScreen(result);
                }

            }

        }
        //should not really happen, indicates a bug!
        // @PMD:REVIEWED:AvoidCatchingThrowable: by Administrator on 18/02/06 00:53
        catch (Throwable t) {
            t.printStackTrace();
            final Alert alert = new Alert(Strings.ERROR, t.toString(), null,
                    AlertType.ERROR);
            callback.reportAlert(alert);
        }
    }
    
    private void updateScreen(Vector result) {
        this.searchStatusItem.setText("");
        
        final int vectorSize = result.size() / 2;
        final StringBuffer buffer = new StringBuffer();
        for (int i=0; i < vectorSize; i++)  {
            buffer.append(result.elementAt(2 * i));
            buffer.append(" - ");
            buffer.append(result.elementAt(2 * i + 1));
            buffer.append('\n');
        }
        this.refreshItem.setText(buffer.toString());

    }
    
    /**
     * Wait for an interrupt to happen (to start new search)
     * 
     */
    // @PMD:REVIEWED:AvoidSynchronizedAtMethodLevel: by Administrator on 23/05/06 03:07
    private synchronized void waitToInterrupt() {
        try {
            wait();
        }
        catch (InterruptedException e) {
        }
    }
    
    /**
     * Search for a given word
     * @param word search word or beginning of it
     * @param stringItem StringItem where the results will be given
     * @param statusItem Status item to show errors like the word not found
     */
    public void searchFor(final String word, final StringItem stringItem, final StringItem statusItem)	{
        if (!word.equals(""))	{
            
            //System.out.println("set search word=" + word);
            this.searchWord = word;
            this.refreshItem = stringItem;
            this.searchStatusItem = statusItem;
            synchronized(this)	{
                notifyAll();//wake up the search thread
            }
        }
    }
    
    /**
     * Stop the search thread 
     */
    public static void cancelSearch() {
        running = false;
    }
    
    /**
     * Start the search thread
     */
    public static void startSearch()  {
        workThread = new SearchThread();
        running = true;
        workThread.start();
    }
    
}
