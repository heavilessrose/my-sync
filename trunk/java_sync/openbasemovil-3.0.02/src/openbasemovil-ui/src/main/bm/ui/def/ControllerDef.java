package bm.ui.def;

import bm.db.index.Index;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:14:26
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Controller bean.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ControllerDef {
	private String className;

	public String getClassName() {
		return className;
	}

	public void read(final byte[] data) throws SerializationException {
		final ByteArrayInputStream bais = new ByteArrayInputStream(data);
		final SerializerInputStream in = new SerializerInputStream(bais);

		in.readByte(); // skip version
		className = in.readString();
	}
}
