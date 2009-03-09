package org.kalmeo.util;

public interface Filter {

	/**
	 * Returns An integer greater than 0 if the <code>obj</code> is acceptable
	 * in this {@link Filter}. If the value is lesser or equals than 0 the
	 * <code>obj</code> is rejected.
	 * 
	 * @param obj
	 * @return An integer greater than 0 if the <code>obj</code> is acceptable
	 *         in this {@link Filter}
	 */
	public int accept(Object obj);

}
