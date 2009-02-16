package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:09:36
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Button definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ButtonDef extends ItemDef {
	public static final byte BUTTON_TYPE_BACK = 0;
	public static final byte BUTTON_TYPE_CANCEL = 1;
	public static final byte BUTTON_TYPE_EXIT = 2;
	public static final byte BUTTON_TYPE_HELP = 3;
	public static final byte BUTTON_TYPE_OK = 4;
	public static final byte BUTTON_TYPE_STOP = 5;
	public static final byte BUTTON_TYPE_HOME = 6;
	public static final byte BUTTON_TYPE_CUSTOM = 7;

	protected String label;
	protected String image;
	protected int code;
	protected int buttonType;

	public String getLabel() {
		return label;
	}

	public String getImage() {
		return image;
	}

	public int getCode() {
		return code;
	}

	public int getButtonType() {
		return buttonType;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version

		buttonType = in.readInt();
		code = in.readInt();
		label = in.readNullableString();
		image = in.readNullableString();
	}
}
