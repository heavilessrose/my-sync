package ixstrim.util.task;

/**
 * Abstract Executor for execute Runnable tasks.
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Aug 17, 2003
 */
public interface Executor {

    /**
     * Executing Runnable task. Blocking method - will wait until resource available.
     * @param task
     */
    public void execute(Runnable task);


    /**
     * @return true if free resources for executing task immediately available in Executor,
     * false otherwise.
     */
    public boolean isResourceAvailavle();

    /**
     * Wait until all executing tasks will finished
     */
    public void flush();

}
