package bm.util;

/* -----------------------------------------------------------------------------
 OpenBaseMovil Util Library, miscellaneous tools
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

import bm.db.PersistentMap;

import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 08-may-2006 14:27:21
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Holds application state, handling save and restore of the attached
 * information.<br/>
 * Works like a persistent map, and both keys and values must be of a type
 * supported by SerializerOutputStream.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ApplicationState extends PersistentMap {
	private static final String RS_NAME = "sys_appstate";

	protected static ApplicationState map = new ApplicationState();
	protected static Hashtable transientItems = new Hashtable(10);

	protected ApplicationState() {
		super(RS_NAME);
	}

	public static synchronized void reload() {
		map = new ApplicationState();
	}

	public static ApplicationState getInstance() {
		return map;
	}

	public static Object peek(final Object key) {
		return transientItems.get(key);
	}

	public static synchronized void poke(final Object key, final Object value) {
		if (value != null) {
			transientItems.put(key, value);
		} else {
			transientItems.remove(key);
		}
	}

	public static void clearTransient() {
		transientItems.clear();
	}
}
