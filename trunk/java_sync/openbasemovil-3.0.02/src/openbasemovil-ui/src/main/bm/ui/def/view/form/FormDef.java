package bm.ui.def.view.form;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.ui.def.view.ViewDef;
import bm.ui.def.view.listBrowser.FieldDef;

import java.util.Vector;
import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:17:37
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Form bean.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class FormDef extends ViewDef {
	private boolean readOnly = true;
	private Boolean saveOnAccept;
	private Boolean defaultButtons;
	private Vector items;
	private Hashtable itemsByName;

	public boolean isReadOnly() {
		return readOnly;
	}

	public Boolean getSaveOnAccept() {
		return saveOnAccept;
	}

	public Boolean getDefaultButtons() {
		return defaultButtons;
	}

	public Vector getItems() {
		return items;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version

		readOnly = in.readBoolean();
		saveOnAccept = in.readNullableBoolean();
		defaultButtons = in.readNullableBoolean();

		final int count = in.readInt();
		items = new Vector(count);
		itemsByName = new Hashtable(count);
		for (int i = 0; i < count; i++) {
			final ItemDef item = ItemDef.create(in);
			items.addElement(item);
			itemsByName.put(item.getName(), item);
		}
	}

	public FieldDef get(final String name) {
		return (FieldDef) itemsByName.get(name);
	}
}
