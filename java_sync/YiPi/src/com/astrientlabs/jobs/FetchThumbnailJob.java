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
package com.astrientlabs.jobs;

import com.astrientlabs.ui.ListWidget;
import com.astrientlabs.ui.SearchResultWidget;
import com.astrientlabs.util.ProgressListener;

public class FetchThumbnailJob implements Runnable, ProgressListener {
	protected SearchResultWidget searchResultWidget;

	protected ListWidget listWidget;

	public FetchThumbnailJob(SearchResultWidget searchResultWidget,
			ListWidget listWidget) {
		this.listWidget = listWidget;
		this.searchResultWidget = searchResultWidget;
	}

	public void run() {
		if (listWidget.contains(searchResultWidget)) {
			try {
				searchResultWidget.setImage(searchResultWidget
						.getSearchResult().getThumbnail(29, 29));
			} catch (Throwable e) {
				// Logger.instance.log("slideshowWidget.run()", e);
			}
		}

	}

	public void update(Object who, long position) {
	}

	public void completed(Object who) {

	}
}
