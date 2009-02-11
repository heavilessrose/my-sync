package ixstrim.config;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public interface HttpServerConfig {
    public int getPort();
    public AcceptorConfig getAcceptorConfig();
    public HttpAcceptorCallbackConfig getHttpAcceptorCallbackConfig();
}
