package ixstrim.examples.servlet;

import ixstrim.examples.Config;
import ixstrim.examples.LogListener;
import ixstrim.context.ApplicationContext;
import ixstrim.service.HttpService;
import ixstrim.service.impl.HttpServiceImpl;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
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
        HttpService service = new HttpServiceImpl(config);

        service.registerServlet("/hello",new HelloServlet());
        service.registerServlet("/ahello",new AsyncHelloServlet());
        service.registerServlet("/channel",new AsyncChannelServlet());
        service.registerServlet("/input",new InputStreamTestServlet());

        ApplicationContext.getDefaultApplicationContext()
                .getEventDispatcher().register(
                new LogListener()
        );
    }


}
