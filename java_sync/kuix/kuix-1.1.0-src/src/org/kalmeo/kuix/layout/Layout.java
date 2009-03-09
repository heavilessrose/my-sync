package org.kalmeo.kuix.layout;

import org.kalmeo.kuix.util.Metrics;
import org.kalmeo.kuix.widget.Widget;

/**
 * 布局类要实现的接口
 * 
 * @author bbeaulant
 */
public interface Layout {

	/**
	 * Returns the {@link Metrics} that represents the widget's preferedSize
	 * 
	 * @param target
	 *            The target widget
	 * @param preferredWidth
	 */
	public void measurePreferredSize(Widget target, int preferredWidth,
			Metrics metrics);

	/**
	 * Do the layout on the specified {@link Widget}
	 * 
	 * @param target
	 *            The target widget
	 */
	public void doLayout(Widget target);

}
