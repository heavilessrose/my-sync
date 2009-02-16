package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:53:59
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A text item.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SpacerDef extends ItemDef {
	protected Integer minWidth;
	protected Integer minHeight;

	public int getMinWidth() {
		return minWidth != null ? minWidth.intValue() : 1;
	}

	public int getMinHeight() {
		return minHeight != null ? minHeight.intValue() : 1;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		minWidth = in.readNullableInt();
		minHeight = in.readNullableInt();
	}
}
