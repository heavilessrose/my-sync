package org.kalmeo.demo.kuix.model;

import org.kalmeo.kuix.core.model.DataProvider;
import org.kalmeo.util.LinkedListItem;

/**
 * @author omarino
 */
public class Media extends DataProvider {

	public final static int TITLE_FLAG = 0;
	public final static int TYPE_FLAG = 1;

	private final static String TITLE_PROPERTY = "sTitle";
	private final static String TYPE_PROPERTY = "vType";

	public int type;
	public String title;

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.core.model.DataProvider#getUserDefinedValue(java.lang
	 * .String)
	 */
	protected Object getUserDefinedValue(String property) {
		if (TITLE_PROPERTY.equals(property)) {
			return title;
		}
		if (TYPE_PROPERTY.equals(property)) {
			return Integer.toString(type);
		}
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seeorg.kalmeo.kuix.core.model.DataProvider#compareTo(org.kalmeo.util.
	 * LinkedListItem, int)
	 */
	public int compareTo(LinkedListItem item, int flag) {
		if (item instanceof Media) {
			Media media = (Media) item;
			switch (flag) {
			case TITLE_FLAG:
				return title.compareTo(media.title);
			case TYPE_FLAG:
				return media.type - type;
			}
		}
		return 0;
	}
}
