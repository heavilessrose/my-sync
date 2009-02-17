/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;

/**
 * Create and configure Log instances, using the link time class provided for
 * implementation.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class LogFactory {
	/**
	 * Get a new Log instance for the given package/class name.<br/>
	 * The method will try to find a suitable Log implementation linked to the
	 * application. It's name is fixed: <code>bm.core.log.LogImpl</code>
	 * 
	 * @param name
	 *            package/class name
	 * @return Log instance, null if no suitable Log implementation found.
	 */
	public static Log getLog(final String name) {
		try {
			final Class clazz = Class.forName("bm.core.log.ConsoleLogImpl"); // [Commenter-Don't]
			final Log log = (Log) clazz.newInstance(); // [Commenter-Don't]
			((LogConfigurator) log).setName(name);
			return log;
		} catch (Exception e) {
			return null;
		}
	}
}
