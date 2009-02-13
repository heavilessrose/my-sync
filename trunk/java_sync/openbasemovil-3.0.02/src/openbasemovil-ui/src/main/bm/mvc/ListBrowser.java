package bm.mvc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.mvc.View;
import bm.db.DBException;
import bm.db.Row;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;

import javax.microedition.lcdui.Image;
import java.util.Enumeration;
/*
 * File Information
 *
 * Created on       : 14-ago-2007 16:34:11
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * ListBrowsers allow the browsing of a RowSet.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface ListBrowser
        extends View,
                ScrollerView,
                DataboundView
{
    /**
     * Set the view Id
     * @param viewId view id
     */
    void setId( int viewId );

    /**
     * Check the dirty mark.
     * @return if is dirty and rowset should be re rendered.
     */
    boolean isDirty();

    /**
     * Set the dirty mark. If view is shown, it should rebuild the list.
     * @param dirty dirty
     * @throws RSException on errors
     * @throws RecordStoreFullException if no space left
     * @throws DBException on errors
     */
    void setDirty( boolean dirty )
            throws RSException,
                   RecordStoreFullException,
                   DBException;

    /**
     * Add an speed option.
     *
     * @param text text
     * @param code code
     */
    void addSpeedOption( String text, int code );

    /**
     * Rebuild the list
     * @throws RecordStoreFullException if no space left
     * @throws DBException on errors
     * @throws RSException on errors
     */
    void rebuild()
           throws RecordStoreFullException,
                  DBException,
                  RSException;

    /**
     * Set the selected item. It should scroll to make it visible and selected.
     * @param id row id
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors
     */
    void setSelected( long id )
          throws DBException,
                 RecordStoreFullException,
                 RSException;

    /**
     * Get the separator between fields to build the label.
     * @return separator
     */
    String getSeparator();

    /**
     * Set the separator between fields to build the label.
     * @param separator separator
     */
    void setSeparator( String separator );

    /**
     * Add a field.
     * @param fieldName field name
     */
    void addField( String fieldName );

    /**
     * Remove a field.
     * @param fieldName field name
     */
    void removeField( String fieldName );

    /**
     * Remove all fields.
     */
    void removeAllFields();

    /**
     * Enumeration of field names.
     * @return field names
     */
    Enumeration fields();

    /**
     * Get the selected row.
     * @return selected row, null if none selected
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space left
     * @throws RSException on errors
     */
    Row getSelected()
            throws DBException,
                   RecordStoreFullException,
                   RSException;

    /**
     * Add an speed option.
     *
     * @param code code
     * @param text text
     * @param help help text
     */
    void addSpeedOption( int code, String text, String help );

    /**
     * Add an speed option.
     *
     * @param code code
     * @param image image
     * @param help help text
     */
    void addSpeedOption( int code, Image image, String help );

    /**
     * Add an speed option.
     *
     * @param code code
     * @param iconSet iconSet name
     * @param index icon index
     * @param help help text
     */
    void addSpeedOption(
            int       code,
            String    iconSet,
            int       index,
            String    help
    );

    void addField( String name, int mode );

    void addField( String name, String label, int mode );

    void addField(
            String  name,
            String  label,
            int     mode,
            String  trueLabel,
            String  falseLabel,
            String  resource
    );
}
