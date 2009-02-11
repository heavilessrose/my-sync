package ixstrim.service;

import javax.servlet.http.HttpServletRequest;
import java.nio.channels.WritableByteChannel;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 19, 2003
 */
public interface AsyncHttpServletRequest extends HttpServletRequest{
    /**
     * @param channel must be selectable, otherwise RuntimeException will thrown.
     */
    public void setChannel(WritableByteChannel channel);
}
