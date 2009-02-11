package ixstrim.net.http.bridge;

import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 17, 2003
 */
public interface ChannelNotReadyCallback {
    public void channelNotReady(ReadableByteChannel channel,ChannelReadyCallback callback);
    /*
    public void channelNotReady(WritableByteChannel channel);
    */
}
