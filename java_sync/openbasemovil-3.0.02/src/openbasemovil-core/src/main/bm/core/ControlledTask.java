package bm.core;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.event.Event;
import bm.core.event.ProgressEvent;

import java.util.Vector;
import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 17-ago-2007 11:43:44
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * A Background task, that registers it self on Event so no progress events
 * are thrown under this task.<br/>
 * It also provides control to stop every BackgroundTask, so application is
 * not held up by a background task when the user is trying to exit the
 * application.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public abstract class ControlledTask
        extends Thread
{
    /**
     * Maximum wait time for a Task to cancel.
     */
    protected static final long MAX_WAIT  = 5000;
    /**
     * Wait periods to check if the Task has ended.
     */
    protected static final long WAIT      = 100;

    /**
     * List of registered tasks.
     */
    protected static Vector allTasks = new Vector();

    /**
     * Convenience method that starts the task and then joins it until it
     * finishes.
     * @param task task
     */
    public static void runAndWait( final ControlledTask task )
    {
        task.start();
        try
        {
            task.join();
        }
        catch( InterruptedException e )
        {
        }
    }

    /**
     * Cancel all running Tasks.
     */
    public static synchronized void cancelAll()
    {
        if( allTasks.size() > 0 )
        {
            final ProgressEvent pe = new ProgressEvent();
            pe.setAnimate( true );
            pe.setCurrentValue( 0 );
            pe.setMaxValue( new Integer( allTasks.size() ) );
            pe.setMessage( "" );
            final String message = ResourceManager.getResource( "task.closingAll" );
            pe.setTitle( message );
            pe.setPhase( message );
            pe.dispatch();

            final Vector v = new Vector();
            for( final Enumeration i = allTasks.elements(); i.hasMoreElements(); )
            {
                v.addElement( i.nextElement() );
            }

            pe.setAnimate( false );
            for( final Enumeration i = v.elements(); i.hasMoreElements(); )
            {
                final ControlledTask task = (ControlledTask) i.nextElement();
                if( task != Thread.currentThread() )
                {
                    task.cancel();
                    //#ifdef cldc11
                    try
                    {
                        sleep( MAX_WAIT );
                    }
                    catch( InterruptedException e )
                    {
                    }
                    if( task.isAlive() )
                    {
                        task.interrupt();
                    }
                    //#else
                    try
                    {
                        long time = 0;
                        while( task.isAlive() && time < MAX_WAIT )
                        {
                            sleep( WAIT );
                            time += WAIT;
                        }
                        if( task.isAlive() )
                        {
                            task.die();
                        }
                    }
                    catch( InterruptedException e )
                    {
                    }
                    //#endif
                }
                pe.increment();
                pe.dispatch();
            }
        }
    }

    /**
     * Check if the current executing thread is a ControlledTask, and if so
     * if it has been signaled for cancel.
     * @return true if the thread is a ControlledTask and has been signaled for
     * cancel
     */
    public static boolean isCancelled()
    {
        return Thread.currentThread() instanceof ControlledTask &&
               ( (ControlledTask) Thread.currentThread() )
                       .cancel;
    }

    /**
     * Check if the current executing thread is a ControlledTask, and if so
     * if it is a background task.
     * @return true if the thread is a ControlledTask and is a background task
     */
    public static boolean isBackgroundTask()
    {
        return Thread.currentThread() instanceof ControlledTask &&
               ( (ControlledTask) Thread.currentThread() )
                       .background;
    }

    /**
     * Cancel the currently executing thread if it's a ControlledTask.
     */
    public static void cancelThisTask()
    {
        if( Thread.currentThread() instanceof ControlledTask )
        {
            ((ControlledTask) Thread.currentThread()).cancel();
        }
    }

    /**
     * Cancel flag, set to true by cancel method to indicate that Task should
     * end.
     */
    protected boolean   cancel      = false;
    protected boolean   background;
    protected String    taskName;

    protected ControlledTask( final String name, final boolean background )
    {
        //#ifdef cldc11
        super( name );
        //#endif
        taskName = name;
        this.background = background;
    }

    protected ControlledTask( final boolean background )
    {
        this.background = background;
        taskName = this.getClass().getName() + " " + super.toString();
    }

    /**
     * Cancel task and end it's execution gracefully.
     */
    public synchronized void cancel()
    {
        cancel = true;
    }

    public void run()
    {
        allTasks.addElement( this );
        if( background )
        {
            Event.addBackgroundTask( this );
        }
        try
        {
            task();
        }
        finally
        {
            allTasks.removeElement( this );
            if( background )
            {
                Event.removeBackgroundTask( this );
            }
        }
    }

    /**
     * Check if this task is running at background
     * @return background
     */
    public boolean isBackground()
    {
        return background;
    }

    /**
     * Put this thread to the background or to the foreground.
     *
     * @param background background
     */
    public synchronized void setBackground( final boolean background )
    {
        if( this.background != background )
        {
            this.background = background;
            if( background )
            {
                Event.addBackgroundTask( this );
            }
            else
            {
                Event.removeBackgroundTask( this );
            }
        }
    }

    /**
     * If the maximum time for the thread to cancel is reached, die is called
     * to try to stop it. The subclass should do whatever is possible to
     * end its execution, for example, if it's blocked in an InputStream for
     * an external peripheral, it could try to close the stream so an exception
     * is trhown at the blocking read method.
     */
    public abstract void die();

    /**
     * Perform the task.
     */
    protected abstract void task();

    public String getTaskName()
    {
        return taskName;
    }

    public void setTaskName( final String taskName )
    {
        this.taskName = taskName;
    }

    public String toString()
    {
        //#ifdef cldc11
        //# return taskName + "[" + getPriority() + "]";
        //#else
        return super.toString();
        //#endif
    }
}
