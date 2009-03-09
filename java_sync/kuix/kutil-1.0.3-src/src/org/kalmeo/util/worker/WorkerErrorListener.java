package org.kalmeo.util.worker;

/**
 * @author omarino
 */
public interface WorkerErrorListener {

	/**
	 * The catched error in Worker loop
	 * 
	 * @param task
	 * @param error
	 */
	public void onWorkerError(WorkerTask task, Error error);

	/**
	 * The catched exception in Worker loop
	 * 
	 * @param exception
	 * @param task
	 */
	public void onWorkerException(WorkerTask task, Exception exception);

}
