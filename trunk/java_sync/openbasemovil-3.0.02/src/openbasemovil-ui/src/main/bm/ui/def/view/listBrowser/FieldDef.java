package bm.ui.def.view.listBrowser;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.math.FixedPoint;

import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.DateField;
import javax.microedition.lcdui.Gauge;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:17:47
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * List field definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class FieldDef {
	public static final byte PLAIN = 0;
	public static final byte DATE = 1;
	public static final byte TIME = 2;
	public static final byte MONEY = 3;
	public static final byte STATIC = 4;

	private String name;
	private Byte mode;
	private String label;
	private String trueLabel;
	private String falseLabel;
	private String resource;

	public String getName() {
		return name;
	}

	public Byte getMode() {
		return mode;
	}

	public String getLabel() {
		return label;
	}

	public String getTrueLabel() {
		return trueLabel;
	}

	public String getFalseLabel() {
		return falseLabel;
	}

	public String getResource() {
		return resource;
	}

	public void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version

		name = in.readString();
		mode = in.readNullableByte();
		label = in.readNullableString();
		trueLabel = in.readNullableString();
		falseLabel = in.readNullableString();
		resource = in.readNullableString();
	}
}
