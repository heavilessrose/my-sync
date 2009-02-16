package bm.ui.def.view.menu;

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.ui.def.view.ViewDef;

import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:38:01
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Menu definition.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuDef extends ViewDef {
	public static final byte ENHANCED = 0;
	public static final byte NATIVE = 1;

	public static final byte LIST = 0;
	public static final byte GRID = 1;

	private byte type;
	private String helpId;
	private String imageMap;
	private String imageMapBig;
	private byte mode;
	private String iconSet;
	private Integer fontFace;
	private Integer fontStyle;
	private Integer fontSize;
	private Integer color;
	private Integer selectedColor;
	private Integer background;
	private Integer selectedBackground;
	private Integer helpLines;
	private Vector items = new Vector(10);

	public byte getType() {
		return type;
	}

	public String getHelpId() {
		return helpId;
	}

	public String getImageMap() {
		return imageMap;
	}

	public String getImageMapBig() {
		return imageMapBig;
	}

	public byte getMode() {
		return mode;
	}

	public String getIconSet() {
		return iconSet;
	}

	public Integer getFontFace() {
		return fontFace;
	}

	public Integer getFontStyle() {
		return fontStyle;
	}

	public Integer getFontSize() {
		return fontSize;
	}

	public Integer getColor() {
		return color;
	}

	public Integer getSelectedColor() {
		return selectedColor;
	}

	public Integer getBackground() {
		return background;
	}

	public Integer getSelectedBackground() {
		return selectedBackground;
	}

	public Integer getHelpLines() {
		return helpLines;
	}

	public Vector getItems() {
		return items;
	}

	protected void read(final SerializerInputStream in)
			throws SerializationException {
		in.readByte(); // skip version
		type = in.readByte();
		helpId = in.readNullableString();
		imageMap = in.readNullableString();
		imageMapBig = in.readNullableString();
		mode = in.readByte();
		iconSet = in.readNullableString();
		fontFace = in.readNullableInt();
		fontStyle = in.readNullableInt();
		fontSize = in.readNullableInt();
		color = in.readNullableInt();
		selectedColor = in.readNullableInt();
		background = in.readNullableInt();
		selectedBackground = in.readNullableInt();
		helpLines = in.readNullableInt();

		items.removeAllElements();
		final int count = in.readInt();
		for (int i = 0; i < count; i++) {
			final MenuItemDef item = new MenuItemDef();
			item.read(in);
			items.addElement(item);
		}
	}
}
