package ixstrim.examples;

import ixstrim.config.*;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public class Config implements AcceptorConfig,
        ConnectionContextConfig,HttpAcceptorCallbackConfig
        ,HttpServerConfig,SelectorHandlerConfig{

    private int port = 80;

    public int getTimeout() {
        return 5;
    }

    public int getSelectorHandlersCount() {
        return 1;
    }

    public ConnectionContextConfig getConnectionContextConfig() {
        return this;
    }

    public SelectorHandlerConfig getSelectorHandlerConfig() {
        return this;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public int getPort() {
        return port;
    }

    public AcceptorConfig getAcceptorConfig() {
        return this;
    }

    public HttpAcceptorCallbackConfig getHttpAcceptorCallbackConfig() {
        return this;
    }

    public int getSelectorBufferSize() {
        return 8192;
    }

    public int getSelectorTimeout() {
        return 5;
    }

    public int getBuffersPoolSize() {
        return 4096;
    }

    public int getBuffersPoolLimit() {
        return 50;
    }
}
