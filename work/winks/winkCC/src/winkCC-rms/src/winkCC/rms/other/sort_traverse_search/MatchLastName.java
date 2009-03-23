package winkCC.rms.other.sort_traverse_search;

import java.io.IOException;

// A filter that matches a specific last name
// in an employee record.
public class MatchLastName extends FieldBasedFilter {
	public MatchLastName(String name) {
		this(name, null);
	}

	public MatchLastName(String name, byte[] ignore) {
		super(ignore);
		_name = name;
	}

	protected boolean matchFields() {
		try {
			readField(FieldList.TYPE_INT);
			readField(FieldList.TYPE_STRING);

			String ln = (String) readField(FieldList.TYPE_STRING);

			return ln.equals(_name);
		} catch (IOException e) {
			return false;
		}
	}

	private String _name;
}
