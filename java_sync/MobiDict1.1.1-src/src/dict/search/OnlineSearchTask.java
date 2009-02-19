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
package dict.search;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.Ticker;

import dict.util.AppSettings;
import dict.util.Strings;

/**
 * Task, that implements the google translate query
 * @author Georgi R. Georgiev
 */
public class OnlineSearchTask implements Runnable {
    
    private final String searchWord;
    private final Ticker refreshTicker;
    private final StringItem resultItem;
    private final int langChoice;
    private boolean active;
    private Callback callback;
    
    /**
     * URL of google translate
     */
    public static final String TRANSL_URL = "http://translate.google.com/translate_t";
    
    /**
     * Creates new search task
     * @param word search word
     * @param ticker ticker to show status information during search
     * @param item where the result will be saved
     * @param choice 0 for EN->DE, 1 for DE->EN
     * @param cb callback interface to report back errors
     */
    public OnlineSearchTask(String word, Ticker ticker, StringItem item,
            int choice, Callback cb) {
        super();
        this.searchWord = word;
        this.refreshTicker = ticker;
        this.resultItem = item;
        this.langChoice = choice;
        this.callback = cb;
    }
    
    /**
     * Query the online service
     * Set the ticker with statis text
     */
    public void run()   {
        this.active = true;
        this.refreshTicker.setString(Strings.OPEN_CONN);
        try {
            final HttpConnection conn = (HttpConnection) Connector.open(TRANSL_URL,
                    Connector.READ_WRITE, true);
        
            this.refreshTicker.setString(Strings.SEND_DATA);
            sendData(conn);
            
            if (!this.active)
                return;
            
            final InputStream is = conn.openInputStream();

            this.refreshTicker.setString(Strings.READ_DATA);
            
            final String result = readData(conn, is);
            this.resultItem.setLabel(Strings.TRANSLATION);
            this.resultItem.setText(result);
            //System.out.println("result=" + result);
            
            is.close();//all streams are closed when exception occurs
        }
        catch (IOException e) {
            if (this.active)    {
                final Alert alert = new Alert(Strings.ERROR, Strings.CONN_ERROR,
                        null, AlertType.ERROR);
                this.callback.reportAlert(alert);
            }
        }
        catch (SecurityException se)    {
            final Alert alert = new Alert(Strings.ERROR, Strings.CONN_ERROR,
                    null, AlertType.ERROR);
            this.callback.reportAlert(alert);
        }
        // @PMD:REVIEWED:AvoidCatchingThrowable: by Administrator on 10/03/06 11:51
        catch (Throwable t) {//bug or schweres Ausnahmefehler
            final Alert alert = new Alert(Strings.ERROR, t.toString(),
                    null, AlertType.ERROR);
            this.callback.reportAlert(alert);
        }
        finally {
            this.callback.reportEndTask();
        }
    }
    
    /**
     * Send data to the online service
     * @param conn
     * @throws IOException
     */
    protected void sendData(final HttpConnection conn) throws IOException {
        conn.setRequestMethod(HttpConnection.POST);
        conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        
        final String text = "text=" + urlEncode(this.searchWord);
        final String langpair = "langpair="
                + ((this.langChoice == AppSettings.TRANSL_EN_DE) ? "en%7Cde"
                        : "de%7Cen");
        final String postText = text + '&' + langpair + "&hl=en&ie=ASCII";
        
        final OutputStream os = conn.openOutputStream();
        
        os.write(postText.getBytes());
        os.close();
    }
    
/**
 * Read the data from the service. Basicly extract the 
 * info from the &lttextarea&gt tag
 * @param conn
 * @param is
 * @return the readed string
 * @throws IOException
 */
    protected static String readData(final HttpConnection conn, final InputStream is)
            throws IOException {
        final int respCode = conn.getResponseCode();
        if (respCode != HttpConnection.HTTP_OK)//error, no need to read anymore
            throw new IOException(conn.getResponseMessage());
        int readed;
        final StringBuffer buffer = new StringBuffer();
        boolean readTag = false;
        boolean readValue = false;
        while ((readed = is.read()) != -1) {
            if (readed == '<') {
                if (readValue)
                    return buffer.toString();
                readTag = true;
            }
            if (readTag || readValue)
                buffer.append((char) readed);
            if (readed == '>') {
                readTag = false;
//              the result is in the first textarea field
                if (buffer.toString().startsWith("<textarea")) {
                    // System.out.println("textarea found");
                    readValue = true;
                }
                buffer.setLength(0);
            }
        }
        return null;
    }
    
    /**
     * URL Encode a string 
     * @param s
     * @return the url encoded string
     */
    private static String urlEncode(final String s) {
        final StringBuffer result = new StringBuffer();
        int i = 0;
        char ch;
        while (i < s.length()) {
            ch = s.charAt(i);
            if (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A'
                    && ch <= 'Z' || ch == '.' || ch == '-' || ch == '*'
                    || ch == '_') {
                result.append(ch);
            }
            else
                if (ch == ' ')
                    result.append('+');
                else {
                    ch <<= 8;
                    ch >>>= 8;
                    final int b = ch;

                    result.append('%');
                    if (b < 0x10)
                        result.append('0');
                    result.append(Integer.toHexString(b).toUpperCase());
                }
            i++;
        }
        return result.toString();
    }
    
    /**
     * Deactivate the task (when the user cancel the query)
     * and no more information is needed
     */
    public void deactivate()    {
        this.active = false;
    }
    
}
