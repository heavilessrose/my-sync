package ora.ch5;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.StringItem;
import javax.microedition.midlet.MIDlet;

public class AttributesMIDlet extends MIDlet 
                        implements CommandListener {

    // The MIDlet's Display object
    private Display display;
        
    // Flag indicating first call of startApp
    protected boolean started;
    
    // Exit command
    private Command exitCommand;
    
    protected void startApp() {
        if (!started) {
            display = Display.getDisplay(this);
            Canvas canvas = new DummyCanvas();
            
            // Build a Form displaying the Display 
            // and Canvas attributes.
            Form form = new Form("Attributes");
            exitCommand = new Command("Exit", Command.EXIT, 0);
            form.addCommand(exitCommand);
            
            boolean isColor = display.isColor();
            form.append(new StringItem(isColor ? "Colors: " : "Grays: ",
                                String.valueOf(display.numColors())));
            form.append(new StringItem("Width: ", String.valueOf(canvas.getWidth())));
            form.append(new StringItem("Height: ", String.valueOf(canvas.getHeight())));
            form.append(new StringItem("Pointer? ", 
                                String.valueOf(canvas.hasPointerEvents())));
            form.append(new StringItem("Motion? ",
                                String.valueOf(canvas.hasPointerMotionEvents())));
            form.append(new StringItem("Repeat? ", 
                                String.valueOf(canvas.hasRepeatEvents())));
            form.append(new StringItem("Buffered? ", 
                                String.valueOf(canvas.isDoubleBuffered())));
            
            form.setCommandListener(this);

            display.setCurrent(form);
            
            started = true;
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        if (c == exitCommand) {
            // Exit. No need to call destroyApp
            // because it is empty.
            notifyDestroyed();
        }
    } 
    
    // A Canvas that has no painting logic
    static class DummyCanvas extends Canvas {
        protected void paint(Graphics g) {
            // Do nothing
        }
    }
}