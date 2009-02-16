package bm.ui;

import bm.core.Application;

import javax.microedition.lcdui.*;
import java.util.TimerTask;

/*
 * File Information
 *
 * Created on       : 10-may-2007 11:53:42
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A custom, thin progress meter that takes little space on screen.<br/>
 * It's in fact a proxy to ProgressMeterComp to make it look like a CustomItem.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ProgressMeter extends CustomItem {
	ProgressMeterComp meter;
	Displayable parent;

	boolean animated;
	private Animator animator = new Animator();

	public ProgressMeter(final Displayable parent, final String label) {
		super(label);
		meter = new ProgressMeterComp(label);
		meter.setShowLabel(false);
		this.parent = parent;
	}

	protected int getMinContentWidth() {
		return meter.getMinContentWidth();
	}

	protected int getMinContentHeight() {
		return meter.getMinContentHeight();
	}

	protected int getPrefContentWidth(int width) {
		return meter.getPrefContentWidth(width);
	}

	protected int getPrefContentHeight(int height) {
		return meter.getPrefContentHeight(height);
	}

	public int getMaxValue() {
		return meter.getMaxValue();
	}

	public void setMaxValue(final int maxValue) {
		meter.setMaxValue(maxValue);
		repaint();
	}

	public int getValue() {
		return meter.getValue();
	}

	public void setValue(final int value) {
		meter.setValue(value);
		if (value == Gauge.CONTINUOUS_RUNNING) {
			setAnimated(true);
		}
		repaint();
	}

	// public String getLabel()
	// {
	// return meter.getLabel();
	// }
	//
	// public void setLabel( final String label )
	// {
	// meter.setLabel( label );
	// invalidate();
	// }

	// public boolean isShowLabel()
	// {
	// return meter.isShowLabel();
	// }
	//
	// public void setShowLabel( final boolean showLabel )
	// {
	// meter.setShowLabel( showLabel );
	// invalidate();
	// }

	public Font getFont() {
		return meter.getFont();
	}

	public void setFont(final Font font) {
		meter.setFont(font);
		repaint();
	}

	public int getColor() {
		return meter.getColor();
	}

	public void setColor(final int color) {
		meter.setColor(color);
		repaint();
	}

	public boolean isLabelAbove() {
		return meter.isLabelAbove();
	}

	public void setLabelAbove(final boolean labelAbove) {
		meter.setLabelAbove(labelAbove);
		repaint();
	}

	public boolean isAnimated() {
		return animated;
	}

	/**
	 * Start or stop animation on the gauge.
	 * 
	 * @param animated
	 *            if it should be animated or not
	 * @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc
	 * */
	public void setAnimated(final boolean animated) {
		if (!animated) {
			if (this.animated) {
				animator.cancel();
			}
		} else {
			if (!this.animated) {
				meter.setMaxValue(Gauge.INDEFINITE);
				animator = new Animator();
				Application.getTimer().schedule(animator, 1000, 1000);
			}
		}
		this.animated = animated;
	}

	protected void paint(Graphics graphics, int w, int h) {
		meter.paint(graphics, 0, 0, w, h);
	}

	private class Animator extends TimerTask {
		private transient Display display = Application.getManager()
				.getDisplay();

		/** @noinspection FieldRepeatedlyAccessedInMethod */
		public void run() {
			if (meter.getMaxValue() == Gauge.INDEFINITE) {
				meter.setValue(Gauge.INCREMENTAL_UPDATING);
				repaint();
			}
			if (meter.getMaxValue() != Gauge.INDEFINITE
					|| display.getCurrent() != parent) {
				cancel();
				animated = false;
			}
		}
	}
}
