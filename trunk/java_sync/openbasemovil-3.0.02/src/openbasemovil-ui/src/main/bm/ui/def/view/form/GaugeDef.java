package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:52:22
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
public class GaugeDef extends ItemDef {
	protected String label;
	protected String labelExtra;
	protected Boolean readOnly;
	protected String bind;
	protected Integer maxValue;

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

	public int getMaxValue() {
		return maxValue != null ? maxValue.intValue() : 100;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		label = in.readNullableString();
		labelExtra = in.readNullableString();
		readOnly = in.readNullableBoolean();
		bind = in.readNullableString();
		maxValue = in.readNullableInt();
	}
}
