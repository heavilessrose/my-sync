package bm.ui.def;

import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;

import java.io.ByteArrayInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:11:18
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Icon set definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class IconSetDef {
	private String source;
	private int width;
	private int height;

	public String getSource() {
		return source;
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public void read(final byte[] data) throws SerializationException {
		final ByteArrayInputStream bais = new ByteArrayInputStream(data);
		final SerializerInputStream in = new SerializerInputStream(bais);

		in.readByte(); // skip version
		source = in.readString();
		width = in.readInt();
		height = in.readInt();
	}
}
