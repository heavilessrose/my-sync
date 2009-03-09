package org.kalmeo.apps.kaprice.core.model;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.util.BooleanUtil;
import org.kalmeo.util.LinkedListItem;

/**
 * @author omarino
 */
public class Product extends DataProvider {

	private static final String NAME_PROPERTY = "name";
	private static final String QUANTITY_PROPERTY = "quantity";
	private static final String QUANTITY_GT1_PROPERTY = "quantitygt1";
	private static final String IN_CART_PRODUCTS_PROPERTY = "inCart";

	// Flag used for name or 'in cart' sort
	public static final int NAME = 0;
	public static final int IN_CART = 1;

	// The parent ShopList
	private final ShopList shopList;

	// Product's name
	public String name;

	// Product's quantity
	public short quantity;

	// Product's 'in cart' state (true if 'in cart', false else)
	public boolean inCart;

	/**
	 * Construct a {@link Product}
	 * 
	 * @param shopList
	 */
	public Product(ShopList shopList) {
		this.shopList = shopList;
	}

	/**
	 * @param name
	 *            the name to set
	 */
	public void setName(String name) {
		this.name = name;
		dispatchUpdateEvent(NAME_PROPERTY);
	}

	/**
	 * @param quantity
	 *            the quantity to set
	 */
	public void setQuantity(short quantity) {
		this.quantity = quantity;
		dispatchUpdateEvent(QUANTITY_PROPERTY);
		dispatchUpdateEvent(QUANTITY_GT1_PROPERTY);
	}

	/**
	 * @param inCart
	 *            the inCart to set
	 */
	public void setInCart(boolean inCart) {
		this.inCart = inCart;
		dispatchUpdateEvent(IN_CART_PRODUCTS_PROPERTY);
		shopList.updateNumInCartProducts(inCart ? 1 : -1);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.core.model.DataProvider#getUserDefinedValue(java.lang
	 * .String)
	 */
	protected Object getUserDefinedValue(String property) {
		if (NAME_PROPERTY.equals(property)) {
			return name;
		}
		if (QUANTITY_PROPERTY.equals(property)) {
			return String.valueOf(quantity);
		}
		if (QUANTITY_GT1_PROPERTY.equals(property)) {
			return BooleanUtil.toString(quantity > 1);
		}
		if (IN_CART_PRODUCTS_PROPERTY.equals(property)) {
			return BooleanUtil.toString(inCart);
		}
		return null;
	}

	/**
	 * Take a DataOutputStream and fill it with product name, quantity and added
	 * state
	 * 
	 * @param dataOutputStream
	 *            is a stream wich be filled
	 * @throws IOException
	 */
	public void serialize(DataOutputStream dataOutputStream) throws IOException {
		dataOutputStream.writeUTF(name);
		dataOutputStream.writeShort(quantity);
		dataOutputStream.writeBoolean(inCart);
	}

	/**
	 * Take a DataInputStream to rebuilt itself
	 * 
	 * @param dataInputStream
	 *            containing serialized Product
	 * @throws IOException
	 */
	public void unserialize(DataInputStream dataInputStream) throws IOException {
		name = dataInputStream.readUTF();
		quantity = dataInputStream.readShort();
		inCart = dataInputStream.readBoolean();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seeorg.kalmeo.kuix.core.data.DataProvider#compareTo(org.kalmeo.util.
	 * LinkedListItem, int)
	 */
	public int compareTo(LinkedListItem item, int flag) {
		switch (flag) {
		case NAME:
			return name.compareTo(((Product) item).name);
		case IN_CART:
			int thisAdded = inCart ? 1 : 0;
			int productAdded = ((Product) item).inCart ? 1 : 0;

			return thisAdded - productAdded;
		}
		return 0;
	}
}
