/*
 * Copyright (c) 2005 Elondra S.L. All Rights Reserved.
 */
package bm.core.log;

/* -----------------------------------------------------------------------------
 OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
/**
 * Create and configure Log instances, using the link time class provided for
 * implementation.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class LogFactory {
	/**
	 * Get a new Log instance for the given package/class name.<br/>
	 * The method will try to find a suitable Log implementation linked to the
	 * application. It's name is fixed: <code>bm.core.log.LogImpl</code>
	 * 
	 * @param name
	 *            package/class name
	 * @return Log instance, null if no suitable Log implementation found.
	 */
	public static Log getLog(final String name) {
		try {
			final Class clazz = Class.forName("bm.core.log.LogImpl"); // [Commenter-Don't]
			final Log log = (Log) clazz.newInstance(); // [Commenter-Don't]
			((LogConfigurator) log).setName(name);
			return log;
		} catch (Exception e) {
			return null;
		}
	}
}
