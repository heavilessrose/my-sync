package ora.ch4;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;

import javax.microedition.lcdui.*;

public class TextBox2MIDlet extends TextBoxMIDlet implements CommandListener {

    // Exit command
    private static final Command EXIT_COMMAND = 
                        new Command("Exit", Command.EXIT, 0);
    
    // OK command
    private static final Command OK_COMMAND =
                        new Command("OK", Command.OK, 0);
    
    // Clear text box content
    private static final Command CLEAR_COMMAND =
                        new Command("Clear", Command.SCREEN, 1);
    
    // Reverse the content of the text box
    private static final Command REVERSE_COMMAND =
                        new Command("Reverse", Command.SCREEN, 1);

    protected void startApp() {
        boolean firstTime = !started;
        super.startApp();
        
        // If this is the first execution
        // of startApp, install commands
        if (firstTime) {
            textBox.addCommand(OK_COMMAND);            
            textBox.addCommand(EXIT_COMMAND);
            textBox.addCommand(CLEAR_COMMAND);            
            textBox.addCommand(REVERSE_COMMAND);            
            textBox.setCommandListener(this);
        }
    }
    
    // Command implementations.
    public void commandAction(Command c, Displayable d) {
        if (c == EXIT_COMMAND) {
            destroyApp(true);
            notifyDestroyed();
        } else if (c == OK_COMMAND) {
            System.out.println("OK pressed");
        } else if (c == CLEAR_COMMAND) {
            textBox.setString(null);
        } else if (c == REVERSE_COMMAND) {
            String str = textBox.getString();
            if (str != null) {
                StringBuffer sb = new StringBuffer(str);
                textBox.setString(sb.reverse().toString());
            }            
        }
    }    
}
