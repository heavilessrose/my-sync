package bm.mvc;

import bm.core.mvc.*;
import bm.db.Row;
import bm.db.RowSet;
import bm.db.DBException;
import bm.db.ScrollSet;
import bm.storage.RecordStoreFullException;
import bm.storage.RSException;
import bm.ui.DataList;

import javax.microedition.lcdui.*;
import java.util.Enumeration;

/*
 * File Information
 *
 * Created on       : 07-ago-2007 18:57:50
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
public class DataListView extends AbstractView implements ListBrowser {
	public DataListView(final Controller controller, final String title,
			final int id) {
		this(controller, id);
		setTitle(title);
	}

	public DataListView(final Controller controller, final int id) {
		super(controller);
		this.id = id;
		final DataList list = new DataList(this, controller);
		list.setCommandListener(this);
		displayable = list;
		this.controller = controller;
	}

	/**
	 * Get the scroll set.
	 * 
	 * @return scroll set
	 */
	public ScrollSet getScrollSet() {
		return ((DataList) displayable).getRowSet();
	}

	/**
	 * Set the scroll set.
	 * 
	 * @param scrollSet
	 *            scroll set
	 */
	public synchronized void setScrollSet(ScrollSet scrollSet)
			throws DBException, RecordStoreFullException, RSException {
		((DataList) displayable).setRowSet((RowSet) scrollSet);
	}

	public DataList getDataList() {
		return (DataList) displayable;
	}

	/**
	 * Set the view Id
	 * 
	 * @param viewId
	 *            view id
	 */
	public void setId(int viewId) {
		this.id = viewId;
	}

	/**
	 * Check if the underlying rows have been signaled as changed an list should
	 * be rebuilt.
	 * 
	 * @return dirty flag
	 */
	public boolean isDirty() {
		return ((DataList) displayable).isDirty();
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
		((DataList) displayable).setDirty(dirty);
	}

	public void setController(final Controller controller) {
		super.setController(controller);
		((DataList) displayable).setController(controller);
	}

	/**
	 * Rebuild the list
	 * 
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public void rebuild() {
		((DataList) displayable).rebuild();
	}

	public String getSeparator() {
		return ((DataList) displayable).getSeparator();
	}

	public void setSeparator(final String separator) {
		((DataList) displayable).setSeparator(separator);
	}

	/**
	 * Add an speed option.
	 * 
	 * @param text
	 *            text
	 * @param code
	 *            code
	 */
	public void addSpeedOption(String text, int code) {
		((DataList) displayable).addToolbarItem(text, code);
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
	public void addSpeedOption(final int code, final String text,
			final String help) {
		((DataList) displayable).addToolbarItem(code, text, help);
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
	public void addSpeedOption(final int code, final Image image,
			final String help) {
		((DataList) displayable).addToolbarItem(code, image, help);
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
	public void addSpeedOption(final int code, final String iconSet,
			final int index, final String help) {
		((DataList) displayable).addToolbarItem(code, iconSet, index, help);
	}

	/**
	 * Add a field, with plain mode.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void addField(String fieldName) {
		((DataList) displayable).addField(fieldName);
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
		((DataList) displayable).addField(name, mode);
	}

	public void addField(final String name, final String label, final int mode) {
		((DataList) displayable).addField(name, label, mode);
	}

	public void addField(String name, String label, int mode, String trueLabel,
			String falseLabel, String resource) {
		((DataList) displayable).addField(name, label, mode, trueLabel,
				falseLabel, resource);
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
		((DataList) displayable).setSelected(id);
	}

	/**
	 * Remove all fields.
	 */
	public void removeAllFields() {
		((DataList) displayable).removeAllFields();
	}

	/**
	 * Enumeration of field names.
	 * 
	 * @return field names
	 */
	public Enumeration fields() {
		return ((DataList) displayable).fields();
	}

	/**
	 * Remove a field.
	 * 
	 * @param fieldName
	 *            field name
	 */
	public void removeField(String fieldName) {
		((DataList) displayable).removeField(fieldName);
	}

	/**
	 * Get the field resolver.
	 * 
	 * @return field resolver
	 */
	public FieldResolver getResolver() {
		return ((DataList) displayable).getResolver();
	}

	/**
	 * Set the field resolver.
	 * 
	 * @param resolver
	 *            resolver
	 */
	public void setResolver(final FieldResolver resolver) {
		((DataList) displayable).setResolver(resolver);
	}

	public Row getSelected() throws DBException, RecordStoreFullException,
			RSException {
		return ((DataList) displayable).getSelected();
	}
}
