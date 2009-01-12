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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.lcdui.Image;

import com.astrientlabs.log.Logger;
import com.astrientlabs.net.Network;
import com.astrientlabs.text.Strings;
import com.astrientlabs.ui.AdWindow;
import com.astrientlabs.util.ProgressListener;
import com.astrientlabs.util.TimeoutConnection;

public class SearchResult {
	protected String title;
	protected String summary;
	protected String url;
	protected String thumbnailUrl;
	protected long size;
	protected String sizeString;
	protected String format;
	protected int height;
	protected int width;
	protected boolean cancel = false;
	protected SearchQuery searchQuery;

	protected boolean valid = true;

	public SearchResult(SearchQuery searchQuery) {
		this.searchQuery = searchQuery;
	}

	public SearchQuery getSearchQuery() {
		return searchQuery;
	}

	public String getFormat() {
		return format;
	}

	public int getHeight() {
		return height;
	}

	public long getSize() {
		return size;
	}

	public String getSummary() {
		return summary;
	}

	public String getTitle() {
		return title;
	}

	public String getUrl() {
		return url;
	}

	public int getWidth() {
		return width;
	}

	public String getSizeString() {
		return (sizeString == null) ? Strings.getSizeString(size) : sizeString;
	}

	public void writeTo(OutputStream os, ProgressListener listener)
			throws IOException {
		cancel = false;
		int total = 0;

		InputStream is = null;
		HttpConnection c = null;
		try {
			c = (HttpConnection) Connector.open(url, Connector.READ);
			if (c.getResponseCode() == HttpConnection.HTTP_OK) {
				TimeoutConnection timeoutConnection = new TimeoutConnection(c,
						System.currentTimeMillis() + (1000 * 90));
				try {
					Network.setTimeout(timeoutConnection);
					AdWindow.instance.setMessage("contacting...");
					is = c.openInputStream();
					AdWindow.instance.setMessage("downloading...");
				} finally {
					Network.clearTimeout(timeoutConnection);
				}

				byte[] buffer = new byte[4 * 1024];
				int r;
				while ((r = is.read(buffer)) != -1 && !cancel) {
					total += r;
					os.write(buffer, 0, r);
					if (listener != null)
						listener.update(this, total);
				}
			} else {
				// throw new IOException();
			}
		} catch (IOException e) {
			Logger.instance.log("searchResult", "File write failure: " + title
					+ ": " + url);
			Logger.instance.log("searchResult", e);
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
		}
	}

	public Image getThumbnail(int width, int height) throws IOException {
		return getImage(this.thumbnailUrl, 256, width, height);
	}

	public Image getImage(int width, int height) throws IOException {
		return getImage(this.url, 0, width, height);
	}

	public Image getImage(String iURL, int bufferSize, int width, int height)
			throws IOException {
		cancel = false;
		HttpConnection c = null;
		InputStream is = null;

		try {
			String imageUrl = "http://www.hostj2me.com/imageproxy?key=astrient:1e4e4d:-1985900&f=JPEG&u="
					+ Strings.URLEncode(iURL) + "&w=" + width + "&h=" + height;
			c = (HttpConnection) Connector.open(imageUrl, Connector.READ);

			if (c.getResponseCode() == HttpConnection.HTTP_OK) {

				TimeoutConnection timeoutConnection = new TimeoutConnection(c,
						System.currentTimeMillis() + (1000 * 45));
				try {
					Network.setTimeout(timeoutConnection);
					is = c.openInputStream();
				} finally {
					Network.clearTimeout(timeoutConnection);
				}

				if (bufferSize == 0) {
					return Image.createImage(is);
				} else {
					byte[] buffer = new byte[256];
					ByteArrayOutputStream baos = new ByteArrayOutputStream(1024);

					int i;
					while (!cancel && ((i = is.read(buffer)) != -1)) {
						baos.write(buffer, 0, i);
					}

					buffer = baos.toByteArray();

					if (buffer.length > 0 && !cancel) {
						return Image.createImage(buffer, 0, buffer.length);
					}

					return null;
				}
			} else {
				valid = false;
				throw new IOException("HTTP ERR: " + c.getResponseCode() + " "
						+ c.getResponseMessage());
			}
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
		}
	}

	public boolean isValid() {
		return valid;
	}

	public void cancel() {
		cancel = true;
	}

	public String getThumbnailUrl() {
		return thumbnailUrl;
	}
}
