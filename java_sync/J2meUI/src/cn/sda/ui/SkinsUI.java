package cn.sda.ui;

import java.io.IOException;
import java.util.Hashtable;
import javax.microedition.lcdui.Image;

/**
 * 
 * @author Administrator
 */
public class SkinsUI {
	private Image button_bg;
	private Image button_down_bg;
	private Image form_bg;
	private Hashtable skins = new Hashtable();

	private SkinsUI() {
		try {
			String skinPath = "/cn/sda/ui/skins/";
			String skinKey = "";
			skinKey = "button_bg.png";
			skins.put(skinKey, Image.createImage(skinPath + skinKey));
			skinKey = "button_down_bg.png";
			skins.put(skinKey, Image.createImage(skinPath + skinKey));
			skinKey = "form_bg.png";
			skins.put(skinKey, Image.createImage(skinPath + skinKey));
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public static SkinsUI CreateSkins() {
		return new SkinsUI();
	}

	public Image getImage(String skinKey) {
		return (Image) skins.get(skinKey);
	}
}
