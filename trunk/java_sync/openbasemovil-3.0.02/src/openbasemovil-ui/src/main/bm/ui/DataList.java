package bm.ui;

import bm.core.Application;
import bm.core.ErrorHandler;
import bm.core.ResourceManager;
import bm.core.event.Event;
import bm.core.event.EventListener;
import bm.core.event.ProgressEvent;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.math.FixedPoint;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.View;
import bm.core.mvc.AbstractView;
import bm.core.tools.LocalFormatTools;
import bm.core.tools.Tools;
import bm.db.DBException;
import bm.db.Row;
import bm.db.RowSet;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.mvc.FieldResolver;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;
import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 14-ago-2007 19:58:30
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Custom rowset lis view.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DataList extends Canvas implements EventListener {
	static final Log log = LogFactory.getLog("DataList");

	public static final int PLAIN = 0;
	public static final int DATE = 1;
	public static final int TIME = 2;
	public static final int MONEY = 3;
	public static final int STATIC = 4;

	private static final int LIST_PANE = 0;
	private static final int TOOLBAR_PANE = 1;

	// Properties
	private int color;
	private int selectedColor = 0xFFFFFF;
	private int selectedBackground = 0x00496E;
	private int background = 0xFFFFFF;
	private Vector fields = new Vector(10);
	private Hashtable fieldsByName = new Hashtable(10);
	private String separator = " ";
	private boolean wrap;
	RowSet rowSet;
	Font font = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Ui
			.getFontSize());
	Vector buttons = new Vector(10);
	int maxLines = 4;
	int currentMaxLines = 4;
	boolean dirty = true;

	// Transient items
	/** @noinspection FieldCanBeLocal */
	private int dimmBackground = 0x85B0C6;
	private int selectedButton;
	private int selectedIndex;
	private int selectedPane = LIST_PANE;
	private FieldResolver resolver;
	private View parent;
	private Controller controller;
	private ProgressMeterComp meter = new ProgressMeterComp("");
	int lineHeight = font.getHeight() + 1;
	String[] labels;
	int[] indexes;
	int firstIndex;
	Hashtable lineCache = new Hashtable(10);
	int maxButtonHeight;
	boolean rebuilding;
	boolean softRebuild;

	public DataList(final View parent) {
		this.parent = parent;
		Event.register(this, Ui.ET_FONTSIZE);
	}

	public DataList(final View parent, final Controller controller) {
		this.parent = parent;
		this.controller = controller;
		Event.register(this, Ui.ET_FONTSIZE);
	}

	public Controller getController() {
		return controller;
	}

	public void setController(final Controller controller) {
		this.controller = controller;
	}

	public RowSet getRowSet() {
		return rowSet;
	}

	/**
	 * Set the row set.
	 * 
	 * @param rowSet
	 *            row set
	 * @throws bm.db.DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws RSException
	 *             on errors
	 */
	public synchronized void setRowSet(RowSet rowSet) throws DBException,
			RecordStoreFullException, RSException {
		this.rowSet = rowSet;
		firstIndex = 0;
		selectedIndex = 0;
		lineCache.clear();
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public int getMaxLines() {
		return maxLines;
	}

	public void setMaxLines(final int maxLines) {
		this.maxLines = maxLines;
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	/**
	 * Check if the underlying rows have been signaled as changed an list should
	 * be rebuilt.
	 * 
	 * @return dirty flag
	 */
	public boolean isDirty() {
		return dirty;
	}

	/**
	 * Set dirty flag, meaning that list should be rebuilt.
	 * 
	 * @param dirty
	 *            dirty flag
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 * @throws bm.db.DBException
	 *             on errors
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public void setDirty(final boolean dirty) throws RSException,
			RecordStoreFullException, DBException {
		this.dirty = dirty;
		softRebuild = false;
		if (dirty && isShown()) {
			repaint();
		}
	}

	public boolean isWrap() {
		return wrap;
	}

	public void setWrap(final boolean wrap) {
		this.wrap = wrap;
	}

	/**
	 * Rebuild the list
	 * 
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public synchronized void rebuild() {
		rebuilding = true;
		if (rowSet != null && rowSet.size() > 0) {
			try {
				final Vector oldListeners = Event.getListeners(Event.PROGRESS);
				Event.unregisterAll(Event.PROGRESS);
				Event.register(this, Event.PROGRESS);
				final boolean isShown = isShown();
				int availHeight = getHeight();
				if (buttons.size() > 0) {
					// Buttons take 1 pixel above, 2 pixels for icon border,
					// maxButtonHeight,
					// 1 pixel below,
					// 1 pixel for separator, 1 pixel below separator, total:
					// 6 pixels + maxButtonHeight
					availHeight -= 6 + maxButtonHeight;
				}
				// Anyway, the message line takes fontHeight plus 2 pixels for
				// space
				// and separator line
				availHeight -= 2 + font.getHeight();
				final int lineCount = availHeight / lineHeight;
				currentMaxLines = maxLines >= lineCount ? lineCount : maxLines;

				int itemCount = lineCount - currentMaxLines + 1;
				if (itemCount > rowSet.size()) {
					itemCount = rowSet.size();
				}
				indexes = new int[itemCount];
				labels = new String[itemCount];
				final Hashtable newCache = new Hashtable(itemCount);
				if (!softRebuild) {
					lineCache.clear();
				}
				rowSet.openSource();
				for (int i = 0; i < itemCount; i++) {
					indexes[i] = firstIndex + i;
					try {
						final Integer cacheKey = new Integer(indexes[i]);
						labels[i] = (String) lineCache.get(cacheKey);
						if (labels[i] == null) {
							labels[i] = buildLabel(rowSet.getRowAt(indexes[i]));
						}
						newCache.put(cacheKey, labels[i]);
					} catch (Exception e) {
						ErrorHandler.handleError(this, e);
						labels[i] = "<error>";
					}
				}
				lineCache = newCache;
				if (isShown && !isShown()) {
					Application.getManager().getDisplay().setCurrent(this);
				}
				Event.setListeners(Event.PROGRESS, oldListeners);
			} catch (Exception e) {
				log.error(e);
			} finally {
				dirty = false;
				softRebuild = false;
				rebuilding = false;
				try {
					rowSet.closeSource();
				} catch (Exception e) {
					log.error(e);
				}
			}
		} else {
			indexes = null;
			labels = null;
		}
		dirty = false;
		softRebuild = false;
		rebuilding = false;
	}

	public String getSeparator() {
		return separator;
	}

	public void setSeparator(final String separator) {
		this.separator = separator;
	}

	public Font getFont() {
		return font;
	}

	public void setFont(final Font font) {
		this.font = font;
		lineHeight = font.getHeight() + 1;
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public int getColor() {
		return color;
	}

	public void setColor(final int color) {
		this.color = color;
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public int getSelectedColor() {
		return selectedColor;
	}

	public void setSelectedColor(final int selectedColor) {
		this.selectedColor = selectedColor;
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public int getSelectedBackground() {
		return selectedBackground;
	}

	public void setSelectedBackground(final int selectedBackground) {
		this.selectedBackground = selectedBackground;
		dimmBackground = Ui.addLight(selectedBackground, 0xB0);
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public int getSelectedIndex() {
		return selectedIndex;
	}

	public void setSelectedIndex(final int selectedIndex) {
		if (rowSet != null && selectedIndex > -1
				&& selectedIndex < rowSet.size()) {
			this.selectedIndex = selectedIndex;
			dirty = true;
			if (isShown()) {
				repaint();
			}
		}
	}

	public int getBackground() {
		return background;
	}

	public void setBackground(final int background) {
		this.background = background;
	}

	/**
	 * Add an speed option.
	 * 
	 * @param text
	 *            text
	 * @param code
	 *            code
	 */
	public void addToolbarItem(String text, int code) {
		addToolbarItem(code, text, null);
	}

	/**
	 * Add an speed option.
	 * 
	 * @param code
	 *            code
	 * @param text
	 *            text
	 * @param help
	 *            help text
	 */
	public void addToolbarItem(final int code, final String text,
			final String help) {
		final ToolButton button = new ToolButton();
		button.action = code;
		button.help = help != null ? ResourceManager.getResource(help) : null;
		button.text = text != null ? ResourceManager.getResource(text) : null;
		buttons.addElement(button);
		dirty = true;
		if (font.getHeight() > maxButtonHeight) {
			maxButtonHeight = font.getHeight();
		}
		if (isShown()) {
			repaint();
		}
	}

	/**
	 * Add an speed option.
	 * 
	 * @param code
	 *            code
	 * @param image
	 *            image
	 * @param help
	 *            help text
	 */
	public void addToolbarItem(final int code, final Image image,
			final String help) {
		final ToolButton button = new ToolButton();
		button.action = code;
		button.help = help != null ? ResourceManager.getResource(help) : null;
		button.image = image;
		buttons.addElement(button);
		dirty = true;
		if (image != null) {
			if (image.getHeight() > maxButtonHeight) {
				maxButtonHeight = image.getHeight();
			}
		} else if (button.help != null) {
			button.text = button.help;
		} else {
			button.text = "!";
		}
		if (isShown()) {
			repaint();
		}
	}

	/**
	 * Add an speed option.
	 * 
	 * @param code
	 *            code
	 * @param iconSet
	 *            iconSet name
	 * @param index
	 *            icon index
	 * @param help
	 *            help text
	 */
	public void addToolbarItem(final int code, final String iconSet,
			final int index, final String help) {
		addToolbarItem(code, IconStore.getImage(iconSet, index), help);
	}

	/**
	 * Add a field, with plain mode.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void addField(String fieldName) {
		addField(fieldName, null, PLAIN, null, null, null);
	}

	/**
	 * Add a field.
	 * 
	 * @param name
	 *            field name
	 * @param mode
	 *            mode, one of PLAIN, DATE, TIME or MONEY
	 */
	public void addField(final String name, final int mode) {
		addField(name, null, mode, null, null, null);
	}

	public void addField(final String name, final String label, final int mode) {
		addField(name, label, mode, null, null, null);
	}

	public void addField(final String name, final String label, final int mode,
			final String trueLabel, final String falseLabel,
			final String resource) {
		final FieldDescription field = new FieldDescription(name, label, mode,
				trueLabel, falseLabel, resource);
		fieldsByName.put(name, field);
		fields.addElement(field);
	}

	/**
	 * Set the selected item. It should scroll to make it visible and selected.
	 * 
	 * @param id
	 *            row id
	 * @throws bm.db.DBException
	 *             on errors
	 * @throws bm.storage.RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public void setSelected(long id) throws DBException,
			RecordStoreFullException, RSException {
		// ToDo: implement
	}

	/**
	 * Remove all fields.
	 */
	public void removeAllFields() {
		fields.removeAllElements();
		fieldsByName.clear();
	}

	/**
	 * Enumeration of field names.
	 * 
	 * @return field names
	 */
	public Enumeration fields() {
		return fields.elements();
	}

	/**
	 * Remove a field.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void removeField(String fieldName) {
		final DataList.FieldDescription fd = (DataList.FieldDescription) fieldsByName
				.get(fieldName);
		if (fd != null) {
			fields.removeElement(fd);
			fieldsByName.remove(fieldName);
		}
	}

	protected void paint(Graphics graphics) {
		AbstractView.touchLastAccess();
		if (dirty) {
			if (!rebuilding) {
				rebuild();
			}
		}
		if (rebuilding) {
			final int prefContentHeight = meter.getPrefContentHeight(-1);
			meter.paint(graphics, 2, getHeight() - prefContentHeight - 2,
					getWidth() - 4, prefContentHeight);
		} else {
			Image buffer = null;
			try {
				final int selectedIndex = this.selectedIndex;
				final Font font = this.font;
				final RowSet rowSet = this.rowSet;
				final String[] labels = this.labels;
				Graphics g = graphics;
				final int height = getHeight();
				if (!isDoubleBuffered()) {
					buffer = Image.createImage(getWidth(), height);
					g = buffer.getGraphics();
				}
				g.setColor(background);
				g.fillRect(0, 0, getWidth(), height);
				// g.setColor( 0x85B0C6 );
				// g.fillRect( 0, 0, getWidth(), height );
				// g.setColor( background );
				// g.fillArc( 0, 0, getWidth(), height, 45, -45 );
				g.setFont(font);
				g.setColor(color);
				int y = 0;
				// Paint buttons
				final int fontHeight = font.getHeight();
				String status = null;
				if (buttons.size() > 0) {
					status = paintButtons(g);
					y += 7 + maxButtonHeight;
				}
				if (rowSet != null && rowSet.size() > 0 && fields.size() > 0
						&& labels != null && labels.length > 0) {
					final int size = rowSet.size();
					// Paint rows
					rowSet.openSource();
					final int itemCount = indexes.length;
					for (int i = 0; i < itemCount; i++) {
						final int index = indexes[i];
						final boolean isSelectedLine = index == selectedIndex;
						paintLine(g, y, Tools.splitInLines(labels[i], font,
								font, getWidth() - 2,
								isSelectedLine ? currentMaxLines : 1, wrap),
								isSelectedLine);
						y += lineHeight
								* (isSelectedLine ? currentMaxLines : 1);
					}
					if (selectedPane == LIST_PANE) {
						status = (selectedIndex + 1) + "/" + size;
					}
				} else if (selectedPane == LIST_PANE) {
					status = ResourceManager
							.getResource("rowSetListView.noData");
				}
				y = height - fontHeight - 2;
				g.setColor(color);
				g.drawLine(1, y, getWidth() - 2, y);
				g.drawString(status != null ? status : "", 1, height
						- fontHeight + font.getBaselinePosition(),
						Graphics.LEFT | Graphics.BASELINE);
			} catch (Exception e) {
				ErrorHandler.handleError(this, e);
			} finally {
				if (buffer != null) {
					graphics.drawImage(buffer, 0, 0, Graphics.TOP
							| Graphics.LEFT);
				}
				try {
					rowSet.closeSource();
				} catch (Exception e) {
					ErrorHandler.handleError(this, e);
				}
			}
		}
	}

	private void paintLine(final Graphics g, final int y, final String[] text,
			final boolean selectedLine) {
		final boolean selected = selectedPane == LIST_PANE && selectedLine;
		if (selected || selectedLine) {
			g.setColor(selected ? selectedBackground : dimmBackground);
			g.fillRoundRect(0, y, getWidth(), currentMaxLines * lineHeight, 2,
					2);
		}

		final int lines = selected || selectedLine ? text.length : 1;
		g.setColor(selected ? selectedColor : color);
		int y0 = y;
		final int baseLine = font.getBaselinePosition();
		for (int i = 0; i < lines; i++) {
			if (text[i] != null) // Should never happen, but just in case
			{
				g.drawString(text[i], 1, y0 + baseLine, Graphics.LEFT
						| Graphics.BASELINE);
				y0 += lineHeight;
			}
		}
	}

	private String paintButtons(final Graphics g) {
		final Font font = this.font;
		int y = 1;
		int x = 1;
		final int maxX = getWidth() - 2;
		final int count = buttons.size();
		g.setColor(color);
		g.setStrokeStyle(Graphics.SOLID);
		ToolButton selected = null;
		final int fontHeight = font.getHeight();
		final int baseLine = font.getBaselinePosition();
		for (int i = 0; i < count && x < maxX; i++) {
			final ToolButton button = (ToolButton) buttons.elementAt(i);
			final boolean isSelected = selectedPane == TOOLBAR_PANE
					&& i == selectedButton;
			if (button.image != null) {
				if (isSelected) {
					g.setColor(0);
					g.drawRect(x, y, button.image.getWidth() + 2, button.image
							.getHeight() + 2);
					selected = button;
				}
				g.drawImage(button.image, x + 1, y + 1, Graphics.LEFT
						| Graphics.TOP);
				x += button.image.getWidth() + 2;
			} else if (button.text != null) {
				final String text = button.text;
				final int width = font.charsWidth(text.toCharArray(), 0, text
						.length()) + 2;
				if (isSelected) {
					g.setColor(0);
					g.drawRect(x, y, width + 2, fontHeight + 2);
					g.setColor(0xD0D0D0);
					g.fillRect(x + 1, y + 1, width, fontHeight);
					selected = button;
				} else {
					g.setColor(0xC0C0C0);
					g.fillRect(x, y, width, fontHeight);
				}
				g.setColor(0);
				g.drawString(text, x + 2, y + 2 + baseLine, Graphics.LEFT
						| Graphics.BASELINE);
				x += width + 1;
			}
		}
		y += maxButtonHeight + 6;
		g.drawLine(1, y, getWidth() - 2, y);

		String text = null;
		if (selected != null) {
			if (selected.help != null) {
				text = selected.help;
			} else if (selected.image != null && selected.text != null) {
				text = selected.text;
			}
		}
		return text;
	}

	/**
	 * Create the label to show on the list for the ScrollSet current element.<br/>
	 * Subclases can override this method to provide a custom implementation.
	 * 
	 * @param row
	 *            the row to represent
	 * @return label or null if for any reason this row should not be included
	 * @noinspection SingleCharacterStartsWith
	 */
	protected String buildLabel(final Row row) {
		final StringBuffer label = new StringBuffer();
		try {
			boolean lastWasStatic = false;
			// noinspection MethodCallInLoopCondition
			for (final Enumeration i = fields.elements(); i.hasMoreElements();) {
				String value = null;
				final DataList.FieldDescription field = (DataList.FieldDescription) i
						.nextElement();
				final Object raw = field.mode != STATIC ? resolve(row,
						field.name) : field.label;
				if (raw != null) {
					switch (field.mode) {
					case STATIC:
						value = (String) raw;
						break;

					case MONEY:
						if (raw instanceof FixedPoint) {
							value = ((FixedPoint) raw).toMoneyString();
						}
						break;

					case DATE:
						if (raw instanceof Date) {
							value = LocalFormatTools.formatDate((Date) raw);
						}
						break;

					case TIME:
						if (raw instanceof Date) {
							value = LocalFormatTools.formatTime((Date) raw);
						}
						break;
					}
					if (value == null) {
						if (raw instanceof Date) {
							value = LocalFormatTools
									.formatTimestamp((Date) raw);
						} else if (raw instanceof Boolean) {
							value = ((Boolean) raw).booleanValue() ? field.trueLabel
									: field.falseLabel;
						} else if (field.resource != null) {
							value = ResourceManager.getResource(field.resource
									+ raw.toString());
						} else {
							value = raw.toString();
						}
					}
					if (field.mode != STATIC) {
						if (!lastWasStatic && label.length() > 0) {
							label.append(separator);
						}
						if (field.label != null) {
							label.append(field.label).append(": ");
						}
						lastWasStatic = false;
					} else {
						lastWasStatic = true;
					}
					label.append(value);
				}
			}
		} catch (Exception e) {
			ErrorHandler.handleError(this, e);
		}
		return label.toString();
	}

	/**
	 * Get the field resolver.
	 * 
	 * @return field resolver
	 */
	public FieldResolver getResolver() {
		return resolver;
	}

	/**
	 * Set the field resolver.
	 * 
	 * @param resolver
	 *            resolver
	 */
	public void setResolver(final FieldResolver resolver) {
		this.resolver = resolver;
	}

	Object resolve(final Row row, final String fieldName) {
		try {
			if (row.getParent().hasField(fieldName)) {
				return row.getField(fieldName);
			} else if (resolver != null) {
				return resolver.resolve(parent, fieldName, row);
			} else {
				return null;
			}
		} catch (Exception e) {
			ErrorHandler.handleError(this, e);
			return null;
		}
	}

	public class FieldDescription {
		String name;
		int mode;
		String label;
		String trueLabel;
		String falseLabel;
		String resource;

		public FieldDescription(final String name, final String label,
				final int mode, final String trueLabel,
				final String falseLabel, final String resource) {
			this.name = name;
			this.mode = mode;
			this.label = label != null ? ResourceManager.getResource(label)
					: null;
			this.trueLabel = trueLabel != null ? ResourceManager
					.getResource(trueLabel) : ResourceManager
					.getResource("global.Yes");
			this.falseLabel = falseLabel != null ? ResourceManager
					.getResource(falseLabel) : ResourceManager
					.getResource("global.No");
			this.resource = resource;
		}
	}

	protected void keyPressed(int keyCode) {
		processKey(keyCode);
	}

	public Row getSelected() throws DBException, RecordStoreFullException,
			RSException {
		final RowSet rowSet = this.rowSet;
		final int selectedIndex = this.selectedIndex;
		if (rowSet != null && selectedIndex >= 0
				&& selectedIndex < rowSet.size()) {
			return rowSet.getRowAt(selectedIndex);
		} else {
			return null;
		}
	}

	private void processKey(final int keyCode) {
		final int action = getGameAction(keyCode);
		switch (action) {
		case Canvas.FIRE:
			if (selectedPane == LIST_PANE) {
				if (rowSet != null && rowSet.size() > 0) {
					controller.handle(new ControllerEvent(
							ControllerEvent.LIST_ITEM, parent, selectedIndex));
				}
			} else {
				final ToolButton button = (ToolButton) buttons
						.elementAt(selectedButton);
				controller.handle(new ControllerEvent(button.action, parent));
			}
			break;

		case Canvas.LEFT:
			moveToolbar(-1);
			break;

		case Canvas.RIGHT:
			moveToolbar(1);
			break;

		case Canvas.UP:
			moveList(-1);
			break;

		case Canvas.DOWN:
			moveList(1);
			break;
		}
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private void moveList(final int offset) {
		if (selectedPane == TOOLBAR_PANE) {
			selectedPane = LIST_PANE;
		} else if (rowSet != null && rowSet.size() > 0) {
			final int count = indexes.length;
			final int size = rowSet.size();
			selectedIndex += offset;
			boolean rebuild = false;

			if (selectedIndex < firstIndex) {
				firstIndex--;
				rebuild = true;
			} else if (selectedIndex >= size) {
				selectedIndex = 0;
				firstIndex = 0;
				rebuild = true;
			} else if ((selectedIndex - firstIndex) >= count) {
				firstIndex++;
				rebuild = true;
			}

			if (firstIndex < 0) {
				firstIndex = size - count;
				selectedIndex = size - 1;
			} else if (firstIndex >= size) {
				firstIndex = 0;
				selectedIndex = 0;
			}

			if (rebuild) {
				dirty = rebuild;
				softRebuild = true;
			}
			dirty = rebuild;
		}
		repaint();
		// make sure the view is kept visible in case a progress view has been
		// shown
		Application.getManager().getDisplay().setCurrent(this);
	}

	/** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc */
	private void moveToolbar(final int offset) {
		if (buttons.size() > 0) {
			if (selectedPane == LIST_PANE) {
				selectedPane = TOOLBAR_PANE;
				repaint();
			} else {
				selectedButton += offset;
				if (selectedButton < 0) {
					selectedButton = buttons.size() - 1;
				} else if (selectedButton >= buttons.size()) {
					selectedButton = 0;
				}
				repaint();
			}
		}
	}

	protected void keyRepeated(int keyCode) {
		processKey(keyCode);
	}

	protected void sizeChanged(int width, int height) {
		dirty = true;
		if (isShown()) {
			repaint();
		}
	}

	public void handleEvent(Event e) {
		if (e instanceof ProgressEvent) {
			try {
				final ProgressEvent event = (ProgressEvent) e;
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
				repaint();
				Thread.yield();
			} catch (Exception e1) {
			}
		} else if (e.getType().equals(Ui.ET_FONTSIZE)) {
			font = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Ui
					.getFontSize());
			dirty = true;
			if (isShown()) {
				repaint();
			}
		}
	}
}
