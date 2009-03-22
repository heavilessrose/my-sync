package winkCC.rms.finals;

import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;
import javax.microedition.rms.RecordStoreNotOpenException;

public class RmsUtils {
	protected static RecordStore _rs = null;

	/**
	 * Record store名字.
	 */
	public static final String RS_NAME = "winksCC2";
	/** boolean flag true identifier */
	private static final byte[] TRUE = { (byte) 1 };

	/** boolean flag false identifier */
	private static final byte[] FALSE = { (byte) 0 };

	/** Dummy data for new keys */
	private static final byte[] DUMMY = new byte[0];

	/**
	 * 增添一条记录.
	 */
	public void add() {
		try {
			getStore().addRecord(DUMMY, 0, 0);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
		} catch (RecordStoreNotFoundException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}

	//	private void open() throws RecordStoreFullException,
	//			RecordStoreNotFoundException, RecordStoreException {
	//		_rs = RecordStore.openRecordStore(RS_NAME, true);
	//	}

	/**
	 * 读取一个记录.
	 * 
	 * @param key
	 */
	public byte[] getBytes(int key) {
		byte[] b = null;
		try {
			b = getStore().getRecord(key);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (InvalidRecordIDException e) {
			e.printStackTrace();
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
		} catch (RecordStoreNotFoundException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}

		return b;
	}

	/**
	 * 写一个记录
	 * 
	 * @param key
	 * @param data
	 */
	public void writeBytes(int key, byte[] data) {
		try {
			if (data == null) {
				getStore().setRecord(key, DUMMY, 0, 0);
			} else {
				getStore().setRecord(key, data, 0, data.length);
			}
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 返回record store.
	 * 
	 * @return record store.
	 * @throws RecordStoreFullException
	 * @throws RecordStoreNotFoundException
	 * @throws RecordStoreException
	 */
	private RecordStore getStore() throws RecordStoreFullException,
			RecordStoreNotFoundException, RecordStoreException {
		if (_rs == null) {
			_rs = RecordStore.openRecordStore(RS_NAME, true);
		}
		return _rs;
	}

	/**
	 * 将int写入指定key.
	 * 
	 * @param key
	 * @param i
	 */
	public void writeInt(int key, int i) {
		byte[] intData = new byte[4];
		intData[0] = (byte) ((i & 0xff000000) >> 24);
		intData[1] = (byte) ((i & 0x00ff0000) >> 16);
		intData[2] = (byte) ((i & 0x0000ff00) >> 8);
		intData[3] = (byte) (i & 0x000000ff);
		writeBytes(key, intData);
	}

	public int getInt(int key) {
		byte[] b = getBytes(key);
		if (b == null || b.length != 4) {
			return 0;
		} else {
			int i = ((b[0] & 0xff) << 24) | ((b[1] & 0xff) << 16)
					| ((b[2] & 0xff) << 8) | (b[3] & 0xff);
			return i;
		}
	}

	/**
	 * 将String写入指定key.
	 * 
	 * @param key
	 * @param str
	 */
	public void writeString(int key, String str) {
		if (str != null || !str.equals(""))
			writeBytes(key, str.getBytes());
	}

	public String getString(int key) {
		return new String(getBytes(key));
	}

	/**
	 * 将long写入指定key.
	 * 
	 * @param key
	 * @param l
	 */
	public void writeLong(int key, long l) {
		byte[] longData = new byte[8];
		longData[0] = (byte) ((l & 0xff00000000000000L) >> 58);
		longData[1] = (byte) ((l & 0x00ff000000000000L) >> 48);
		longData[2] = (byte) ((l & 0x0000ff0000000000L) >> 40);
		longData[3] = (byte) ((l & 0x000000ff00000000L) >> 32);
		longData[4] = (byte) ((l & 0x00000000ff000000L) >> 24);
		longData[5] = (byte) ((l & 0x0000000000ff0000L) >> 16);
		longData[6] = (byte) ((l & 0x000000000000ff00L) >> 8);
		longData[7] = (byte) (l & 0x00000000000000ffL);
		writeBytes(key, longData);
	}

	/**
	 * 
	 * @param key
	 * @return
	 */
	public long getLong(int key) {
		byte[] b = getBytes(key);
		if (b == null || b.length != 8) {
			return 0;
		} else {
			int i = ((b[0] & 0xff) << 56) | ((b[1] & 0xff) << 48)
					| ((b[2] & 0xff) << 40) | ((b[3] & 0xff) << 32)
					| ((b[4] & 0xff) << 24) | ((b[5] & 0xff) << 16)
					| ((b[6] & 0xff) << 8) | (b[7] & 0xff);
			return i;
		}
	}

	/**
	 * 将char[]写入指定key.
	 * 
	 * @param key
	 * @param txt
	 */
	public void writeChars(int key, char[] txt) {
		byte[] b = null;
		if (txt != null) {
			b = new byte[txt.length << 1];
			for (int i = 0; i < txt.length; i++) {
				b[(i << 1)] = (byte) ((txt[i] & 0xff00) >> 8);
				b[(i << 1) + 1] = (byte) (txt[i] & 0x00ff);
			}
		}
		writeBytes(key, b);
	}

	/**
	 * 从指定key中读取char数组.
	 * 
	 * @param key
	 * @return
	 */
	public char[] getChars(int key) {
		byte[] b = getBytes(key);
		if (b == null) {
			return null;
		}
		// char应为偶数个字节
		if ((b.length & 1) > 0) {
			throw new IllegalArgumentException(key
					+ " does not point to char array");
		}
		char[] txt = new char[b.length >> 1];
		for (int i = 0; i < txt.length; i++) {
			txt[i] = (char) (((b[i << 1] & 0xff) << 8) | (b[(i << 1) + 1] & 0xff));
		}
		return txt;
	}

	/**
	 * 将short写入指定key.
	 * 
	 * @param key
	 * @param s
	 */
	public void writeShort(int key, short s) {
		byte[] shortData = new byte[2];
		shortData[0] = (byte) ((s & 0xff00) >> 8);
		shortData[1] = (byte) (s & 0x00ff);
		writeBytes(key, shortData);
	}

	/**
	 * 读取指定key的short值.
	 * 
	 * @param key
	 * @return
	 */
	public short getShort(int key) {
		byte[] b = null;
		b = getBytes(key);
		if (b == null || b.length != 2) {
			return 0;
		} else {
			short s = (short) (((b[0] & 0xff) << 8) | (b[1] & 0xff));
			return s;
		}
	}

	/**
	 * 将boolean写入指定key.
	 * 
	 * @param key
	 * @param b
	 */
	public void writeBoolean(int key, boolean b) {
		writeBytes(key, b ? TRUE : FALSE);
	}

	/**
	 * 得到boolean值.
	 * 
	 * @param key
	 * @return
	 */
	public boolean getBoolean(int key) {
		byte[] b = getBytes(key);
		if (b == null || b.length == 0)
			return false;
		else if (b[0] > 0)
			return true;
		else
			return false;
	}

	/**
	 * 关闭record store.
	 */
	public void close() {
		try {
			_rs.closeRecordStore();
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}

}
