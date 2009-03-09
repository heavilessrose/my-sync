package org.kalmeo.kuix.layout;

import org.kalmeo.kuix.core.KuixConstants;
import org.kalmeo.kuix.util.Alignment;
import org.kalmeo.kuix.util.Gap;
import org.kalmeo.kuix.util.Insets;
import org.kalmeo.kuix.util.Metrics;
import org.kalmeo.kuix.widget.Widget;

/**
 * @author bbeaulant
 */
public class FlowLayout implements Layout {

	/**
	 * This class hold line infos
	 */
	private class LineInfo {

		public LineInfo next;
		public Metrics firstMetrics;
		private Metrics lastMetrics;
		public int width;
		public int height;

		public void addMetrics(Metrics metrics) {
			if (firstMetrics == null) {
				firstMetrics = lastMetrics = metrics;
			} else {
				lastMetrics.next = metrics;
				lastMetrics = metrics;
			}
		}

	}

	public Alignment alignment;

	/**
	 * Construct a {@link FlowLayout}
	 */
	public FlowLayout() {
		this(Alignment.TOP_LEFT);
	}

	/**
	 * Construct a {@link FlowLayout}
	 * 
	 * @param alignment
	 */
	public FlowLayout(Alignment alignment) {
		this.alignment = alignment;
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

		Alignment targetAlignment = target.getAlign();
		Insets insets = target.getInsets();
		Metrics minSize = target.getMinSize();
		Gap gap = target.getGap();
		int width = preferredWidth - insets.left - insets.right;
		int height = target.getHeight() - insets.top - insets.bottom;
		int contentWidth = 0;
		int contentHeight = 0;

		LineInfo firstLine = new LineInfo();
		LineInfo currentLine = firstLine;

		for (Widget widget = target.getChild(); widget != null; widget = widget.next) {

			if (!widget.isIndividualyVisible()) {
				continue;
			}

			boolean isBreak = KuixConstants.BREAK_WIDGET_TAG.equals(widget
					.getTag());

			Metrics preferredSize = widget.getPreferredSize(width);
			if (isBreak
					|| currentLine.width
							+ (currentLine.width == 0 ? 0 : gap.horizontalGap)
							+ preferredSize.width > width) {

				// Compute content size
				contentWidth = Math.max(contentWidth, currentLine.width);
				contentHeight += currentLine.height
						+ (contentHeight == 0 ? 0 : gap.verticalGap);

				// Create a new line
				currentLine.next = new LineInfo();
				currentLine = currentLine.next;
				currentLine.width = preferredSize.width;
				currentLine.height = preferredSize.height;

			} else {
				currentLine.width += (currentLine.width == 0 ? 0
						: gap.horizontalGap)
						+ preferredSize.width;
				currentLine.height = Math.max(currentLine.height,
						preferredSize.height);
			}

			if (!isBreak) {
				// Add metrics to current line
				currentLine.addMetrics(preferredSize);
			}

		}
		if (currentLine.width != 0) {
			contentWidth = Math.max(contentWidth, currentLine.width);
			contentHeight += currentLine.height
					+ (contentHeight == 0 ? 0 : gap.verticalGap);
		}

		if (!layout) {
			metrics.width = insets.left + Math.max(minSize.width, contentWidth)
					+ insets.right;
			metrics.height = insets.top
					+ Math.max(minSize.height, contentHeight) + insets.bottom;
			return;
		}

		int x = 0;
		int y = 0;
		int contentX = 0;
		int contentY = 0;

		if (targetAlignment != null) {
			x = insets.left + targetAlignment.alignX(width, contentWidth);
			y = insets.top + targetAlignment.alignY(height, contentHeight);
		}

		for (LineInfo line = firstLine; line != null; line = line.next) {
			if (targetAlignment != null) {
				contentX = targetAlignment.alignX(contentWidth, line.width);
			}
			for (Metrics widgetMetrics = line.firstMetrics; widgetMetrics != null; widgetMetrics = widgetMetrics.next) {
				Widget widget = widgetMetrics.widget;
				int h = widgetMetrics.height;
				contentY = alignment.alignY(line.height, h);
				if (alignment.isFill()) {
					h = line.height;
				}
				widget.setBounds(x + contentX, y + contentY,
						widgetMetrics.width, h);
				contentX += widgetMetrics.width + gap.horizontalGap;
			}
			y += line.height + gap.verticalGap;
		}

	}

}
