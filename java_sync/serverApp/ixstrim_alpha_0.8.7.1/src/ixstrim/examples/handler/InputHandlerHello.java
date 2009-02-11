package ixstrim.examples.handler;

import ixstrim.context.RequestContext;
import ixstrim.net.http.HttpConstants;
import ixstrim.net.http.InputHandler;

import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class InputHandlerHello implements InputHandler {

    private static final byte[] content = "<html><body><h1>Hello world</h1></body></html>".getBytes();

    private static String len = "" + content.length;

    private ByteBuffer result;

    public InputHandlerHello(RequestContext requestContext) {
        result = ByteBuffer.wrap(content);
        requestContext.getResponse().setHeader(HttpConstants.HEADER_CONTENT_LENGTH, len);
        requestContext.getCallback().readyForOutput();
        requestContext.getCallback().close();
    }

    public ByteBuffer read() {
        return result;
    }
}
