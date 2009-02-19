package ora.ch5;

import java.util.Timer;
import java.util.TimerTask;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.ItemStateListener;
import javax.microedition.midlet.MIDlet;

public class AnimationMIDlet extends MIDlet 
                        implements CommandListener, ItemStateListener {

    // The MIDlet's Display object
    private Display display;
        
    // Flag indicating first call of startApp
    protected boolean started;
    
    // Exit command
    private Command exitCommand;
    
    // Setup command
    private Command setupCommand;
    
    // Run command
    private Command runCommand;
    
    // Configuration form
    private Form form;
    
    // Animation canvas
    private AnimationCanvas canvas;
    
    // Gauge for block count
    private Gauge blockGauge;
    
    // Gauge for frame rate
    private Gauge rateGauge;
    
    // Initial frame rate
    private static final int FRAME_RATE = 1;
    
    // Initial number of blocks
    private static final int BLOCK_COUNT = 1;
    
    protected void startApp() {
        if (!started) {
            display = Display.getDisplay(this);
            form = new Form("Animation");
            rateGauge = new Gauge("Frame rate", true, 10, FRAME_RATE);
            blockGauge = new Gauge("Blocks", true, 4, BLOCK_COUNT);
            form.append(rateGauge);
            form.append(blockGauge);
            form.setItemStateListener(this);
            
            canvas = createAnimationCanvas();            
            
            exitCommand = new Command("Exit", Command.EXIT, 0);
            setupCommand = new Command("Setup", Command.SCREEN, 0);
            runCommand = new Command("Run", Command.SCREEN, 0);
            
            canvas.addCommand(exitCommand);
            canvas.addCommand(setupCommand);
            form.addCommand(exitCommand);
            form.addCommand(runCommand);
            
            form.setCommandListener(this);
            canvas.setCommandListener(this);
            
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
        } else if (c == runCommand) {
            display.setCurrent(canvas);
        } else if (c == setupCommand) {
            display.setCurrent(form);
        }
    }
    
    public void itemStateChanged(Item item) {
        if (item == blockGauge) {
            int count = blockGauge.getValue();
            if (count < 1) {
                count = 1;
            }
            canvas.setBlockCount(count);
        } else if (item == rateGauge) {
            int count = rateGauge.getValue();
            if (count < 1) {
                count = 1;
            }
            canvas.setFrameRate(count);
        }            
    } 
    
    // Creates the canvas that will draw the block
    protected AnimationCanvas createAnimationCanvas() {
        return new AnimationCanvas();
    }
    
    class AnimationCanvas extends Canvas {

        // Size of each block
        protected static final int SIZE = 4;
        
        // Initial speeds in the X direction
        protected final int[] xSpeeds = { 2, -2, 0, -2 };
        
        // Initial speeds in the Y direction
        protected final int[] ySpeeds = { 2, -2, 2, -0 };
        
        // Background color
        protected int background = display.isColor() ? 0 : 0xc0c0c0;
            
        // Foreground color
        protected int foreground = display.isColor() ? 0xffff00 : 0;
        
        // Width of screen
        protected int width = getWidth();
        
        // Height of screen
        protected int height = getHeight();
        
        // The screen update rate
        protected int frameRate;
        
        // The blocks to draw on the screen
        protected Block[] blocks;
        
        // The update timer
        protected Timer timer;
        
        // The update timer task
        protected TimerTask updateTask;
        
        // Gets the maximum number of blocks
        public int getMaxBlocks() {
            return blocks.length;
        }
        
        // Constructs a canvas with default settings
        AnimationCanvas() {
            setBlockCount(BLOCK_COUNT);
            setFrameRate(FRAME_RATE);
        }
        
        // Sets the number of blocks to draw
        public void setBlockCount(int count) {
            if (count > xSpeeds.length) {
                throw new IllegalArgumentException("Cannot have more than " 
                                + xSpeeds.length + " blocks");
            }
            
            blocks = new Block[count];
            createBlocks();
        }
        
        // Gets the number of blocks to draw
        public int getBlockCount() {
            return blocks.length;
        }
        
        // Sets the number of updates per second
        public void setFrameRate(int frameRate) {
            if (frameRate < 1 || frameRate > 10) {
                throw new IllegalArgumentException("Frame rate must be > 0 and <= 10");
            }
            this.frameRate = frameRate;
            if (isShown()) {
                startFrameTimer();
            }
        }
        
        // Gets the number of updates per second
        public int getFrameRate() {
            return frameRate;
        }  
          
        // Paint canvas background and all
        // of the blocks in their correct locations.
        protected void paint(Graphics g) {
            // Paint with the background color
            g.setColor(background);
            g.fillRect(0, 0, width, height);
            
            // Draw all of the blocks
            g.setColor(foreground);
            synchronized (this) {
                for (int i = 0, count = blocks.length; i < count; i++) {
                    g.fillRect(blocks[i].x, blocks[i].y, SIZE, SIZE);
                }
            }
        }
        
        // Notification that the canvas has been made visible
        protected void showNotify() {
            // Start the frame timer running
            startFrameTimer();
        }
        
        // Notification that the canvas is no longer visible
        protected void hideNotify() {
            // Stop the frame timer 
            stopFrameTimer();
        }
        
        // Creates the blocks to be displayed
        private void createBlocks() {
            int startX = (width - SIZE)/2;
            int startY = (height - SIZE)/2;
            for (int i = 0, count = blocks.length; i < count; i++) {
                blocks[i] = new Block(startX, startY, xSpeeds[i], ySpeeds[i]);
            }
        }
        
        // Starts the frame redraw timer
        protected void startFrameTimer() {
            timer = new Timer();
            
            updateTask = new TimerTask() {
                public void run() {
                    moveAllBlocks();
                }
            };
            long interval = 1000/frameRate;
            timer.schedule(updateTask, interval, interval);
        }
        
        // Stops the frame redraw timer
        protected void stopFrameTimer() {
            timer.cancel();            
        }
        
        // Called on expiry of timer.
        public synchronized void moveAllBlocks() {
            // Update the positions and speeds
            // of all of the blocks
            for (int i = 0, count = blocks.length; i < count; i++) {
                blocks[i].move();
                
                // Request a repaint of the screen
                repaint();                
            }
        }
        
        // Inner class used to represent a block on the screen
        class Block {
            int x;      // X position
            int y;      // Y position
            int xSpeed; // Speed in the X direction
            int ySpeed; // Speed in the Y direction
            
            Block(int x, int y, int xSpeed, int ySpeed) {
                this.x = x;
                this.y = y;
                this.xSpeed = xSpeed;
                this.ySpeed = ySpeed;
            }
            
            void move() {
                x += xSpeed;
                if (x <= 0 || x + SIZE >= width) {
                    xSpeed = -xSpeed;
                }
                
                y += ySpeed;
                if (y <= 0 || y + SIZE >= height) {
                    ySpeed = -ySpeed;
                }                
            }            
        }
    }
}
