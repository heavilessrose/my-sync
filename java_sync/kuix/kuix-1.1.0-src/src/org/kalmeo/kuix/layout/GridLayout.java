package org.kalmeo.kuix.layout;

import org.kalmeo.kuix.util.Gap;
import org.kalmeo.kuix.util.Insets;
import org.kalmeo.kuix.util.Metrics;
import org.kalmeo.kuix.widget.Widget;

/**
 * @author bbeaulant
 */
public class GridLayout implements Layout {

	// Static instance of a [1, 1] GridLayout
	public static final GridLayout instanceOneByOne = new GridLayout();

	// GridLayout properties
	public int numCols;
	public int numRows;

	/**
	 * Construct a {@link GridLayout}
	 */
	public GridLayout() {
		this(1, 1);
	}

	/**
	 * Construct a {@link GridLayout}
	 * 
	 * @param numCols
	 * @param numRows
	 */
	public GridLayout(int numCols, int numRows) {
		this.numCols = numCols;
		this.numRows = numRows;
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
		Gap gap = target.getGap();
		int gapWidth = gap.horizontalGap * (numCols - 1);
		int gapHeight = gap.verticalGap * (numRows - 1);
		int width = preferredWidth - insets.left - insets.right;
		int height = target.getHeight() - insets.top - insets.bottom;
		int cellWidth = (width - gapWidth) / numCols;
		int cellHeight = (height - gapHeight) / numRows;
		int col = 0;
		int row = 0;
		int preferredContentWidth = 0;
		int preferredContentHeight = 0;

		for (Widget widget = target.getChild(); widget != null; widget = widget.next) {

			if (!widget.isIndividualyVisible()) {
				continue;
			}

			if (layout) {
				widget
						.setBounds(insets.left + col
								* (cellWidth + gap.horizontalGap), insets.top
								+ row * (cellHeight + gap.verticalGap),
								cellWidth, cellHeight);
			} else {
				Metrics preferedSize = widget.getPreferredSize(width);
				preferredContentWidth = Math.max(preferredContentWidth,
						preferedSize.width);
				preferredContentHeight = Math.max(preferredContentHeight,
						preferedSize.height);
			}
			col++;
			if (col >= numCols) {
				col = 0;
				row++;
				if (row >= numRows) {
					break;
				}
			}

		}

		if (!layout) {
			metrics.width = insets.left
					+ Math.max(minSize.width, preferredContentWidth * numCols
							+ gapWidth) + insets.right;
			metrics.height = insets.top
					+ Math.max(minSize.height, preferredContentHeight * numRows
							+ gapHeight) + insets.bottom;
		}

	}

}
