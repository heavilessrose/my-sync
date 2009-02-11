package ixstrim.service.impl;

import ixstrim.service.HttpService;
import ixstrim.service.AsyncHttpServlet;
import ixstrim.net.http.HttpRequestDispatcher;
import ixstrim.net.http.server.SingleHttpServer;
import ixstrim.context.RequestContext;
import ixstrim.context.ApplicationContext;
import ixstrim.event.ServerErrorEvent;
import ixstrim.config.HttpServerConfig;
import ixstrim.util.task.Executor;
import ixstrim.util.task.DirectExecutor;

import javax.servlet.http.HttpServlet;
import java.util.Map;
import java.util.HashMap;
import java.io.IOException;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 20, 2003
 */
public class HttpServiceImpl implements HttpService,HttpRequestDispatcher{

    private Map servlets = new HashMap();
    private UserSelectorHandler selectorHandler;
    private Executor executor = new DirectExecutor();


    public HttpServiceImpl(HttpServerConfig config)throws IOException{
        new SingleHttpServer(config,this);
        selectorHandler = new UserSelectorHandler();
        Thread thread = new Thread(selectorHandler);
        thread.setName("UserSelectorHandler-Thread");
        thread.start();
    }

    public void registerServlet(String path, HttpServlet servlet) {
        servlets.put(path,servlet);
    }

    public void unregister(String path) {
        servlets.remove(path);
    }

    public void dispatch(RequestContext requestContext) {
        Object obj = servlets.get(requestContext.getRequest().getURI());
        if(obj != null){
            try {
                final HttpRequestImpl httpRequest = new HttpRequestImpl(requestContext);
                final HttpResponseImpl httpResponse = new HttpResponseImpl(requestContext);
                httpResponse.setSelectorHandler(selectorHandler);
                if(obj instanceof AsyncHttpServlet){
                    requestContext.setAsynchronousProcessing(true);
                    AsyncHttpServlet servlet = (AsyncHttpServlet)obj;
                    servlet.service(httpRequest,httpResponse);
                }else{
                    requestContext.setAsynchronousProcessing(false);
                    final HttpServlet servlet = (HttpServlet)obj;
                    executor.execute(
                            new Runnable(){
                                public void run(){
                                    try{
                                        servlet.service(httpRequest,httpResponse);
                                    }catch(Exception e){/*Do nothing*/}//todo : Exception handling?
                                }
                            }
                    );
                }
            } catch(Exception e) {
                ApplicationContext.getDefaultApplicationContext()
                        .getEventDispatcher().errorOccured(new ServerErrorEvent(this,e)
                );
            }
        }
    }
}
