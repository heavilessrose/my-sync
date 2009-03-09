package org.kalmeo.kuix.util;

/**
 * @author bbeaulant
 */
public class Span {

	public int colspan;
	public int rowspan;

	/**
	 * Construct a {@link Span}
	 */
	public Span() {
	}

	/**
	 * Construct a {@link Span}
	 * 
	 * @param colspan
	 * @param rowspan
	 */
	public Span(int colspan, int rowspan) {
		this.colspan = colspan;
		this.rowspan = rowspan;
	}

}
