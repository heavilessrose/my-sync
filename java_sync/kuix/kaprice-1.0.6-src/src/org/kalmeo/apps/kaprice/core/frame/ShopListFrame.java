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
import org.kalmeo.apps.kaprice.core.model.Product;
import org.kalmeo.apps.kaprice.core.model.ShopList;
import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.kuix.widget.CheckBox;
import org.kalmeo.kuix.widget.Screen;
import org.kalmeo.kuix.widget.Widget;
import org.kalmeo.util.frame.Frame;

/**
 * @author omarino
 */
public class ShopListFrame implements Frame {

	// Static instance of ShopListFrame 
	public static ShopListFrame instance = new ShopListFrame();

	// Products's linked list
	private ShopList shopList;

	// MainFrame's Screen
	private Screen screen;

	/**
	 * Initialize shopList with a ShopList given in parameter. setList MUST BE
	 * called before frame is pushed in FrameHandler
	 * 
	 * @param shopList the ShopList wich be display (threw list)
	 */
	public void setShopList(ShopList shopList) {
		this.shopList = shopList;
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onMessage(java.lang.Object, java.lang.Object[])
	 */
	public boolean onMessage(Object identifier, Object[] arguments) {
		if ("askAdd".equals(identifier)) {
			Kuix.showPopupBox(KapriceConstants.SHOPLISTFRAME_ADD_POPUP, null);
			return false;
		}
		if ("add".equals(identifier)) {
			// Add a new product only if TextFields are corectly filled 
			if (arguments != null && arguments.length > 1 && arguments[0] instanceof String && arguments[1] instanceof String) {
				String productName = (String) arguments[0];
				String productQuantityString = (String) arguments[1];
				short productQuantity = -1;
				if (productQuantityString != null && productQuantityString.length() > 0) {
					productQuantity = Short.parseShort(productQuantityString);
				}

				if (!shopList.addProduct(productName, productQuantity)) {
					Kuix.alert(Kuix.getMessage("ERROR"), KuixConstants.ALERT_ERROR);
				}
			}
			return false;
		}
		if ("askEdit".equals(identifier)) {
			// Call to showAskBox if ListItem is selected, show warning alert else
			if (arguments != null && arguments.length > 0 && arguments[0] instanceof Product) {
				Kuix.showPopupBox(KapriceConstants.SHOPLISTFRAME_EDIT_POPUP, (Product) arguments[0]);
			} else {
				Kuix.alert(Kuix.getMessage("WARNING_UNSELECTED_PRODUCT"), KuixConstants.ALERT_WARNING);
			}
			return false;
		}
		if ("edit".equals(identifier)) {
			// Call to editProduct method givin it new name and quantity of the selected item
			// Show error message if it failed
			if (arguments != null && arguments.length > 1 && arguments[0] instanceof String && arguments[1] instanceof String) {
				String newProductName = (String) arguments[0];
				String newProductQuantityString = (String) arguments[1];
				short newProductQuantity = -1;
				if (newProductQuantityString != null && newProductQuantityString.length() > 0) {
					newProductQuantity = Short.parseShort(newProductQuantityString);
				}
				
				Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
				if (focusedWidget instanceof CheckBox) {
					DataProvider dataProvider = ((CheckBox) focusedWidget).getDataProvider();
					if (dataProvider instanceof Product) {
						if (!shopList.modifyProduct((Product) dataProvider, newProductName, newProductQuantity)) {
							Kuix.alert(Kuix.getMessage("ERROR"), KuixConstants.ALERT_ERROR);
						}
					}
				}
			}
			return false;
		}
		if ("askDelete".equals(identifier)) {
			// Call onMessage method with "delete" name and null arguments only if a ListItem value (so a Product) is selected
			Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
			if (focusedWidget instanceof CheckBox) {
				DataProvider dataProvider = ((CheckBox) focusedWidget).getDataProvider();
				if (dataProvider instanceof Product) {
					Kuix.alert(	Kuix.getMessage("ASK_DELETE", new String[] { ((Product) dataProvider).name }),
								KuixConstants.ALERT_QUESTION | KuixConstants.ALERT_YES | KuixConstants.ALERT_NO,
								"delete", 
								null);
				}
			} else {
				Kuix.alert(Kuix.getMessage("WARNING_UNSELECTED_PRODUCT"), KuixConstants.ALERT_WARNING);
			}
			return false;
		}
		if ("delete".equals(identifier)) {
			// Ask for a delete to deleteProduct method
			Widget focusedWidget = screen.getFocusManager().getFocusedWidget();
			if (focusedWidget instanceof CheckBox) {
				DataProvider dataProvider = ((CheckBox) focusedWidget).getDataProvider();
				if (dataProvider != null) {
					shopList.removeProduct((Product) dataProvider);
				}
			}
			return false;
		}
		if ("askClear".equals(identifier)) {
			// show askClear box wich call onMessage method with "clear" name and null arguments
			Kuix.alert(	Kuix.getMessage("REMOVEALL_PRODUCTS"),
						KuixConstants.ALERT_QUESTION | KuixConstants.ALERT_YES | KuixConstants.ALERT_NO,
						"clear", 
						null);
			return false;
		}
		if ("clear".equals(identifier)) {
			shopList.removeAllProducts();
			return false;
		}
		if ("select".equals(identifier)) {
			if (arguments != null && arguments.length == 1 && arguments[0] instanceof Product) {
				Product product = ((Product) arguments[0]);
				product.setInCart(!product.inCart);
			}
			return false;
		}
		// Sort the list
		if ("sort".equals(identifier)) {
			if (arguments != null && arguments[0] instanceof String) {
				int flag = 0;
				if ("inCart".equals(arguments[0])) {
					flag = Product.IN_CART;
				}
				if ("name".equals(arguments[0])) {
					flag = Product.NAME;
				}
				shopList.sortProducts(flag);
			}
		}
		if ("setFilter".equals(identifier)) {
			shopList.filterProducts(arguments.length == 1);
		}
		if ("return".equals(identifier)) {
			// Remove itself and call onRemove method
			Kuix.getFrameHandler().removeFrame(instance);
			return false;
		}
		return true;
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onAdded()
	 */
	public void onAdded() {
		
		// Load the screen
		screen = Kuix.loadScreen(KapriceConstants.SHOPLISTFRAME_SCREEN, shopList);
		
		// Set screen as current screen
		screen.setCurrent();
		
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.util.frame.Frame#onRemoved()
	 */
	public void onRemoved() {
		
		// show MainFrame screen
		MainFrame.instance.showScreen();
		
		// Save the shopList
		// /!\ all changes are saved only if ShopListFrame (this) is removed
		MainFrame.instance.storeShopList(shopList);

		// On MainScreen, ShopList containt isn't displayed
		//  it will be reload on next selection of the ShopList
		//
		// /!\ On unserialize ShopList's method we just add product form the record store
		//  so if shopList is already loaded is contain is doubled
		shopList.unloadProducts();
		
		// Clean unused variables
		shopList = null;
		screen = null;
		
	}
}
