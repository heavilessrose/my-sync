package ora.ch5;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Graphics;
import javax.microedition.midlet.MIDlet;

public class EventsMIDlet extends MIDlet 
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
            Canvas canvas = new EventsCanvas();            
            exitCommand = new Command("Exit", Command.EXIT, 0);
            canvas.addCommand(exitCommand);
            canvas.setCommandListener(this);
            display.setCurrent(canvas);
            
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
}

class EventsCanvas extends Canvas {
    
    static int[] keyCodes = {KEY_NUM0, KEY_NUM1, KEY_NUM2, KEY_NUM3, KEY_NUM4,
                             KEY_NUM5, KEY_NUM6, KEY_NUM7, KEY_NUM8, KEY_NUM9,
                             KEY_POUND, KEY_STAR};
    static String[] keyNames = {"KEY_NUM0", "KEY_NUM1", "KEY_NUM2", "KEY_NUM3", "KEY_NUM4",
                             "KEY_NUM5", "KEY_NUM6", "KEY_NUM7", "KEY_NUM8", "KEY_NUM9",
                             "KEY_POUND", "KEY_STAR"};
                             
    static int[] gameActions = {
                            UP, DOWN, LEFT, RIGHT, FIRE,
                            GAME_A, GAME_B, GAME_C, GAME_D};
    static String[] gameNames = {
                            "UP", "DOWN", "LEFT", "RIGHT", "FIRE",
                            "GAME_A", "GAME_B", "GAME_C", "GAME_D" };

    
    int lastKeyCode = 0;
    
    int lastX;
    
    int lastY;
    
    boolean pointer;

    protected void keyPressed(int keyCode) {
        lastKeyCode = keyCode;
        repaint();
    }
        
    protected void keyRepeated(int keyCode) {
        lastKeyCode = keyCode;
        repaint();
    }
        
    protected void keyReleased(int keyCode) {
        lastKeyCode = 0;
        repaint();
    }    

    protected void pointerPressed(int x, int y) {
        lastX = x;
        lastY = y;
        pointer = true;
        repaint();
    }
        
    protected void pointerDragged(int x, int y) {
        lastX = x;
        lastY = y;
        pointer = true;
        repaint();
    }
    
    protected void pointerReleased(int x, int y) {
        pointer = false;
        repaint();
    }
    
    protected void paint(Graphics g) {
        g.setColor(0xffffff);
        g.fillRect(0, 0, getWidth(), getHeight());
        
        g.setColor(0);
        if (lastKeyCode != 0) {
            String keyText = "keyCode " + lastKeyCode;
            String keyName = null;

            // See if it is a standard key
            for (int i = 0; i < keyCodes.length; i++) {
                if (lastKeyCode == keyCodes[i]) {
                    keyName = keyNames[i];
                    break;
                }
            }   
            
            if (keyName == null) {
                // See if it is a game action
                for (int i = 0; i < gameActions.length; i++) {
                    if (lastKeyCode == getKeyCode(gameActions[i])) {
                        keyName = gameNames[i];
                        break;
                    }
                }
            }
            
            g.drawString(keyText, getWidth()/2, getHeight()/2, 
                            Graphics.BASELINE|Graphics.HCENTER);
                    
            if (keyName != null) {
                g.drawString(keyName, getWidth()/2, getHeight()/2 + g.getFont().getHeight(), 
                            Graphics.BASELINE|Graphics.HCENTER);    
            }
        } else if (pointer) {
            g.drawString("(" + lastX + ", " + lastY + ")", getWidth()/2, getHeight()/2, 
                            Graphics.BASELINE|Graphics.HCENTER);
        }
    }
}