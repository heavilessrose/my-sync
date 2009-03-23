package winkCC.rms.other.sort_traverse_search;

import java.io.*;
import javax.microedition.rms.*;

// A base class for writing and reading arbitrary
// data as defined by a FieldList
public abstract class FieldBasedRecordMapper {

	// Some useful constants
	public static Boolean TRUE = new Boolean(true);
	public static Boolean FALSE = new Boolean(false);

	// Markers for the types of string we support
	private static final byte NULL_STRING_MARKER = 0;
	private static final byte UTF_STRING_MARKER = 1;

	private DirectByteArrayInputStream _bin;
	private DirectByteArrayOutputStream _bout;
	private DataInputStream _din;
	private DataOutputStream _dout;

	// Constructs the mapper for the given list
	protected FieldBasedRecordMapper() {
	}

	// Prepares for input by setting the data buffer.
	protected void prepareForInput(byte[] data) {
		if (_bin == null) {
			_bin = new DirectByteArrayInputStream(data);
			_din = new DataInputStream(_bin);
		} else {
			_bin.resetByteArray(data);
		}
	}

	// Prepares the store for output. The streams are reused.
	protected void prepareForOutput() {
		if (_bout == null) {
			_bout = new DirectByteArrayOutputStream();
			_dout = new DataOutputStream(_bout);
		} else {
			_bout.reset();
		}
	}

	// Reads a field from the buffer.
	protected Object readField(int type) throws IOException {
		switch (type) {
		case FieldList.TYPE_BOOLEAN:
			return _din.readBoolean() ? TRUE : FALSE;
		case FieldList.TYPE_BYTE:
			return new Byte(_din.readByte());
		case FieldList.TYPE_CHAR:
			return new Character(_din.readChar());
		case FieldList.TYPE_SHORT:
			return new Short(_din.readShort());
		case FieldList.TYPE_INT:
			return new Integer(_din.readInt());
		case FieldList.TYPE_LONG:
			return new Long(_din.readLong());
		case FieldList.TYPE_STRING: {
			byte marker = _din.readByte();
			if (marker == UTF_STRING_MARKER) {
				return _din.readUTF();
			}
		}
		}

		return null;
	}

	/** 把一个对象转换为boolean值 */
	public static boolean toBoolean(Object value) {
		if (value instanceof Boolean) {
			return ((Boolean) value).booleanValue();
		} else if (value != null) {
			String str = value.toString().trim();

			if (str.equals("true"))
				return true;
			if (str.equals("false"))
				return false;

			return (toInt(value) != 0);
		}

		return false;
	}

	/** 把一个对象转换为char */
	public static char toChar(Object value) {
		if (value instanceof Character) {
			return ((Character) value).charValue();
		} else if (value != null) {
			String s = value.toString();
			if (s.length() > 0) {
				return s.charAt(0);
			}
		}

		return 0;
	}

	/** 把一个对象转换为int */
	public static int toInt(Object value) {
		if (value instanceof Integer) {
			return ((Integer) value).intValue();
		} else if (value instanceof Boolean) {
			return ((Boolean) value).booleanValue() ? 1 : 0;
		} else if (value instanceof Byte) {
			return ((Byte) value).byteValue();
		} else if (value instanceof Character) {
			return ((Character) value).charValue();
		} else if (value instanceof Short) {
			return ((Short) value).shortValue();
		} else if (value instanceof Long) {
			return (int) ((Long) value).longValue();
		} else if (value != null) {
			try {
				return Integer.parseInt(value.toString());
			} catch (NumberFormatException e) {
			}
		}

		return 0;
	}

	/** 把一个对象转换为long */
	public static long toLong(Object value) {
		if (value instanceof Integer) {
			return ((Integer) value).longValue();
		} else if (value instanceof Boolean) {
			return ((Boolean) value).booleanValue() ? 1 : 0;
		} else if (value instanceof Byte) {
			return ((Byte) value).byteValue();
		} else if (value instanceof Character) {
			return ((Character) value).charValue();
		} else if (value instanceof Short) {
			return ((Short) value).shortValue();
		} else if (value instanceof Long) {
			return ((Long) value).longValue();
		} else if (value != null) {
			try {
				return Long.parseLong(value.toString());
			} catch (NumberFormatException e) {
			}
		}

		return 0;
	}

	/** 把字段写入 output buffer */
	protected void writeField(int type, Object value) throws IOException {
		switch (type) {
		case FieldList.TYPE_BOOLEAN:
			_dout.writeBoolean(toBoolean(value));
			break;
		case FieldList.TYPE_BYTE:
			_dout.write((byte) toInt(value));
			break;
		case FieldList.TYPE_CHAR:
			_dout.writeChar(toChar(value));
			break;
		case FieldList.TYPE_SHORT:
			_dout.writeShort((short) toInt(value));
			break;
		case FieldList.TYPE_INT:
			_dout.writeInt(toInt(value));
			break;
		case FieldList.TYPE_LONG:
			_dout.writeLong(toLong(value));
			break;
		case FieldList.TYPE_STRING:
			if (value != null) {
				String str = value.toString();
				_dout.writeByte(UTF_STRING_MARKER);
				_dout.writeUTF(str);
			} else {
				_dout.writeByte(NULL_STRING_MARKER);
			}
			break;
		}
	}

	// Writes a set of fields to the output stream.
	protected byte[] writeStream(FieldList list, Object[] fields)
			throws IOException {
		int count = list.getFieldCount();
		int len = (fields != null ? fields.length : 0);

		prepareForOutput();

		for (int i = 0; i < count; ++i) {
			writeField(list.getFieldType(i), (i < len ? fields[i] : null));
		}

		return _bout.getByteArray();
	}
}
