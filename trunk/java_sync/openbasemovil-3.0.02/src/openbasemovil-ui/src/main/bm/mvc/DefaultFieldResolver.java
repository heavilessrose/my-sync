package bm.mvc;

import bm.core.Properties;
import bm.core.mvc.View;
import bm.core.io.SerializationException;
import bm.db.*;
import bm.storage.RSException;

import bm.storage.RecordStoreFullException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 15-abr-2007 12:39:17
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Default field resolver tries to resolve fields using the view attachments and
 * a map of field names to table name and field name.<br/>
 * The map can be feeded manually or by means of a property file, using the
 * format:<br/>
 * 
 * <pre>
 * toresolve.table=target table name
 * toresolve.field=source field name
 * toresolve.realName=field real name on the target table
 * </pre>
 * 
 * Where <i>toresolve</i> is the field name to resolve, the target table is the
 * table that contains that field, and the source field is the name of the field
 * in the source table that points to the target row (the foreign key).<br/>
 * <b>Before using the DefaultFieldResolver, you must set the working
 * database.</b>
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DefaultFieldResolver implements FieldResolver {
	private static DefaultFieldResolver instance = new DefaultFieldResolver();

	private static Database database;
	private static Properties definitions = new Properties();
	private static Hashtable cache = new Hashtable(10);

	public static DefaultFieldResolver getInstance() {
		return instance;
	}

	protected DefaultFieldResolver() {
	}

	/**
	 * Register a definition from a resource in the class path.
	 * 
	 * @param resource
	 *            resource path
	 * @throws IOException
	 *             on errors
	 */
	public static void registerDefinition(final String resource)
			throws IOException {
		final Properties aux = new Properties();
		aux.load(aux.getClass().getResourceAsStream(resource));
		definitions.putAll(aux);
	}

	/**
	 * Load a definition from an input stream and add it to the current ones.
	 * 
	 * @param is
	 *            input stream
	 * @throws IOException
	 *             on errors
	 */
	public static void registerDefinition(final InputStream is)
			throws IOException {
		final Properties aux = new Properties(is);
		definitions.putAll(aux);
	}

	/**
	 * Get the working database.
	 * 
	 * @return working database
	 */
	public static Database getDatabase() {
		return database;
	}

	/**
	 * Set the working database.
	 * 
	 * @param database
	 *            working database
	 */
	public static synchronized void setDatabase(final Database database) {
		DefaultFieldResolver.database = database;
	}

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
	 * @throws RSException
	 *             on errors
	 * @return resolved object, null if not found
	 */
	public Object resolve(final View view, String fieldName, Row source)
			throws DBException, RecordStoreFullException, RSException {
		try {
			final Object attachment = view.getAttachment(fieldName);
			if (attachment != null) {
				return attachment;
			}

			final Properties definitions = DefaultFieldResolver.definitions;
			Row target = null;
			if (definitions.containsKey(fieldName + ".table")) {
				target = getTarget(fieldName, source);
			}
			try {
				return target != null ? target.getField(definitions
						.containsKey(fieldName + ".realName") ? definitions
						.getProperty(fieldName + ".realName") : fieldName)
						: null;
			} catch (InvalidFieldException e) {
				return null;
			}
		} catch (SerializationException e) {
			throw new DBException(e.getErrorNumber(), e);
		}
	}

	private Row getTarget(final String fieldName, final Row source)
			throws SerializationException, RecordStoreFullException,
			DBException, RSException {
		final Properties definitions = DefaultFieldResolver.definitions;
		final Long targetId = (Long) source.getField(definitions
				.getProperty(fieldName + ".field"));
		final String tableName = definitions.getProperty(fieldName + ".table");
		if (targetId == null || tableName == null) {
			return null;
		}
		Row target = (Row) cache.get(tableName);
		if (target != null && target.getId() != targetId.longValue()) {
			target = null;
		}
		if (target == null) {
			target = lookUpEntity(tableName, targetId);
			if (target != null) {
				cache.put(tableName, target);
			}
		}
		return target;
	}

	private static Row lookUpEntity(final String tableName, final Long id)
			throws SerializationException, RecordStoreFullException,
			DBException, RSException {
		final Table table = database.getTable(tableName.toLowerCase());
		if (table != null) {
			final RowSet rowSet = table.find("remote_id", id);
			if (rowSet != null && rowSet.size() > 0) {
				rowSet.next();
				return rowSet.getCurrent();
			} else {
				return null;
			}
		} else {
			return null;
		}
	}
}
