/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.prefs;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreNotFoundException;

import com.astrientlabs.log.Logger;

public class PropertyMap extends Hashtable {
	public static final char RECORD_DELIMITER = '=';

	private PropertyMap parent;
	private String name;

	public PropertyMap(String name) {
		this.name = name;
	}

	public PropertyMap(PropertyMap parent, String name) {
		this.name = name;
		this.parent = parent;
	}

	public void initialize() {
		InputStream is = null;

		try {
			is = getClass().getResourceAsStream(
					"/config/" + name + ".properties");
			parse(is);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (is != null)
					is.close();
			} catch (Exception e) {
			}
		}
	}

	public void parse(InputStream is) throws IOException {
		int b;
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		while ((b = is.read()) != -1) {
			if (b == '\n') {
				addLine(new String(baos.toByteArray()));
				baos.reset();
			} else {
				if (b != '\n' && b != '\r')
					baos.write(b);
			}
		}

		if (baos.size() != 0) {
			addLine(new String(baos.toByteArray()));
		}
	}

	private void addLine(String line) {
		if (line.charAt(0) != '#') {
			int ii = line.indexOf(RECORD_DELIMITER);
			if (ii != -1 && ii != (line.length() - 1)) {
				put(line.substring(0, ii), line.substring(ii + 1));
			}
		}
	}

	public void delete() {
		String recordStoreName = "props-" + name;
		try {
			RecordStore.deleteRecordStore(recordStoreName);
		} catch (RecordStoreNotFoundException e) {
		} catch (Exception e) {
			Logger.instance.log("propertyMap.delete()", e);
		}
	}

	public void read() {
		String recordStoreName = "props-" + name;
		RecordStore preferenceDB = null;

		try {
			preferenceDB = RecordStore.openRecordStore(recordStoreName, false);
			RecordEnumeration renum = preferenceDB.enumerateRecords(null, null,
					false);

			while (renum.hasNextElement()) {
				addLine(new String(renum.nextRecord()));
			}
		} catch (RecordStoreNotFoundException e) {
		} catch (Exception e) {
			Logger.instance.log("propertyMap.read()", e);
		} finally {
			try {
				if (preferenceDB != null) {
					preferenceDB.closeRecordStore();
				}
			} catch (Exception e) {
			}
		}
	}

	public void write() {
		String recordStoreName = "props-" + name;
		RecordStore recordStore = null;

		try {
			delete();

			if (!isEmpty()) {
				recordStore = RecordStore
						.openRecordStore(recordStoreName, true);

				Object key;
				byte[] bytes;

				StringBuffer buffer = new StringBuffer();
				Enumeration keys = keys();
				while (keys.hasMoreElements()) {
					key = keys.nextElement();
					buffer.append(key);
					buffer.append(RECORD_DELIMITER);
					buffer.append(get(key));

					bytes = buffer.toString().getBytes();

					recordStore.addRecord(bytes, 0, bytes.length);
					buffer.setLength(0);
				}
			}

			Logger.instance.log("propertyMap.write()", name + " saved to "
					+ recordStoreName);
		} catch (Exception e) {
			Logger.instance.log("propertyMap.write().1", e);
		} finally {
			try {
				if (recordStore != null) {
					recordStore.closeRecordStore();
				}
			} catch (Exception e) {
				Logger.instance.log("propertyMap.write().2", e);
			}
		}
	}

	// TODO: only strings
	public Object put(Object arg0, Object arg1) {
		return super.put(arg0.toString(), arg1.toString());
	}

	public String get(String name) {
		String value = (String) super.get(name);
		if (value == null) {
			return (parent == null) ? null : parent.get(name);
		} else {
			return value;
		}
	}

	public String get(String name, String def) {
		String value = get(name);
		return (value == null) ? def : String.valueOf(value);
	}

	public boolean getBoolean(String name, boolean def) {
		String value = get(name);
		return (value == null) ? def : value.equalsIgnoreCase("true");
	}

	public int getInt(String name, int def) {
		String value = get(name);
		if (value != null) {
			try {
				if (value.startsWith("-")) {
					return -1 * Integer.parseInt(value.substring(1));
				} else {
					return Integer.parseInt(value);
				}
			} catch (Exception e) {
			}
		}

		return def;
	}

	public int getInt(String name, int def, int r) {
		String value = get(name);
		if (value != null) {
			try {
				if (value.startsWith("-")) {
					return -1 * Integer.parseInt(value.substring(1), r);
				} else {
					return Integer.parseInt(value, r);
				}
			} catch (Exception e) {
			}
		}

		return def;
	}
}