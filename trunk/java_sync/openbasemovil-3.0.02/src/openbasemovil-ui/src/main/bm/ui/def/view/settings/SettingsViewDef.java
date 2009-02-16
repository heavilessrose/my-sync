package bm.ui.def.view.settings;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.ui.def.view.ViewDef;

import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:30:48
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Settings view definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SettingsViewDef extends ViewDef {
	private String helpId;
	private String imageMap;
	private String imageMapBig;

	private Vector items = new Vector(10);

	public String getHelpId() {
		return helpId;
	}

	public String getImageMap() {
		return imageMap;
	}

	public String getImageMapBig() {
		return imageMapBig;
	}

	public Vector getItems() {
		return items;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		helpId = in.readNullableString();
		imageMap = in.readNullableString();
		imageMapBig = in.readNullableString();

		items.removeAllElements();
		final int count = in.readInt();
		for (int i = 0; i < count; i++) {
			final Setting setting = new Setting();
			setting.read(in);
			items.addElement(setting);
		}
	}
}
