package org.kalmeo.util.worker;

/**
 * @author bbeaulant
 */
public interface WorkerTask {

	/**
	 * Call by the {@link Worker} when the {@link WorkerTask} need to be
	 * execute.
	 * 
	 * @return <code>true</code> if the task need to be removed after execute
	 */
	public boolean run();

}
