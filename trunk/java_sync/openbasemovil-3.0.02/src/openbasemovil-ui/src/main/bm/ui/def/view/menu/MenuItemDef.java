package bm.ui.def.view.menu;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:02:39
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Menu item definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuItemDef {
	private String title;
	private int action;
	private String onlyIf;
	private String unless;
	private Integer iconIndex;
	private Integer selectedIconIndex;
	private String help;
	private String view;

	public String getTitle() {
		return title;
	}

	public int getAction() {
		return action;
	}

	public String getOnlyIf() {
		return onlyIf;
	}

	public String getUnless() {
		return unless;
	}

	public Integer getIconIndex() {
		return iconIndex;
	}

	public Integer getSelectedIconIndex() {
		return selectedIconIndex;
	}

	public String getHelp() {
		return help;
	}

	public String getView() {
		return view;
	}

	public void read(final SerializerInputStream in)
			throws SerializationException {
		final byte version = in.readByte();
		title = in.readString();
		action = in.readInt();
		onlyIf = in.readNullableString();
		unless = in.readNullableString();
		iconIndex = in.readNullableInt();
		selectedIconIndex = in.readNullableInt();
		help = in.readNullableString();
		if (version > 1) {
			view = in.readNullableString();
		}
	}
}
