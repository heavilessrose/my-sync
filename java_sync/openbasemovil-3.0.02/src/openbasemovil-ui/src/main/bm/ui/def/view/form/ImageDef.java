package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

import javax.microedition.lcdui.Item;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 21:55:32
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
public class ImageDef extends ItemDef {
	protected String label;
	protected String labelExtra;
	protected Integer appearance;
	protected String bind;
	protected String altText;

	public String getLabel() {
		return label;
	}

	public String getLabelExtra() {
		return labelExtra;
	}

	public int getAppearance() {
		return appearance != null ? appearance.intValue() : Item.PLAIN;
	}

	public String getBind() {
		return bind;
	}

	public String getAltText() {
		return altText;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version

		label = in.readNullableString();
		labelExtra = in.readNullableString();
		bind = in.readNullableString();
		altText = in.readNullableString();
		appearance = in.readNullableInt();
	}
}
