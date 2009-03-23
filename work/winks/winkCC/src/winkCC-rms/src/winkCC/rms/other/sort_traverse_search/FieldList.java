package winkCC.rms.other.sort_traverse_search;

import java.io.*;
import javax.microedition.rms.*;

// Maintains information about the fields in a
// field-based record store. Currently just a list of
// field types and (optional) field names, but could
// easily be expanded to store other information.
public class FieldList {

	private static final int VERSION = 1;

	// 字段类型
	public static final byte TYPE_BOOLEAN = 1;
	public static final byte TYPE_BYTE = 2;
	public static final byte TYPE_CHAR = 3;
	public static final byte TYPE_SHORT = 4;
	public static final byte TYPE_INT = 5;
	public static final byte TYPE_LONG = 6;
	public static final byte TYPE_STRING = 7;

	/** 字段名字 */
	private String[] _names;
	/** 字段类型 */
	private byte[] _types;

	public FieldList() {
	}

	/**
	 * 
	 * @param numFields
	 *            字段数
	 */
	public FieldList(int numFields) {
		if (numFields < 0 || numFields > 255) {
			throw new IllegalArgumentException("Bad number of fields");
		}

		_types = new byte[numFields];
		_names = new String[numFields];
	}

	public int getFieldCount() {
		return _types != null ? _types.length : 0;
	}

	public String getFieldName(int index) {
		String name = _names[index];
		return name != null ? name : "";
	}

	public byte getFieldType(int index) {
		return _types[index];
	}

	/** 从字节数组中读取一个字段列表 */
	public void fromByteArray(byte[] data) throws IOException {
		ByteArrayInputStream bin = new ByteArrayInputStream(data);
		fromDataStream(new DataInputStream(bin));
		bin.close();
	}

	/** 从data stream中读取一个字段列表 */
	public void fromDataStream(DataInputStream din) throws IOException {
		int version = din.readUnsignedByte();
		if (version != VERSION) {
			throw new IOException("Incorrect version " + version
					+ " for FieldList, expected " + VERSION);
		}

		int numFields = din.readUnsignedByte();

		_types = new byte[numFields];
		_names = new String[numFields];

		if (numFields > 0) {
			// 读满_types
			din.readFully(_types);

			for (int i = 0; i < numFields; ++i) {
				_names[i] = din.readUTF();
			}
		}
	}

	/** 从record store中读取一个字段列表 */
	public void fromRecordStore(RecordStore rs, int index) throws IOException,
			RecordStoreException {
		fromByteArray(rs.getRecord(index));
	}

	/** 设置字段名字 */
	public void setFieldName(int index, String name) {
		_names[index] = name;
	}

	/** 设置字段类型 */
	public void setFieldType(int index, byte type) {
		_types[index] = type;
	}

	/** 保存字段列表到byte数组 */
	public byte[] toByteArray() throws IOException {
		ByteArrayOutputStream bout = new ByteArrayOutputStream();
		toDataStream(new DataOutputStream(bout));
		byte[] data = bout.toByteArray();
		bout.close();
		return data;
	}

	/** 保存字段列表到data stream */
	public void toDataStream(DataOutputStream out) throws IOException {
		out.writeByte(VERSION);

		int count = getFieldCount();

		out.writeByte(count);

		if (count > 0) {
			out.write(_types, 0, count);

			for (int i = 0; i < count; ++i) {
				out.writeUTF(getFieldName(i));
			}
		}
	}

	/** 把字段列表写到record store */
	public int toRecordStore(RecordStore rs, int index) throws IOException,
			RecordStoreException {
		byte[] data = toByteArray();
		boolean add = true;

		if (index > 0) {
			try {
				rs.setRecord(index, data, 0, data.length);
				add = false;
			} catch (InvalidRecordIDException e) {
			}
		}

		// 若record不存在,在此record之前添加空记录,后存入该record
		if (add) {
			synchronized (rs) {
				int nextID = rs.getNextRecordID();
				if (index <= 0)
					index = nextID;

				while (nextID < index) {
					rs.addRecord(null, 0, 0);
				}

				if (nextID == index) {
					rs.addRecord(data, 0, data.length);
				}
			}
		}

		return index;
	}

}
