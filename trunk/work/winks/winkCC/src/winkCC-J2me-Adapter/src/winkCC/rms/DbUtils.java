package winkCC.rms;

import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;
import javax.microedition.rms.RecordStoreNotOpenException;

public class DbUtils {
	protected static RecordStore _rs = null;

	/**
	 * Record store名字.
	 */
	public static String RS_NAME = "winksCC2";
	/** boolean flag true identifier */
	private static final byte[] TRUE = { (byte) 1 };

	/** boolean flag false identifier */
	private static final byte[] FALSE = { (byte) 0 };

	/** Dummy data for new recordIds */
	private static final byte[] DUMMY = new byte[1];

	public static void init(String rsName) {
		RS_NAME = rsName;
	}

	/**
	 * 增添一条记录.
	 */
	public static int add() {
		int id = 0;
		try {
			id = getStore().addRecord(DUMMY, 0, 0);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
		} catch (RecordStoreNotFoundException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
		return id;
	}

	public static boolean existsRecord(int recordId) {
		try {
			getStore().getRecordSize(recordId);
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
			return false;
		} catch (InvalidRecordIDException e) {
			e.printStackTrace();
			return false;
		} catch (RecordStoreFullException e) {
			e.printStackTrace();
			return false;
		} catch (RecordStoreNotFoundException e) {
			e.printStackTrace();
			return false;
		} catch (RecordStoreException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	//	private void open() throws RecordStoreFullException,
	//			RecordStoreNotFoundException, RecordStoreException {
	//		_rs = RecordStore.openRecordStore(RS_NAME, true);
	//	}

	/**
	 * 读取一个记录.
	 * 
	 * @param recordId
	 */
	public static byte[] getBytes(int recordId) {
		byte[] b = null;
		try {
			b = getStore().getRecord(recordId);
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
	 * @param recordId
	 * @param data
	 */
	public static void writeBytes(int recordId, byte[] data) {
		try {
			if (data == null) {
				getStore().setRecord(recordId, DUMMY, 0, 0);
			} else {
				getStore().setRecord(recordId, data, 0, data.length);
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
	private static RecordStore getStore() throws RecordStoreFullException,
			RecordStoreNotFoundException, RecordStoreException {
		if (_rs == null) {
			_rs = RecordStore.openRecordStore(RS_NAME, true);
		}
		return _rs;
	}

	/**
	 * 将int写入recordId.
	 * 
	 * @param recordId
	 * @param i
	 */
	public static void writeInt(int recordId, int i) {
		byte[] intData = new byte[4];
		intData[0] = (byte) ((i & 0xff000000) >> 24);
		intData[1] = (byte) ((i & 0x00ff0000) >> 16);
		intData[2] = (byte) ((i & 0x0000ff00) >> 8);
		intData[3] = (byte) (i & 0x000000ff);
		writeBytes(recordId, intData);
	}

	/**
	 * 得到recordId对应的int值.
	 * 
	 * @param recordId
	 * @return int
	 */
	public static int getInt(int recordId) {
		byte[] b = getBytes(recordId);
		if (b == null || b.length != 4) {
			return 0;
		} else {
			int i = ((b[0] & 0xff) << 24) | ((b[1] & 0xff) << 16)
					| ((b[2] & 0xff) << 8) | (b[3] & 0xff);
			return i;
		}
	}

	/**
	 * 将String写入recordId.
	 * 
	 * @param recordId
	 * @param str
	 */
	public static void writeString(int recordId, String str) {
		if (str != null || !str.equals(""))
			writeBytes(recordId, str.getBytes());
	}

	/**
	 * 得到recordId对应的String值.
	 * 
	 * @param recordId
	 * @return String
	 */
	public static String getString(int recordId) {
		return new String(getBytes(recordId));
	}

	/**
	 * 将long写入recordId.
	 * 
	 * @param recordId
	 * @param l
	 */
	public static void writeLong(int recordId, long l) {
		byte[] longData = new byte[8];
		longData[0] = (byte) ((l & 0xff00000000000000L) >> 58);
		longData[1] = (byte) ((l & 0x00ff000000000000L) >> 48);
		longData[2] = (byte) ((l & 0x0000ff0000000000L) >> 40);
		longData[3] = (byte) ((l & 0x000000ff00000000L) >> 32);
		longData[4] = (byte) ((l & 0x00000000ff000000L) >> 24);
		longData[5] = (byte) ((l & 0x0000000000ff0000L) >> 16);
		longData[6] = (byte) ((l & 0x000000000000ff00L) >> 8);
		longData[7] = (byte) (l & 0x00000000000000ffL);
		writeBytes(recordId, longData);
	}

	/**
	 * 得到recordId的long值.
	 * 
	 * @param recordId
	 * @return long
	 */
	public static long getLong(int recordId) {
		byte[] b = getBytes(recordId);
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
	 * 将char[]写入recordId.
	 * 
	 * @param recordId
	 * @param txt
	 */
	public static void writeChars(int recordId, char[] txt) {
		byte[] b = null;
		if (txt != null) {
			b = new byte[txt.length << 1];
			for (int i = 0; i < txt.length; i++) {
				b[(i << 1)] = (byte) ((txt[i] & 0xff00) >> 8);
				b[(i << 1) + 1] = (byte) (txt[i] & 0x00ff);
			}
		}
		writeBytes(recordId, b);
	}

	/**
	 * 从指定recordId中读取char数组.
	 * 
	 * @param recordId
	 * @return char[]
	 */
	public static char[] getChars(int recordId) {
		byte[] b = getBytes(recordId);
		if (b == null) {
			return null;
		}
		// char应为偶数个字节
		if ((b.length & 1) > 0) {
			throw new IllegalArgumentException(recordId
					+ " does not point to char array");
		}
		char[] txt = new char[b.length >> 1];
		for (int i = 0; i < txt.length; i++) {
			txt[i] = (char) (((b[i << 1] & 0xff) << 8) | (b[(i << 1) + 1] & 0xff));
		}
		return txt;
	}

	/**
	 * 将short写入recordId.
	 * 
	 * @param recordId
	 * @param s
	 */
	public static void writeShort(int recordId, short s) {
		byte[] shortData = new byte[2];
		shortData[0] = (byte) ((s & 0xff00) >> 8);
		shortData[1] = (byte) (s & 0x00ff);
		writeBytes(recordId, shortData);
	}

	/**
	 * 读取recordId的short值.
	 * 
	 * @param recordId
	 * @return short
	 */
	public static short getShort(int recordId) {
		byte[] b = null;
		b = getBytes(recordId);
		if (b == null || b.length != 2) {
			return 0;
		} else {
			short s = (short) (((b[0] & 0xff) << 8) | (b[1] & 0xff));
			return s;
		}
	}

	/**
	 * 将boolean写入recordId.
	 * 
	 * @param recordId
	 * @param b
	 */
	public static void writeBoolean(int recordId, boolean b) {
		writeBytes(recordId, b ? TRUE : FALSE);
	}

	/**
	 * 得到recordId的boolean值.
	 * 
	 * @param recordId
	 * @return boolean
	 */
	public static boolean getBoolean(int recordId) {
		byte[] b = getBytes(recordId);
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
	public static void close() {
		try {
			_rs.closeRecordStore();
		} catch (RecordStoreNotOpenException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}

}
