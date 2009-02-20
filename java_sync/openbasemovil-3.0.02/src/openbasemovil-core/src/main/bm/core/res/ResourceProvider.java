package bm.core.res;

/*
 * File Information
 *
 * Created on       : 15-feb-2007 23:32:56
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * A ResourceProvider provides access to a resource store.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public interface ResourceProvider {
	/**
	 * Set the active locale for resource look up.
	 * 
	 * @param locale
	 *            active locale
	 */
	void setLocale(String locale);

	/**
	 * Get a resource given it's key.
	 * 
	 * @param key
	 *            resource key
	 * @return resource if found, null if not found.
	 */
	String getResource(String key);

	/**
	 * Open any underlying resource so subsequent calls to getResource go
	 * faster.
	 */
	void open();

	/**
	 * Close any underlying resource previously open.
	 */
	void close();
}
