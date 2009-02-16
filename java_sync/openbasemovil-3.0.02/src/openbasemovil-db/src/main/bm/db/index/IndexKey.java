package bm.db.index;

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.math.FixedPoint;
import bm.db.Constants;

import java.util.Date;

/*
 * File Information
 *
 * Created on       : 19-mar-2007 21:37:32
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An index key encapsulates different key types (and null) making it
 * transparent for the index.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
class IndexKey implements Serializable {
	// private static final Log log = LogFactory.getLog( "IndexKey" );

	private Object value;

	public IndexKey(final Object value) {
		this.value = value;
	}

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.db.index.IndexKey";
	}

	/**
	 * Write object status to stream.
	 * 
	 * @param out
	 *            output stream
	 * @throws SerializationException
	 *             on errors
	 */
	public void serialize(SerializerOutputStream out)
			throws SerializationException {
		final Object value = this.value;
		out.writeBoolean(value == null);
		if (value != null) {
			if (value instanceof Long) {
				out.writeByte(Constants.FT_LONG);
				out.writeLong(((Long) value).longValue());
			} else if (value instanceof Integer) {
				out.writeByte(Constants.FT_INT);
				out.writeInt(((Integer) value).intValue());
			} else if (value instanceof Date) {
				out.writeByte(Constants.FT_DATE);
				out.writeLong(((Date) value).getTime());
			} else if (value instanceof Short) {
				out.writeByte(Constants.FT_SHORT);
				out.writeInt(((Short) value).shortValue());
			} else if (value instanceof FixedPoint) {
				out.writeByte(Constants.FT_FIXED_POINT);
				FixedPoint.serialize(out, (FixedPoint) value, false);
			} else {
				out.writeByte(Constants.FT_STRING);
				out.writeString(value.toString());
			}
		}
	}

	/**
	 * Read object status from stream.
	 * 
	 * @param in
	 *            input stream
	 * @throws SerializationException
	 *             on errors
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public void deserialize(SerializerInputStream in)
			throws SerializationException {
		final boolean isNull = in.readBoolean();
		if (isNull) {
			value = null;
		} else {
			final byte type = in.readByte();
			switch (type) {
			case Constants.FT_DATE:
				value = new Date(in.readLong());
				break;

			case Constants.FT_FIXED_POINT:
				value = FixedPoint.deserialize(in, false);
				break;

			case Constants.FT_INT:
				value = new Integer(in.readInt());
				break;

			case Constants.FT_LONG:
				value = new Long(in.readLong());
				break;

			case Constants.FT_SHORT:
				value = new Short(in.readShort());
				break;

			case Constants.FT_STRING:
				value = in.readString();
				break;
			}
		}
	}

	public int compare(final IndexKey key, final boolean caseSensitive) {
		final Object value = this.value;
		if (value == null) {
			return key.value == null ? 0 : -1;
		} else if (key.value == null) {
			return 1;
		} else {
			if (value.getClass().isInstance(key.value)) {
				if (value instanceof Long) {
					return (int) (((Long) value).longValue() - ((Long) key.value)
							.longValue());
				} else if (value instanceof Date) {
					return (int) (((Date) value).getTime() - ((Date) key.value)
							.getTime());
				} else if (value instanceof Integer) {
					return (((Integer) value).intValue() - ((Integer) key.value)
							.intValue());
				} else if (value instanceof Short) {
					return (((Short) value).shortValue() - ((Short) key.value)
							.shortValue());
				} else if (caseSensitive) {
					return value.toString().compareTo(key.value.toString());
				} else {
					return value.toString().toLowerCase().compareTo(
							key.value.toString().toLowerCase());
				}
			} else if (caseSensitive) {
				return value.toString().compareTo(key.value.toString());
			} else {
				return value.toString().toLowerCase().compareTo(
						key.value.toString().toLowerCase());
			}
		}
	}

	public boolean isNull() {
		return value == null;
	}

	public String toString() {
		return "IndexKey{" + "value=" + value + '}';
	}

	public String getString() {
		return value != null ? value.toString() : "";
	}

	public long longValue() {
		final Object value = this.value;
		if (value == null) {
			return 0;
		} else if (value instanceof Long) {
			return ((Long) value).longValue();
		} else if (value instanceof Integer) {
			return ((Integer) value).intValue();
		} else if (value instanceof Short) {
			return ((Short) value).shortValue();
		} else if (value instanceof Date) {
			return ((Date) value).getTime();
		} else {
			return 0;
		}
	}

	public boolean equals(final Object o) {
		if (this == o) {
			return true;
		}
		if (o == null || getClass() != o.getClass()) {
			return false;
		}

		final IndexKey indexKey = (IndexKey) o;

		return !(value != null ? !value.equals(indexKey.value)
				: indexKey.value != null);

	}

	public int hashCode() {
		return (value != null ? value.hashCode() : 0);
	}
}
