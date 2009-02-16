package bm.storage;

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.err.ErrorLog;

import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 26-abr-2007 1:31:32
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A MultiplexedRecord.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
class Record implements Serializable {
	private static final String CLASS_NAME = "Record";

	int recordId;
	int multiplexer;
	int size;
	byte[][] items;
	boolean dirty;

	public Record(final int recordId, final int multiplexer) {
		this.recordId = recordId;
		this.multiplexer = multiplexer;
		items = new byte[multiplexer][];
	}

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.storage.Record";
	}

	public int getRecordId() {
		return recordId;
	}

	public void setRecordId(final int recordId) {
		this.recordId = recordId;
	}

	/**
	 * Write object status to stream.
	 * 
	 * @param out
	 *            output stream
	 * @throws bm.core.io.SerializationException
	 *             on errors
	 */
	public void serialize(final SerializerOutputStream out)
			throws SerializationException {
		out.writeByte((byte) 1); // Version
		out.writeInt(multiplexer);
		out.writeInt(size);
		for (int i = 0; i < multiplexer; i++) {
			out.writeNullableBlob(items[i]);
		}
	}

	/**
	 * Read object status from stream.
	 * 
	 * @param in
	 *            input stream
	 * @throws bm.core.io.SerializationException
	 *             on errors
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public void deserialize(final SerializerInputStream in)
			throws SerializationException {
		try {
			in.readByte(); // Skip version
			multiplexer = in.readInt();
			if (multiplexer < 1 || multiplexer > Constants.MAX_MULTIPLEXED_ROWS) {
				ErrorLog.addError(CLASS_NAME, "deserialize", null,
						"Bad multiplexer record (invalid multiplexer: "
								+ multiplexer + ")", null);
				throw new SerializationException(
						Constants.ERR_REC_INVALID_RECORD_DATA,
						"Bad multiplexed record (invalid multiplexer)");
			}
			size = in.readInt();
			if (size < 0 || size > multiplexer) {
				ErrorLog.addError(CLASS_NAME, "deserialize", null,
						"Bad multiplexer record (bad size: " + size
								+ ", multiplexer: " + multiplexer + ")", null);
				throw new SerializationException(
						Constants.ERR_REC_INVALID_RECORD_DATA,
						"Bad multiplexed record (size too large)");
			}
			if (items == null || items.length != multiplexer) {
				items = new byte[multiplexer][];
			}
			for (int i = 0; i < multiplexer; i++) {
				items[i] = in.readNullableBlob();
			}
		} catch (SerializationException e) {
			ErrorLog.addError(CLASS_NAME, "deserialize", null, null, e);
			throw e;
		}
	}

	/**
	 * Set an item in this record.
	 * 
	 * @param index
	 *            item index, from zero to multiplexer - 1
	 * @param data
	 *            data
	 * @param offset
	 *            offset
	 * @param length
	 *            length
	 * @throws RSException
	 *             if the index is invalid
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public void set(final int index, final byte[] data, final int offset,
			final int length) throws RSException {
		if (index >= 0 && index < multiplexer) {
			if (items[index] == null) {
				size++;
			}
			items[index] = new byte[length];
			System.arraycopy(data, offset, items[index], 0, length);
			dirty = true;
		} else {
			throw new RSException(Constants.ERR_REC_ADD_OVERFLOW);
		}
	}

	public boolean isDirty() {
		return dirty;
	}

	public void setDirty(final boolean dirty) {
		this.dirty = dirty;
	}

	public void delete(final int index) throws RSException {
		if (index >= 0 && index < multiplexer) {
			if (items[index] != null) {
				size--;
			}
			items[index] = null;
			dirty = true;
		} else {
			throw new RSException(Constants.ERR_REC_DELETE_OVERFLOW);
		}
	}

	public byte[] get(final int index) throws RSException {
		if (index >= 0 && index < multiplexer) {
			return items[index];
		} else {
			throw new RSException(Constants.ERR_REC_GET_OVERFLOW);
		}
	}

	public int getSize(final int index) throws RSException {
		if (index >= 0 && index < multiplexer) {
			return items[index] != null ? items[index].length : 0;
		} else {
			throw new RSException(Constants.ERR_REC_GET_OVERFLOW);
		}
	}

	public Vector getValidIds(final int baseId) {
		final Vector list = new Vector(multiplexer);
		for (int i = 0; i < multiplexer; i++) {
			if (items[i] != null) {
				list.addElement(new Integer(baseId + i));
			}
		}
		return list;
	}

	public String toString() {
		final StringBuffer buffer = new StringBuffer();
		buffer.append("Record{").append("recordId=").append(recordId).append(
				", multiplexer=").append(multiplexer).append(", size=").append(
				size).append(", items={");
		for (int i = 0; i < multiplexer; i++) {
			if (i > 0) {
				buffer.append(", ");
			}
			buffer.append(items[i] != null ? items[i].length : 0);
		}
		buffer.append("}, dirty=").append(dirty).append('}');
		return buffer.toString();
	}

	public boolean isNull(final int offset) {
		if (offset > -1 && offset < multiplexer) {
			return items[offset] == null;
		} else {
			throw new IllegalArgumentException("Record.isNull( " + offset + ")");
		}
	}
}
