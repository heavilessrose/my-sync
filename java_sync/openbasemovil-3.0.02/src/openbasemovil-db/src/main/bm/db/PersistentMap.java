package bm.db;

import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.SimpleCache;
import bm.core.ErrorHandler;
import bm.storage.Store;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;

import java.util.Hashtable;
import java.util.Enumeration;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

/*
 * File Information
 *
 * Created on       : 23-may-2007 14:25:04
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Persistent map on a recordstore.<br/>
 * Both keys and values must obey the rules and types imposed by
 * SerializerOutputStream.<br/>
 * The PersistentMap handles Row object in a different way than any other
 * object, as it takes into account the recordId, the table and database the row
 * belongs to and restores that information creating a valid row when read from
 * persistent storage.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class PersistentMap {
	private static final Log log = LogFactory.getLog("PersistentMap");

	protected String rsName;
	protected Hashtable directory = new Hashtable(10);
	protected SimpleCache cache = new SimpleCache(5);

	/**
	 * Create a new persistent map using the given Store name.
	 * 
	 * @param name
	 *            store name
	 */
	public PersistentMap(final String name) {
		this.rsName = name;
		Store st = null;
		try {
			// Load directory
			if (Store.exists(name)) {
				st = Store.get(name, 1);
				st.open(false);
				final byte[] data = st.getRecord(1);
				final ByteArrayInputStream bais = new ByteArrayInputStream(data);
				final DBSerializerInputStream in = new DBSerializerInputStream(
						bais);
				directory = (Hashtable) in.readObject();
			} else {
				st = saveDirectory();
			}
		} catch (Exception e) {
			log.error(e);
			ErrorHandler.handleError(name, e);
		} finally {
			if (st != null)
				try {
					st.close();
				} catch (Exception e) {
				}
		}
	}

	/**
	 * Proxy to the underlying Store packCopy method.
	 * 
	 * @throws RSException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 */
	public void packCopy() throws RSException, RecordStoreFullException {
		Store st = null;
		try {
			st = Store.get(rsName, 1);
			st.packCopy();
		} finally {
			if (st != null)
				try {
					st.close();
				} catch (Exception e) {
				}
		}
	}

	/**
	 * Get an enumeration of all the keys stored in the database, included the
	 * Preferences object.
	 * 
	 * @return enumeration of keys
	 */
	public Enumeration getKeys() {
		return directory.keys();
	}

	private Store saveDirectory() throws DBException {
		Store st = null;
		try {
			st = Store.get(rsName, 1);
			st.open(true);
			final ByteArrayOutputStream baos = new ByteArrayOutputStream();
			final DBSerializerOutputStream out = new DBSerializerOutputStream(
					baos);
			out.writeObject(directory);
			if (st.getNumRecords() > 0) {
				st.setRecord(1, baos.toByteArray());
			} else {
				st.addRecord(baos.toByteArray());
			}
			return st;
		} catch (Exception e) {
			log.error(e);
			throw new DBException(Constants.ERR_PM_SAVE_DIR, e);
		} finally {
			if (st != null)
				try {
					st.close();
				} catch (Exception e) {
				}
		}
	}

	/**
	 * Completly remove the data on the persistent map and reset it to an empty
	 * one.
	 * 
	 * @throws DBException
	 *             on errors removing record store
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 */
	public synchronized void clear() throws DBException,
			RecordStoreFullException {
		try {
			Store.safeDeleteRecordStore(rsName);
			cache.clear();
			directory.clear();
			saveDirectory();
		} catch (RSException e) {
			log.error(e);
			throw new DBException(Constants.ERR_PM_CLEAR, e);
		}
	}

	/**
	 * Get an object given it's key.
	 * 
	 * @param key
	 *            object key
	 * @return object or null if not found
	 * @throws DBException
	 *             on errors reading from persistent storage
	 */
	public Object getElement(final Object key) throws DBException {
		Store st = null;
		try {
			Object obj = cache.get(key);
			if (obj == null) {
				final Integer recordId = (Integer) directory.get(key);
				if (recordId != null) {
					st = Store.get(rsName, 1);
					st.open();
					final ByteArrayInputStream bais = new ByteArrayInputStream(
							st.getRecord(recordId.intValue()));
					final DBSerializerInputStream in = new DBSerializerInputStream(
							bais);
					obj = in.readObject();
					cache.add(key, obj);
				}
			}
			return obj;
		} catch (Exception e) {
			if (!(e instanceof DBException)) {
				throw new DBException(Constants.ERR_PM_GET, e);
			} else {
				throw (DBException) e;
			}
		} finally {
			if (st != null)
				try {
					st.close();
				} catch (Exception e) {
				}
		}
	}

	/**
	 * Set a new name-value pair.
	 * 
	 * @param key
	 *            key
	 * @param value
	 *            value
	 * @throws DBException
	 *             on errors writing to persistent storage
	 */
	public synchronized void setElement(final Object key, final Object value)
			throws DBException {
		final Hashtable directory = this.directory;
		Store st = null;
		try {
			if (value != null) {
				cache.add(key, value);
				final ByteArrayOutputStream baos = new ByteArrayOutputStream();
				final DBSerializerOutputStream out = new DBSerializerOutputStream(
						baos);
				out.writeObject(value);
				final byte[] data = baos.toByteArray();
				st = Store.get(rsName, 1);
				st.open(true);
				if (directory.containsKey(key)) {
					final Integer recordId = (Integer) directory.get(key);
					st.setRecord(recordId.intValue(), data);
				} else {
					final int recordId = st.addRecord(data);
					directory.put(key, new Integer(recordId));
					saveDirectory();
				}
			} else if (directory.containsKey(key)) {
				cache.remove(key);
				st = Store.get(rsName, 1);
				st.open(true);
				st.deleteRecord(((Integer) directory.get(key)).intValue());
				directory.remove(key);
				saveDirectory();
			}
		} catch (Exception e) {
			throw new DBException(Constants.ERR_PM_SET, e);
		} finally {
			if (st != null)
				try {
					st.close();
				} catch (Exception e) {
				}
		}
	}
}
