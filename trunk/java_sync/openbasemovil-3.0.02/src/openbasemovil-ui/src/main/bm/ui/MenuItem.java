package bm.ui;

import bm.core.ResourceManager;

import javax.microedition.lcdui.Image;

/*
 * File Information
 *
 * Created on       : 28-ago-2007 17:56:21
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An item in a menu.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class MenuItem {
	String label;
	Image image;
	Image selectedImage;
	int action;
	String help;
	String iconSet;
	String view;

	public String getLabel() {
		return label;
	}

	public void setLabel(final String label) {
		this.label = label;
	}

	public Image getImage() {
		return image;
	}

	public void setImage(final Image image) {
		this.image = image;
	}

	public Image getSelectedImage() {
		return selectedImage;
	}

	public void setSelectedImage(final Image selectedImage) {
		this.selectedImage = selectedImage;
	}

	public int getAction() {
		return action;
	}

	public void setAction(final int action) {
		this.action = action;
	}

	public String getHelp() {
		return help;
	}

	public void setHelp(final String help) {
		this.help = help;
	}

	public String getView() {
		return view;
	}

	public void setView(final String view) {
		this.view = view;
	}

	public MenuItem(final String label, final int iconIndex,
			final int selectedIconIndex, final int action, final String help,
			final String iconSet, final String view) {
		this.label = ResourceManager.getResource(label);
		this.help = help != null ? ResourceManager.getResource(help) : "";
		image = IconStore.getImage(iconSet, iconIndex);
		selectedImage = IconStore.getImage(iconSet, selectedIconIndex);
		this.action = action;
		this.iconSet = iconSet;
		this.view = view;
	}
}
