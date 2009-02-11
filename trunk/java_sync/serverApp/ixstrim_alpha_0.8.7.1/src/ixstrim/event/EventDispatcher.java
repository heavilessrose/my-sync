package ixstrim.event;

import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public class EventDispatcher implements ServerEventListener{

    private Set listeners = new HashSet();

    public synchronized void errorOccured(ServerErrorEvent event) {
        Iterator iter = listeners.iterator();
        while(iter.hasNext()){
            ServerEventListener listener = (ServerEventListener)iter.next();
            listener.errorOccured(event);
        }
    }

    public synchronized void register(ServerEventListener eventListener){
        listeners.add(eventListener);
    }

    public synchronized void unregister(ServerEventListener eventListener){
        listeners.remove(eventListener);
    }

}
