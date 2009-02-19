package ora.ch6;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;

// A class that scans through an input
// stream for strins without reading the
// entire stream into a large string.
public class InputHelper {

    // Size of the input buffer
    private static final int BUFFER_SIZE = 1024;

    // The input buffer
    private final char[] buffer = new char[BUFFER_SIZE];

    // Number of characters left in the buffer
    private int charsLeft;

    // Index of the next character in the buffer
    private int nextChar;

    // InputStreamReader used to map to Unicode
    private InputStreamReader reader;

    // Constructs a helper to read a given stream
    public InputHelper(InputStream is) {
        reader = new InputStreamReader(is);
    }
    
    // Cleans up when no longer needed
    public void dispose() {
        if (reader != null) {
            try {
                reader.close();
            } catch (IOException ex) {
            }
            reader = null;
        }
    }

    // Looks for a given string in the input
    // stream and positions the stream so that the
    // next character read is one beyond the string.
    // Returns true if the string was found, false if
    // not (and the stream will have been completely read).
    public boolean moveAfterString(String str) throws IOException {
        char[] chars = str.toCharArray();
        int count = chars.length;
        char firstChar = chars[0];

        char c = (char)0;
        for (;;) {
            if (c != firstChar && !findNext(firstChar)) {
                // Reached the end of the input stream
                return false;
            }

            boolean mismatch = false;
            for (int i = 1; i < count; i++) {
                c = getNext();
                if (c != chars[i]) {
                    mismatch = true;
                    break;
                }
            }

            if (!mismatch) {
                return true;
            }

            // Mismatch. 'c' has the first mismatched
            // character - start the loop again with
            // that character. This is necessary because we
            // could have found "wweb" while looking for "web"
        }
    }
    
    // Gets the characters for a number, ignoring
    // the grouping separator. The number starts at the
    // current input position, but any leading non-numerics
    // are skipped.
    public String gatherNumber() throws IOException {
        StringBuffer sb = new StringBuffer();
        boolean gotNumeric = false;
        for (;;) {
            char c = getNext();
            
            // Skip until we find a digit.
            boolean isDigit = Character.isDigit(c);
            if (!gotNumeric && !isDigit) {
                continue;
            }
            gotNumeric = true;
            if (!isDigit) {
                if (c == '.' || c == ',') {
                    continue;
                }
                break;
            }
            sb.append(c);
        }
        return sb.toString();
    }
    
    // Gets the balance of the current line
    // and returns it as a StringBuffer
    public StringBuffer getRestOfLine() throws IOException {
    
        StringBuffer sb = new StringBuffer();
        char c;
        for (;;) {
            c = getNext();
            if (c == '\n' || c == (char)0) {
                break;
            }
            sb.append(c);
        }
        return sb;
    } 
    
    // Gets the next character from the stream,
    // returning (char)0 when all input has been read.
    private char getNext() throws IOException {
        if (charsLeft == 0) {
            charsLeft = reader.read(buffer, 0, BUFFER_SIZE);
            if (charsLeft < 0) {
                return (char)0;
            }
            nextChar = 0;
        }
        charsLeft--;
        return buffer[nextChar++];
    }
       
    // Finds the next instance of a given character in the
    // input stream. The input stream is positioned after
    // the located character. If EOF is reached without
    // finding the character, false is returned.
    private boolean findNext(char c) throws IOException {
        for (;;) {
            if (charsLeft == 0) {
                charsLeft = reader.read(buffer, 0, BUFFER_SIZE);
                if (charsLeft < 0) {
                    return false;
                }
                nextChar = 0;
            }
            charsLeft--;
            if (c == buffer[nextChar++]) {
                return true;
            }
        }
    }
}