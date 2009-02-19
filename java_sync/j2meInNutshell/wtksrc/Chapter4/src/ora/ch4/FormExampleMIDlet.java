package ora.ch4;

import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;

public class FormExampleMIDlet extends MIDlet {
    
    
    // The MIDlet's Display object
    protected Display display;
    
    // Flag indicating first call of startApp
    protected boolean started;
    
    protected void startApp() {
        if (!started) {
            display = Display.getDisplay(this);
            
            Form form = new Form("Item Layout");
            
            form.append("Hello");
            form.append("World");
            
            form.append("\nLet's start\na new line\n");
            form.append("This is quite a long string that may not fit on one line");
            
            form.append(new TextField("Name", "J. Doe", 32, TextField.ANY));
            form.append("Address");
            form.append(new TextField(null, null, 32, TextField.ANY));            
            
            display.setCurrent(form);
            
            started = true;
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }
}
