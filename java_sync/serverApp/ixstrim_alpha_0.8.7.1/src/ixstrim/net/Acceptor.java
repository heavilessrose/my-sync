package ixstrim.net;

import ixstrim.config.AcceptorConfig;
import ixstrim.context.ApplicationContext;
import ixstrim.event.EventDispatcher;
import ixstrim.event.ServerErrorEvent;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.*;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * Acceptor listen some TCP ports and accepts SocketChannels.
 * Then send accepted channels to AcceptorCallback specified for corresponding port
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class Acceptor implements Runnable {

    private static final String ACCEPTOR_THREAD_NAME = "AcceptorThread";

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private Selector selector;
    private boolean started = false;
    private int selectTimeout;
    private Set newChannels = new HashSet();


    public Acceptor(AcceptorConfig config) throws IOException {
        this.selector = Selector.open();
        this.selectTimeout = config.getTimeout();
    }

    /**
     * Register new ServerSocketChannel and bind it to special port.
     * All ServerSocketChannels must binded before Acceptor start.
     */
    public void register(int port, AcceptorCallback callback) throws IOException {
        if(callback == null) throw new NullPointerException("Callback can't be null");
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        serverSocketChannel.configureBlocking(false);
        synchronized(newChannels) {
            newChannels.add(new Object[]{serverSocketChannel, callback});
        }
    }

    /**
     * Main acceptor cycle.
     */
    public void run() {
        synchronized(this) {
            if(started)
                throw new RuntimeException("This Acceptor already started");
            else
                started = true;
        }
        Thread.currentThread().setName(ACCEPTOR_THREAD_NAME);
        while(true) {
            try {
                checkNewChannels();
                selector.select(selectTimeout);
                Set set = selector.selectedKeys();
                if(set.size() == 0) continue;
                Iterator iterator = set.iterator();
                while(iterator.hasNext()) {
                    SelectionKey key = (SelectionKey)iterator.next();
                    if(key.isAcceptable()) {
                        ServerSocketChannel ch = (ServerSocketChannel)key.channel();
                        SocketChannel sch = ch.accept();
                        if(sch != null) {
                            AcceptorCallback callback = (AcceptorCallback)key.attachment();
                            callback.accepted(sch);
                        }
                    }
                }
                set.clear();
            } catch(Exception e) {
                eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
            }
        }
    }

    private void checkNewChannels() {
        synchronized(newChannels) {
            if(newChannels.size() == 0) return;
            Iterator iter = newChannels.iterator();
            while(iter.hasNext()) {
                Object[] pair = (Object[])iter.next();
                ServerSocketChannel serverSocketChannel = (ServerSocketChannel)pair[0];
                AcceptorCallback callback = (AcceptorCallback)pair[1];
                try {
                    serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT, callback);
                } catch(ClosedChannelException e) {
                    eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                    continue;
                }
            }
            newChannels.clear();
        }
    }

}