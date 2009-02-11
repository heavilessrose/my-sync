package ixstrim.examples;

import ixstrim.event.ServerEventListener;
import ixstrim.event.ServerErrorEvent;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class LogListener implements ServerEventListener{

    public void errorOccured(ServerErrorEvent event) {
        System.err.println("Error occured");
        event.getReason().printStackTrace();
    }
}
