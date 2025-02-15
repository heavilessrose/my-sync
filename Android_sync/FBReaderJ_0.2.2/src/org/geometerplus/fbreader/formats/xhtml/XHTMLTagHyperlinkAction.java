/*
 * Copyright (C) 2007-2008 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

package org.geometerplus.fbreader.formats.xhtml;

import org.geometerplus.zlibrary.core.xml.ZLStringMap;
import org.geometerplus.zlibrary.core.util.ZLArrayUtils;

import org.geometerplus.fbreader.bookmodel.*;

class XHTMLTagHyperlinkAction extends XHTMLTagAction {
	private byte[] myHyperlinkStack = new byte[10];
	private int myHyperlinkStackSize;

	private static boolean isReference(String text) {
		return
			text.startsWith("http://") ||
			text.startsWith("https://") ||
			text.startsWith("mailto:") ||
			text.startsWith("ftp://");
	}

	protected void doAtStart(XHTMLReader reader, ZLStringMap xmlattributes) {
		final BookReader modelReader = reader.getModelReader();
		final String href = xmlattributes.getValue("href");
		if (myHyperlinkStackSize == myHyperlinkStack.length) {
			myHyperlinkStack = ZLArrayUtils.createCopy(myHyperlinkStack, myHyperlinkStackSize, 2 * myHyperlinkStackSize);
		}
		if ((href != null) && (href.length() > 0)) {
			final String link = (href.charAt(0) == '#') ? (reader.myReferenceName + href) : href;
			final byte hyperlinkType =
				isReference(link) ?
					FBTextKind.EXTERNAL_HYPERLINK :
					FBTextKind.INTERNAL_HYPERLINK;
			myHyperlinkStack[myHyperlinkStackSize++] = hyperlinkType;
			modelReader.addHyperlinkControl(hyperlinkType, link);
		} else {
			myHyperlinkStack[myHyperlinkStackSize++] = FBTextKind.REGULAR;
		}
		final String name = xmlattributes.getValue("name");
		if (name != null) {
			modelReader.addHyperlinkLabel(reader.myReferenceName + "#" + name);
		}
	}

	protected void doAtEnd(XHTMLReader reader) {
		byte kind = myHyperlinkStack[--myHyperlinkStackSize];
		if (kind != FBTextKind.REGULAR) {
			reader.getModelReader().addControl(kind, false);
		}
	}
}
