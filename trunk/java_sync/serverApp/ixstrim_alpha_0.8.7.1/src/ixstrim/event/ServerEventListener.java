package ixstrim.event;

import java.util.EventListener;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public interface ServerEventListener extends EventListener{
    public void errorOccured(ServerErrorEvent event);
}
