package ixstrim.net;


import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import ixstrim.config.SelectorHandlerConfig;
import ixstrim.event.EventDispatcher;
import ixstrim.event.ServerErrorEvent;
import ixstrim.context.ApplicationContext;

/**
 * This class handle Selector which contain SocketChannels with interest options
 * OP_READ and OP_WRITE. Each channel can change it's interest options in runtime.
 * SocketChannels change it's own status if underlyed processors can't output
 * produce data, or can't accept incoming data - then corresponding interest will
 * reseted, and can be setted again via special IOCallback
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class SelectorHandler implements Runnable {

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();

    private Selector selector;
    private HashSet newChannels = new HashSet();
    private HashSet writableKeys = new HashSet();
    private HashSet readableKeys = new HashSet();
    private ConnectionHandlerFactory factory;
    private ByteBuffer readData;
    private int selectTimeout;

    /**
     * @param factory is ConnectionHandlerFactory which will produce
     * new ConnectioHandlers for accepted connections
     * @throws IOException
     */
    public SelectorHandler(SelectorHandlerConfig config,ConnectionHandlerFactory factory) throws IOException {
        int readBuffSize = config.getSelectorBufferSize();
        this.selectTimeout = config.getSelectorTimeout();
        this.selector = Selector.open();
        this.factory = factory;
        this.readData = ByteBuffer.allocate(readBuffSize);
    }


    /**
     * Return Selector.
     */
    public Selector getSelector() {
        return selector;
    }

    /**
     * Register new SocketChannel for read-write-close handling.
     */
    public void register(SocketChannel ch) {
        synchronized(newChannels) {
            newChannels.add(ch);
        }
    }

    /**
     * Add channel's interest as writable.
     */
    public void markWritable(SelectionKey key) {
        synchronized(writableKeys) {
            writableKeys.add(key);
        }
    }

    /**
     * Add channel's interest as readable.
     */
    public void markReadable(SelectionKey key) {
        synchronized(readableKeys) {
            readableKeys.add(key);
        }
    }

    /**
     * Main processing cycle.
     */
    public void run() {
        while(true) {
            try {
                checkNewChannels();
                checkWritableChannels();
                checkReadableChannels();
                //seting timeout on select
                //if not activity from network then we will
                //register new channel in any case after timeout
                selector.select(selectTimeout);
                Set set = selector.selectedKeys();
                if(set.size() == 0) continue;
                Iterator iterator = set.iterator();
                while(iterator.hasNext()) {
                    SelectionKey key = (SelectionKey)iterator.next();
                    try {
                        if(key.isReadable()) {
                            handleReadableChannel(key);
                        }
                        if(key.isValid() && key.isWritable()) {
                            handleWritableChannel(key);
                        }
                    } catch(Exception e) {
                        eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                        //close channel if any exception occured
                        try {
                            closeChannel(key);
                        } catch(Exception ex) {/*do nothing*/
                        }
                    }
                }
                set.clear();
            } catch(Exception e) {
                eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
            }
        }
    }

    private void handleReadableChannel(SelectionKey key) throws IOException {
        SocketChannel ch = (SocketChannel)key.channel();
        int wasRead = -1;
        try {
            readData.clear();
            wasRead = ch.read(readData);
        } catch(IOException e) {
            //if occured IOException then wasRead will be -1 and channel will closed
        }
        if(wasRead == -1) {
            closeChannel(key);
        } else {
            if(wasRead > 0) {
                ConnectionHandler connectionHandler = (ConnectionHandler)key.attachment();
                readData.flip();
                boolean writedFully = connectionHandler.putWriteBuffer(readData);
                if(!writedFully) {
                    key.interestOps(key.interestOps() & (~SelectionKey.OP_READ));
                }
            }
        }
    }

    private void handleWritableChannel(SelectionKey key) throws IOException {
        SocketChannel ch = (SocketChannel)key.channel();
        ConnectionHandler connectionHandler = (ConnectionHandler)key.attachment();
        ByteBuffer writeData = connectionHandler.getReadBuffer();
        if(writeData != null && writeData.hasRemaining()) {
            try {
                ch.write(writeData);
                //check for data, if not exist then unmark or close key
                if(writeData.remaining() == 0) {
                    connectionHandler.getReadBuffer();
                    if(!connectionHandler.hasData()) cancelWriteOp(key);
                } else {
                    //for detect bug fixed in JDK
                    eventDispatcher.errorOccured(new ServerErrorEvent(this,
                            new Exception("Unexpected behavior. Data writed not fully. Is great, NEED testing !!!")));
                }
            } catch(IOException e) {
                //if can't write then close
                closeChannel(key);
            }
        } else {
            cancelWriteOp(key);
        }
    }


    private void cancelWriteOp(SelectionKey key) throws IOException {
        ConnectionHandler connectionHandler = (ConnectionHandler)key.attachment();
        if(connectionHandler.needClose()) {
            //close if have not data for write and need close
            closeChannel(key);
        } else {
            //remove OP_WRITE from key
            key.interestOps(key.interestOps() & (~SelectionKey.OP_WRITE));
        }
    }


    private void checkNewChannels() {
        synchronized(newChannels) {
            if(newChannels.size() == 0) return;
            Iterator iter = newChannels.iterator();
            while(iter.hasNext()) {
                SocketChannel ch = (SocketChannel)iter.next();
                SelectionKey key = null;
                try {
                    key = ch.register(selector, SelectionKey.OP_READ, null);
                } catch(ClosedChannelException e) {
                    eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                    continue;
                }
                ConnectionHandler connectionHandler = factory.create();
                connectionHandler.setContext(this, key);
                key.attach(connectionHandler);
            }
            newChannels.clear();
        }
    }

    private void checkWritableChannels() {
        synchronized(writableKeys) {
            if(writableKeys.size() == 0) return;
            Iterator iter = writableKeys.iterator();
            while(iter.hasNext()) {
                SelectionKey key = (SelectionKey)iter.next();
                if(key.isValid()) key.interestOps(key.interestOps() | SelectionKey.OP_WRITE);
            }
            writableKeys.clear();
        }
    }

    private void checkReadableChannels() {
        synchronized(readableKeys) {
            if(readableKeys.size() == 0) return;
            Iterator iter = readableKeys.iterator();
            while(iter.hasNext()) {
                SelectionKey key = (SelectionKey)iter.next();
                ConnectionHandler connectionHandler = (ConnectionHandler)key.attachment();
                if(key.isValid()) {
                    if(connectionHandler.flushWriteBuffer())
                        key.interestOps(key.interestOps() | SelectionKey.OP_READ);
                } else {
                    try {
                        closeChannel(key);
                    } catch(IOException e) {
                        eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
                    }
                }
            }
            readableKeys.clear();
        }
    }

    private void closeChannel(SelectionKey key) throws IOException {
        ((ConnectionHandler)key.attachment()).destroy();
        key.channel().close();
    }

}
