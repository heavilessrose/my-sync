package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:49:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Date field definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DateDef extends ItemDef {
	protected String label;
	protected String labelExtra;
	protected Boolean readOnly;
	protected String bind;
	protected String text;

	public DateDef() {
	}

	public String getLabel() {
		return label;
	}

	public String getLabelExtra() {
		return labelExtra;
	}

	public boolean isReadOnly() {
		return readOnly != null && readOnly.booleanValue();
	}

	public String getBind() {
		return bind;
	}

	public String getText() {
		return text;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		label = in.readNullableString();
		labelExtra = in.readNullableString();
		readOnly = in.readNullableBoolean();
		bind = in.readNullableString();
		text = in.readNullableString();
	}
}
