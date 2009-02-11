package ixstrim.examples.servlet;

import ixstrim.service.AsyncHttpServlet;
import ixstrim.service.AsyncHttpServletRequest;
import ixstrim.service.AsyncHttpServletResponse;

import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.net.InetSocketAddress;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class AsyncChannelServlet extends AsyncHttpServlet{

    public void service(AsyncHttpServletRequest request,AsyncHttpServletResponse response){
        try{
            ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
            try{
                serverSocketChannel.socket().bind(new InetSocketAddress(1111));
                SocketChannel ch = serverSocketChannel.accept();
                response.setChannel(ch);
            }finally{
                serverSocketChannel.close();
            }
        }catch(Exception e){
            e.printStackTrace();
        }
    }
}
