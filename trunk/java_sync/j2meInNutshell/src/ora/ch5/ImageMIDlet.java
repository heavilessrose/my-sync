package ora.ch5;

import java.io.IOException;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.List;
import javax.microedition.midlet.MIDlet;

public class ImageMIDlet extends MIDlet implements CommandListener {

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
        "DrawImage", "ImageGraphics"
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
        canvases[0] = createDrawImageCanvas();
        canvases[1] = createImageGraphicsCanvas();
    }

    private void addCommands(Displayable d) {
        d.addCommand(exitCommand);
        d.addCommand(backCommand);
        d.setCommandListener(this);
    }
    
    // Create the Canvas for the image drawing example
    private Canvas createDrawImageCanvas() {
        Canvas canvas = new DrawImageCanvas();        
        addCommands(canvas);
        return canvas;
    } 
    
    // Create the Canvas to demonstrate drawing to an Image
    private Canvas createImageGraphicsCanvas() {
        Canvas canvas = new ImageGraphicsCanvas();        
        addCommands(canvas);
        return canvas;
    } 
}

// A canvas that illustrates image drawing
class DrawImageCanvas extends Canvas {
    static Image image;
    
    int count;
    
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Fill the background using black
        g.setColor(0);
        g.fillRect(0, 0, width, height);
        
        // Load an image from the MIDlet resources
        if (image == null) {
            try {
                image = Image.createImage("/ora/ch5/resources/earth.png");
            } catch (IOException ex) {
                g.setColor(0xffffff);
                g.drawString("Failed to load image!", 0, 0, Graphics.TOP | Graphics.LEFT);
                return;
            }
        }
        
        switch (count % 3) {
        case 0:
            // Draw the image at the top left of the screen
            g.drawImage(image, 0, 0, Graphics.TOP | Graphics.LEFT);
            break;

        case 1:
            // Draw it in the bottom right corner
            g.drawImage(image, width, height, Graphics.BOTTOM | Graphics.RIGHT);
            break;
        case 2:
            // Draw it in the center
            g.drawImage(image, width/2, height/2, Graphics.VCENTER | Graphics.HCENTER);
        }
        count++;
    }
}

// A canvas that illustrates drawing on an Image
class ImageGraphicsCanvas extends Canvas {
    
    public void paint(Graphics g) {
        int width = getWidth();
        int height = getHeight();

        // Create an Image the same size as the
        // Canvas.
        Image image = Image.createImage(width, height);
        Graphics imageGraphics = image.getGraphics();

        // Fill the background of the image black
        imageGraphics.fillRect(0, 0, width, height);

        // Draw a pattern of lines
        int count = 10;
        int yIncrement = height/count;
        int xIncrement = width/count;
        for (int i = 0, x = xIncrement, y = 0; i < count; i++) {
            imageGraphics.setColor(0xC0 + ((128 + 10 * i) << 8) + ((128 + 10 * i) << 16));
            imageGraphics.drawLine(0, y, x, height);
            y += yIncrement;
            x += xIncrement;
        }

        // Add some text
        imageGraphics.setFont(Font.getFont(Font.FACE_PROPORTIONAL,
                                Font.STYLE_UNDERLINED, Font.SIZE_SMALL));
        imageGraphics.setColor(0xffff00);
        imageGraphics.drawString("Image Graphics", width/2, 0, Graphics.TOP | Graphics.HCENTER);

        // Copy the Image to the screen
        g.drawImage(image, 0, 0, Graphics.TOP | Graphics.LEFT);
    }
}