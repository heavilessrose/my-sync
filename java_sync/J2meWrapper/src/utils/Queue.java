package utils;
import java.util.Vector;

/**
 * @author ericanderson
 *         <p/>
 *         TODO To change the template for this generated type comment go to
 *         Window - Preferences - Java - Code Style - Code Templates
 */
public class Queue {
    private Vector myVector;


    /**
     * Constructs the Queue
     */
    public Queue() {
        myVector = new Vector();
    }


    /**
     * Enqueues an object into the queue. This method is Thread Safe (TM)
     *
     * @param o
     */
    public synchronized void enqueue( Object o ) {
        myVector.addElement( o );
    }


    /**
     * Dequeues an object from the queue. This method is Thread Safe (TM)
     *
     * @return the object dequeued
     */
    public synchronized Object dequeue() {
        Object o = null;

        if( myVector.size() > 0 ) {
            o = myVector.elementAt( 0 );

            myVector.removeElementAt( 0 );
        }

        return o;
    }


    /**
     * Returns the number of elements in the queue.
     *
     * @return
     */
    public synchronized int size() {
        return myVector.size();
    }

    public synchronized boolean isEmpty() {
        return (myVector.size() == 0);
    }


    /**
     * Clears the queue
     */
    public synchronized void clear() {
        myVector.removeAllElements();
	}
}