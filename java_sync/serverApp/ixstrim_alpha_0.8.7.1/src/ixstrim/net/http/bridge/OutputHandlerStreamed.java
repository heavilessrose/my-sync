package ixstrim.net.http.bridge;

import ixstrim.net.http.OutputHandler;
import ixstrim.context.RequestContext;
import ixstrim.util.FixedBuffer;
import ixstrim.util.BuffersPool;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.io.InputStream;
import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 11, 2003
 */
public class OutputHandlerStreamed implements OutputHandler{

    private RequestContext requestContext;
    private ArrayList list = new ArrayList();
    private BuffersPool pool;
    private InputStream is;

    public OutputHandlerStreamed(RequestContext requestContext){
        this.requestContext = requestContext;
        this.pool = requestContext.getConnectionContext().getBuffersPool();
    }

    public void write(ByteBuffer bb) {
        synchronized(list){
            while(bb.hasRemaining()){
                if(list.size() > 5)return;//todo : change this hardcode check ?
                checkCurrentBuffer();
                FixedBuffer currentBuffer = (FixedBuffer)list.get(list.size() - 1);
                currentBuffer.appendPosible(bb);
            }
            list.notify();
        }
    }

    private void checkCurrentBuffer() {
        int listSize = list.size();
        if(listSize == 0 || ((FixedBuffer)list.get(listSize - 1)).getFree() == 0){
            list.add(pool.allocate());
        }
    }

    public synchronized InputStream getInputStream(){
        if(is == null){
            is = new InputStreamImpl();
        }
        return is;
    }

    private class InputStreamImpl extends InputStream{
        private int pos = 0;
        public int read() throws IOException {
            synchronized(list){
                if(list.size() == 0){
                    requestContext.getCallback().readyForInput();
                }
                while(list.size() == 0){
                    try{
                        list.wait();
                    }catch(InterruptedException e){/*do nothing*/}
                }
                FixedBuffer buff = (FixedBuffer)list.get(0);//must be not empty
                byte[] array = buff.getArray();
                if((pos + 1) == buff.getLength()){
                    list.remove(0);
                    pos = 0;
                    return array[buff.getLength() - 1];
                }else{
                    return array[pos++];
                }
            }
        }
    }
}
