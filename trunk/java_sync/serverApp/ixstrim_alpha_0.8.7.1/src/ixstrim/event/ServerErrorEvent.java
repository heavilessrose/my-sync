package ixstrim.event;

import java.util.EventObject;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public class ServerErrorEvent extends EventObject{

    public static final int TYPE_WARNING = 5;
    public static final int TYPE_ERROR = 10;
    public static final int TYPE_FATAL = 15;

    private Throwable throwable;
    private int errorType = TYPE_WARNING;

    public ServerErrorEvent(Object source,Throwable throwable,int errorType) {
        super(source);
        this.throwable = throwable;
        this.errorType = errorType;
    }

    public ServerErrorEvent(Object source,Throwable throwable) {
        super(source);
        this.throwable = throwable;
    }

    /**
     * @return the reason of this error event
     */
    public Throwable getReason(){
        return this.throwable;
    }

    /**
     * @return one of codes - TYPE_WARNING, TYPE_ERROR, TYPE_FATAL
     */
    public int getErrorType() {
        return errorType;
    }
}
