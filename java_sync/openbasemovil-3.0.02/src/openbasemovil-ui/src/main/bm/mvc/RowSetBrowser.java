package bm.mvc;

/*
 * File Information
 *
 * Created on       : 05-jul-2006 8:35:19
 * Created by       : javier
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

import bm.core.Application;
import bm.core.ResourceManager;
import bm.core.mvc.View;
import bm.db.DBException;
import bm.db.RowSet;
import bm.storage.RSException;

import javax.microedition.lcdui.*;
import bm.storage.RecordStoreFullException;

/**
 * RowSet Browser.
 * 
 * @author <a href="mailto:javier.chacon@elondra.com">Javier Chacon</a>
 * @version $Revision$
 */

public class RowSetBrowser extends CustomItem {
	private Font font = Font.getDefaultFont();

	private ItemCommandListener itemCommandListener;

	private Command cmd;
	private Listener listener;
	private static Image left;
	private static Image right;

	private RowSet rowset;
	private boolean usedForSelection;
	private boolean inTraversal;
	private boolean singleLabel;
	private int margin = 10;
	private int bottomMargin = 1;
	private int topMargin = 1;
	private int separator = 1;
	private int perimeterColor = 0x00000000;
	private int barFillColor = 0x0085B0C6;
	private int barLightColor = 0x00BAE6FC;
	private int barDarkColor = 0x0000496E;

	// private static Log log = LogFactory.getLog( "RowSetBrowser" );

	public RowSetBrowser(final String title) {
		super(title);
		cmd = new Command(ResourceManager.getResource("rowSetBrowser.select"),
				Command.OK, -1);
	}

	public Command getCommand() {
		return cmd;
	}

	public Listener getListener() {
		return listener;
	}

	public boolean isUsedForSelection() {
		return usedForSelection;
	}

	public void setUsedForSelection(final boolean usedForSelection) {
		this.usedForSelection = usedForSelection;
	}

	public void setListener(final Listener listener) {
		this.listener = listener;
		if (usedForSelection && listener instanceof View) {
			((View) listener).addCommand(cmd);
		}
	}

	public int getMargin() {
		return margin;
	}

	public void setMargin(final int margin) {
		this.margin = margin;
	}

	public int getBottomMargin() {
		return bottomMargin;
	}

	public void setBottomMargin(final int bottomMargin) {
		this.bottomMargin = bottomMargin;
	}

	public int getTopMargin() {
		return topMargin;
	}

	public void setTopMargin(final int topMargin) {
		this.topMargin = topMargin;
	}

	public int getSeparator() {
		return separator;
	}

	public void setSeparator(final int separator) {
		this.separator = separator;
	}

	public int getPerimeterColor() {
		return perimeterColor;
	}

	public void setPerimeterColor(final int perimeterColor) {
		this.perimeterColor = perimeterColor;
	}

	public int getBarFillColor() {
		return barFillColor;
	}

	public void setBarFillColor(final int barFillColor) {
		this.barFillColor = barFillColor;
	}

	public int getBarLightColor() {
		return barLightColor;
	}

	public void setBarLightColor(final int barLightColor) {
		this.barLightColor = barLightColor;
	}

	public int getBarDarkColor() {
		return barDarkColor;
	}

	public void setBarDarkColor(final int barDarkColor) {
		this.barDarkColor = barDarkColor;
	}

	public Font getFont() {
		return font;
	}

	public void setFont(final Font font) {
		this.font = font;
	}

	public void setItemCommandListener(ItemCommandListener itemCommandListener) {
		super.setItemCommandListener(itemCommandListener);
		this.itemCommandListener = itemCommandListener;
	}

	protected void keyReleased(int keyCode) {
		final int action = getGameAction(keyCode);
		if (action == Canvas.FIRE && usedForSelection) {
			if (itemCommandListener != null) {
				itemCommandListener.commandAction(cmd, this);
			}
		} else if (action == Canvas.LEFT) {
			moveLeft();
		} else if (action == Canvas.RIGHT) {
			moveRight();
		}
	}

	protected void traverseOut() {
		inTraversal = false;
		repaint();
	}

	protected boolean traverse(int dir, int viewportWidth, int viewportHeight,
			int[] visRect_inout) {
		final RowSet rowset = this.rowset;
		if (rowset != null) {
			if (!inTraversal) {
				inTraversal = true;
				// return true;
			}
			// else
			// {
			// switch ( dir )
			// {
			// case Canvas.LEFT:
			// // moveLeft( rowset );
			// return true;
			//
			// case Canvas.RIGHT:
			// // moveRight( rowset );
			// return true;
			//
			// default:
			// return false;
			// }
			// }
			return true; // Do not allow the item to loose focus, makes
			// Nokia E70 work and it's fine for our porpouses
		} else {
			return false;
		}
	}

	private void moveRight() {
		if (rowset != null && rowset.hasNext()
				&& (listener == null || listener.beforeMove())) {
			rowset.next();
			repaint();
			if (listener != null) {
				listener.positionChanged();
			}
		}
	}

	private void moveLeft() {
		if (rowset != null && rowset.hasPrevious()
				&& (listener == null || listener.beforeMove())) {
			rowset.previous();
			repaint();
			if (listener != null) {
				listener.positionChanged();
			}
		}
	}

	public void setRowSet(final RowSet rowset) throws RecordStoreFullException,
			DBException, RSException {
		this.rowset = rowset;
		singleLabel = false;

		if (rowset != null) {
			if (rowset.getCurrent() == null) {
				if (rowset.hasNext()) {
					rowset.next();
					if (listener != null) {
						listener.positionChanged();
					}
				}
			} else if (listener != null) {
				listener.positionChanged();
			}
		}
	}

	public RowSet getRowSet() {
		return rowset;
	}

	public boolean isSingleLabel() {
		return singleLabel;
	}

	public void setSingleLabel(final boolean singleLabel) {
		this.singleLabel = singleLabel;
	}

	protected int getMinContentWidth() {
		return Application.getManager().getDisplay().getCurrent().getWidth();
	}

	protected int getMinContentHeight() {
		int height = 0;
		if (font != null && font.getHeight() > height) {
			height = font.getHeight();
		}
		if (left != null && left.getHeight() > height) {
			height = left.getHeight();
		}
		return height + 3;
	}

	protected int getPrefContentWidth(int i) {
		return getMinContentWidth();
	}

	protected int getPrefContentHeight(int i) {
		return getMinContentHeight();
	}

	public static void setLeft(final Image left) {
		RowSetBrowser.left = left;
	}

	public static void setRight(final Image right) {
		RowSetBrowser.right = right;
	}

	public static Image getRight() {
		return right;
	}

	public static Image getLeft() {
		return left;
	}

	protected void paint(final Graphics graphics, final int w, final int h) {
		final int imageHeight = left.getHeight();
		final int imageWidth = right.getWidth();
		final Font font = this.font;
		int height = imageHeight;
		final int barHeight = height * 2 / 3;
		final int barAreaWidth = w - (imageWidth * 2) - (margin * 2)
				- (separator * 2);

		if (font.getHeight() > height) {
			height = font.getHeight();
		}

		height += topMargin + bottomMargin;

		final Image buffer = Image.createImage(w, height);
		Graphics g = buffer.getGraphics();

		final RowSet rowset = this.rowset;
		if (rowset != null && rowset.size() > 1) {
			// g.setColor( 0xFFFFFFFF );
			// g.fillRect( 0, 0, getMinContentWidth(), getMinContentHeight() );
			if (rowset.hasPrevious()) {
				g.drawImage(left, margin, topMargin, 0);
			}

			if (rowset.hasNext()) {
				g.drawImage(right, w - imageWidth - margin, topMargin, 0);
			}

			final int barX = margin + imageWidth + separator;
			final int barY = topMargin + ((imageHeight - barHeight) / 2);
			drawBarPerimeter(g, barX, barY, barAreaWidth, barHeight);

			drawBarArea(g, barX + 1, barY + 1, barAreaWidth - 2, barHeight - 2);

			// g.drawLine( 0,
			// imageHeight + 3,
			// screenWidth,
			// imageHeight + 3
			// );
		}

		graphics.drawImage(Image.createImage(buffer), 0, 0, 0);
	}

	private void drawBarArea(final Graphics g, final int x, final int y,
			final int totalWidth, final int height) {
		final int width = (totalWidth * (rowset.getPosition() + 1))
				/ rowset.size();
		// Color principal
		g.setColor(barFillColor);
		final int x1 = x + width - 1;
		final int y1 = y + height - 1;
		g.fillRect(x, y, width, height);

		g.setColor(barLightColor);
		g.drawLine(x, y, x1, y);

		g.setColor(barDarkColor);
		g.drawLine(x, y1, x1, y1);

		// Vertical izquierda
		final int y0 = y + 1;
		final int y2 = y1 - 1;

		g.setColor(barLightColor);
		g.drawLine(x, y0, x, y2);

		// Vertical derecha
		g.setColor(barDarkColor);
		g.drawLine(x1, y0, x1, y2);
	}

	private void drawBarPerimeter(final Graphics g, final int x, final int y,
			final int width, final int height) {
		g.setColor(perimeterColor);
		// Horizontal superior
		final int x0 = x + 1;
		int x1 = x + width - 2;
		g.drawLine(x0, y, x1, y);
		// Horizontal inferior
		int y1 = y + height - 1;
		g.drawLine(x0, y1, x1, y1);
		// Vertical izquierda
		y1--;
		final int y0 = y + 1;
		g.drawLine(x, y0, x, y1);
		// Vertical derecha
		x1++;
		g.drawLine(x1, y0, x1, y1);
	}

	public interface Listener {
		/**
		 * Called when the user request a movement, prior to making it.
		 * 
		 * @return true if the movement should be allowed, false otherwise.
		 */
		boolean beforeMove();

		/**
		 * Called after the rowset position has changed.
		 */
		void positionChanged();
	}
}