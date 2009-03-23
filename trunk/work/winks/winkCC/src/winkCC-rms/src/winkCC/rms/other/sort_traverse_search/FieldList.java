package winkCC.rms.other.sort_traverse_search;

import java.io.*;
import javax.microedition.rms.*;

// Maintains information about the fields in a
// field-based record store. Currently just a list of
// field types and (optional) field names, but could
// easily be expanded to store other information.
public class FieldList {

	private static final int VERSION = 1;

	// �ֶ�����
	public static final byte TYPE_BOOLEAN = 1;
	public static final byte TYPE_BYTE = 2;
	public static final byte TYPE_CHAR = 3;
	public static final byte TYPE_SHORT = 4;
	public static final byte TYPE_INT = 5;
	public static final byte TYPE_LONG = 6;
	public static final byte TYPE_STRING = 7;

	/** �ֶ����� */
	private String[] _names;
	/** �ֶ����� */
	private byte[] _types;

	public FieldList() {
	}

	/**
	 * 
	 * @param numFields
	 *            �ֶ���
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

	/** ���ֽ������ж�ȡһ���ֶ��б� */
	public void fromByteArray(byte[] data) throws IOException {
		ByteArrayInputStream bin = new ByteArrayInputStream(data);
		fromDataStream(new DataInputStream(bin));
		bin.close();
	}

	/** ��data stream�ж�ȡһ���ֶ��б� */
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
			// ����_types
			din.readFully(_types);

			for (int i = 0; i < numFields; ++i) {
				_names[i] = din.readUTF();
			}
		}
	}

	/** ��record store�ж�ȡһ���ֶ��б� */
	public void fromRecordStore(RecordStore rs, int index) throws IOException,
			RecordStoreException {
		fromByteArray(rs.getRecord(index));
	}

	/** �����ֶ����� */
	public void setFieldName(int index, String name) {
		_names[index] = name;
	}

	/** �����ֶ����� */
	public void setFieldType(int index, byte type) {
		_types[index] = type;
	}

	/** �����ֶ��б�byte���� */
	public byte[] toByteArray() throws IOException {
		ByteArrayOutputStream bout = new ByteArrayOutputStream();
		toDataStream(new DataOutputStream(bout));
		byte[] data = bout.toByteArray();
		bout.close();
		return data;
	}

	/** �����ֶ��б�data stream */
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

	/** ���ֶ��б�д��record store */
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

		// ��record������,�ڴ�record֮ǰ��ӿռ�¼,������record
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
