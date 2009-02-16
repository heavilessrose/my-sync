package bm.ui;

import bm.core.Properties;
import bm.core.ErrorHandler;
import bm.core.event.Event;
import bm.db.Settings;
import bm.db.DBException;

import javax.microedition.lcdui.Font;

/*
 * File Information
 *
 * Created on       : 21-ago-2007 1:18:39
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Global UI settings and common utilities.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Ui {
	private static int fontSize = Font.SIZE_SMALL;
	public static final Integer ET_FONTSIZE = new Integer(1001);

	static {
		try {
			fontSize = (int) Settings.getPropertyLong("Ui.fontSize",
					Font.SIZE_SMALL);
		} catch (DBException e) {
			fontSize = Font.SIZE_SMALL;
		}
	}

	public static int getFontSize() {
		return fontSize;
	}

	public static synchronized void setFontSize(final int fontSize) {
		if (Ui.fontSize != fontSize) {
			Ui.fontSize = fontSize;
			try {
				Settings.setProperty("Ui.fontSize", Integer.toString(fontSize));
			} catch (DBException e) {
				ErrorHandler.handleError("Ui", e);
			}
			new Event(ET_FONTSIZE).dispatch();
		}
	}

	public static Font parseFont(final Properties definitions, final String name) {
		if (definitions.containsKey(name + ".fontFace")) {
			return Font.getFont(definitions.getInt(name + ".fontFace",
					Font.FACE_PROPORTIONAL), definitions.getInt(name
					+ ".fontStyle", Font.STYLE_PLAIN), definitions.getInt(name
					+ ".fontSize", Font.SIZE_SMALL));
		} else if (definitions.containsKey(name + ".fontStyle")) {
			return Font.getFont(Font.FACE_PROPORTIONAL, definitions.getInt(name
					+ ".fontStyle", Font.STYLE_PLAIN), definitions.getInt(name
					+ ".fontSize", Font.SIZE_SMALL));
		} else if (definitions.containsKey(name + ".fontSize")) {
			return Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN,
					definitions.getInt(name + ".fontSize", Font.SIZE_SMALL));
		} else {
			return null;
		}
	}

	static int addLight(final int color, final int light) {
		int red = ((color & 0xFF0000) >> 16) + light;
		int green = ((color & 0xFF00) >> 8) + light;
		int blue = (color & 0xFF) + light;
		if (red > 0xFF) {
			red = 0xFF;
		} else if (red < 0) {
			red = 0;
		}
		if (green > 0xFF) {
			green = 0xFF;
		} else if (green < 0) {
			green = 0;
		}
		if (blue > 0xFF) {
			blue = 0xFF;
		} else if (blue < 0) {
			blue = 0;
		}
		return (red << 16) | (green << 8) | blue;
	}
}
