package org.kalmeo.apps.kaprice.core.model;

import java.util.Date;

import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;

import org.kalmeo.apps.kaprice.core.KapriceConstants;
import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.util.LinkedList;
import org.kalmeo.util.LinkedListItem;

/**
 * @author bbeaulant
 */
public class Pad extends DataProvider {

	// Property names
	private static final String SHOP_LISTS_PROPERTY = "shopLists";

	// The associated recordStore
	private RecordStore recordStore;

	/**
	 * Open the pad and then the RecordStore and load stored shopLists without
	 * products.
	 * 
	 * @throws Exception
	 */
	public void open() throws Exception {
		// Open the recordStore
		recordStore = RecordStore.openRecordStore(
				KapriceConstants.DEFAULT_RECORDSTORE, true);

		// Load shopLists from recordStore
		RecordEnumeration recordEnumeration = recordStore.enumerateRecords(
				null, null, true);
		while (recordEnumeration.hasPreviousElement()) {

			int recordId = recordEnumeration.previousRecordId();
			byte[] rawData = recordStore.getRecord(recordId);

			ShopList shopList = new ShopList();
			if (shopList.unserialized(rawData, false)) {
				shopList.recordId = recordId;
				addItem(SHOP_LISTS_PROPERTY, shopList);
			} else {
				// Invalid record : we delete it
				recordStore.deleteRecord(recordId);
			}

		}
	}

	/**
	 * Close the Pad and then the RecordStore
	 * 
	 * @throws Exception
	 */
	public void close() throws Exception {
		recordStore.closeRecordStore();
	}

	/**
	 * Relead shopList with products
	 * 
	 * @param shopList
	 * @return <code>true</code> if all is right
	 */
	public boolean reloadShopListWithProducts(ShopList shopList) {
		if (shopList.recordId != -1) {
			byte[] rawData;
			try {
				rawData = recordStore.getRecord(shopList.recordId);
				shopList.unserialized(rawData, true);
				return true;
			} catch (Exception e) {
				e.printStackTrace();
				return false;
			}
		}
		return false;
	}

	/**
	 * @param shopList
	 */
	public boolean createShopList(String name) {
		if (name != null && name.length() > 0) {

			// Create the model
			ShopList shopList = new ShopList();
			shopList.setName(name);
			shopList.setDate(new Date().getTime());

			// Add shopList model
			addItem(SHOP_LISTS_PROPERTY, shopList);

			// Store it in the recordStore
			byte[] rawData = shopList.serialize();
			int recordId = -1;
			try {
				recordId = recordStore.addRecord(rawData, 0, rawData.length);
			} catch (Exception e) {
				e.printStackTrace();
			}
			shopList.recordId = recordId;

			return true;
		}
		return false;
	}

	/**
	 * @param shopList
	 */
	public void removeShopList(ShopList shopList) {

		// Remove shopList model
		removeItem(SHOP_LISTS_PROPERTY, shopList);

		// Remove the record
		if (shopList.recordId != -1) {
			try {
				recordStore.deleteRecord(shopList.recordId);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	/**
	 * Modify shopList's name
	 * 
	 * @param shopList
	 * @param name
	 * @return <code>true</code> if all is right
	 */
	public boolean modifyShopList(ShopList shopList, String name) {
		if (name != null && name.length() > 0) {

			// Update shopList name
			shopList.setName(name);

			// Store the shop list
			storeShopList(shopList);

			return true;
		}
		return false;
	}

	/**
	 * Store the Shoplist if it has been already stored.
	 * 
	 * @param shopList
	 */
	public void storeShopList(ShopList shopList) {
		if (shopList.recordId != -1) {
			byte[] rawData = shopList.serialize();
			try {
				recordStore.setRecord(shopList.recordId, rawData, 0,
						rawData.length);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Remove all shopLists.
	 */
	public void removeAllShopLists() {

		// Remove all records
		Object tmpShopLists = getValue(SHOP_LISTS_PROPERTY);
		if (tmpShopLists instanceof LinkedList) {
			LinkedList shopLists = (LinkedList) tmpShopLists;
			for (LinkedListItem shopList = shopLists.getFirst(); shopList != null; shopList = shopList
					.getNext()) {
				int recordId = ((ShopList) shopList).recordId;
				if (recordId != -1) {
					try {
						recordStore.deleteRecord(recordId);
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		}

		// Remove all shopList models
		removeAllItems(SHOP_LISTS_PROPERTY);

	}

}
