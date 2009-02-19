package ora.ch5;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.List;
import javax.microedition.midlet.MIDlet;

public class GraphicsMIDlet extends MIDlet implements CommandListener {

    // The MIDlet's Display object
    private Display display;
        
    // Flag indicating first call of startApp
    protected boolean started;
    
    // Exit command
    private Command exitCommand;
    
    // Back to examples list command
    private Command backCommand;
    
    // The example selection list
    private List examplesList;
    
    // The Canvases used to demonstrate different Items
    private Canvas[] canvases;
    
    // The example names. Used to populate the list.
    private String[] examples = {
        "Lines", "Rectangles", "RectangleFills",
        "Arcs", "FilledArcs", "Text"
    };

    protected void startApp() {
        if (!started) {
            started = true;
            display = Display.getDisplay(this);
            
            // Create the common commands
            createCommands();
            
            // Create the canvases
            createCanvases();
            
            // Create the list of examples
            createList();
            
            // Start with the List
            display.setCurrent(examplesList);
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        if (d == examplesList) {
            // New example selected
            int index = examplesList.getSelectedIndex();
            display.setCurrent(canvases[index]);
        } else if (c == exitCommand) {
            // Exit. No need to call destroyApp
            // because it is empty.
            notifyDestroyed();
        } else if (c == backCommand) {
            // Go back to main selection list
            display.setCurrent(examplesList);
        }
    }
    
    private void createCommands() {
        exitCommand = new Command("Exit", Command.EXIT, 0);
        backCommand = new Command("Back", Command.BACK, 1);
    }
    
    private void createList() {
        examplesList = new List("Select Example", List.IMPLICIT);
        for (int i = 0; i < examples.length; i++) {
            examplesList.append(examples[i], null);
        } 
        examplesList.setCommandListener(this);
    }
    
    private void createCanvases() {
        canvases = new Canvas[examples.length];
        canvases[0] = createLinesCanvas();
        canvases[1] = createRectanglesCanvas();
        canvases[2] = createRectangleFillsCanvas();
        canvases[3] = createArcsCanvas();
        canvases[4] = createFilledArcsCanvas();
        canvases[5] = createTextCanvas();
        
    }

    private void addCommands(Displayable d) {
        d.addCommand(exitCommand);
        d.addCommand(backCommand);
        d.setCommandListener(this);
    }
    
    // Create the Canvas for the line drawing example
    private Canvas createLinesCanvas() {
        Canvas canvas = new LineCanvas();        
        addCommands(canvas);
        return canvas;
    }    
        
    // Create the Canvas for the rectangles example
    private Canvas createRectanglesCanvas() {
        Canvas canvas = new RectanglesCanvas();        
        addCommands(canvas);
        return canvas;
    }
        
    // Create the Canvas for the filled rectangles example
    private Canvas createRectangleFillsCanvas() {
        Canvas canvas = new RectangleFillsCanvas();        
        addCommands(canvas);
        return canvas;
    }
        
    // Create the Canvas for the arcs example
    private Canvas createArcsCanvas() {
        Canvas canvas = new ArcsCanvas();        
        addCommands(canvas);
        return canvas;
    }
           
    // Create the Canvas for the filled arcs example
    private Canvas createFilledArcsCanvas() {
        Canvas canvas = new FilledArcsCanvas();        
        addCommands(canvas);
        return canvas;
    }    
               
    // Create the Canvas for the text example
    private Canvas createTextCanvas() {
        Canvas canvas = new TextCanvas();        
        addCommands(canvas);
        return canvas;
    }

}

// A canvas that illustrates line drawing
class LineCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Fill the background using black
        g.setColor(0);
        g.fillRect(0, 0, width, height);

        // White horizontal line
        g.setColor(0xFFFFFF);
        g.drawLine(0, height/2, width - 1, height/2);

        // Yellow dotted horizontal line
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xFFFF00);
        g.drawLine(0, height/4, width - 1, height/4);

        // Solid diagonal line in brightest gray
        g.setGrayScale(255);
        g.setStrokeStyle(Graphics.SOLID);
        g.drawLine(0, 0, width - 1, height - 1);
    }    
}

// A canvas that illustrates rectangles
class RectanglesCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create a white background
        g.setColor(0xffffff);
        g.fillRect(0, 0, width, height);
        
        // Draw a solid rectangle
        g.setColor(0);
        g.drawRect(width/4, 0, width/2, height/4);
                
        // Draw a dotted rectangle inside
        // the solid rectangle.
        g.setStrokeStyle(Graphics.DOTTED);
        g.drawRect(width/4 + 4, 4, width/2 - 8, height/4 - 8);
        
        // Draw a rounded rectangle
        g.setStrokeStyle(Graphics.SOLID);
        g.drawRoundRect(width/4, height/2, width/2, height/4, 16, 8);
    }
}

// A canvas that illustrates filled rectangles
class RectangleFillsCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create a black background
        g.setColor(0);
        g.fillRect(0, 0, width, height);

        // Draw a filled rectangle with
        // a dotted rectangle around it
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0x00ff00);
        g.fillRect(width/4, height/4, width/2, height/2);
        g.setColor(0xffff00);
        g.drawRect(width/4, height/4, width/2, height/2);
    }
}

// A canvas that illustrates arcs
class ArcsCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create a black background
        g.setColor(0);
        g.fillRect(0, 0, width, height);

        // A quarter circle, clockwise 90 degrees
        // from the 3 o'clock position. Show the
        // bounding rectangle as well.
        g.setColor(0xffffff);
        g.drawArc(0, 0, width/2, height/2, 0, 90);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(0, 0, width/2, height/2);

        // A quarter circle, anticlockwise 90 degrees
        // from the 3 o'clock position. 
        g.setStrokeStyle(Graphics.SOLID);
        g.setColor(0xffffff);
        g.drawArc(width/2, 0, width/2, height/2, 0, -90);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(width/2, 0, width/2, height/2);
       
        // An elliptical arc from the six o'clock
        // position, counterclockwise 180 degrees
        g.setStrokeStyle(Graphics.SOLID);
        g.setColor(0xffffff);
        g.drawArc(0, height/2, width, height/2, -90, -180);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(0, height/2, width, height/2);        
    }
}

// A canvas that illustrates filled arcs
class FilledArcsCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create a black background
        g.setColor(0);
        g.fillRect(0, 0, width, height);

        // A quarter circle, clockwise 90 degrees
        // from the 3 o'clock position. Show the
        // bounding rectangle as well.
        g.setColor(0xffffff);
        g.fillArc(0, 0, width/2, height/2, 0, 90);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(0, 0, width/2, height/2);

        // A quarter circle, anticlockwise 90 degrees
        // from the 3 o'clock position. 
        g.setStrokeStyle(Graphics.SOLID);
        g.setColor(0xffffff);
        g.fillArc(width/2, 0, width/2, height/2, 0, -90);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(width/2, 0, width/2, height/2);
       
        // An elliptical arc from the six o'clock
        // position, counterclockwise 180 degrees
        g.setStrokeStyle(Graphics.SOLID);
        g.setColor(0xffffff);
        g.fillArc(0, height/2, width, height/2, -90, -180);
        g.setStrokeStyle(Graphics.DOTTED);
        g.setColor(0xffff00);
        g.drawRect(0, height/2, width, height/2);        
    }
}

// A canvas that illustrates text rendering
class TextCanvas extends Canvas {
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create a black background
        g.setColor(0);
        g.fillRect(0, 0, width, height);
        
        // Top-left of canvas
        g.setColor(0xffffff);
        g.drawString("Top left", 0, 0, Graphics.TOP | Graphics.LEFT);
        
        // Draw another string one line below
        Font font = g.getFont();
        g.drawString("Below top left", 0, font.getHeight(), Graphics.TOP | Graphics.LEFT);
        
        // Bottom-right of canvas
        g.drawString("Bottom right", width, height, Graphics.BOTTOM | Graphics.RIGHT);
        
        // Mixed fonts and colors
        String str = "Multi-font ";        
        font = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_UNDERLINED, Font.SIZE_LARGE);
        g.setFont(font);
        g.drawString(str, 0, height/2, Graphics.LEFT | Graphics.BASELINE);
        
        int x = font.stringWidth(str);
        g.setColor(0x00ff00);
        g.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD | Font.STYLE_ITALIC, Font.SIZE_MEDIUM));
        g.drawString("and multi-color", x, height/2, Graphics.LEFT | Graphics.BASELINE);
    }
}        