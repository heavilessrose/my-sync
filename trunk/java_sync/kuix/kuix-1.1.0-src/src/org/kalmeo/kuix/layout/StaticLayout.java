package org.kalmeo.kuix.layout;

import org.kalmeo.kuix.util.Alignment;
import org.kalmeo.kuix.util.Insets;
import org.kalmeo.kuix.util.Metrics;
import org.kalmeo.kuix.widget.Widget;
import org.kalmeo.util.MathFP;

/**
 * @author bbeaulant
 */
public class StaticLayout implements Layout {

	// Static instance of a StaticLayout
	public static final StaticLayout instance = new StaticLayout();

	/**
	 * Construct a {@link StaticLayout}
	 */
	private StaticLayout() {
		// Constructor is private because only the static instance could be use
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.layout.Layout#computePreferredSize(org.kalmeo.kuix.widget
	 * .Widget, int, org.kalmeo.kuix.util.Metrics)
	 */
	public void measurePreferredSize(Widget target, int preferredWidth,
			Metrics metrics) {
		measure(target, false, preferredWidth, metrics);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.layout.Layout#doLayout(org.kalmeo.kuix.widget.Widget)
	 */
	public void doLayout(Widget target) {
		measure(target, true, target.getWidth(), null);
	}

	/**
	 * Measure <code>target</code> children layout
	 * 
	 * @param target
	 * @param layout
	 * @param preferredWidth
	 * @param metrics
	 */
	private void measure(Widget target, boolean layout, int preferredWidth,
			Metrics metrics) {

		Insets insets = target.getInsets();
		Metrics minSize = target.getMinSize();
		int width = preferredWidth - insets.left - insets.right;
		int height = target.getHeight() - insets.top - insets.bottom;

		int maxWidth = 0;
		int maxHeight = 0;

		int x = 0;
		int y = 0;
		int widgetWidth = 0;
		int widgetHeight = 0;
		Metrics preferredSize;

		for (Widget widget = target.getChild(); widget != null; widget = widget.next) {

			if (!widget.isIndividualyVisible()) {
				continue;
			}

			x = 0;
			y = 0;
			widgetWidth = 0;
			widgetHeight = 0;
			preferredSize = null;

			LayoutData layoutData = widget.getLayoutData();
			if (layoutData instanceof StaticLayoutData) {

				StaticLayoutData staticLayoutData = (StaticLayoutData) layoutData;
				Alignment alignment = staticLayoutData.alignment;

				x = staticLayoutData.x;
				y = staticLayoutData.y;

				// Width
				if (staticLayoutData.width > MathFP.ONE) {
					// Pixel
					widgetWidth = MathFP.toInt(staticLayoutData.width);
				} else if (staticLayoutData.width < 0) {
					// Min-size
					preferredSize = widget.getPreferredSize(width);
					widgetWidth = preferredSize.width;
				} else {
					// %
					widgetWidth = MathFP.toInt(MathFP.mul(MathFP.toFP(width),
							staticLayoutData.width));
				}

				// Height
				if (staticLayoutData.height > MathFP.ONE) {
					// Pixel
					widgetHeight = MathFP.toInt(staticLayoutData.height);
				} else if (staticLayoutData.height < 0) {
					// Min-size
					if (preferredSize == null) {
						preferredSize = widget.getPreferredSize(widgetWidth);
					}
					widgetHeight = preferredSize.height;
				} else {
					// %
					widgetHeight = MathFP.toInt(MathFP.mul(MathFP.toFP(height),
							staticLayoutData.height));
				}

				if (alignment != null) {
					if (alignment.isRight()) {
						x += width - widgetWidth;
					} else if (alignment.isHorizontalCenter()) {
						x += (width - widgetWidth) / 2;
					}
					if (alignment.isBottom()) {
						y += height - widgetHeight;
					} else if (alignment.isVerticalCenter()) {
						y += (height - widgetHeight) / 2;
					}
				}

			} else {

				preferredSize = widget.getPreferredSize(width);
				widgetWidth = preferredSize.width;
				widgetHeight = preferredSize.height;

			}

			if (layout) {
				widget.setBounds(insets.left + x, insets.top + y, widgetWidth,
						widgetHeight);
			}

			maxWidth = Math.max(maxWidth, widgetWidth);
			maxHeight = Math.max(maxHeight, widgetHeight);
		}

		if (!layout) {
			metrics.width = insets.left + Math.max(minSize.width, maxWidth)
					+ insets.right;
			metrics.height = insets.top + Math.max(minSize.height, maxHeight)
					+ insets.bottom;
		}

	}

}
