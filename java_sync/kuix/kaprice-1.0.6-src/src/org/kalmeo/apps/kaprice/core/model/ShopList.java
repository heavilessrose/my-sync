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

package org.kalmeo.apps.kaprice.core.model;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Calendar;
import java.util.Date;

import org.kalmeo.kuix.core.Kuix;
import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.util.Filter;
import org.kalmeo.util.NumberUtil;
import org.kalmeo.util.LinkedList.LinkedListEnumeration;

public class ShopList extends DataProvider {

	private static final String NAME_PROPERTY = "name";
	private static final String DATE_PROPERTY = "date";
	private static final String NUM_PRODUCTS_PROPERTY = "numProducts";
	private static final String NUM_IN_CART_PRODUCTS_PROPERTY = "numInCartProducts";
	private static final String PRODUCTS_PROPERTY = "products";

	// Id used for update or delete shopList from the record store
	public int recordId = -1;

	// ShopList's name
	public String name;

	// ShopList's date
	public long date;

	// ShopList's products count
	private int numProducts;

	// ShopList's 'in cart' products count
	private int numInCartProducts;
	
	/**
	 * @param name the name to set
	 */
	public void setName(String name) {
		this.name = name;
		dispatchUpdateEvent(NAME_PROPERTY);
	}

	/**
	 * @param date the date to set
	 */
	public void setDate(long date) {
		this.date = date;
		dispatchUpdateEvent(DATE_PROPERTY);
	}

	/**
	 * @param numProducts the numProducts to set
	 */
	public void setNumProducts(int numProducts) {
		this.numProducts = numProducts;
		dispatchUpdateEvent(NUM_PRODUCTS_PROPERTY);
	}
	
	/**
	 * @param inc
	 */
	public void updateNumInCartProducts(int inc) {
		this.numInCartProducts += inc;
		dispatchUpdateEvent(NUM_IN_CART_PRODUCTS_PROPERTY);
	}

	/* (non-Javadoc)
	 * @see org.kalmeo.kuix.core.model.DataProvider#getUserDefinedValue(java.lang.String)
	 */
	protected Object getUserDefinedValue(String property) {
		if (NAME_PROPERTY.equals(property)) {
			return this.name;
		}
		if (DATE_PROPERTY.equals(property)) {
			Calendar calendar = Calendar.getInstance();

			// Set calendar time with date value
			calendar.setTime(new Date(date));

			// Get value for day, month and year
			int day = calendar.get(Calendar.DAY_OF_MONTH);
			int month = calendar.get(Calendar.MONTH) + 1;
			int year = calendar.get(Calendar.YEAR);

			// Define an array of string adding a "0" character to day or month if necessary
			String decomposedDate[] = { NumberUtil.formatInt(day, 2), NumberUtil.formatInt(month, 2), NumberUtil.formatInt(year, 4) };

			// Get message DATE from messages.property file, givin it decomposedDate array
			return Kuix.getMessage("DATE", decomposedDate);
		}
		if (NUM_PRODUCTS_PROPERTY.equals(property)) {
			return Integer.toString(numProducts);
		}
		if (NUM_IN_CART_PRODUCTS_PROPERTY.equals(property)) {
			return Integer.toString(numInCartProducts);
		}
		return null;
	}

	/**
	 * Create a new Product and add it to shopList and list
	 * 
	 * @param name product's name
	 * @param quantity product's quantity
	 * @return the new Product if it success, null else
	 */
	public boolean addProduct(String name, short quantity) {
		if (name != null && name.length() > 0 && quantity > -1) {

			// Create the product
			Product product = new Product(this);
			product.setName(name);
			product.setQuantity(quantity);

			// Add to the shopList
			int newSize = addItem(PRODUCTS_PROPERTY, product);
			if (newSize != -1) {
				setNumProducts(newSize);
			}

			return true;
		}
		return false;
	}

	/**
	 * Remove a {@link Product} from the list.
	 * 
	 * @param product
	 */
	public void removeProduct(Product product) {
		int newSize = removeItem(PRODUCTS_PROPERTY, product);
		if (newSize != -1) {
			setNumProducts(newSize);
		}
		if (product.inCart) {
			updateNumInCartProducts(-1);
		}
	}

	/**
	 * Modify <code>product</code>'s name and quantity.
	 * 
	 * @param product the Product to modify
	 * @param name the new product's name (if null, previous name is kept)
	 * @param quantity the new product's quantity (if negative, previous
	 *            quantity is kept)
	 * @return <code>true</code> if all is right
	 */
	public boolean modifyProduct(Product product, String name, short quantity) {
		if (product != null && ((name != null && name.length() > 0) || quantity > -1)) {
			if (name != null && name.length() > 0) {
				product.setName(name);
			}
			if (quantity > -1) {
				product.setQuantity(quantity);
			}
			return true;
		}
		return false;
	}

	/**
	 * Unload all products to preserve memory usage. This method do not remove
	 * products form recordStore.
	 */
	public void unloadProducts() {
		removeAllItems(PRODUCTS_PROPERTY);
	}
	
	/**
	 * Remove all products
	 */
	public void removeAllProducts() {
		removeAllItems(PRODUCTS_PROPERTY);
		setNumProducts(0);
		numInCartProducts = 0;
		dispatchUpdateEvent(NUM_IN_CART_PRODUCTS_PROPERTY);
	}

	/**
	 * Sort products
	 * 
	 * @param flag
	 */
	public void sortProducts(int flag) {
		sortItems(PRODUCTS_PROPERTY, flag);
	}
	
	/**
	 * Filter the products
	 * 
	 * @param unaddedOnly
	 */
	public void filterProducts(final boolean unaddedOnly) {
		setItemsFilter(PRODUCTS_PROPERTY, new Filter() {
			public int accept(Object obj) {
				if (unaddedOnly) {
					return ((Product) obj).inCart ? 0 : 1;
				}
				return 1;
			}
		});
	}

	/**
	 * Serialise a list and return it as a byte array
	 * 
	 * @return Byte array containing ShopList under serilaized form
	 */
	public byte[] serialize() {
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
		DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
		try {

			dataOutputStream.writeUTF(name);
			dataOutputStream.writeLong(date);

			LinkedListEnumeration productsEnumeration = enumerateItems(PRODUCTS_PROPERTY, false);
			if (productsEnumeration != null)  {
				dataOutputStream.writeInt(productsEnumeration.getList().getLength());
				productsEnumeration.reset();
				while (productsEnumeration.hasNextItems()) {
					((Product) productsEnumeration.nextItem()).serialize(dataOutputStream);
					
				}
			} else {
				dataOutputStream.writeInt(0);
			}

			return byteArrayOutputStream.toByteArray();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * Unserialize a byte array to get name and date of the ShopList and also
	 * fill it with products if withproducts is true
	 * 
	 * @param rawData byte array containing ShopList under serialized form
	 * @param withProducts true if needed to get Products in the ShopList, false
	 *            else
	 */
	public boolean unserialized(byte[] rawData, boolean withProducts) {
		ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(rawData);
		DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream);
		try {
			name = dataInputStream.readUTF();
			date = dataInputStream.readLong();
			numProducts = dataInputStream.readInt();
			numInCartProducts = 0;
			if (withProducts) {
				for (int i = 0; i < numProducts; i++) {
					Product product = new Product(this);
					product.unserialize(dataInputStream);
					addItem(PRODUCTS_PROPERTY, product);
					if (product.inCart) {
						numInCartProducts++;
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

}
