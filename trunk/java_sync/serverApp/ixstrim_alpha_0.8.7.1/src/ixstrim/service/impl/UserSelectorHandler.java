package ixstrim.service.impl;

import ixstrim.net.http.bridge.ChannelNotReadyCallback;
import ixstrim.net.http.bridge.ChannelReadyCallback;
import ixstrim.event.EventDispatcher;
import ixstrim.event.ServerErrorEvent;
import ixstrim.context.ApplicationContext;

import java.nio.channels.*;
import java.util.Set;
import java.util.Iterator;
import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Dec 17, 2003
 */
public class UserSelectorHandler implements ChannelNotReadyCallback,Runnable{

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private Selector selector;

    public void channelNotReady(ReadableByteChannel channel,ChannelReadyCallback callback) {
        try {
            ((SelectableChannel)channel).register(selector,SelectionKey.OP_READ,callback);
        } catch(ClosedChannelException e) {
            eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
        }
    }

    public UserSelectorHandler()throws IOException{
        this.selector = Selector.open();
    }

    public void run() {
        while(true) {
            try {
                selector.select(100);
                Set set = selector.selectedKeys();
                if(set.size() == 0) continue;
                Iterator iterator = set.iterator();
                while(iterator.hasNext()) {
                    SelectionKey key = (SelectionKey)iterator.next();
                    try {
                        if(key.isReadable()) {
                            key.cancel();
                            ((ChannelReadyCallback)key.attachment()).channelReady();
                        }
                        /*
                        if(key.isValid() && key.isWritable()) {
                        }
                        */
                    } catch(Exception e) {
                        eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                        try {
                            key.cancel();
                        } catch(Exception ex) {/*do nothing*/}
                    }
                }
                set.clear();
            } catch(Exception e) {
                eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
            }
        }
    }


}
