package ora.ch3;

import java.util.Timer;
import java.util.TimerTask;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class ExampleMIDlet extends MIDlet {
    
    // Flag to indicate first call to startApp
    private boolean started = false;
    
    // Background thread
    private Thread thread;
    
    // Timer interval
    private int timerInterval;
    
    // Timer
    private Timer timer;
    
    // Task to run via the timer
    private TimerTask task;
    
    // Required public constructor. Can be
    // omitted if nothing to do and no other
    // constructors are created.
    public ExampleMIDlet() {
        System.out.println("Constructor executed");
        
        // Get the timer interval from the 
        // manifest or JAD file.
        String interval = getAppProperty("Timer-Interval");
        timerInterval = Integer.parseInt(interval); 
        System.out.println("Timer interval is " + interval);
    }
        
    protected void startApp() throws MIDletStateChangeException {
        if (!started) {
            // First invocation. Create and start
            // a timer.
            started = true;            
            System.out.println("startApp called for the first time");
            startTimer();
        } else {
            // Resumed after pausing. 
            System.out.println("startApp called following pause");
        }
        
        // In all cases, start a background thread.
        synchronized (this) {
            if (thread == null) {
                thread = new Thread() {
                    public void run() {
                        System.out.println("Thread running");
                        while (thread == this) {
                            try {
                                Thread.sleep(1000);
                                System.out.println("Thread still active");
                            } catch (InterruptedException ex) {
                            }
                        }
                        System.out.println("Thread terminating");
                    }
                };
            }
        }
        thread.start();
    }

    protected void pauseApp() {
        // Called from the timer task to
        // do whatever is necessary to 
        // pause the MIDlet.
        // Tell the background thread to stop.
        System.out.println("pauseApp called.");
        synchronized (this) {
            if (thread != null) {
                thread = null;
            }
        }
    }

    protected void destroyApp(boolean unconditional) 
                            throws MIDletStateChangeException {
        // Called to destroy the MIDlet.
        System.out.println("destroyApp called - unconditional = " 
                            + unconditional);
        if (thread != null) {
            Thread bgThread = thread;
            thread = null;      // Signal thread to die
            try {
                bgThread.join();
            } catch (InterruptedException ex) {
            }
        }
        stopTimer();
    }
    
    // Starts a timer to run a simple task
    private void startTimer() {
        
        // Create a task to be run
        task = new TimerTask() {
            private boolean isPaused;
            private int count;
        
            public void run() {
                // Pause or resume the MIDlet.
                System.out.println("Timer scheduled");
                if (count++ == 4) {
                    // Terminate the MIDlet
                    try {
                        ExampleMIDlet.this.destroyApp(true);
                    } catch (MIDletStateChangeException ex) {
                        // Ignore pleas for mercy!
                    }
                    ExampleMIDlet.this.notifyDestroyed();
                    return;
                }
                if (isPaused) {
                    System.out.println(">> Resuming MIDlet");
                    ExampleMIDlet.this.resumeRequest();
                    isPaused = false;
                } else {
                    System.out.println(">> Pausing MIDlet");
                    isPaused = true;
                    ExampleMIDlet.this.pauseApp();
                    ExampleMIDlet.this.notifyPaused();
                }                
            }
        };
        
        // Create a timer and schedule it to run
        timer = new Timer();
        timer.schedule(task, timerInterval, timerInterval); 
        System.out.println("Timer started.");
    }
    
    // Stops the timer
    private void stopTimer() {
        if (timer != null) {
            System.out.println("Stopping the timer");
            timer.cancel();
        }
    }
}
