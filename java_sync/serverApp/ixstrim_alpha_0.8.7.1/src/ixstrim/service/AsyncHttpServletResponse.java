package ixstrim.service;

import javax.servlet.http.HttpServletResponse;
import java.nio.channels.ReadableByteChannel;
import java.nio.ByteBuffer;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 19, 2003
 */
public interface AsyncHttpServletResponse extends HttpServletResponse{
    /**
     * @param channel must be selectable, otherwise RuntimeException will thrown.
     */
    public void setChannel(ReadableByteChannel channel);
    public void setContent(ByteBuffer buffer);
}
