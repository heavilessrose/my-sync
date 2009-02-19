/*
 * Created on 03-May-2006
 * 
 * Author: Georgi R. Georgiev
 * Copyright: Georgi R. Georgiev
 */
package dict.util;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Gives a useful function to read the dictionary and all the data saved in the 
 * applications jar file
 * @author Georgi R. Georgiev
 */
public class DataAccess {
    
    /**
     * Default encoding of the dictionary format and the menu texts
     */
    private static final String DICT_CHARSET = "UTF-8";
    
    private static final int BOM_LENGTH = 3;
    
    private static StringBuffer buffer = new StringBuffer();;
    
    /**
     * Return an InputStreamReader for a resource. The UTF-8 3-byte header is
     * stripped from the stream.
     * 
     * @param resPath
     * @return InputStreamReader ready to read
     * @throws IOException
     */
    public static InputStreamReader getRessourceStream(final String resPath)
            throws IOException {
        final InputStream is = (new Object()).getClass().getResourceAsStream(
                resPath);
        if (is != null) {
            is.skip(BOM_LENGTH);// skip 3 bytes UTF-8 Byte Order Mark
            return new InputStreamReader(is, DICT_CHARSET);
        }
        throw new IOException("Error finding a ressource " + resPath);
    }
    
    /**
     * Reads a line with '\n' or '\r\n' end character
     * @param isr
     * @return line as String or null, when no more data in the stream
     * @throws IOException
     */
    public static String readLine(final InputStreamReader isr) throws IOException  {
        int readed;
        while ( (readed = isr.read()) != -1)    {
            final char c = (char) readed;
            buffer.append(c);
            if (c == '\n')  {
                if (buffer.charAt(buffer.length() - 2) == '\r')
                    buffer.setLength(buffer.length() - 2);
                else
                    buffer.setLength(buffer.length() - 1);
                break;
            }
        }
        final String read = buffer.toString();
        buffer.setLength(0);
        return read.length() > 0 ? read : null;
    }
    
    /**
     * Reads word from the input stream, delimited with '|'
     * @param isr read the data from isr
     * @return -1 if the end of stream is reached, should be checked only against -1
     * @throws IOException
     */
    public static String readString(final InputStreamReader isr) throws IOException  {
        int readed;
        while ( (readed = isr.read()) != -1 )   {
            final char readedChar = (char) readed;
            if (readedChar == '|')
                break;
            buffer.append(readedChar);
        }
        final String word = buffer.toString();
        buffer.setLength(0);//buffer löschen
        return word.length() > 0 ? word : null;
    }
}
