package winkCC.net;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Hashtable;

import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;

/**
 * A simple logging class loosely adapted from the java.util.logging.Logger class. See http://developers.sun.com/techtopics/mobility/midp/ttips/logging/
 * @author WangYinghua
 *
 */
public class Logger {

	private Level _level;
	private String _name;
	private Logger _parent;
	private int _version;

	// Static data
	private static Hashtable _loggers = new Hashtable();
	private static boolean _logToStdout = true;
	private static boolean _logToRS = false;
	private static Logger _root = new Logger("");
	private static RecordStore _rs;

	static {
		// Initialize the root level
		_loggers.put("", _root);
		_root.setParent(null);
		_root.setLevel(Level.WARNING);
	}

	private Logger(String name) {
		_name = name;
	}

	/**
	 * Call to clear the log, which must be closed.
	 * @throws RecordStoreException
	 */
	public synchronized void clearLog() throws RecordStoreException {
		RecordStore.deleteRecordStore("log");
	}

	/** Call to close the rs log. */
	public static void closeLog() {
		try {
			if (_rs != null) {
				_rs.closeRecordStore();
			}
		} catch (RecordStoreException e) {
		}

		_rs = null;
	}

	/**
	 * Finds the logger's parent by parsing the name into parts separated by '.'
	 * @param name
	 * @return
	 */
	private Logger findParent(String name) {
		if (name == null || name.length() == 0) {
			return null;
		}

		Logger parent;

		synchronized (_loggers) {
			while (true) {
				int pos = name.lastIndexOf('.');
				if (pos < 0) {
					parent = _root;
					break;
				}

				String pname = name.substring(0, pos);
				parent = (Logger) _loggers.get(pname);
				if (parent != null)
					break;

				name = pname;
			}
		}

		return parent;
	}

	/**
	 * Log a message with the FINE level
	 * @param msg
	 */
	public void fine(String msg) {
		log(Level.FINE, msg);
	}

	/** Get this logger's level, which may be null. */
	public Level getLevel() {
		return _level;
	}

	/**
	 * Find the logger registered with a specific name.
	 * @param name
	 * @return
	 */
	public static synchronized Logger getLogger(String name) {
		Logger l = (Logger) _loggers.get(name);

		if (l == null) {
			l = new Logger(name);
			_loggers.put(name, l);
		}

		return l;
	}

	/** Return the logger's name. */
	public String getName() {
		return _name;
	}

	/** 
	 * Return the logger's parent. 
	 * The parent is reset if other loggers were created, in case children are created after their parents.
	 * @return
	 */
	public Logger getParent() {
		if (_loggers.size() != _version) {
			setParent(findParent(_name));
		}

		return _parent;
	}

	/**
	 * Log a message with the INFO level.
	 * @param msg
	 */
	public void info(String msg) {
		log(Level.INFO, msg);
	}

	/**
	 * Test whether the given level is loggable according to this Logger.
	 * @param test
	 * @return
	 */
	public boolean isLoggable(Level test) {
		if (test == null)
			return false;

		Logger curr = this;
		while (curr != null) {
			Level actual = curr.getLevel();
			if (actual != null) {
				if (actual == Level.OFF)
					return false;
				return (test.intValue() >= actual.intValue());
			}

			curr = curr.getParent();
		}

		return false;
	}

	/**
	 * Log a message using the given level.
	 * @param level
	 * @param msg
	 */
	public void log(Level level, String msg) {
		log(level, msg, null);
	}

	/** 
	 * Log a message using the given level, including optional exception information.
	 * @param level
	 * @param msg
	 * @param e
	 */
	public void log(Level level, String msg, Throwable e) {
		if (level == null)
			level = Level.FINE;
		if (!isLoggable(level))
			return;

		long time = System.currentTimeMillis();
		String tname = Thread.currentThread().toString();

		if (_logToStdout) {
			String tmp = msg;

			if (e != null) {
				tmp = msg + " " + e;
			}

			System.out.println(time + " " + tname + " " + level + " " + tmp);
		}

		if (!_logToRS)
			return;

		// Prepare the data
		ByteArrayOutputStream bout = new ByteArrayOutputStream();
		DataOutputStream dout = new DataOutputStream(bout);

		try {
			dout.writeInt(level.intValue());
			dout.writeLong(System.currentTimeMillis());
			dout.writeUTF(Thread.currentThread().toString());
			dout.writeUTF(msg != null ? msg : "");
			dout.writeUTF(e != null ? e.toString() : "");
			dout.flush();
		} catch (IOException ex) {
		}

		byte[] data = bout.toByteArray();

		// Write it out
		synchronized (this) {
			RecordStore rs = null;
			boolean closeIt = (_rs == null);

			try {
				rs = openLog();
				rs.addRecord(data, 0, data.length);
			} catch (RecordStoreException ex) {
			} finally {
				if (closeIt) {
					closeLog();
				}
			}
		}
	}

	/**
	 * Open the log rs if not already open.
	 * @return
	 * @throws RecordStoreException
	 */
	public static RecordStore openLog() throws RecordStoreException {
		if (_rs == null) {
			_rs = RecordStore.openRecordStore("log", true);
		}

		return _rs;
	}

	/**
	 * Sets the logger's level. May be null.
	 * @param level
	 */
	public void setLevel(Level level) {
		_level = level;
	}

	/**
	 * Whether or not to log to stdout
	 * @param log
	 */
	public void setLogToStdout(boolean log) {
		_logToStdout = log;
	}

	/**
	 * Whether or not to log to the record store
	 * @param log
	 */
	public void setLogToRS(boolean log) {
		_logToRS = log;
	}

	/**
	 * Sets the logger's parent.
	 * @param parent
	 */
	public void setParent(Logger parent) {
		_parent = parent;
		_version = _loggers.size();
	}

	/**
	 * Log a message using the SEVERE level.
	 * @param msg
	 */
	public void severe(String msg) {
		log(Level.SEVERE, msg);
	}

	/**
	 * Log a message using the WARNING level.
	 * @param msg
	 */
	public void warning(String msg) {
		log(Level.WARNING, msg);
	}
}
