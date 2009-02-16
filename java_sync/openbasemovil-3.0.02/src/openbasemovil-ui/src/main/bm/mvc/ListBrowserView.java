package bm.mvc;

import bm.core.ErrorHandler;
import bm.core.ResourceManager;
import bm.core.event.ProgressEvent;
import bm.core.math.FixedPoint;
import bm.core.mvc.AbstractController;
import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.mvc.ControllerEvent;
import bm.core.tools.DeviceInfo;
import bm.core.tools.LocalFormatTools;
import bm.db.DBException;
import bm.db.Row;
import bm.db.ScrollSet;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.util.Util;

import javax.microedition.lcdui.*;
import java.util.Date;
import java.util.Enumeration;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 08-may-2006 10:08:29
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2006/01/03 13:17:46 $
 * Revision         : $Revision: 1.10 $
 */

/**
 * ListBrowserView browses a ScrollSet showing the results using a list, with
 * implicit selection, and showing results in pages for faster browsing.<br/>
 * It requires that subclasess create the labels for the rows, and allows them
 * to handle custom commands and control different aspects of the browsing, such
 * as stablishing the page size.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 1.10 $
 */
public class ListBrowserView extends AbstractView implements ListBrowser {
	/**
	 * Default page size.
	 */
	public static final int DEFAULT_PAGE_SIZE = 20;

	/**
	 * ControllerEvent code for page down.
	 */
	public static final int PAGE_DOWN = 100000;
	/**
	 * ControllerEvent code for page up.
	 */
	public static final int PAGE_UP = 100001;

	/**
	 * The associated ScrollSet.
	 */
	protected ScrollSet set;
	/**
	 * The page size, by default DEFAULT_PAGE_SIZE.
	 */
	protected int pageSize = DEFAULT_PAGE_SIZE;
	/**
	 * The current page.
	 */
	protected int page;
	/**
	 * Total number of pages.
	 */
	protected int pageCount;
	/**
	 * Number of rows to show.
	 */
	protected int size;
	/**
	 * ProgressEvent object for reusage on lengthy operations.
	 */
	protected ProgressEvent pe;
	/**
	 * Vector of field names in the order they should be used to build the
	 * labels.
	 */
	protected Vector fieldNames = new Vector(3);
	/**
	 * Default separator for field content.
	 */
	protected String separator = " ";
	protected int viewId;

	protected FieldResolver resolver;
	protected boolean dirty;
	protected Vector speedOptionsText = new Vector(1);
	protected Vector speedOptionsAction = new Vector(1);

	private boolean rebuilding;

	/**
	 * Constructs a ListBrowserView with default page size.
	 * 
	 * @param controller
	 *            view controller
	 * @param title
	 *            view title
	 * @param viewId
	 *            view id
	 */
	public ListBrowserView(final Controller controller, final String title,
			final int viewId) {
		this(controller, title, viewId, DEFAULT_PAGE_SIZE);
	}

	/**
	 * Constructor.<br/>
	 * If the controller is also an ErrorHandler it automatically configures it
	 * as the view error handler.
	 * 
	 * @param controller
	 *            view controller
	 * @param title
	 *            view title
	 * @param pageSize
	 *            page size
	 * @param viewId
	 *            view id
	 */
	public ListBrowserView(final Controller controller, final String title,
			final int viewId, final int pageSize) {
		super(controller);
		this.pageSize = pageSize;
		this.viewId = viewId;

		final ProgressEvent pe = new ProgressEvent(this, new Integer(pageSize),
				0);
		pe.setTitle(ResourceManager.getResource("global.Searching"));
		pe.setPhase(ResourceManager.getResource("global.Searching"));

		final List list = new List(title, List.IMPLICIT);
		list.setCommandListener(this);
		list.setFitPolicy(Choice.TEXT_WRAP_ON);
		displayable = list;
		this.pe = pe;
	}

	/**
	 * Each view must have an id that identifies it.
	 * 
	 * @return view id
	 */
	public int getId() {
		return viewId;
	}

	public void setId(final int viewId) {
		this.viewId = viewId;
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
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws DBException
	 *             on errors
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public void setDirty(final boolean dirty) throws RSException,
			RecordStoreFullException, DBException {
		this.dirty = dirty;
		if (dirty && isShown()) {
			rebuild();
		}
	}

	public void addSpeedOption(final String text, final int code) {
		speedOptionsText.addElement(text);
		speedOptionsAction.addElement(new Integer(code));
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
	public void addSpeedOption(int code, String text, String help) {
		speedOptionsText.addElement(text);
		speedOptionsAction.addElement(new Integer(code));
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
	public void addSpeedOption(int code, Image image, String help) {
		speedOptionsText.addElement(help);
		speedOptionsAction.addElement(new Integer(code));
		// ToDo: do something with image
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
	public void addSpeedOption(int code, String iconSet, int index, String help) {
		speedOptionsText.addElement(help);
		speedOptionsAction.addElement(new Integer(code));
		// ToDo: do something with image
	}

	/**
	 * Make the displayable object the current at the display.
	 */
	public void show() {
		if (dirty && !rebuilding) {
			try {
				rebuild();
			} catch (Exception e) {
				ErrorHandler.handleError(this, e);
			}
		}
		super.show();
	}

	/**
	 * Enable or disable text wrapping, enabled by default.
	 * 
	 * @param wrap
	 *            enable or disable text wrapping
	 */
	public void setWrap(final boolean wrap) {
		((List) displayable).setFitPolicy(wrap ? Choice.TEXT_WRAP_ON
				: Choice.TEXT_WRAP_OFF);
	}

	/**
	 * Set the scroll set.
	 * 
	 * @param scrollSet
	 *            scroll set
	 */
	public synchronized void setScrollSet(ScrollSet scrollSet)
			throws DBException, RecordStoreFullException, RSException {
		this.set = scrollSet;
		final int size = scrollSet.size();
		this.size = size;
		pageCount = size / pageSize + (size % pageSize > 0 ? 1 : 0);
		page = 0;
		rebuild();
	}

	/**
	 * Rebuild the list and go to first page (0).
	 * 
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws DBException
	 *             on errors
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public void rebuild() throws RecordStoreFullException, DBException,
			RSException {
		if (set != null) {
			rebuilding = true;
			try {
				final int size = set.size();
				this.size = size;
				pageCount = size / pageSize + (size % pageSize > 0 ? 1 : 0);
				page = 0;
				fill();
				dirty = false;
			} finally {
				rebuilding = false;
			}
		}
	}

	/**
	 * Find a row by it's id, move the set to the appropriate page and make that
	 * row selected.
	 * 
	 * @param id
	 *            row id
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             on errors
	 * @throws RSException
	 *             on errors
	 */
	public void setSelected(final long id) throws DBException,
			RecordStoreFullException, RSException {
		for (int i = 0; i < size; i++) {
			if (set.getRowAt(i).getId() == id) {
				final int page = i / pageSize;
				int offset = i % pageSize;
				if (page > 0) {
					offset++;
				}
				offset += speedOptionsText.size();
				setPage(page);
				((List) displayable).setSelectedIndex(offset, true);
			}
		}
	}

	/**
	 * Get the scroll set.
	 * 
	 * @return scroll set
	 */
	public ScrollSet getScrollSet() {
		return set;
	}

	/**
	 * Separator for field values.
	 * 
	 * @return separator, by default a single space
	 */
	public String getSeparator() {
		return separator;
	}

	/**
	 * Set the separator for field values.
	 * 
	 * @param separator
	 *            new separator
	 */
	public void setSeparator(final String separator) {
		this.separator = separator;
	}

	/**
	 * Add a field name to he list.<br/>
	 * If the field name ends with a dollar sign, it means that the field is a
	 * FixedPoint and should be rendered using toMoneyString instead of
	 * toString.<br/>
	 * If the field name ends with an admiration sign, it means that the field
	 * is a Date and should be rendered as a Date only (whitout hour).<br/>
	 * If the field name ends with a pound sign, it means that the field is a
	 * Date and should be rendered as a Time only.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void addField(final String fieldName) {
		if (!fieldNames.contains(fieldName)) {
			fieldNames.addElement(fieldName);
		}
	}

	public void addField(String name, int mode) {
		if (!fieldNames.contains(name)) {
			fieldNames.addElement(name);
		}
		// ToDo: do something with mode
	}

	public void addField(String name, String label, int mode) {
		if (!fieldNames.contains(name)) {
			fieldNames.addElement(name);
		}
	}

	public void addField(String name, String label, int mode, String trueLabel,
			String falseLabel, String resource) {
		if (!fieldNames.contains(name)) {
			fieldNames.addElement(name);
		}
	}

	/**
	 * Remove a field name from the list.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void removeField(final String fieldName) {
		if (fieldNames.contains(fieldName)) {
			fieldNames.removeElement(fieldName);
		}
	}

	/**
	 * Remove all fields.
	 */
	public void removeAllFields() {
		fieldNames.removeAllElements();
	}

	/**
	 * Enumeration of field names.
	 * 
	 * @return field names
	 */
	public Enumeration fields() {
		return fieldNames.elements();
	}

	/**
	 * Go to the next page.
	 * 
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public synchronized void pageDown() throws DBException,
			RecordStoreFullException, RSException {
		setPage(page + 1);
	}

	/**
	 * Go to the previous page.
	 * 
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public synchronized void pageUp() throws DBException,
			RecordStoreFullException, RSException {
		setPage(page - 1);
	}

	/**
	 * Go to the specified page.
	 * 
	 * @param page
	 *            page number, starting at 0
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public synchronized void setPage(final int page)
			throws RecordStoreFullException, DBException, RSException {
		if (page >= 0 && page < pageCount) {
			this.page = page;
			fill();
		}
	}

	private void fill() throws DBException, RecordStoreFullException,
			RSException {
		final int page = this.page;
		final List list = (List) displayable;
		final ProgressEvent pe = this.pe;

		onBeforeFillPage();

		list.deleteAll();
		final ScrollSet set = this.set;
		if (set != null) {
			try {
				set.openSource();
				final int speed = speedOptionsAction.size();
				for (int i = 0; i < speed; i++) {
					list.append(
							ResourceManager
									.getResource((String) speedOptionsText
											.elementAt(i)), null);
				}
				if (page > 0) {
					list.append(ResourceManager
							.getResource("listBrowserView.prevPage"), null);
				}
				final int pageSize = this.pageSize;
				int count = 0;
				int index = page * pageSize;
				pe.setValue(new Integer(0));
				pe.dispatch();
				// noinspection MethodCallInLoopCondition
				while (count < pageSize && index < size) {
					final String label = buildLabel(set.getRowAt(index));
					if (label != null) {
						list.append(label, null);
						count++;
					}
					index++;
					pe.increment();
					pe.dispatch();
				}
				if (page < (pageCount - 1)) {
					list.append(ResourceManager
							.getResource("listBrowserView.nextPage"), null);
				}
				if (!DeviceInfo.getDeviceInfo().hasListFontBug()) {
					final int itemCount = list.size();
					for (int i = 0; i < itemCount; i++) {
						list.setFont(i, Util.SMALL_FONT);
					}
				}
			} finally {
				set.closeSource();
			}
		}
		onAfterFillPage();
		show();
	}

	/**
	 * Get the selected element from the ScrollSet.
	 * 
	 * @return selected element or null if none selected or no ScrollSet.
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	public Row getSelected() throws DBException, RecordStoreFullException,
			RSException {
		final ScrollSet scrollSet = this.set;
		if (scrollSet != null) {
			return scrollSet.getRowAt(getSelectedIndex());
		} else {
			return null;
		}
	}

	/**
	 * Get the real selected index in the scrollset.
	 * 
	 * @return index or -1 if no item selected
	 */
	protected int getSelectedIndex() {
		final int pageSize = this.pageSize;
		final int page = this.page;
		final List list = (List) displayable;
		if (list != null) {
			int firstValid = speedOptionsAction.size();
			if (page > 0) {
				firstValid++;
			}
			int lastValid = list.size();
			if (page < (pageCount - 1)) {
				lastValid--;
			}
			final int listSelectedIndex = list.getSelectedIndex();
			if (listSelectedIndex >= firstValid
					&& listSelectedIndex <= lastValid) {
				return (page * pageSize) + (listSelectedIndex - firstValid);
			} else {
				return listSelectedIndex;
			}
		} else {
			return -1;
		}
	}

	/**
	 * Handle commands. First asks the subclass through the handleCommand method
	 * and if the subclass does not process the command, it processes it to
	 * handle page and selection events.
	 * 
	 * @param command
	 *            command the command
	 * @param displayable
	 *            displayable
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public final void commandAction(final Command command,
			final Displayable displayable) {
		if (!handleAction(command)) {
			if (!handleCommand(command)) {
				final List list = (List) displayable;
				final ControllerEvent event = AbstractController.buildEvent(
						this, command, list);
				if (event.getCode() == ControllerEvent.LIST_ITEM) {
					final int listSelectedIndex = list.getSelectedIndex();
					if (listSelectedIndex < speedOptionsAction.size()) {
						event.setCode(((Integer) speedOptionsAction
								.elementAt(listSelectedIndex)).intValue());
					} else {
						if (page == 0) {
							if (listSelectedIndex < pageSize) {
								event.setIndex(listSelectedIndex);
							} else {
								event.setCode(PAGE_DOWN);
							}
						} else {
							if (listSelectedIndex == speedOptionsAction.size()) {
								event.setCode(PAGE_UP);
							} else if (listSelectedIndex < (pageSize
									+ speedOptionsAction.size() + 1)) {
								event.setIndex(listSelectedIndex);
							} else {
								event.setCode(PAGE_DOWN);
							}
						}
					}
				}
				controller.handle(event);
			}
		}
	}

	/**
	 * Create the label to show on the list for the ScrollSet current element.<br/>
	 * Subclases can override this method to provide a custom implementation.
	 * 
	 * @param row
	 *            the row to represent
	 * @return label or null if for any reason this row should not be included
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 * @noinspection SingleCharacterStartsWith
	 */
	protected String buildLabel(final Row row) throws DBException,
			RecordStoreFullException, RSException {
		final StringBuffer label = new StringBuffer();
		// noinspection MethodCallInLoopCondition
		for (final Enumeration i = fieldNames.elements(); i.hasMoreElements();) {
			String value = null;
			final String field = (String) i.nextElement();
			final char lastChar = field.charAt(field.length() - 1);
			final Object raw = resolve(row, lastChar == '$' || lastChar == '!'
					|| lastChar == '&' ? field.substring(0, field.length() - 1)
					: field);
			if (raw != null) {
				switch (lastChar) {
				case '$':
					if (raw instanceof FixedPoint) {
						value = ((FixedPoint) raw).toMoneyString();
					}
					break;

				case '!':
					if (raw instanceof Date) {
						value = LocalFormatTools.formatDate((Date) raw);
					}
					break;

				case '&':
					if (raw instanceof Date) {
						value = LocalFormatTools.formatTime((Date) raw);
					}
					break;
				}
				if (value == null) {
					if (raw instanceof Date) {
						value = LocalFormatTools.formatTimestamp((Date) raw);
					} else {
						value = raw.toString();
					}
				}
				if (label.length() > 0) {
					label.append(separator);
				}
				label.append(value);
			}
		}
		return label.toString();
	}

	private Object resolve(final Row row, final String fieldName)
			throws RSException, RecordStoreFullException, DBException {
		if (row.getParent().hasField(fieldName)) {
			return row.getField(fieldName);
		} else if (resolver != null) {
			return resolver.resolve(this, fieldName, row);
		} else {
			return null;
		}
	}

	/**
	 * Called before the screen is emptied and filled with a new page of data.<br/>
	 * Default implementation is empty.
	 * 
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	protected void onBeforeFillPage() throws DBException,
			RecordStoreFullException, RSException {
	}

	/**
	 * Allows subclasses to trap commands before they are processed.<br/>
	 * Default implementation does nothing.
	 * 
	 * @param command
	 *            command to process
	 * @return true if the command has been consumed, false otherwise
	 */
	protected boolean handleCommand(final Command command) {
		return false;
	}

	/**
	 * Called after the screen is emptied and filled with a new page of data.<br/>
	 * Default implementation is empty.
	 * 
	 * @throws DBException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws bm.storage.RSException
	 *             on errors
	 */
	protected void onAfterFillPage() throws DBException,
			RecordStoreFullException, RSException {
	}
}
