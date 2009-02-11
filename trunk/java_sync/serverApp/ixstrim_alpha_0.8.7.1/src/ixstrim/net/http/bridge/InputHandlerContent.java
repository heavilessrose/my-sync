package ixstrim.net.http.bridge;

import ixstrim.context.RequestContext;
import ixstrim.net.http.InputHandler;

import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class InputHandlerContent implements InputHandler {

    private ByteBuffer buff;

    public InputHandlerContent(RequestContext requestContext,ByteBuffer buff) {
        this.buff = buff;
        requestContext.getCallback().readyForOutput();
        requestContext.getCallback().close();
    }

    public ByteBuffer read() {
        return buff;
    }
}
