/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.colors;

import com.astrientlabs.midlet.Yipi;

public class Colors {
	public static int BACKGROUND;
	public static int TEXT;
	public static int HEADER_TEXT;

	public static int SOFTKEY_TEXT;
	public static int HEADER_BGCOLOR;
	public static int FOOTER_BGCOLOR;

	public static int SUBMENU_BGCOLOR;
	public static int SUBMENU_HIGHTLIGHT_BG;

	public static int CHECKBOX;

	public static int HIGHLIGHT_TEXT;
	public static int HIGHTLIGHT_BG;

	public static int POPUP_BG;
	public static int POPUP_ERROR_BG;
	public static int POPUP_BORDER;

	public static int HIGHLIGHT_LINE;
	public static int HORIZONTAL_RULE;
	public static int AD_HEADER_BG;
	public static int PUSHBUTTON_BG;
	public static int PUSHBUTTON_FG;
	public static int PUSHBUTTON_HL_BG;
	public static int PUSHBUTTON_HL_FG;
	public static int TEXTINPUT_BG;
	public static int SCROLLBAR;

	public static void init() {
		Colors.BACKGROUND = Yipi.instance.systemPrefs.getInt("ui.bg", 0x0027FF,
				16);
		Colors.TEXT = Yipi.instance.systemPrefs.getInt("ui.txt", 0xFFFFFF, 16);
		Colors.HEADER_TEXT = Yipi.instance.systemPrefs.getInt("ui.htxt",
				0xFFFFFF, 16);
		Colors.SOFTKEY_TEXT = Yipi.instance.systemPrefs.getInt("ui.sktxt",
				0xFFFFF, 16);
		Colors.HEADER_BGCOLOR = Yipi.instance.systemPrefs.getInt("ui.hbg",
				0x252020, 16);
		Colors.FOOTER_BGCOLOR = Yipi.instance.systemPrefs.getInt("ui.fbgc",
				0x000000, 16);
		Colors.SUBMENU_BGCOLOR = Yipi.instance.systemPrefs.getInt("ui.sbg",
				0xEBC961, 16);
		Colors.SUBMENU_HIGHTLIGHT_BG = Yipi.instance.systemPrefs.getInt(
				"ui.shbg", 0xD4B147, 16);
		Colors.CHECKBOX = Yipi.instance.systemPrefs.getInt("ui.chb", 0xEBC961,
				16);
		Colors.HIGHLIGHT_TEXT = Yipi.instance.systemPrefs.getInt("ui.hltxt",
				0xFACF26, 16);
		Colors.HIGHTLIGHT_BG = Yipi.instance.systemPrefs.getInt("ui.hbg",
				0xE8E170, 16);
		Colors.POPUP_BG = Yipi.instance.systemPrefs.getInt("ui.pubg", 0x0000FF,
				16);
		Colors.POPUP_ERROR_BG = Yipi.instance.systemPrefs.getInt("ui.puebg",
				0xEE5555, 16);
		Colors.POPUP_BORDER = Yipi.instance.systemPrefs.getInt("ui.pub",
				0xFFFFFF, 16);
		Colors.HIGHLIGHT_LINE = Yipi.instance.systemPrefs.getInt("ui.hl",
				0xFACF26, 16);
		Colors.HORIZONTAL_RULE = Yipi.instance.systemPrefs.getInt("ui.hr",
				0x344EF3, 16);
		Colors.AD_HEADER_BG = Yipi.instance.systemPrefs.getInt("ui.ahbg",
				0x444444, 16);
		Colors.PUSHBUTTON_BG = Yipi.instance.systemPrefs.getInt("ui.pbg",
				0xB4FF00, 16);
		Colors.PUSHBUTTON_FG = Yipi.instance.systemPrefs.getInt("ui.pfg",
				0x000000, 16);
		Colors.PUSHBUTTON_HL_BG = Yipi.instance.systemPrefs.getInt("ui.pbhfg",
				0xB4FF00, 16);
		Colors.PUSHBUTTON_HL_FG = Yipi.instance.systemPrefs.getInt("ui.pbhbg",
				0xFFFFFF, 16);
		Colors.SCROLLBAR = Yipi.instance.systemPrefs.getInt("ui.sb", 0xC8D8FB,
				16);

		TEXTINPUT_BG = Yipi.instance.systemPrefs.getInt("ti.bg", 0x000000, 16);
	}
}
