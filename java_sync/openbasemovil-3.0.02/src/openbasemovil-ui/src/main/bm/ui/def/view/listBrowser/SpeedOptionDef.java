package bm.ui.def.view.listBrowser;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 23:25:25
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Speed Option definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class SpeedOptionDef {
	private int code;
	private String text;
	private String iconSet;
	private Integer icon;
	private String help;
	private String onlyIf;
	private String unless;

	public int getCode() {
		return code;
	}

	public String getText() {
		return text;
	}

	public String getIconSet() {
		return iconSet;
	}

	public Integer getIcon() {
		return icon;
	}

	public String getHelp() {
		return help;
	}

	public String getOnlyIf() {
		return onlyIf;
	}

	public String getUnless() {
		return unless;
	}

	public void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		code = in.readInt();
		text = in.readNullableString();
		iconSet = in.readNullableString();
		icon = in.readNullableInt();
		help = in.readNullableString();
		onlyIf = in.readNullableString();
		unless = in.readNullableString();
	}
}
