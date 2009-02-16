package bm.core.tools;

import java.util.Enumeration;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 30-oct-2007 23:37:37
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A union of two or more enumerations that look like a single one.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class EnumerationUnion implements Enumeration {
	private Vector elements = new Vector(10);
	private int current;

	public void add(final Enumeration i) {
		elements.addElement(i);
	}

	public boolean hasMoreElements() {
		if (current <= elements.size()) {
			final Enumeration i = (Enumeration) elements.elementAt(current);
			if (i.hasMoreElements()) {
				return true;
			} else {
				current++;
				return hasMoreElements();
			}
		} else {
			return false;
		}
	}

	public Object nextElement() {
		if (current <= elements.size()) {
			final Enumeration i = (Enumeration) elements.elementAt(current);
			if (i.hasMoreElements()) {
				return i.nextElement();
			} else {
				current++;
				return nextElement();
			}
		} else {
			return null;
		}
	}
}