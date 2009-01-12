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
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import com.astrientlabs.prefs.PropertyMap;
import com.astrientlabs.util.ProgressListener;
import com.astrientlabs.xml.MiniPushHandler;
import com.astrientlabs.xml.MiniPushParser;

public class SearchQuery implements MiniPushHandler {
	public static final Vector cannedResults = new Vector();

	private Vector results = new Vector();
	private MiniPushParser parser = new MiniPushParser();

	private String appId;
	private String term;
	private boolean filter;
	private int pageNumber;
	private int pageSize;

	private SearchResult current = null;
	private boolean cancelled;
	private boolean running;
	private ProgressListener progressListener;
	private boolean thumbnail;

	public ProgressListener getProgressListener() {
		return progressListener;
	}

	public void setProgressListener(ProgressListener progressListener) {
		this.progressListener = progressListener;
	}

	public void cancel() {
		cancelled = true;
		parser.cancel();
	}

	public Vector execute() throws IOException {
		if (!cancelled && !running) {
			running = true;
			try {
				results.removeAllElements();

				if (pageNumber == 0) {
					CannedResult cannedResult;
					for (Enumeration e = cannedResults.elements(); e
							.hasMoreElements();) {
						cannedResult = (CannedResult) e.nextElement();

						if (cannedResult.getTitle().startsWith(term)) {
							results.addElement(cannedResult);
							progressListener.update(this, results.size());

							// cannedResult.searchQuery = this;
						}
					}
				}

				if (term.length() > 1) {
					HttpConnection c = null;
					InputStream is = null;
					try {
						StringBuffer buffer = new StringBuffer(
								"http://search.yahooapis.com/ImageSearchService/V1/imageSearch?")
								.append("appid=").append(appId).append(
										"&query=").append(term).append(
										"&results=").append(pageSize).append(
										"&adult_ok=").append(filter ? 0 : 1)
								.append("&start=")
								.append(pageNumber * pageSize);
						pageNumber++;

						// Logger.instance.log(buffer.toString());

						c = (HttpConnection) Connector.open(buffer.toString(),
								Connector.READ);
						is = c.openInputStream();
						parser.parse(is, this, 1024);
					} finally {
						try {
							if (is != null) {
								is.close();
							}
						} catch (Exception ignore) {
						}
						try {
							if (c != null) {
								c.close();
							}
						} catch (Exception ignore) {
						}

						progressListener.completed(this);
					}
				}

			} finally {
				running = false;
			}

		}

		return results;
	}

	public void handleAttribute(String tag, String attribute, String value) {

	}

	public void handleEndTag(String tag) {
		if (tag.equalsIgnoreCase("result")) {
			if (current != null) {
				results.addElement(current);
				progressListener.update(this, results.size());
			}
		} else if (tag.equalsIgnoreCase("thumbnail")) {
			thumbnail = false;
		}
	}

	public void handleStartTag(String tag) {
		if (tag.equalsIgnoreCase("result")) {
			current = new SearchResult(this);
		} else if (tag.equalsIgnoreCase("thumbnail")) {
			thumbnail = true;
		}
	}

	public void handleText(String tag, String text) {
		text = text.trim();

		if (tag.equalsIgnoreCase("Url")) {
			if (thumbnail) {
				current.thumbnailUrl = text;
			} else {
				current.url = text;
			}
		} else if (tag.equalsIgnoreCase("title")) {
			current.title = text;
		} else if (tag.equalsIgnoreCase("filesize")) {
			try {
				current.size = Long.parseLong(text);
			} catch (Exception e) {
				current.sizeString = text;
			}
		}
	}

	public String getAppId() {
		return appId;
	}

	public void setAppId(String appId) {
		this.appId = appId;
	}

	public boolean isFilter() {
		return filter;
	}

	public void setFilter(boolean filter) {
		if (filter != this.filter) {
			this.filter = filter;
			pageNumber = 0;
		}
	}

	public int getPageNumber() {
		return pageNumber;
	}

	public void setPageNumber(int pageNumber) {
		this.pageNumber = pageNumber;
	}

	public int getPageSize() {
		return pageSize;
	}

	public void setPageSize(int pageSize) {
		this.pageSize = pageSize;
	}

	public Vector getResults() {
		return results;
	}

	public String getTerm() {
		return term;
	}

	public void setTerm(String term) {
		term = term.trim().replace(' ', '+');
		if (!term.equals(this.term)) {
			pageNumber = 0;
			this.term = term;
			cancelled = false;
		}
	}

	public static void loadCannedResults() {
		PropertyMap map = new PropertyMap("cannedresults");
		map.initialize();

		String temp;
		CannedResult cannedResult;
		for (Enumeration e = map.keys(); e.hasMoreElements();) {
			cannedResult = new CannedResult(null);
			cannedResult.title = e.nextElement().toString();

			temp = map.get(cannedResult.title);
			cannedResult.type = Integer.parseInt(temp.substring(0, 1));
			cannedResult.url = temp.substring(1);

			cannedResults.addElement(cannedResult);
		}
	}
}
