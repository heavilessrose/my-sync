package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:57:40
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
public class ChoiceDef extends ItemDef {
	public static final int EXCLUSIVE = 1;
	public static final int MULTIPLE = 2;
	public static final int POPUP = 4;

	protected String label;
	protected String labelExtra;
	protected String bind;
	protected Boolean wrap;
	protected int choiceType;
	protected Vector options = new Vector(10);

	public String getLabel() {
		return label;
	}

	public String getLabelExtra() {
		return labelExtra;
	}

	public String getBind() {
		return bind;
	}

	public boolean isWrap() {
		return wrap == null || wrap.booleanValue();
	}

	public int getChoiceType() {
		return choiceType;
	}

	public Vector getOptions() {
		return options;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		label = in.readNullableString();
		labelExtra = in.readNullableString();
		wrap = in.readNullableBoolean();
		bind = in.readNullableString();
		choiceType = in.readInt();

		options.removeAllElements();
		final int count = in.readInt();
		for (int i = 0; i < count; i++) {
			final Option option = new Option();
			option.read(in);
			options.addElement(option);
		}
	}
}
