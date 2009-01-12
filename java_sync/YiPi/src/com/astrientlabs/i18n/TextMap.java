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
package com.astrientlabs.i18n;

import com.astrientlabs.prefs.PropertyMap;

public class TextMap {
	public static final TextMap instance = new TextMap();
	private PropertyMap strings;

	private TextMap() {
	}

	public void init(String locale) {

		if (locale == null) {
			strings = new PropertyMap("text");
		} else {
			PropertyMap parent = new PropertyMap("text");
			parent.initialize();

			strings = new PropertyMap(parent, "text_" + locale);
		}

		strings.initialize();
	}

	public String get(String key) {
		String value = strings.get(key);
		return (value == null) ? "" : value;
	}
}
