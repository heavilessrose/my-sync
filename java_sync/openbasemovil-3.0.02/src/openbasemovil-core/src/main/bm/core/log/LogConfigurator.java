/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;

/**
 * LogImpl must implement this interface in order to be able to configure the
 * instance, as the Log interface must remain clean for logging purposes.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface LogConfigurator {
	/**
	 * Set the name (class or package) that this instance represents.
	 * 
	 * @param name
	 *            instance name
	 */
	void setName(String name);
}
