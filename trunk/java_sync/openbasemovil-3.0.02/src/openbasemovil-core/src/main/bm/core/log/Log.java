/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;

/**
 * Log system interface.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface Log {
	/**
	 * Log a debug message.
	 * 
	 * @param message
	 *            message to log
	 */
	void debug(String message);

	/**
	 * Log a debug message.
	 * 
	 * @param message
	 *            message to log
	 * @param e
	 *            throwable associated
	 */
	void debug(String message, Throwable e);

	/**
	 * Log a debug message.
	 * 
	 * @param e
	 *            throwable
	 */
	void debug(Throwable e);

	/**
	 * Log an information message.
	 * 
	 * @param message
	 *            message to log
	 */
	void info(String message);

	/**
	 * Log a warning message.
	 * 
	 * @param message
	 *            message to log
	 */
	void warn(final String message);

	/**
	 * Log a warning message.
	 * 
	 * @param message
	 *            message to log
	 * @param e
	 *            throwable associated
	 */
	void warn(String message, Throwable e);

	/**
	 * Log a warning message.
	 * 
	 * @param e
	 *            throwable
	 */
	void warn(Throwable e);

	/**
	 * Log an error message.
	 * 
	 * @param message
	 *            message to log
	 */
	void error(final String message);

	/**
	 * Log an error message.
	 * 
	 * @param message
	 *            message to log
	 * @param e
	 *            throwable associated
	 */
	void error(String message, Throwable e);

	/**
	 * Log an error message.
	 * 
	 * @param e
	 *            throwable
	 */
	void error(Throwable e);

	/**
	 * Log a fatal error message.
	 * 
	 * @param message
	 *            message to log
	 */
	void fatal(final String message);

	/**
	 * Log a fatal error message.
	 * 
	 * @param message
	 *            message to log
	 * @param e
	 *            throwable associated
	 */
	void fatal(String message, Throwable e);

	/**
	 * Log a fatal error message.
	 * 
	 * @param e
	 *            throwable
	 */
	void fatal(Throwable e);

	/**
	 * Get log contents.
	 * 
	 * @return content or null if not supported
	 */
	public String getContent();

	/**
	 * Clear log contents.
	 */
	public void clear();
}
