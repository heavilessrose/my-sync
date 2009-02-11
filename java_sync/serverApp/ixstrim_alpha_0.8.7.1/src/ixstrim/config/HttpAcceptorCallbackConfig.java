package ixstrim.config;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Nov 16, 2003
 */
public interface HttpAcceptorCallbackConfig {
    public int getSelectorHandlersCount();
    public ConnectionContextConfig getConnectionContextConfig();
    public SelectorHandlerConfig getSelectorHandlerConfig();
}
