package bm.mvc;

import bm.db.Row;
import bm.db.DBException;
import bm.storage.RSException;

import bm.storage.RecordStoreFullException;
import bm.core.mvc.View;

/*
 * File Information
 *
 * Created on       : 20-mar-2007 3:40:07
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Used from data bound views to resolve fields not found in the binded row.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface FieldResolver {
	/**
	 * Resolve the field name for a given row.
	 * 
	 * @param view
	 *            view requesting resolution
	 * @param fieldName
	 *            field name
	 * @param source
	 *            the row @return value or null if not known
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 * @return resolved object, null if not found
	 */
	Object resolve(final View view, String fieldName, Row source)
			throws DBException, RecordStoreFullException, RSException;
}
