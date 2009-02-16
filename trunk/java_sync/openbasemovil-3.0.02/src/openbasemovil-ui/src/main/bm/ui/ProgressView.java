package bm.ui;

import bm.util.Util;
import bm.core.event.EventListener;
import bm.core.event.Event;
import bm.core.event.ProgressEvent;
import bm.core.Application;
import bm.core.ControlledTask;
import bm.core.ApplicationManager;
import bm.core.tools.Tools;
import bm.core.mvc.View;
import bm.core.mvc.AbstractView;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.game.Sprite;
import java.util.Date;

/*
 * File Information
 *
 * Created on       : 15-may-2007 3:55:06
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The enhanced splahs view has an image that serves as the background and a
 * progress bar to show application startup progress. The image is fitted to the
 * size of the device screen.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ProgressView extends Canvas implements EventListener,
		CommandListener {
	public static final int TOP = 0;
	public static final int BOTTOM = 1;

	protected Image image;
	protected int meterPlacement = TOP;
	protected String message;
	protected boolean cancellable;
	protected Font font = Font.getFont(Font.FACE_PROPORTIONAL,
			Font.STYLE_PLAIN, Font.SIZE_SMALL);
	protected Date lastKey;
	protected int keyCount;
	protected int imageX;
	protected int buttonBackground = 0x00496E;
	protected int buttonColor = 0xDFDFDF;
	protected boolean fillScale;

	Animator animator;
	ProgressMeterComp meter = new ProgressMeterComp("");

	/**
	 * Create a new ProgressView.
	 */
	public ProgressView() {
	}

	public int getMeterPlacement() {
		return meterPlacement;
	}

	public synchronized void setMeterPlacement(final int meterPlacement) {
		this.meterPlacement = meterPlacement;
		if (isShown()) {
			repaint();
		}
	}

	public boolean isFillScale() {
		return fillScale;
	}

	public synchronized void setFillScale(final boolean fillScale) {
		this.fillScale = fillScale;
		adjustImage();
		if (isShown()) {
			repaint();
		}
	}

	public String getMessage() {
		return message;
	}

	public synchronized void setMessage(final String message) {
		this.message = message;
		if (isShown()) {
			repaint();
		}
	}

	public Font getFont() {
		return font;
	}

	public synchronized void setFont(final Font font) {
		this.font = font;
		if (isShown()) {
			repaint();
		}
	}

	/**
	 * Get the current image.
	 * 
	 * @return image, null if not set
	 */
	public Image getImage() {
		return image;
	}

	/**
	 * Set the current image.
	 * 
	 * @param image
	 *            background image
	 */
	public synchronized void setImage(final Image image) {
		this.image = image;
		adjustImage();
		if (isShown()) {
			repaint();
		}
	}

	private void adjustImage() {
		final int availHeight = getHeight() - getMeterHeight()
				- font.getHeight() - 2;
		final Image image = this.image;
		if (image != null) {
			final int whRatio = (image.getWidth() * 100) / image.getHeight();
			final int totalWidth = getWidth();
			int width = totalWidth;
			int height = (width * whRatio) / 100;
			if (height > availHeight) {
				height = availHeight;
				final int hwRatio = (image.getHeight() * 100)
						/ image.getWidth();
				width = (height * hwRatio) / 100;
				imageX = (totalWidth - width) / 2;
			} else {
				imageX = 0;
			}
			Image scaled;
			if (fillScale && totalWidth > width) {
				scaled = Util.scale(image, width, height);
				final Image img = Image.createImage(totalWidth, height);
				final int diff = (totalWidth - width) / 2;
				final Graphics g = img.getGraphics();
				for (int i = 0; i < diff; i++) {
					g.drawRegion(scaled, 0, 0, 1, height, Sprite.TRANS_NONE, i,
							0, Graphics.TOP | Graphics.LEFT);
				}
				for (int i = totalWidth - diff - 1; i < totalWidth; i++) {
					g.drawRegion(scaled, width - 1, 0, 1, height,
							Sprite.TRANS_NONE, i, 0, Graphics.TOP
									| Graphics.LEFT);
				}
				g.drawImage(scaled, diff, 0, Graphics.LEFT | Graphics.TOP);
				scaled = img;
			} else {
				scaled = Util.scale(image, width, height);
			}
			this.image = Util.crop(scaled, 0, 0, totalWidth, getHeight());
		}
	}

	protected int getMeterHeight() {
		return meter.getPrefContentHeight(-1);
	}

	protected void keyPressed(final int keyCode) {
		AbstractView.touchLastAccess();
		if (keyCode == Canvas.KEY_STAR) {
			if (lastKey == null) {
				lastKey = new Date();
				keyCount = 0;
			} else {
				if (System.currentTimeMillis() - lastKey.getTime() < 1000) {
					keyCount++;
				} else {
					keyCount = 0;
				}
				lastKey.setTime(System.currentTimeMillis());
			}
			if (keyCount > 3) {
				final ApplicationManager manager = Application.getManager();
				if (manager != null) {
					View view = manager.getBestView();
					if (view != null) {
						try {
							view.show();
						} catch (Exception e) {
							view = null;
						}
					}
					if (view == null) {
						view = manager.getMainView();
						if (view != null) {
							try {
								view.show();
							} catch (Exception e) {
								view = null;
							}
						}
					}
					if (view == null) {
						manager.exitApplication();
					}
				} else {
					System.exit(1);
				}
			}
		}
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	public void handleEvent(Event e) {
		if (e instanceof ProgressEvent) {
			try {
				if (!isShown()) {
					Application.getManager().getDisplay().setCurrent(this);
				}
				final ProgressEvent event = (ProgressEvent) e;
				if (event.getTitle() != null) {
					setTitle(event.getTitle());
				}
				final ProgressMeterComp meter = this.meter;
				if (event.getPhase() != null) {
					meter.setLabel(event.getPhase());
				}
				if (event.getMaxValue() != null) {
					meter.setMaxValue(event.getMaxValue().intValue());
				}
				if (event.getValue() != null) {
					meter.setValue(event.getValue().intValue());
				}
				if (event.getMessage() != null) {
					message = event.getMessage();
				}
				if (event.isAnimate() && animator != null) {
					animator = new Animator(this);
					animator.start();
				}
				if (event.isCancellable() != null
						&& event.isCancellable().booleanValue() != cancellable) {
					cancellable = event.isCancellable().booleanValue();
					if (cancellable) {
						addCommand(Application.getCommandFactory()
								.cancelCommand());
					} else {
						removeCommand(Application.getCommandFactory()
								.cancelCommand());
					}
				}
				repaint();
				Thread.yield();
			} catch (Exception e1) {
			}
		}
	}

	protected void paint(final Graphics graphics) {
		AbstractView.touchLastAccess();
		Graphics g = graphics;
		Image buffer = null;
		if (!isDoubleBuffered()) {
			buffer = Image.createImage(getWidth(), getHeight());
			g = buffer.getGraphics();
		}
		g.setColor(0xFFFFFF);
		g.fillRect(0, 0, getWidth(), getHeight());
		int y = 0;
		if (meterPlacement == TOP) {
			y = paintMessage(g, paintMeter(g, y));
		}
		final Image image = this.image;
		if (image != null) {
			g.drawImage(image, fillScale ? 0 : imageX, y, Graphics.TOP
					| Graphics.LEFT);
		}
		if (meterPlacement == BOTTOM) {
			y = getHeight() - getMeterHeight() - font.getHeight() - 2;
			paintMessage(g, paintMeter(g, y));
		}
		if (buffer != null) {
			graphics.drawImage(buffer, 0, 0, Graphics.LEFT | Graphics.TOP);
		}
	}

	protected int paintMeter(final Graphics g, final int y) {
		int y0 = y;
		final int prefContentHeight = meter.getPrefContentHeight(-1);
		meter.paint(g, 2, y0, getWidth() - 4, prefContentHeight);
		y0 += prefContentHeight + 1;
		return y0;
	}

	protected int paintMessage(final Graphics g, int y) {
		final Font font = this.font;
		if (message != null) {
			g.setFont(font);
			g.setColor(0);
			final String[] lines = Tools.splitInLines(message, font, font,
					getWidth() - 4, getHeight() - y - 1, true);
			final int length = lines.length;
			for (int i = 0; i < length; i++) {
				if (lines[i] != null) {
					g.drawString(lines[i], 1, y + font.getBaselinePosition(),
							Graphics.BASELINE | Graphics.LEFT);
					y += font.getHeight() + 1;
				}
			}
		}
		return y;
	}

	public void commandAction(final Command command, final Displayable d) {
		if (command == Application.getCommandFactory().cancelCommand()) {
			ControlledTask.cancelThisTask();
		}
	}

	protected class Animator extends ControlledTask {
		public static final int WAIT_TIME = 1000;

		private ProgressView parent;
		private transient Display display = Application.getManager()
				.getDisplay();

		public Animator(final ProgressView parent) {
			super("ProgressView.Animator", true);
			this.parent = parent;
		}

		/**
		 * If the maximum time for the thread to cancel is reached, die is
		 * called to try to stop it. The subclass should do whatever is possible
		 * to end its execution, for example, if it's blocked in an InputStream
		 * for an external peripheral, it could try to close the stream so an
		 * exception is trhown at the blocking read method.
		 */
		public void die() {
			animator = null;
		}

		/**
		 * Perform the task.
		 */
		protected void task() {
			while (!cancel) {
				if (meter.getMaxValue() == Gauge.INDEFINITE) {
					meter.setValue(Gauge.INCREMENTAL_UPDATING);
				} else {
					cancel();
				}
				parent.repaint();
				if (!cancel) {
					try {
						sleep(WAIT_TIME);
					} catch (InterruptedException e) {
					}
					if (display.getCurrent() != parent) {
						cancel();
					}
				}
			}
			animator = null;
		}
	}
}
