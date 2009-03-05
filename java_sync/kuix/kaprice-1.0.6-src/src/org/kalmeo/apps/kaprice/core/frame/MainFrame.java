/*
 * This file is part of org.kalmeo.apps.kaprice.
 * 
 * org.kalmeo.apps.kaprice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * org.kalmeo.apps.kaprice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with org.kalmeo.apps.kaprice.  If not, see <http://www.gnu.org/licenses/>.
 *  
 * Creation date : 12 mars 08
 * Copyright (c) Kalmeo 2007-2008. All rights reserved.
 */

package org.kalmeo.apps.kaprice.core.frame;

import org.kalmeo.apps.kaprice.core.KapriceConstants;
import org.kalmeo.apps.kaprice.core.model.Pad;
import org.kalmeo.apps.kaprice.core.model.ShopList;
import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.kuix.widget.ListItem;
import org.kalmeo.kuix.widget.Screen;
import org.kalmeo.kuix.widget.Widget;
import org.kalmeo.util.frame.Frame;

/**
 * @author omarino
 */
public class MainFrame implements Frame {

	// static instance of MainFrame
	public static MainFrame instance = new MainFrame();

	// MainFrame's Screen
	private Screen screen;

	// The Pad that hold all shopLists
	private final Pad pad = new Pad();

	/**
	 * Show the screen
	 */
	public void showScreen() {
		if (screen != null) {
			screen.setCurrent();
		}
	}

	/**
	 * Force the ShopList to be save in the recordStore, it used in
	 * ShopListFrame onRemove method
	 * 
	 * @param shopList the ShopList wich be saved
	 */
	public void storeShopList(ShopList shopList) {
		pad.storeShopList(shopList);
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onMessage(java.lang.Object, java.lang.Object[])
	 */
	public boolean onMessage(Object identifier, Object[] arguments) {
		if ("askAdd".equals(identifier)) {
			Kuix.showPopupBox(KapriceConstants.MAINFRAME_ADD_POPUP, null);
			return false;
		}
		if ("add".equals(identifier)) {
			// Add a new product only if the TextField is corectly filled 
			if (arguments != null && arguments.length != 0) {
				if (arguments[0] instanceof String) {
					String shopListName = (String) arguments[0];
					if (!pad.createShopList(shopListName)) {
						Kuix.alert(Kuix.getMessage("WARNING_UNNAMED"), KuixConstants.ALERT_WARNING);
					}
				}
			}
			return false;
		}
		if ("askEdit".equals(identifier)) {
			// Call to showAskBox if ListItem is selected, show warning alert else
			if (arguments != null && arguments.length > 0 && arguments[0] instanceof DataProvider) {
				Kuix.showPopupBox(KapriceConstants.MAINFRAME_EDIT_POPUP, (DataProvider) arguments[0]);
			} else {
				Kuix.alert(Kuix.getMessage("WARNING"), KuixConstants.ALERT_WARNING);
			}
			return false;
		}
		if ("edit".equals(identifier)) {
			// Call to editShopList method givin it new name of the selected item
			// Show error message if it failed
			if (arguments != null && arguments.length > 0 && arguments[0] instanceof String) {
				String newShopListName = (String) arguments[0];
				Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
				if (focusedWidget instanceof ListItem) {
					DataProvider dataProvider = ((ListItem) focusedWidget).getDataProvider();
					if (dataProvider instanceof ShopList) {
						pad.modifyShopList((ShopList) dataProvider, newShopListName);
					}
				}
			}
			return false;
		}
		if ("askDelete".equals(identifier)) {
			// Call onMessage method with "delete" name and null arguments only if a ListItem value (so a ShopList) is selected
			Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
			if (focusedWidget instanceof ListItem) {
				DataProvider dataProvider = ((ListItem) focusedWidget).getDataProvider();
				if (dataProvider instanceof ShopList) {
					Kuix.alert(	Kuix.getMessage("ASK_DELETE", new String[] { ((ShopList) dataProvider).name }),
								KuixConstants.ALERT_QUESTION | KuixConstants.ALERT_YES | KuixConstants.ALERT_NO,
								"delete", 
								null);
				}
			} else {
				Kuix.alert(Kuix.getMessage("WARNING"), KuixConstants.ALERT_WARNING);
			}
			return false;
		}
		if ("delete".equals(identifier)) {
			// Ask for a delete to deleteProduct method
			Widget focused = screen.getFocusManager().getFocusedWidget();
			if (focused instanceof ListItem) {
				DataProvider dataProvider = ((ListItem) focused).getDataProvider();
				if (dataProvider instanceof ShopList) {
					pad.removeShopList((ShopList) dataProvider);
				}
			}
			return false;
		}
		if ("askClear".equals(identifier)) {
			// show askClear box wich call onMessage method with "clear" name and null arguments
			Kuix.alert(	Kuix.getMessage("ASK_CLEAR"),
					KuixConstants.ALERT_QUESTION | KuixConstants.ALERT_YES | KuixConstants.ALERT_NO,
					"clear", 
					null);
			return false;
		}
		if ("clear".equals(identifier)) {
			pad.removeAllShopLists();
			return false;
		}
		if ("askQuit".equals(identifier)) {
			// show askQuit box wich call onMessage method with "quit" name and null arguments
			Kuix.alert(	Kuix.getMessage("ASK_QUIT"),
					KuixConstants.ALERT_QUESTION | KuixConstants.ALERT_YES | KuixConstants.ALERT_NO,
					"!exit", 
					null);
			return false;
		}
		if ("show".equals(identifier)) {
			// Select the list and go inside with loading it products before
			Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
			if (focusedWidget instanceof ListItem) {
				DataProvider dataprovider = ((ListItem) focusedWidget).getDataProvider();
				if (dataprovider != null) {
					ShopList shopList = (ShopList) dataprovider;
					if (pad.reloadShopListWithProducts(shopList)) {
						ShopListFrame.instance.setShopList(shopList);
						Kuix.getFrameHandler().pushFrame(ShopListFrame.instance);
					}
				}
			}
			return false;
		}
		if ("showMainScreen".equals(identifier)) {
			showScreen();
		}
		return true;
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onAdded()
	 */
	public void onAdded() {
		try {
		
			// Open the Pad
			pad.open();
			
			// Preload the screen
			screen = Kuix.loadScreen(KapriceConstants.MAINFRAME_SCREEN, pad);
			
			// Open the splash screen
			Widget splash = Kuix.loadWidget(KapriceConstants.SPLASH_XML_FILE, null);
			Kuix.splash(2000, splash, "showMainScreen");
		
		} catch (Exception e) {
			Kuix.alert(e);
			e.printStackTrace();
		}
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onRemoved()
	 */
	public void onRemoved() {
		try {
		
			// Close the Pad
			pad.close();
		
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
