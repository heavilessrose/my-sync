package winkCC.rms.other.sort_traverse_search;

import javax.microedition.rms.*;

// A record filter for filtering records whose data
// is mapped to a field list. The actual filter will
// extend this class and implement the matchFields
// method appropriately.
public abstract class FieldBasedFilter extends FieldBasedRecordMapper implements
		RecordFilter {

	// Constructs the filter. The optional byte
	// array is an array that we want ignored,
	// usually the first record in the record store
	// where we store the field information.
	protected FieldBasedFilter() {
		this(null);
	}

	protected FieldBasedFilter(byte[] ignore) {
		_ignore = ignore;
	}

	// Compares two byte arrays.
	private boolean equal(byte[] a1, byte[] a2) {
		int len = a1.length;

		if (len != a2.length)
			return false;

		for (int i = 0; i < len; ++i) {
			if (a1[i] != a2[i])
				return false;
		}

		return true;
	}

	// Called to filter a record.
	public boolean matches(byte[] data) {
		if (_ignore != null) {
			if (equal(_ignore, data))
				return false;
		}

		prepareForInput(data);
		return matchFields();
	}

	// The actual filter implements this method.
	protected abstract boolean matchFields();

	private byte[] _ignore;
}
