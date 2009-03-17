import java.util.*;

/**
 * Contains the pool of treads to execute differet jobs represented by runnable
 * classes.
 * 
 * @author Serguei Eremenko
 * @version $Revision: 1.3 $, $Date: 2002/11/05 12:57:30 $
 */

public class ExecutorPool implements Haltable {

	public ExecutorPool(int capacity, int capacityIncrement) {
		executors = new Vector();
		pendingJobs = new Vector();
		executorGroup = new ThreadGroup("ExecutorPool");
		executorGroup.setMaxPriority(Thread.MAX_PRIORITY);
		this.capacity = capacity;
		this.capacityIncrement = capacityIncrement;
		executorRemoveCount = 0;
		errorHandler = new DefaultErrorHandler(System.out);
	}

	public ExecutorPool(int capacity) {
		this(capacity, 1);
	}

	public ExecutorPool() {
		this(10, 1);
	}

	public synchronized void execute(Runnable task, int priority, String name)
			throws ExecutorException {
		if (task == null)
			throw new ExecutorException("No task to execute!");
		if (priority < Thread.MIN_PRIORITY || priority > Thread.MAX_PRIORITY)
			throw new ExecutorException("Priority is incorrect: " + priority);
		ensureExecutorCapacity();
		pendJob(new Job(task, priority, name));
	}

	public synchronized void execute(Runnable task, int priority)
			throws ExecutorException {
		execute(task, priority, task.getClass().getName());
	}

	public synchronized int getExecutorPoolSize() {
		return executors.size();
	}

	public synchronized int getExecutorPoolCapacity() {
		return capacity;
	}

	public synchronized void setExecutorPoolSize(int newSize) {
		if (newSize > executors.size()) {
			addExecutorThread(newSize - executors.size());
		} else {
			removeExecutorThread(executors.size() - newSize);
		}
	}

	public synchronized void clearExecutorPool() {
		removeExecutorThread(executors.size());
		pendingJobs.clear();
	}

	public void halt() {
		clearExecutorPool();
	}

	public void setErrorHandler(ErrorHandler errorHandler) {
		this.errorHandler = errorHandler;
	}

	public ErrorHandler getErrorHandler() {
		return errorHandler;
	}

	protected Job getNextJob() {
		if (pendingJobs.size() == 0)
			return null;
		return (Job) pendingJobs.remove(0);
	}

	private void addExecutorThread(int size) {
		int currSize = executors.size();
		for (int i = 0; i < size; i++) {
			executors.add(new Executor("Executor_" + (currSize + i + 1)));
		}
	}

	protected void removeExecutorThread(int size) {
		synchronized (executorRemoveCountLock) {
			executorRemoveCount = size;
		}
		synchronized (executorLock) {
			executorLock.notifyAll();
		}
	}

	protected void ensureExecutorCapacity() {
		try {
			if (executors.size() < capacity) { // add a new executor
				addExecutorThread(1);
			} else {
				if (capacityIncrement > 0) { // increase the capacity
					capacity += capacityIncrement;
					addExecutorThread(1);
				}// executor pool does not grow if capacityIncrement is 0
			}
		} catch (Throwable t) {
		}
	}

	protected void pendJob(Job j) throws ExecutorException {
		if ((pendingJobs.size() + 1) >= MAX_PENDING_JOB_CAPACITY)
			throw new TooManyPendingJobsException();
		try {
			// Takes into account the task priority
			if (pendingJobs.size() == 0
					|| j.priority <= ((Job) pendingJobs.lastElement()).priority) {
				pendingJobs.add(j);
			} else if (j.priority > ((Job) pendingJobs.firstElement()).priority) {
				pendingJobs.add(0, j);
			} else {
				for (int i = (pendingJobs.size() - 2); i > 0; i--) {
					if (j.priority > ((Job) pendingJobs.elementAt(i)).priority
							&& j.priority <= ((Job) pendingJobs
									.elementAt(i - 1)).priority) {
						pendingJobs.add(i, j);
						return;
					} else if (j.priority == ((Job) pendingJobs.elementAt(i)).priority) {
						pendingJobs.add(i + 1, j);
						return;
					} else {
					}
				}
			}
			synchronized (executorLock) {
				executorLock.notifyAll();
			}
		} catch (Throwable t) {
			throw new ExecutorException(t.toString());
		}
	}

	protected final int MAX_PENDING_JOB_CAPACITY = 500;
	protected int capacity;
	protected int capacityIncrement;
	protected int executorRemoveCount;
	protected final Object executorLock = new Object();
	protected final Object executorRemoveCountLock = new Object();
	protected Vector executors;
	protected Vector pendingJobs;
	protected ThreadGroup executorGroup;
	protected ErrorHandler errorHandler;

	protected class Job {

		public Job(Runnable task, int priority, String name) {
			this.task = task;
			this.priority = priority;
			if (name == null)
				name = "";
			this.name = name;
		}

		public final Runnable task;
		public final int priority;
		public final String name;
	}

	protected class Executor extends Thread implements Haltable {

		public Executor(String name) {
			super(executorGroup, name);
			setPriority(Thread.NORM_PRIORITY);
			this.name = name;
			start();
		}

		public void run() {
			isActive = true;
			Job job = null;
			while (isActive) {
				try {
					if (isRemoved())
						return;
					while ((job = getNextJob()) == null) {
						synchronized (executorLock) {
							executorLock.wait();
						}
						if (isRemoved() || !isActive)
							return;
					}
					setName(name + ":" + job.name);
					setPriority(job.priority);
					job.task.run();
				} catch (Throwable t) {
					if (errorHandler != null)
						errorHandler.onError(name, t);
				} finally {
					job = null;
					setName(name);
					setPriority(Thread.NORM_PRIORITY);
				}
			}// executor stopped
		}

		public void halt() {
			isActive = false;
			synchronized (executorLock) {
				executorLock.notifyAll();
			}
		}

		protected boolean isRemoved() {
			if (executorRemoveCount > 0) {
				synchronized (executorRemoveCountLock) {
					executorRemoveCount--;
				}
				synchronized (executors) {
					executors.remove(this);
				}
				// System.out.println(this.getName()+" stopped");
				return true;
			}
			return false;
		}

		public String name;
		public volatile boolean isActive;
	}
}