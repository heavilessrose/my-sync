package ora.ch4;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.TextBox;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.Ticker;
import javax.microedition.midlet.MIDlet;

public class TextBoxMIDlet extends MIDlet {

    // Maximum size of the text in the TextBox
    private static final int MAX_TEXT_SIZE = 64;
    
    // The TextBox
    protected TextBox textBox;
    
    // The MIDlet's Display object
    protected Display display;
    
    // Flag indicating first call of startApp
    protected boolean started;
    
    protected void startApp() {
        if (!started) {
            // First time through - initialize            
            // Get the text to be displayed
            String str = null;
            try {
                InputStream is = getClass().getResourceAsStream("resources/text.txt");
                InputStreamReader r = new InputStreamReader(is);
                char[] buffer = new char[32];
                StringBuffer sb = new StringBuffer();
                int count;
                while ((count = r.read(buffer, 0, buffer.length)) > -1) {
                    sb.append(buffer, 0, count);
                }
                str = sb.toString();
            } catch (IOException ex) {
                str = "Failed to load text";
            }
            
            // Create the TextBox
            textBox = new TextBox("TextBox Example", str, 
                                MAX_TEXT_SIZE, TextField.ANY);
            
            // Create a ticker and install it
            Ticker ticker = new Ticker("This is a ticker...");
            textBox.setTicker(ticker);
            
            // Install the TextBox as the current screen
            display = Display.getDisplay(this);            
            display.setCurrent(textBox);

            started = true;
        }        
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }
}
