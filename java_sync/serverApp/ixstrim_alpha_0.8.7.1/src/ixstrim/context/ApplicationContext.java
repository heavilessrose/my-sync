package ixstrim.context;

import ixstrim.event.EventDispatcher;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public class ApplicationContext {

    private static ApplicationContext defaultApplicationContext = new ApplicationContext();

    private EventDispatcher eventDispatcher = new EventDispatcher();

    public EventDispatcher getEventDispatcher(){
        return eventDispatcher;
    }

    public static ApplicationContext getDefaultApplicationContext(){
        return defaultApplicationContext;
    }

}
