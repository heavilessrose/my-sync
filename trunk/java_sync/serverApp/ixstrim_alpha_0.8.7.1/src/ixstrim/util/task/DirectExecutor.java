package ixstrim.util.task;

import java.util.HashSet;
import java.util.Set;

/**
 * Simple threads pool implementation.
 * Threads count will increased/decreased automatically if
 * min threads count < current threads count < max threads count
 *
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Aug 17, 2003
 */
public class DirectExecutor implements Executor {


    public static int DEFAULT_MAX_THREADS = 10;
    public static int DEFAULT_MIN_THREADS = 0;
    public static int DEFAULT_PRIORITY = Thread.NORM_PRIORITY;
    public static int DEFAULT_IDELE_TIME = 1000;


    private int minThreadsCount;
    private int maxThreadsCount;
    private int priority;
    private int idleTime;
    private String name;

    private Set activeTaskSet = new HashSet();
    private Set passiveTaskSet = new HashSet();
    private int threadNum = 1;

    private Object waitResource = new Object();

    /**
     *  Constructor with threads priority and max/min threads count.
     */
    public DirectExecutor(String name, int priority, int minThreads, int maxThreads, int ideleTime) {
        if(priority > Thread.MAX_PRIORITY || priority < Thread.MIN_PRIORITY)
            throw new RuntimeException("Invalid priority : " + priority);
        this.name = (name == null ? "noname" : name);
        this.minThreadsCount = minThreads;
        this.maxThreadsCount = maxThreads;
        this.priority = priority;
        this.idleTime = ideleTime;
    }

    /**
     *  Constructor with threads priority and max threads count.
     */
    public DirectExecutor(int priority, int maxThreads) {
        this(null, priority, DEFAULT_MIN_THREADS, maxThreads, DEFAULT_IDELE_TIME);
    }

    /**
     *  Constructor with threads priority.
     */
    public DirectExecutor(int priority) {
        this(priority, DEFAULT_MAX_THREADS);

    }

    /**
     * Default constructor.
     * Pool will created with default priority,
     * default max and min threads count.
     */
    public DirectExecutor() {
        this(DEFAULT_PRIORITY);
    }

    public synchronized boolean isResourceAvailavle() {
        return activeTaskSet.size() < maxThreadsCount;
    }

    /**
     * Executor.execute(Runnable task) method implementation.
     */
    public void execute(Runnable task) {
        while (!tryExecute(task)) {
            try {
                synchronized (waitResource) {
                    waitResource.wait();
                }
            } catch (InterruptedException e) {

            }
        }
    }

    public synchronized void flush() {
        synchronized (waitResource) {
            while (activeTaskSet.size() > 0) {
                try {
                    waitResource.wait();
                } catch (InterruptedException e) {
                    throw new RuntimeException("Interrupted Exception catched", e);
                }
            }
        }
    }

    /**
     * Executor.execute(Runnable task) method implementation.
     */
    private synchronized boolean tryExecute(Runnable task) {
        if(passiveTaskSet.size() > 0) {
            Task t = (Task)passiveTaskSet.iterator().next();
            passiveTaskSet.remove(t);
            activeTaskSet.add(t);
            t.execute(task);
        } else {
            if (activeTaskSet.size() >= maxThreadsCount) {
                return false;
            } else {
            Task t = new Task();
            activeTaskSet.add(t);
            Thread thread = new Thread(t);
                thread.setName("Executor-" + name + "-Thread-" + (threadNum++) + ")");
            thread.setDaemon(false);
            thread.setPriority(priority);
            thread.start();
            t.execute(task);
        }
    }
        return true;
    }


    private class Task implements Runnable {
        private Runnable task;
        private long lastWorkTime;

        public Task() {
            this.lastWorkTime = System.currentTimeMillis();
        }

        public synchronized void execute(Runnable task) {
            this.task = task;
            notify();
        }


        public void run() {
            while(true) {
                if(this.task != null) {
                    try {
                        task.run();
                    } catch(Exception e) {
                        e.printStackTrace();
                    } finally {
                        this.task = null;
                        this.lastWorkTime = System.currentTimeMillis();
                        synchronized(DirectExecutor.this) {
                            activeTaskSet.remove(this);
                            passiveTaskSet.add(this);
                        }
                        //notify when work finished
                        synchronized (waitResource) {
                            waitResource.notify();
                        }
                    }
                } else {
                    if(System.currentTimeMillis() - lastWorkTime >= idleTime) {
                        synchronized(DirectExecutor.this) {
                            if (this.task != null) continue;//check again in synchronized section
                            if (passiveTaskSet.size() + activeTaskSet.size() > minThreadsCount) {
                                passiveTaskSet.remove(this);
                                //notify when thread finishing
                                synchronized (waitResource) {
                                    waitResource.notify();
                                }
                                return;
                            } else {
                                lastWorkTime = System.currentTimeMillis();
                            }
                        }
                    } else {
                        try {
                            long time = idleTime - (System.currentTimeMillis() - lastWorkTime);
                            if(time <= 0) time = 20;
                            synchronized(this) {
                                wait(time);
                            }
                        } catch(InterruptedException e) {/*do nothing*/
                        }
                    }
                }
            }
        }
    }


}





