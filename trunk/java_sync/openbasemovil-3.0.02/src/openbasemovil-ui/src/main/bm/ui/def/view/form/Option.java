package bm.ui.def.view.form;

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:02:32
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Option definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Option {
	private String label;

	public String getLabel() {
		return label;
	}

	public void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		label = in.readString();
	}
}
