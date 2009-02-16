package bm.mvc;

import bm.storage.RecordStoreFullException;

import bm.db.DBException;
import bm.db.ScrollSet;
import bm.storage.RSException;

/*
 * File Information
 *
 * Created on       : 09-ene-2006 17:13:50
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A ScrollerView can scroll results through a ScrollSet.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface ScrollerView {
	/**
	 * Set the scroll set.
	 * 
	 * @param scrollSet
	 *            scroll set
	 * @throws RecordStoreFullException
	 *             if rs is out of space
	 * @throws DBException
	 *             on db errors
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	void setScrollSet(ScrollSet scrollSet) throws DBException,
			RecordStoreFullException, RSException;

	/**
	 * Get the scroll set.
	 * 
	 * @return scroll set
	 */
	ScrollSet getScrollSet();
}
