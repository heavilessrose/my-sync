package bm.ui;

import bm.core.event.Event;
import bm.core.event.EventListener;
import bm.core.tools.Tools;
import bm.core.mvc.AbstractView;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 20-ago-2007 18:03:02
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A graphical menu.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Menu extends Canvas implements EventListener {
	public static final int GRID = 0;
	public static final int LIST = 1;

	private static final int RADIUS = 10;

	private Vector items = new Vector(10);
	private Listener listener;
	private int selectedIndex;
	private int mode = LIST;
	private int firstVisible;

	private int selectedColor = 0xFFFFFF;
	private int background = 0xFFFFFF;
	private int selectedBackground = 0x00496E;
	private int color;
	private Font helpFont = Font.getFont(Font.FACE_PROPORTIONAL,
			Font.STYLE_PLAIN, Ui.getFontSize());
	private Font listFont = Font.getFont(Font.FACE_PROPORTIONAL,
			Font.STYLE_BOLD, Ui.getFontSize());
	private Font gridFont = Font.getFont(Font.FACE_PROPORTIONAL,
			Font.STYLE_BOLD, Font.SIZE_LARGE);
	private Rectangle[] zones;
	private int cols;
	private int rows;
	private boolean dirty;
	private int helpLines = 2;

	String iconSet;
	private int labelX;
	private int labelY;

	// private int selectedBorder;

	public interface Listener {
		void itemSelected(MenuItem item);
	}

	public Menu(final Listener listener) {
		this.listener = listener;
		Event.register(this, Ui.ET_FONTSIZE);
	}

	public void addItem(final String label, final int iconIndex,
			final int selectedIconIndex, final int action, final String view) {
		items.addElement(new MenuItem(label, iconIndex, selectedIconIndex,
				action, null, iconSet, view));
		dirty = true;
	}

	public void addItem(final String label, final int iconIndex,
			final int selectedIconIndex, final int action, final String help,
			final String view) {
		items.addElement(new MenuItem(label, iconIndex, selectedIconIndex,
				action, help, iconSet, view));
		dirty = true;
	}

	public int getHelpLines() {
		return helpLines;
	}

	public void setHelpLines(final int helpLines) {
		this.helpLines = helpLines;
		dirty = true;
	}

	public int getSelectedIndex() {
		return selectedIndex;
	}

	public int getMode() {
		return mode;
	}

	public void setMode(final int mode) {
		if (this.mode != mode) {
			this.mode = mode;
			dirty = true;
		}
	}

	public int getBackground() {
		return background;
	}

	public void setBackground(final int background) {
		if (this.background != background) {
			this.background = background;
			dirty = true;
		}
	}

	public int getSelectedBackground() {
		return selectedBackground;
	}

	public void setSelectedBackground(final int selectedBackground) {
		if (this.selectedBackground != selectedBackground) {
			this.selectedBackground = selectedBackground;
			dirty = true;
		}
	}

	public int getSelectedColor() {
		return selectedColor;
	}

	public void setSelectedColor(final int selectedColor) {
		if (this.selectedColor != selectedColor) {
			this.selectedColor = selectedColor;
			dirty = true;
		}
	}

	public int getColor() {
		return color;
	}

	public void setColor(final int color) {
		if (this.color != color) {
			this.color = color;
			dirty = true;
		}
	}

	public Font getHelpFont() {
		return helpFont;
	}

	public void setHelpFont(final Font helpFont) {
		this.helpFont = helpFont;
	}

	public Font getFont() {
		return mode == GRID ? gridFont : listFont;
	}

	public void setFont(final Font font) {
		if (mode == GRID) {
			gridFont = font;
		} else {
			listFont = font;
		}
		this.gridFont = font;
		dirty = true;
	}

	public void setIconSet(final String iconSet) {
		this.iconSet = iconSet;
	}

	protected void paint(Graphics graphics) {
		AbstractView.touchLastAccess();
		if (items.size() > 0) {
			Image buffer = null;
			Graphics g = graphics;
			final int height = getHeight();
			if (!isDoubleBuffered()) {
				buffer = Image.createImage(getWidth(), height);
				g = buffer.getGraphics();
			}
			g.setColor(background);
			g.fillRect(0, 0, getWidth(), getHeight());
			if (mode == GRID) {
				paintGrid(g);
			} else {
				paintList(g);
			}
			if (!isDoubleBuffered()) {
				graphics.drawImage(buffer, 0, 0, Graphics.TOP | Graphics.LEFT);
			}
		}
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private synchronized void calcList() {
		final Vector items = this.items;

		final int lineHeight = listFont.getHeight() + 1;

		int maxHeight = lineHeight;
		final MenuItem item0 = ((MenuItem) items.elementAt(0));
		final Image image0 = item0.image;
		final int imageHeight = image0 != null ? image0.getHeight() + 1 : 0;
		if (imageHeight > maxHeight) {
			maxHeight = imageHeight;
		}

		int selectedHeight = lineHeight
				+ ((helpFont.getHeight() + 1) * helpLines);
		if (imageHeight > selectedHeight) {
			selectedHeight = imageHeight;
		}
		selectedHeight += 4;

		final int width = getWidth() - 2;
		final int height = getHeight() - 2;
		int visibleCount = 1 + ((height - selectedHeight) / maxHeight);
		if (visibleCount > items.size()) {
			visibleCount = items.size();
		}
		if (zones == null || zones.length != visibleCount) {
			zones = new Rectangle[visibleCount];
		}

		// selectedBorder = Ui.addLight( selectedBackground, 0xB0 );
		int y = 1;
		int index = firstVisible;
		for (int i = 0; i < visibleCount; i++) {
			if (index == selectedIndex) {
				zones[i] = new Rectangle(1, y, width, selectedHeight);
				y += selectedHeight + 1;
			} else {
				zones[i] = new Rectangle(1, y, width, maxHeight);
				y += maxHeight + 1;
			}
			index++;
			if (index >= items.size()) {
				index = 0;
			}
		}
		dirty = false;
	}

	private void paintList(final Graphics g) {
		final Font listFont = this.listFont;
		final Font helpFont = this.helpFont;

		if (dirty) {
			calcList();
		}
		final Vector items = this.items;

		int index = firstVisible;
		final int listBaseLine = listFont.getBaselinePosition();
		final int length = zones.length;
		for (int i = 0; i < length; i++) {
			final MenuItem item = (MenuItem) items.elementAt(index);
			final int imageWidth = item.image != null ? item.image.getWidth()
					: 0;
			final Rectangle zone = zones[i];
			if (index == selectedIndex) {
				g.setColor(selectedBackground);
				g.fillRoundRect(1, zone.getY(), zone.getWidth(), zone
						.getHeight(), RADIUS, RADIUS);
				// g.setColor( selectedBorder );
				// g.setStrokeStyle( Graphics.SOLID );
				// g.drawRoundRect(
				// 1,
				// zone.getY(),
				// zone.getWidth(),
				// zone.getHeight(),
				// RADIUS,
				// RADIUS
				// );
				if (item.image != null) {
					g.drawImage(item.image, 3, zone.getY(), Graphics.LEFT
							| Graphics.TOP);
				}
				final String[] help = Tools.splitInLines(item.help, helpFont,
						helpFont, zone.getWidth() - imageWidth - 1, helpLines,
						true);
				final int x = 4 + imageWidth;
				g.setFont(listFont);
				g.setColor(selectedColor);
				g.drawString(item.label, x, zone.getY() + listBaseLine,
						Graphics.LEFT | Graphics.BASELINE);
				if (help != null && help.length > 0) {
					g.setFont(helpFont);
					final int helpBaseLine = helpFont.getBaselinePosition();
					int y0 = zone.getY() + listFont.getHeight() + 1;
					final int helpCount = help.length;
					for (int j = 0; j < helpCount; j++) {
						if (help[j] != null) {
							g.drawString(help[j], x, y0 + helpBaseLine,
									Graphics.LEFT | Graphics.BASELINE);
							y0 += helpFont.getHeight() + 1;
						}
					}
				}
			} else {
				if (item.image != null) {
					g.drawImage(item.image, 3, zone.getY(), Graphics.LEFT
							| Graphics.TOP);
				}
				g.setFont(listFont);
				g.setColor(color);
				g.drawString(item.label, 4 + imageWidth, zone.getY()
						+ listBaseLine, Graphics.LEFT | Graphics.BASELINE);
			}
			index++;
			if (index >= items.size()) {
				index = 0;
			}
		}
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private synchronized void calcGrid() {
		final Vector items = this.items;
		final Font font = this.gridFont;

		final int itemCount = items.size();
		final int width = getWidth() - 2;
		final int height = getHeight() - font.getHeight()
				- helpFont.getHeight() - 4;
		final MenuItem item0 = ((MenuItem) items.elementAt(0));
		final int iconWidth = item0.image.getWidth();
		final int itemWidth = iconWidth + 2;
		final int iconHeight = item0.image.getHeight();
		final int itemHeight = iconHeight + 2;

		final int maxCols = width / itemWidth;
		final int maxRows = height / itemHeight;

		if (itemCount < maxCols * maxRows) {
			cols = 1;
			rows = 1;
			int total = 0;
			while (total < itemCount) {
				if (cols < maxCols) {
					cols++;
				}
				total = cols * rows;
				if (total < itemCount) {
					if (rows < maxRows) {
						rows++;
					}
					total = cols * rows;
				}
			}
		} else {
			cols = maxCols;
			rows = maxRows;
		}
		final int hSpace = (width - (cols * itemWidth)) / (cols + 1);
		final int vSpace = (height - (rows * itemHeight)) / (rows + 1);
		int visibleCount = cols * rows;
		if (visibleCount > itemCount) {
			visibleCount = itemCount;
		}

		if (zones == null || zones.length != visibleCount) {
			zones = new Rectangle[visibleCount];
		}

		int y = 1;
		labelX = 1 + (width / 2);
		labelY = y + font.getBaselinePosition();
		y += font.getHeight() + vSpace;

		int count = 0;
		int index = firstVisible;
		for (int i = 0; i < rows && count < itemCount; i++) {
			int x = hSpace;
			for (int j = 0; j < cols && count < itemCount; j++) {
				zones[index - firstVisible] = new Rectangle(x, y, itemWidth,
						itemHeight);
				x += itemWidth + hSpace;
				index++;
				if (index >= itemCount) {
					index = 0;
				}
				count++;
			}
			y += itemHeight + vSpace;
		}
		dirty = false;
	}

	private void paintGrid(final Graphics g) {
		final Vector items = this.items;
		final Font helpFont = this.helpFont;

		if (dirty) {
			calcGrid();
		}
		final int labelX = this.labelX;
		final Rectangle[] zones = this.zones;
		final MenuItem selected = (MenuItem) items.elementAt(selectedIndex);
		int x, align;
		if (gridFont.stringWidth(selected.label) > getWidth() - 2) {
			x = 1;
			align = Graphics.LEFT;
		} else {
			x = labelX;
			align = Graphics.HCENTER;
		}
		g.setFont(gridFont);
		g.setColor(0xC0C0C0);
		g.drawString(selected.label, x + 1, labelY + 1, Graphics.BASELINE
				| align);
		g.setColor(color);
		g.drawString(selected.label, x, labelY, Graphics.BASELINE | align);
		final int zoneCount = zones.length;
		final int itemCount = items.size();
		int index = firstVisible;
		for (int i = 0; i < zoneCount; i++) {
			final MenuItem item = (MenuItem) items.elementAt(index);
			g.drawImage(index == selectedIndex ? item.selectedImage
					: item.image, zones[i].getX() + 2, zones[i].getY() + 2,
					Graphics.LEFT | Graphics.TOP);
			index++;
			if (index >= itemCount) {
				index = 0;
			}
		}

		if (selected.help != null) {
			g.setFont(helpFont);
			g.setColor(color);
			if (helpFont.stringWidth(selected.help) > getWidth() - 2) {
				g.drawString(selected.help, 1, getHeight()
						- helpFont.getHeight() + helpFont.getBaselinePosition()
						- 1, Graphics.BASELINE | Graphics.LEFT);
			} else {
				g.drawString(selected.help, labelX, getHeight()
						- helpFont.getHeight() + helpFont.getBaselinePosition()
						- 1, Graphics.BASELINE | Graphics.HCENTER);
			}
		}
	}

	protected void keyPressed(int keyCode) {
		processKey(keyCode);
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private void processKey(final int keyCode) {
		AbstractView.touchLastAccess();
		final int mode = this.mode;
		final int action = getGameAction(keyCode);
		switch (action) {
		case LEFT:
			if (mode == GRID) {
				selectedIndex--;
				dirty = true;
				adjust();
			}
			break;

		case RIGHT:
			if (mode == GRID) {
				selectedIndex++;
				dirty = true;
				adjust();
			}
			break;

		case UP:
			if (mode == GRID) {
				selectedIndex -= cols;
			} else {
				selectedIndex--;
			}
			dirty = true;
			adjust();
			break;

		case DOWN:
			if (mode == GRID) {
				selectedIndex += cols;
			} else {
				selectedIndex++;
			}
			dirty = true;
			adjust();
			break;

		case FIRE:
			listener.itemSelected((MenuItem) items.elementAt(selectedIndex));
			break;
		}
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private void adjust() {
		final int count = zones.length;
		final int size = items.size();

		if (selectedIndex < firstVisible) {
			firstVisible--;
		} else if (selectedIndex >= size) {
			selectedIndex = 0;
			firstVisible = 0;
		} else if ((selectedIndex - firstVisible) >= count) {
			firstVisible++;
		}

		if (firstVisible < 0) {
			firstVisible = size - count;
			selectedIndex = size - 1;
		} else if (firstVisible >= size) {
			firstVisible = 0;
			selectedIndex = 0;
		}
		repaint();
	}

	protected void keyRepeated(int keyCode) {
		processKey(keyCode);
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	protected void pointerPressed(int x, int y) {
		AbstractView.touchLastAccess();
		final Rectangle[] zones = this.zones;
		if (zones != null) {
			final int length = zones.length;
			for (int i = 0; i < length; i++) {
				if (zones[i].isIn(x, y)) {
					if (i != selectedIndex) {
						selectedIndex = i;
						dirty = true;
						repaint();
					} else {
						listener.itemSelected((MenuItem) items
								.elementAt(selectedIndex));
					}

				}
			}
		}
	}

	protected void sizeChanged(int width, int heigth) {
		repaint();
	}

	public void handleEvent(Event event) {
		if (event.getType().equals(Ui.ET_FONTSIZE)) {
			helpFont = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN,
					Ui.getFontSize());
			listFont = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Ui
					.getFontSize());
			dirty = true;
			if (isShown()) {
				repaint();
			}
		}
	}

}
