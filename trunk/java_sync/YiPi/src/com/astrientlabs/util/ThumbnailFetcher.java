package com.astrientlabs.util;

import com.astrientlabs.threads.JobRunner;

public class ThumbnailFetcher extends JobRunner
{    
    //private Object pauseSemaphore = new Object();
    private boolean paused = false;
    
    public ThumbnailFetcher(int numberOfFetchers)
    {
        super(numberOfFetchers,Thread.MIN_PRIORITY);
    }
    
    public synchronized void setPaused(boolean paused)
    {
        this.paused = paused;
        if ( !paused )
        {
            //pauseSemaphore.
            notifyAll();
        }
    }
    
    
    public Runnable getNextJob()
    {
        if ( paused )
        {
            try
            {
                synchronized (this)
                {
                    wait();
                }
            }
            catch (InterruptedException e)
            {
            }
        }
        
        return super.getNextJob();
    }

    public void clear()
    {
        //TODO: cancel pending connections;
        removeAllElements();
    }
}
