package ixstrim.examples.handler;

import ixstrim.net.http.server.SingleHttpServer;
import ixstrim.context.ApplicationContext;
import ixstrim.examples.Config;
import ixstrim.examples.LogListener;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 17, 2003
 */
public class HttpServer {

    public static void main(String args[]) throws Exception {
        if(args.length == 0) {
            System.err.println("Please specify listening port");
            System.exit(0);
        }
        int port = Integer.parseInt(args[0]);
        Config config = new Config();
        config.setPort(port);
        new SingleHttpServer(config, new HttpRequestDispatcherImpl());
        ApplicationContext.getDefaultApplicationContext()
                .getEventDispatcher().register(
                new LogListener()
        );
    }

}
