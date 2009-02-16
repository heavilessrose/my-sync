package bm.ui.def.view.form;

import bm.core.io.SerializerInputStream;
import bm.core.io.SerializationException;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:08:53
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
public class FixedPointDef extends TextDef {
	protected int decimals;

	public int getDecimals() {
		return decimals;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		super.read(in);
		decimals = in.readInt();
	}
}
