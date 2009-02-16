package bm.db.index;

import bm.core.ResourceManager;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.tools.Tools;
import bm.db.Constants;
import bm.db.DBException;
import bm.err.ErrorLog;
import bm.storage.RSException;
import bm.storage.Store;
import bm.storage.RecordStoreFullException;
import bm.storage.InvalidRecordIDException;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 06-jul-2005 13:38:16
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-10-19 12:25:27 +0200 (vie, 19 oct 2007) $
 * Revision         : $Revision: 16 $
 */

// ToDo: Node data should be independent from tree, and should be able to reside on a different record
/**
 * Balanced Tree Index for Tables.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 16 $
 */
public class Index implements Store.Listener {
	private static Log log = LogFactory.getLog("Index");

	private static final int NONE = -1;
	/**
	 * findFuzzy with all words
	 */
	public static final int AND = 0;
	/**
	 * findFuzzy with any words
	 */
	public static final int OR = 1;

	/**
	 * Index keys are of type String.
	 */
	public static final byte KT_STRING = IndexInfo.KT_STRING;
	/**
	 * Index keys are of type long.
	 */
	public static final byte KT_LONG = IndexInfo.KT_LONG;
	/**
	 * Index keys are of type String, but insert and delete result in more than
	 * one key index: one per word.
	 */
	public static final byte KT_FULL_TEXT = IndexInfo.KT_FULL_TEXT;

	/**
	 * The insert operation inserted nothing: both the key and the record id
	 * were already indexed.
	 */
	public static final int INSERTED_NOTHING = 0;
	/**
	 * The insert operation inserted just the record id, there was already a
	 * node with the same key.
	 */
	public static final int INSERTED_RECORD_ID = 1;
	/**
	 * The insert operation inserted a new node.
	 */
	public static final int INSERTED_NODE = 2;

	/**
	 * Default tree order.
	 */
	public static final int DEFAULT_ORDER = 2;
	/**
	 * Default start size for record id arrays when a new node in inserted.
	 */
	public static final int DEFAULT_RECORD_ID_ARRAY_INIT_SIZE = 16;
	/**
	 * Default growth factor for record id arrays.
	 */
	public static final int DEFAULT_RECORD_ID_ARRAY_GROW_SIZE = 16;

	private static final int GROWTH_FACTOR = 64;

	private static final int ROOT_NODE = 1;

	private static long wideAccesses;
	private static long wideAccumulatedTime;
	private static long wideFuzzyAccesses;
	private static long wideFuzzyAccumulatedTime;

	private final int order;
	private String name;
	private byte type;
	private boolean caseSensitive;
	boolean tableIndex = true;

	private boolean sendProgressEvents = true;

	private transient int recordIdArrayInit = DEFAULT_RECORD_ID_ARRAY_INIT_SIZE;
	private transient int recordIdArrayGrow = DEFAULT_RECORD_ID_ARRAY_GROW_SIZE;
	private transient Store rs;
	private transient byte[] buffer;
	private transient ByteArrayOutputStream baos = new ByteArrayOutputStream();
	private transient SerializerOutputStream out = new SerializerOutputStream(
			baos);
	private transient Hashtable toSave = new Hashtable(1);
	private transient ProgressEvent event;
	private transient Node rootNode;

	private transient long totalAccesses;
	private transient long accumulatedTime;
	private transient long totalFuzzyAccesses;
	private transient long accumulatedFuzzyTime;
	private static final String BLANK = " \t\r\n,.:;(){}";

	/**
	 * Construct a new Index for a table.
	 * 
	 * @param name
	 *            name of index and underlying record store
	 * @param order
	 *            tree order
	 * @param type
	 *            index type, one of the KT_* constants
	 * @param caseSensitive
	 *            if type is KT_STRING if the index is case sensitive
	 */
	public Index(final String name, final int order, final byte type,
			final boolean caseSensitive) {
		this(name, order, type, caseSensitive, true);
	}

	/**
	 * Construct a new Index.
	 * 
	 * @param name
	 *            name of index and underlying record store
	 * @param order
	 *            tree order
	 * @param type
	 *            index type, one of the KT_* constants
	 * @param caseSensitive
	 *            if type is KT_STRING if the index is case sensitive
	 * @param tableIndex
	 *            if the index is a table index or a generic index
	 */
	public Index(final String name, final int order, final byte type,
			final boolean caseSensitive, final boolean tableIndex) {
		this.order = order;
		this.name = name;
		this.type = type;
		this.caseSensitive = caseSensitive;
		this.tableIndex = tableIndex;
		rs = Store.get(name, 1);
		event = new ProgressEvent(this);
		event.setAnimate(true);
	}

	public boolean isSendProgressEvents() {
		return sendProgressEvents;
	}

	public void setSendProgressEvents(final boolean sendProgressEvents) {
		this.sendProgressEvents = sendProgressEvents;
	}

	/**
	 * Get underlying storage size in bytes.
	 * 
	 * @return size in bytes
	 */
	public int getSize() {
		return (int) rs.getSize();
	}

	/**
	 * Get index key type.
	 * 
	 * @return key type
	 */
	public int getType() {
		return type;
	}

	/**
	 * Get index order.
	 * 
	 * @return index order
	 */
	public int getOrder() {
		return order;
	}

	/**
	 * Get index name.
	 * 
	 * @return index name
	 */
	public String getName() {
		return name;
	}

	/**
	 * Check if the index is case sensitive.
	 * 
	 * @return true if so
	 */
	public boolean isCaseSensitive() {
		return caseSensitive;
	}

	/**
	 * Initial size for new record id arrays on new nodes.
	 * 
	 * @return initial size
	 * @see #DEFAULT_RECORD_ID_ARRAY_INIT_SIZE
	 */
	public int getRecordIdArrayInit() {
		return recordIdArrayInit;
	}

	/**
	 * Set the intial size for record id arrays.
	 * 
	 * @param recordIdArrayInit
	 *            size
	 * @see #DEFAULT_RECORD_ID_ARRAY_INIT_SIZE
	 */
	public void setRecordIdArrayInit(final int recordIdArrayInit) {
		this.recordIdArrayInit = recordIdArrayInit;
	}

	/**
	 * Get the growth factor for record id arrays.
	 * 
	 * @return growth factor
	 * @see #DEFAULT_RECORD_ID_ARRAY_GROW_SIZE
	 */
	public int getRecordIdArrayGrow() {
		return recordIdArrayGrow;
	}

	/**
	 * Set the growth factor for record id arrays.
	 * 
	 * @param recordIdArrayGrow
	 *            growth factor
	 * @see #DEFAULT_RECORD_ID_ARRAY_GROW_SIZE
	 */
	public void setRecordIdArrayGrow(final int recordIdArrayGrow) {
		this.recordIdArrayGrow = recordIdArrayGrow;
	}

	public int size() throws DBException, SerializationException,
			RecordStoreFullException {
		return getRootNode().size();
	}

	/**
	 * Injection for server side usage.
	 * 
	 * @param rs
	 *            record store
	 */
	public void setRecordStore(final Store rs) {
		this.rs = rs;
	}

	public Store getRecordStore() {
		return rs;
	}

	/**
	 * Shutdown index.
	 * 
	 * @throws RSException
	 *             on errors
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 */
	public void shutdown() throws RecordStoreFullException, RSException {
		rs.shutdown();
		rootNode = null;
	}

	/**
	 * Physically drop the index.
	 * 
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 */
	public synchronized void drop() throws RecordStoreFullException {
		try {
			rs.drop();
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "drop", null,
					"Error droping recordstore", e);
		}
	}

	/**
	 * Insert a recordId in a tableIndex.<br/>
	 * If a previous entry with that key exists, if it does not contain the
	 * recordId it's inserted into that key, if it's already contained in the
	 * entry, nothing is done.<br/>
	 * If no previous entry with that key exists, a new one is created and the
	 * recordId is inserted in it.<br/>
	 * 
	 * @param key
	 *            object key
	 * @param recordId
	 *            to insert
	 * 
	 * @return items inserted: INSERTED_NOTHING key and record id existed,
	 *         nothing inserted; INSERTED_RECORD_ID key existed, but record id
	 *         did not, inserted record id; INSERTED_NODE key did no exist, both
	 *         things inserted If the index is of full text type, the key is
	 *         splitted into words and each of them is inserted separately, thus
	 *         the return value is the highest produced.
	 * @noinspection MethodCallInLoopCondition
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 * @throws DBException
	 *             on errors
	 */
	public int insert(final Object key, final int recordId) throws DBException,
			RecordStoreFullException {
		return insertObject(key, new Integer(recordId));
	}

	/**
	 * Insert an object.<br/>
	 * If a previous entry with that key exists, if it does not contain the
	 * recordId it's inserted into that key, if it's already contained in the
	 * entry, nothing is done.<br/>
	 * If no previous entry with that key exists, a new one is created and the
	 * recordId is inserted in it.<br/>
	 * It the index is a generic one, then the index data will be just an object
	 * that will be checked for equality.
	 * 
	 * @param key
	 *            object key
	 * @param value
	 *            to insert
	 * 
	 * @return items inserted: INSERTED_NOTHING key and record id existed,
	 *         nothing inserted; INSERTED_RECORD_ID key existed, but record id
	 *         did not, inserted record id; INSERTED_NODE key did no exist, both
	 *         things inserted If the index is of full text type, the key is
	 *         splitted into words and each of them is inserted separately, thus
	 *         the return value is the highest produced.
	 * @noinspection MethodCallInLoopCondition
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 * @throws DBException
	 *             on errors
	 */
	public synchronized int insertObject(final Object key, final Object value)
			throws DBException, RecordStoreFullException {
		try {
			return doInsert(new IndexKey(key), value);
		} catch (DBException e) {
			markDamaged();
			throw e;
		} catch (RecordStoreFullException e) {
			markDamaged();
			throw e;
		}
	}

	private void markDamaged() {
		try {
			rs.setDamaged(true);
		} catch (RSException e) {
			ErrorLog.addError("Index", "markDamaged", null, null, e);
		}
	}

	private synchronized int doInsert(final IndexKey key, final Object value)
			throws DBException, RecordStoreFullException {
		try {
			rs.open();
			if (sendProgressEvents) {
				event.dispatch();
			}
			if (key.isNull() || type != KT_FULL_TEXT) {
				return insertSingle(key, value);
			} else {
				final Vector words = getWords(key.getString(), true);
				final int length = words.size();
				int insertMax = INSERTED_NOTHING;
				for (int i = 0; i < length; i++) {
					final int insert = insertSingle(new IndexKey(words
							.elementAt(i)), value);
					if (insert > insertMax) {
						insertMax = insert;
					}
				}
				return insertMax;
			}
		} catch (RSException e) {
			clear();
			log.error(e);
			ErrorLog.addError("Index", "doInsert", null,
					"Error inserting node", e);
			throw new DBException(Constants.ERR_IDX_INSERT_NODE);
		} catch (SerializationException e) {
			clear();
			log.error(e);
			ErrorLog.addError("Index", "doInsert", null,
					"Error inserting node", e);
			throw new DBException(Constants.ERR_IDX_INSERT_NODE);
		} finally {
			doClose("doInsert");
		}
	}

	private void doClose(final String method) {
		try {
			rs.close();
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", method, null,
					"Error closing recordstore", e);
		}
	}

	public void close() {
		try {
			rs.close();
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "close", null,
					"Error closing recordstore", e);
		}
	}

	public static Vector getWords(final String str, final boolean useStopWords) {
		final String stopWords = ResourceManager
				.getResource("db.index.stopWords");
		final char[] chars = str.toCharArray();
		final int length = chars.length;
		final StringBuffer buffer = new StringBuffer();
		final Vector words = new Vector(10);
		for (int i = 0; i < length; i++) {
			if (BLANK.indexOf(chars[i]) > -1) {
				if (buffer.length() > 0) {
					if (!useStopWords
							|| stopWords == null
							|| stopWords.indexOf("#" + buffer.toString() + "#") == -1) {
						words.addElement(buffer.toString());
					}
					buffer.delete(0, buffer.length());
				}
			} else {
				buffer.append(chars[i]);
			}
		}
		if (buffer.length() > 0) {
			if (!useStopWords || stopWords == null
					|| stopWords.indexOf("#" + buffer.toString() + "#") == -1) {
				words.addElement(buffer.toString());
			}
		}
		return words;
	}

	private int insertSingle(final IndexKey key, final Object value)
			throws DBException, RecordStoreFullException,
			SerializationException {
		final SearchResult searchResult = search(getRootNode(), key);
		if (searchResult.getNode() != null) {
			final NodeKey nodeKey = searchResult.getNode().getNodeKey(
					searchResult.getKeyIndex());
			if (tableIndex) {
				final int recordId = ((Integer) value).intValue();
				final SortedIntArray data = (SortedIntArray) nodeKey.getData();
				if (data.findIndex(recordId) == -1) {
					if (sendProgressEvents) {
						event.dispatch();
					}
					data.insert(recordId);
					saveNode(searchResult.getNode());
					return INSERTED_RECORD_ID;
				} else {
					return INSERTED_NOTHING;
				}
			} else {
				final Object data = nodeKey.getData();
				if (Tools.objectEquals(data, value)) {
					return INSERTED_NOTHING;
				} else {
					nodeKey.setData(value);
					saveNode(searchResult.getNode());
					return INSERTED_NODE;
				}
			}
		} else {
			final Object data;
			if (tableIndex) {
				data = new SortedIntArray(recordIdArrayInit, recordIdArrayGrow);
				((SortedIntArray) data).insert(((Integer) value).intValue());
			} else {
				data = value;
			}
			final NodeKey keyNode = new NodeKey(this, key, data);
			Node node = getRootNode();
			// noinspection MethodCallInLoopCondition
			while (!node.isLeaf()) {
				int i = 0;
				// noinspection MethodCallInLoopCondition
				while (keyNode.compareTo(node.getKey(i)) > 0) {
					i++;
					if (i == node.getKeyCount()) {
						break;
					}
				}
				node = node.getChildNode(i);
				if (sendProgressEvents) {
					event.dispatch();
				}
			}
			node.insert(keyNode, null);
			flush();
			return INSERTED_NODE;
		}
	}

	/**
	 * Delete an object, for a generic index.<br/>
	 * If the node does not exist, nothing is done.<br/>
	 * If the node exists, and the recordId is in it, the recordId is deleted
	 * from it. If the node has more recordIds into it, we're done.<br/>
	 * If the node is empty, it's deleted from the tree.
	 * 
	 * @param key
	 *            key node key
	 * @return deleted node key if node is deleted, null if is not found or has
	 *         more recordIds into it
	 * @throws DBException
	 *             on errors
	 * @throws SerializationException
	 *             writing or reading items
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public NodeKey delete(final Object key) throws DBException,
			SerializationException, RecordStoreFullException {
		return deleteObject(key, null);
	}

	/**
	 * Delete an object and record id.<br/>
	 * If the node does not exist, nothing is done.<br/>
	 * If the node exists, and the recordId is in it, the recordId is deleted
	 * from it. If the node has more recordIds into it, we're done.<br/>
	 * If the node is empty, it's deleted from the tree.
	 * 
	 * @param key
	 *            key node key
	 * @param recordId
	 *            record id
	 * @return deleted node key if node is deleted, null if is not found or has
	 *         more recordIds into it
	 * @throws DBException
	 *             on errors
	 * @throws SerializationException
	 *             writing or reading items
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public NodeKey delete(final Object key, final int recordId)
			throws DBException, SerializationException,
			RecordStoreFullException {
		return deleteObject(key, new Integer(recordId));
	}

	/**
	 * Delete an object and record id (if it's a tableIndex).<br/>
	 * If the node does not exist, nothing is done.<br/>
	 * If the node exists, and the recordId is in it, the recordId is deleted
	 * from it. If the node has more recordIds into it, we're done.<br/>
	 * If the node is empty, it's deleted from the tree.<br/>
	 * When it's a generic index, then the node is just removed if found, so
	 * value can just be null.
	 * 
	 * @param key
	 *            key node key
	 * @param value
	 *            value to remove
	 * @return deleted node key if node is deleted, null if is not found or has
	 *         more recordIds into it
	 * @throws DBException
	 *             on errors
	 * @throws SerializationException
	 *             writing or reading items
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public NodeKey deleteObject(final Object key, final Object value)
			throws DBException, SerializationException,
			RecordStoreFullException {
		try {
			return doDelete(new IndexKey(key), value);
		} catch (DBException e) {
			markDamaged();
			throw e;
		} catch (RecordStoreFullException e) {
			markDamaged();
			throw e;
		}
	}

	public boolean isDamaged() {
		return rs.isDamaged();
	}

	private NodeKey doDelete(final IndexKey key, final Object value)
			throws DBException, RecordStoreFullException {
		final ProgressEvent event = this.event;
		final boolean sendProgressEvents = this.sendProgressEvents;
		try {
			if (sendProgressEvents) {
				event.dispatch();
			}
			SearchResult searchResult = search(getRootNode(), key);
			if (searchResult.getNode() == null) {
				return null;
			}
			final Object data = searchResult.getNode().getNodeKey(
					searchResult.getKeyIndex()).getData();
			if (tableIndex) {
				((SortedIntArray) data).remove(((Integer) value).intValue());
				addToSave(searchResult.getNode());
				if (((SortedIntArray) data).size() > 0) {
					flush();
					return null;
				}
			}
			if (sendProgressEvents) {
				event.dispatch();
			}
			final NodeKey deletedKey = searchResult.getNode().getNodeKey(
					searchResult.getKeyIndex());
			if (!searchResult.getNode().isLeaf()) {
				searchResult = swapWithLeaf(searchResult);
			}
			searchResult.getNode().extractNodeKey(searchResult.getKeyIndex());
			if (sendProgressEvents) {
				event.dispatch();
			}
			if (searchResult.getNode().getKeyCount() < order) {
				redistribute(searchResult.getNode());
			}
			flush();
			return deletedKey;
		} catch (DBException e) {
			clear();
			throw e;
		}
	}

	/**
	 * Find with fuzzy match, this means, by now, that the string key is checked
	 * with "startsWith". Only works with String normal or full text indexes. If
	 * the Index is a full text one, the text is splitted into words and the
	 * result is the union of the results of searching for the different words
	 * using AND mode, that is, that all the results must contain all the given
	 * words.
	 * 
	 * @param key
	 *            key to find
	 * @return array of resulting recordIds or null if no matches
	 * @throws DBException
	 *             on errors
	 * @throws SerializationException
	 *             error writing or reading items
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public Object findFuzzy(final String key) throws DBException,
			SerializationException, RecordStoreFullException {
		return findFuzzy(key, AND);
	}

	/**
	 * Find with fuzzy match, this means, by now, that the string key is checked
	 * with "startsWith". Only works with String normal or full text indexes. If
	 * the Index is a full text one, the text is splitted into words and the
	 * result is the union of the results of searching for the different words.
	 * 
	 * @param key
	 *            key to find
	 * @param mode
	 *            mode for matching AND (all words) or OR (any of the words)
	 * @return array of resulting recordIds or null if no matches
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public Object findFuzzy(final String key, final int mode)
			throws DBException, RecordStoreFullException {
		final byte type = this.type; // Local variable to improve speed
		if (key == null || type == KT_LONG) {
			return null;
		}
		try {
			rs.open();
			final long start = System.currentTimeMillis();
			final SearchResult results = new SearchResult();
			if (type != KT_FULL_TEXT) {
				searchFuzzy(getRootNode(), caseSensitive ? key : key
						.toLowerCase(), results, NONE);
			} else {
				final Vector words = getWords(caseSensitive ? key : key
						.toLowerCase(), false);
				final int length = words.size();
				for (int i = 0; i < length; i++) {
					searchFuzzy(getRootNode(), (String) words.elementAt(i),
							results, mode);
				}
			}
			final long end = System.currentTimeMillis();
			final long ellapsed = (end - start);
			addFuzzyStats(ellapsed);
			log.info(name + ": findFuzzy " + key + " took " + ellapsed + "ms");
			return mode == AND && type == KT_FULL_TEXT ? results.merge()
					: results.getRecordIds();
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "findFuzzy", new Object[] { key,
					new Integer(mode) }, null, e);
			throw new DBException(Constants.ERR_IDX_FIND_FUZZY, e);
		} finally {
			doClose("findFuzzy");
		}
	}

	/**
	 * Find a key in the index.
	 * 
	 * @param key
	 *            key to find
	 * @return int array with recordIds for the given key, null if not found
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public Object find(final Object key) throws DBException,
			RecordStoreFullException {
		final IndexKey indexKey = new IndexKey(key);
		try {
			rs.open();
			final long start = System.currentTimeMillis();
			final SearchResult searchResult = search(getRootNode(), indexKey);
			final long end = System.currentTimeMillis();
			final long ellapsed = (end - start);
			addStats(ellapsed);
			log.info(name + ": find " + indexKey + " took " + ellapsed + "ms");
			if (searchResult.getNode() != null) {
				if (tableIndex) {
					return ((SortedIntArray) searchResult.getNode().getNodeKey(
							searchResult.getKeyIndex()).getData()).toIntArray();
				} else {
					return searchResult.getNode().getNodeKey(
							searchResult.getKeyIndex()).getData();
				}
			} else {
				return null;
			}
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "doFind", new Object[] { indexKey },
					null, e);
			throw new DBException(Constants.ERR_IDX_FIND, e);
		} finally {
			doClose("doFind");
		}

	}

	public static long getAverageWideFindTime() {
		return wideAccesses > 0 && wideAccumulatedTime > 0 ? wideAccumulatedTime
				/ wideAccesses
				: 0;
	}

	public static long getWideAccesses() {
		return wideAccesses;
	}

	public static long getWideAccumulatedTime() {
		return wideAccumulatedTime;
	}

	public long getAverageFindTime() {
		return totalAccesses > 0 && accumulatedTime > 0 ? accumulatedTime
				/ totalAccesses : 0;
	}

	public long getTotalAccesses() {
		return totalAccesses;
	}

	public long getAccumulatedTime() {
		return accumulatedTime;
	}

	public static long getAverageWideFuzzyFindTime() {
		return wideFuzzyAccesses > 0 && wideFuzzyAccumulatedTime > 0 ? wideFuzzyAccumulatedTime
				/ wideFuzzyAccesses
				: 0;
	}

	public static long getWideFuzzyAccesses() {
		return wideFuzzyAccesses;
	}

	public static long getWideFuzzyAccumulatedTime() {
		return wideFuzzyAccumulatedTime;
	}

	public long getAverageFuzzyFindTime() {
		return totalFuzzyAccesses > 0 && accumulatedFuzzyTime > 0 ? accumulatedFuzzyTime
				/ totalFuzzyAccesses
				: 0;
	}

	public long getTotalFuzzyAccesses() {
		return totalFuzzyAccesses;
	}

	public long getAccumulatedFuzzyTime() {
		return accumulatedFuzzyTime;
	}

	private synchronized void addStats(final long ellapsed) {
		accumulatedTime += ellapsed;
		totalAccesses++;
		wideAccesses++;
		wideAccumulatedTime += ellapsed;
	}

	private synchronized void addFuzzyStats(final long ellapsed) {
		accumulatedFuzzyTime += ellapsed;
		totalFuzzyAccesses++;
		wideFuzzyAccesses++;
		wideFuzzyAccumulatedTime += ellapsed;
	}

	/**
	 * Returns an array of ints with the record ids stored under the given key.
	 * 
	 * @param key
	 *            index key
	 * @return array of record ids, or null if key not found
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public Object getKeyObject(final Object key) throws DBException,
			RecordStoreFullException {
		return doGetKeyObject(new IndexKey(key));
	}

	// Private method to improve obfuscation
	private Object doGetKeyObject(final IndexKey key) throws DBException,
			RecordStoreFullException {
		try {
			rs.open();
			final SearchResult result = search(getRootNode(), key);
			if (result.getNode() == null) {
				return null;
			} else if (tableIndex) {
				return ((SortedIntArray) result.getNode().getNodeKey(
						result.getKeyIndex()).getData()).toIntArray();
			} else {
				return result.getNode().getNodeKey(result.getKeyIndex())
						.getData();
			}
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "doGetKeyObject", new Object[] { key },
					null, e);
			throw new DBException(Constants.ERR_IDX_FIND, e);
		} finally {
			doClose("doGetKeyObject");
		}
	}

	public long getLastModified() throws RSException {
		return rs.getLastModified();
	}

	/**
	 * Redistribute entries from neighbour (and parent) nodes to keep nodes with
	 * at least &quot;order&quot; keys.
	 * 
	 * @param node
	 *            node to redistribute
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	private void redistribute(final Node node) throws DBException,
			RecordStoreFullException {
		if (node.getParent() == null) {
			return; // no redistribution of root node
		}
		// Find index of node in it's parent
		final int parentIndex = findParentIndex(node);
		// ToDo: try to balance number of keys per node between children
		if (parentIndex == 0) {
			// When a node is the leftmost child, and thus has no left
			// neighbours, keys must be borrowed from a right neighbour
			borrowFromRight(node);
		} else {
			borrowFromLeft(node, parentIndex);
		}
	}

	private int findParentIndex(final Node node) {
		final Node parent = node.getParent();
		final int keyCount = parent.getKeyCount();
		final int recordId = node.getRecordId().intValue();
		for (int i = 0; i <= keyCount; i++) {
			if (parent.getChild(i) == recordId) {
				return i;
			}
			if (sendProgressEvents) {
				event.dispatch();
			}
		}
		return -1;
	}

	/**
	 * Borrow keys from the right neighbour, shifting the trhough the parent
	 * node if enough keys are left on the neighbour. Otherwise merge this node
	 * with it's neighbour shifting through the parent.
	 * 
	 * @param node
	 *            underflowed node
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	private void borrowFromRight(final Node node) throws DBException,
			RecordStoreFullException {
		// log.debug( "borrowFromRight" );
		// final Hashtable toSave = this.toSave;
		final Node neighbour = node.getParent().getChildNode(1);
		if (neighbour.getKeyCount() > order) {
			// Can borrow a key from it
			// 1. Get parent key from parent node
			// and append key to end of the leftmost node
			node.setKey(node.getKeyCount(), node.getParent().getKey(0));
			node.incKeyCount();
			// 3. Drop first key from neighbour
			final NodeKey neighbourKey = neighbour.getKey(0);
			final int neighbourChild = neighbour.getChild(0);
			neighbour.extractNodeKey(0);
			// 4. Put the neighbour key in the parent position
			node.getParent().setKey(0, neighbourKey);
			// 5. Put the neighbour child at the end of this node
			node.setChild(node.getKeyCount(), neighbourChild);
			// 6. Be sure to update cached child parent
			// final Node movedChild = (Node) CacheManager.getInstance(
			// CACHE_ZONE ).get(
			// getRecordCacheKey( neighbourChild )
			// );
			// if( movedChild != null )
			// {
			// movedChild.getParent() = node;
			// }
			if (sendProgressEvents) {
				event.dispatch();
			}
		} else {
			// we need to merge both nodes using the parent key
			// 1. Drop the parent key from the parent node
			final NodeKey parentKey = node.getParent().extractNodeKey(0);
			// extractNodeKey deletes the key and the child at the same index,
			// i.e. this node, but we want to delete the right node so we need
			// to update child array at parent
			node.getParent().setChild(0, node.getRecordId().intValue());
			// 2. Append the key to the end of the leftmost node
			node.setKey(node.getKeyCount(), parentKey);
			// 3. Move all the keys from the neighbour node (and children)
			node.appendNode(neighbour);
			// 4. Make sure neighbour is deleted
			if (sendProgressEvents) {
				event.dispatch();
			}
			dropNode(neighbour);
			// 5. Check that parent is ok
			checkParent(node);
		}
	}

	private void checkParent(final Node node) throws DBException,
			RecordStoreFullException {
		if (node.getParent().getParent() == null) {
			// Root node is allowed to have as little as 1 key, but not 0
			if (node.getParent().getKeyCount() == 0) {
				// In this case we will copy all data from this node to
				// root and delete this node
				node.moveToParent();
				dropNode(node);
			}
		} else if (node.getParent().getKeyCount() < order) {
			redistribute(node.getParent());
		}
	}

	/**
	 * Borrow keys from the left neighbour, shifting the trhough the parent node
	 * if enough keys are left on the neighbour. Otherwise merge this node with
	 * it's neighbour shifting through the parent.
	 * 
	 * @param node
	 *            underflowed node
	 * @param parentIndex
	 *            index of this node in its parent node
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	private void borrowFromLeft(final Node node, final int parentIndex)
			throws DBException, RecordStoreFullException {
		final Node neighbour = node.getParent().getChildNode(parentIndex - 1);
		if (neighbour.getKeyCount() > order) {
			// Can borrow a key from it
			// 1. Get parent key from parent node
			final NodeKey parentKey = node.getParent().getKey(parentIndex - 1);
			// 2. Insert key at the start of the rightmost node
			node.shift(0);
			node.setKey(0, parentKey);
			node.incKeyCount();
			// 3. Drop last key from neighbour
			final NodeKey neighbourKey = neighbour.getKey(neighbour
					.getKeyCount() - 1);
			final int neighbourChild = neighbour.getChild(neighbour
					.getKeyCount() - 1);
			neighbour.extractNodeKey(neighbour.getKeyCount() - 1);
			// 4. Put the neighbour key in the parent position
			node.getParent().setKey(parentIndex - 1, neighbourKey);
			// 5. Migrate dropped child (extract node key will drop the middle
			// child)
			node.setChild(0, neighbour.getChild(neighbour.getKeyCount()));
			neighbour.setChild(neighbour.getKeyCount(), neighbourChild);
			if (sendProgressEvents) {
				event.dispatch();
			}
		} else {
			// we need to merge both nodes using the parent key
			// 1. Drop the parent key from the parent node
			final NodeKey parentKey = node.getParent().extractNodeKey(
					parentIndex - 1);
			// 2. Insert key at the start of the rightmost node
			node.shift(0);
			node.setKey(0, parentKey);
			// 3. Move all the keys from the neighbour node (and children)
			node.insertNode(neighbour);
			// 4. Make sure neighbour is deleted
			if (sendProgressEvents) {
				event.dispatch();
			}
			dropNode(neighbour);
			// 5. Check that parent is ok
			checkParent(node);
		}
	}

	/**
	 * Swap a key in a non-leaf node with it's logical left key in a leaf, so it
	 * can be deleted.
	 * 
	 * @param searchResult
	 *            node and key index
	 * @return new result with the leaf node and correspongind key index
	 * @throws DBException
	 *             on errors
	 */
	private SearchResult swapWithLeaf(final SearchResult searchResult)
			throws DBException {
		// Find a leaf to swap keys
		Node nextNode = searchResult.getNode().getChildNode(
				searchResult.getKeyIndex() + 1);
		// noinspection MethodCallInLoopCondition
		while (!nextNode.isLeaf()) {
			nextNode = nextNode.getChildNode(0);
			if (sendProgressEvents) {
				event.dispatch();
			}
		}
		// swap nodes
		final NodeKey tmpNodeKey = nextNode.getNodeKey(0);
		nextNode.setKey(0, searchResult.getNode().getKey(
				searchResult.getKeyIndex()));
		searchResult.getNode().setKey(searchResult.getKeyIndex(), tmpNodeKey);
		if (sendProgressEvents) {
			event.dispatch();
		}
		return new SearchResult(nextNode, 0);
	}

	/**
	 * Search the node and index that contain a given key.
	 * 
	 * @param node
	 *            start node
	 * @param key
	 *            key to find
	 * @param results
	 *            search accumulated results
	 * @param mode
	 *            search mode (AND or OR)
	 * @throws DBException
	 *             on errors
	 */
	void searchFuzzy(final Node node, final String key,
			final SearchResult results, final int mode) throws DBException {
		// log.debug( "search: " + key + " on node: " + node );
		if (tableIndex && node != null) {
			final int keyCount = node.getKeyCount();
			int lastMatch = -2;
			for (int i = 0; i < keyCount; i++) {
				// log.debug( "node.keys[" + i + "]: " + node.keys[i] );
				final NodeKey nodeKey = node.getKey(i);
				String nodeValue = nodeKey.getKey().getString();
				if (!caseSensitive) {
					nodeValue = nodeValue.toLowerCase();
				}
				if (nodeValue.startsWith(key)) {
					if (mode != AND) {
						results.addResults(((SortedIntArray) nodeKey.getData())
								.toIntArray());
					} else {
						results.addResults(key, ((SortedIntArray) nodeKey
								.getData()).toIntArray());
					}
					if (!node.isLeaf()) {
						if (lastMatch != (i - 1)) {
							searchFuzzy(node.getChildNode(i), key, results,
									mode);
						}
						searchFuzzy(node.getChildNode(i + 1), key, results,
								mode);
						lastMatch = i;
					}
				}
			}
			if (lastMatch == -2 && !node.isLeaf()) {
				for (int i = 0; i < keyCount; i++) {
					if (node.getKey(i).compareTo(new IndexKey(key)) > 0) {
						searchFuzzy(node.getChildNode(i), key, results, mode);
						return;
					}
				}
				searchFuzzy(node.getChildNode(keyCount), key, results, mode);
			}
		} else {
			log.warn("search on null node");
		}
	}

	/**
	 * Search the node and index that contain a given key.
	 * 
	 * @param node
	 *            start node
	 * @param key
	 *            key to find
	 * @return SearchResult object with found data, or with null node and -1 as
	 *         keyIndex if not found
	 * @throws DBException
	 *             on errors
	 */
	SearchResult search(final Node node, final IndexKey key) throws DBException {
		// log.debug( "search: " + key + " on node: " + node );
		if (node != null) {
			final int keyCount = node.getKeyCount();
			for (int i = 0; i < keyCount; i++) {
				// log.debug( "node.keys[" + i + "]: " + node.keys[i] );
				if (node.getKey(i).compareTo(key) == 0) {
					// log.debug( "found" );
					return new SearchResult(node, i);
				}
			}
			if (node.isLeaf()) {
				// log.debug( "not found" );
				return new SearchResult(null, -1);
			} else {
				for (int i = 0; i < keyCount; i++) {
					if (node.getKey(i).compareTo(key) > 0) {
						return search(node.getChildNode(i), key);
					}
				}
				return search(node.getChildNode(keyCount), key);
			}
		} else {
			log.warn("search on null node");
			return new SearchResult(null, -1);
		}
	}

	/**
	 * Get a sorted list of record ids, by ascending order on the index key.
	 * 
	 * @return sorted list of record ids, null if the index is empty
	 * @throws DBException
	 *             on errors
	 * @throws SerializationException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	public int[] buildSortedList() throws DBException, SerializationException,
			RecordStoreFullException {
		return doBuildSortedList();
	}

	// Private method to improve obfuscation
	private int[] doBuildSortedList() throws DBException,
			RecordStoreFullException {
		// Find left most deepest child
		// take its keys
		// go up one level, take the parent key
		// then go down to the right node
		// and repeat
		try {
			rs.open();
			final Vector list = new Vector(10);
			buildSortedList(list, null, getRootNode(), -1);
			int[] result = null;
			final int count = list.size();
			for (int i = 0; i < count; i++) {
				final int[] item = (int[]) list.elementAt(i);
				if (result == null) {
					result = new int[item.length];
					System.arraycopy(item, 0, result, 0, item.length);
				} else {
					final int[] aux = new int[result.length + item.length];
					System.arraycopy(result, 0, aux, 0, result.length);
					System.arraycopy(item, 0, aux, result.length, item.length);
					result = aux;
				}
			}
			return result;
		} catch (RSException e) {
			log.error(e);
			ErrorLog.addError("Index", "doBuildSortedList", null, null, e);
			throw new DBException(Constants.ERR_IDX_FIND, e);
		} finally {
			doClose("doBuildSortedList");
		}
	}

	void buildSortedList(final Vector records, final Vector keys,
			final Node node, final int parentIndex) throws DBException {
		final Log log = Index.log;

		if (!tableIndex) {
			log.warn("not a table index");
			return;
		}
		if (node == null) {
			log.warn("node is null");
			return;
		}
		if (node.isLeaf()) {
			final int keyCount = node.getKeyCount();
			for (int i = 0; i < keyCount; i++) {
				final NodeKey key = node.getKey(i);
				if (key != null) {
					final SortedIntArray data = (SortedIntArray) key.getData();
					if (data != null) {
						records.addElement(data.toIntArray());
						if (keys != null) {
							keys.addElement(key.getKey());
						}
					} else {
						log.warn("data is null");
					}
				} else {
					log.warn("key " + i + " is null");
				}
			}
		} else {
			System.gc();
			buildSortedList(records, keys, node.getChildNode(0), 0);
		}
		if (parentIndex != -1 && parentIndex < node.getParent().getKeyCount()) {
			final NodeKey key = node.getParent().getKey(parentIndex);
			if (key != null) {
				final SortedIntArray data = (SortedIntArray) key.getData();
				if (data != null) {
					records.addElement(data.toIntArray());
					if (keys != null) {
						keys.addElement(key.getKey());
					}
				} else {
					log
							.warn("data at parent index " + parentIndex
									+ " is null");
				}
			} else {
				log.warn("key (parent index)" + parentIndex + " is null");
			}
			System.gc();
			buildSortedList(records, keys, node.getParent().getChildNode(
					parentIndex + 1), parentIndex + 1);
		}
	}

	/**
	 * Get the root node from storage.<br/>
	 * If it does not exist it's created.
	 * 
	 * @return the root node
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	Node getRootNode() throws DBException, RecordStoreFullException {
		if (rootNode == null) {
			Node node = getNode(null, ROOT_NODE);
			if (node == null) {
				node = new Node(this, order, null);
				saveNode(node);
			}
			rootNode = node;
		}
		return rootNode;
	}

	/**
	 * Get a node from storage.<br/>
	 * CacheManager is used to improve performance.
	 * 
	 * @param parent
	 *            parent node
	 * @param recordId
	 *            record id where the node is stored
	 * @return node
	 * @throws DBException
	 *             on errors
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public synchronized Node getNode(final Node parent, final int recordId)
			throws DBException {
		final Store rs = this.rs;
		Node node = null;

		byte[] buffer = this.buffer;
		try {
			if (sendProgressEvents) {
				event.dispatch();
			}
			rs.open();
			final int recordSize = rs.getRecordSize(recordId);
			if (buffer == null || recordSize > buffer.length) {
				buffer = Tools.secureAlloc(recordSize + GROWTH_FACTOR);
				this.buffer = buffer;
			}
			rs.getRecord(recordId, buffer, 0);
			if (sendProgressEvents) {
				event.dispatch();
			}
			node = new Node(this, order, parent);
			node.setRecordId(new Integer(recordId));
			node.deserialize(new SerializerInputStream(
					new ByteArrayInputStream(buffer, 0, recordSize)));
		} catch (InvalidRecordIDException e) {
			return null;
		} catch (Exception e) {
			ErrorLog.addError("Index", "doGetNode", new Object[] {
					new Integer(recordId), parent }, null, e);
			throw new DBException(Constants.ERR_IDX_GET_NODE, e);
		} finally {
			try {
				rs.close();
			} catch (RSException e) {
				ErrorLog.addError("Index", "doGetNode", new Object[] {
						new Integer(recordId), parent }, null, e);
			}
		}
		return node;
	}

	/**
	 * Save node on storage.<br/>
	 * If the node is new it's created, otherwise it's updated.<br/>
	 * CacheManager is updated.
	 * 
	 * @param node
	 *            node to save
	 * @return saved node, for convenience as it's the same object
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	Node saveNode(final Node node) throws RecordStoreFullException, DBException {
		final Store rs = this.rs;
		try {
			if (sendProgressEvents) {
				event.dispatch();
			}
			rs.open();
			baos.reset();
			node.serialize(out);
			final byte[] data = baos.toByteArray();
			if (sendProgressEvents) {
				event.dispatch();
			}
			int recordId;
			if (node.getRecordId() != null) {
				recordId = node.getRecordId().intValue();
				// Update
				rs.setRecord(recordId, data, 0, data.length);
			} else {
				// log.debug( "creating new node: " + node );
				// New
				recordId = rs.addRecord(data, 0, data.length);
				node.setRecordId(new Integer(recordId));
			}
			return node;
		} catch (RecordStoreFullException e) {
			throw e;
		} catch (Exception e) {
			log.error(e);
			ErrorLog.addError("Index", "saveNode", new Object[] { node }, null,
					e);
			throw new DBException(Constants.ERR_IDX_SAVE_NODE, e);
		} finally {
			try {
				rs.close();
			} catch (RSException e) {
				log.error(e);
				ErrorLog.addError("Index", "saveNode", new Object[] { node },
						null, e);
			}
		}
	}

	/**
	 * Drop a node from storage.<br/>
	 * CacheManager is updated.
	 * 
	 * @param node
	 *            node to drop
	 * @return dropped node
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	Node dropNode(final Node node) throws DBException, RecordStoreFullException {
		final Store rs = this.rs;

		// log.debug( "dropping node: " + node );
		if (node.getRecordId() != null) {
			if (toSave.containsKey(node.getRecordId())) {
				toSave.remove(node.getRecordId());
			}

			if (findParentIndex(node) != -1) {
				final CantDropNodeException cantDropNodeException = new CantDropNodeException(
						Constants.ERR_IDX_DROP_NODE, node.getRecordId() + ", "
								+ node.getParent().getRecordId());
				ErrorLog.addError("Index", "dropNode", new Object[] { node,
						node.getParent() }, null, cantDropNodeException);
				throw cantDropNodeException;
			}
			try {
				rs.open();
				if (sendProgressEvents) {
					event.dispatch();
				}
				rs.deleteRecord(node.getRecordId().intValue());
				// CacheManager.getInstance( CACHE_ZONE ).remove(
				// getRecordCacheKey( node.getRecordId().intValue() )
				// );
			} catch (RSException e) {
				ErrorLog.addError("Index", "dropNode", new Object[] { node },
						null, e);
				throw new DBException(Constants.ERR_IDX_DROP_NODE, e);
			} catch (InvalidRecordIDException e) {
				ErrorLog.addError("Index", "dropNode", new Object[] { node },
						null, e);
				throw new DBException(Constants.ERR_IDX_DROP_NODE, e);
			} finally {
				doClose("dropNode");
			}
		}
		return node;
	}

	/** @noinspection MethodCallInLoopCondition */

	/**
	 * Write pending data.
	 * 
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space is left on record store
	 */
	private void flush() throws DBException, RecordStoreFullException {
		final Hashtable toSave = this.toSave;
		try {
			// noinspection MethodCallInLoopCondition
			for (Enumeration i = toSave.elements(); i.hasMoreElements();) {
				final Node node = (Node) i.nextElement();
				saveNode(node);
			}
		} finally {
			toSave.clear();
		}
	}

	private void clear() {
		toSave.clear();
	}

	/**
	 * Add a node to the toSave list.
	 * 
	 * @param node
	 *            node
	 */
	void addToSave(final Node node) {
		if (node.getRecordId() != null) {
			toSave.put(node.getRecordId(), node);
		}
	}

	public void rsOpen() {
	}

	public void rsClose() {
		rootNode = null;
	}

	public void open() throws RecordStoreFullException, RSException {
		rs.open();
	}
}
