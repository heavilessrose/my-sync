package winkCC.rms.other.sort_traverse_search;

import java.io.*;
import java.util.*;
import javax.microedition.rms.*;

// Finds the contacts whose first and/or last
// names match the given values.

public class FindContacts {

	// Constructs the finder for the given names. If
	// both names are non-null, both names must match,
	// otherwise only the given name needs to match.
	public FindContacts(String fname, String lname) {
		_fname = normalize(fname);
		_lname = normalize(lname);
	}

	// Traverses the data in the record store and
	// returns a list of matching Contact objects.
	public Vector list(RecordStore rs) throws RecordStoreException, IOException {

		Vector v = new Vector();
		Filter f = new Filter(v);
		RecordEnumeration enum = rs.enumerateRecords(f, null, false);

		// The enum will never have any elements in it,
		// but we call this to force it to traverse
		// its list.
		enum.hasNextElement();
		enum.destroy();

		return v;
	}

	// Returns whether or not a given Contact
	// instance matches our criteria.
	public boolean matchesContact(Contact c) {
		boolean sameFirst = false;
		boolean sameLast = false;

		if (_fname != null) {
			sameFirst = c.getFirstName().toLowerCase().equals(_fname);
		}

		if (_lname != null) {
			sameLast = c.getLastName().toLowerCase().equals(_lname);
		}

		if (_fname != null && _lname != null) {
			return sameFirst && sameLast;
		}

		return sameFirst || sameLast;
	}

	// Normalize our name data
	private static String normalize(String name) {
		return (name != null ? name.trim().toLowerCase() : null);
	}

	private String _fname;
	private String _lname;

	// A record filter that always returns false but
	// whenever it finds a matching contact it adds it
	// to the given list.
	private class Filter implements RecordFilter {
		private Filter(Vector list) {
			_list = list;
		}

		public boolean matches(byte[] data) {
			try {
				Contact c = new Contact();
				c.fromByteArray(data);

				if (matchesContact(c)) {
					_list.addElement(c);
				}
			} catch (IOException e) {
			}

			return false;
		}

		private Vector _list;
	}
}
