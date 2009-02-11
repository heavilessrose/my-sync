package ixstrim.context;

import ixstrim.config.ConnectionContextConfig;
import ixstrim.net.http.HttpRequestDispatcher;
import ixstrim.util.BuffersPool;
import ixstrim.util.StringUtil;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class ConnectionContext {

    private BuffersPool buffersPool;
    private StringUtil stringUtil;
    private HttpRequestDispatcher requestDispatcher;

    public ConnectionContext(ConnectionContextConfig config) {
        this.buffersPool = new BuffersPool(
                config.getBuffersPoolSize(),
                config.getBuffersPoolLimit());
        this.stringUtil = new StringUtil();
    }

    public void setRequestDispatcher(HttpRequestDispatcher requestDispatcher) {
        this.requestDispatcher = requestDispatcher;
    }

    public BuffersPool getBuffersPool() {
        return buffersPool;
    }

    public StringUtil getStringUtil() {
        return stringUtil;
    }

    public HttpRequestDispatcher getRequestDispatcher() {
        return requestDispatcher;
    }

}
