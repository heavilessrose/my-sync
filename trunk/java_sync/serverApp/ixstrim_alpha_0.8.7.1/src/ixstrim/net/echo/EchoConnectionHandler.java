package ixstrim.net.echo;

import ixstrim.net.ConnectionHandler;

import java.nio.ByteBuffer;
import java.util.Vector;

/**
 * Example class
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class EchoConnectionHandler extends ConnectionHandler {
    private Vector v = new Vector();

    public void write(ByteBuffer bb) {
        byte[] bytes = new byte[bb.remaining()];
        bb.get(bytes);
        v.add(ByteBuffer.wrap(bytes));
        readyForOutput();
    }

    public ByteBuffer read() {
        if(v.size() == 0) return null;
        ByteBuffer result = (ByteBuffer)v.remove(0);
        if(result == null) return null;
        return result;
    }

}