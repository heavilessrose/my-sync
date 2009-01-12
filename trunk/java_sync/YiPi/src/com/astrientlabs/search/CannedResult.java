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
package com.astrientlabs.search;

import java.io.IOException;
import java.io.OutputStream;

import com.astrientlabs.util.ProgressListener;

public class CannedResult extends SearchResult {
	public static final int TYPE_LINK = 0;
	public static final int TYPE_POPUP = 1;
	public static final int TYPE_SETTING = 2;

	protected int type;

	public CannedResult(SearchQuery searchQuery) {
		super(searchQuery);
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public void writeTo(OutputStream os, ProgressListener listener)
			throws IOException {
	}

	public String toString() {
		return url;
	}
}
