package ixstrim.net.http.bridge;

import ixstrim.net.http.InputHandler;
import ixstrim.context.RequestContext;
import ixstrim.util.FixedBuffer;

import java.nio.ByteBuffer;
import java.nio.channels.ReadableByteChannel;
import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 17, 2003
 */
public class InputHandlerChanneled  implements InputHandler,ChannelReadyCallback {

    private RequestContext requestContext;
    private ReadableByteChannel channel;
    private ChannelNotReadyCallback callback;
    private FixedBuffer buffer;

    public InputHandlerChanneled(RequestContext requestContext, ReadableByteChannel channel,ChannelNotReadyCallback callback) {
        this.requestContext = requestContext;
        this.channel = channel;
        this.callback = callback;
        channelReady();
    }

    public ByteBuffer read() {
        if(buffer == null){
            buffer = requestContext.getConnectionContext().getBuffersPool().allocate();
        }
        byte[] array = buffer.getArray();
        ByteBuffer byteBuffer = ByteBuffer.wrap(array);
        byteBuffer.clear();
        int wasRead = -1;
        try{
            wasRead = channel.read(byteBuffer);
        }catch(IOException e){
            throw new RuntimeException("IOException occured",e);
        }
        if(wasRead < 0){
            requestContext.getConnectionContext().getBuffersPool().free(buffer);
            buffer = null;
            requestContext.getCallback().close();
            return null;
        }
        byteBuffer.flip();
        if(!byteBuffer.hasRemaining()){
            callback.channelNotReady(channel,this);
        }
        return byteBuffer;
    }

    public void channelReady(){
        requestContext.getCallback().readyForOutput();
    }
}
