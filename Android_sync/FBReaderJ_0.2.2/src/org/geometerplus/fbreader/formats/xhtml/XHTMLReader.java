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

import java.util.*;
import java.io.InputStream;

import org.geometerplus.zlibrary.core.library.ZLibrary;
import org.geometerplus.zlibrary.core.xml.*;

import org.geometerplus.fbreader.bookmodel.*;

public class XHTMLReader extends ZLXMLReaderAdapter {
	private static final HashMap ourTagActions = new HashMap();

	public static XHTMLTagAction addAction(String tag, XHTMLTagAction action) {
		XHTMLTagAction old = (XHTMLTagAction)ourTagActions.get(tag);
		ourTagActions.put(tag, action);
		return old;
	}

	public static void fillTagTable() {
		if (ourTagActions.isEmpty()) {
			//addAction("html", new XHTMLTagAction());
			addAction("body", new XHTMLTagParagraphAction());
			//addAction("title", new XHTMLTagAction());
			//addAction("meta", new XHTMLTagAction());
			//addAction("script", new XHTMLTagAction());

			//addAction("font", new XHTMLTagAction());
			//addAction("style", new XHTMLTagAction());

			addAction("p", new XHTMLTagParagraphAction());
			addAction("h1", new XHTMLTagParagraphWithControlAction(FBTextKind.H1));
			addAction("h2", new XHTMLTagParagraphWithControlAction(FBTextKind.H2));
			addAction("h3", new XHTMLTagParagraphWithControlAction(FBTextKind.H3));
			addAction("h4", new XHTMLTagParagraphWithControlAction(FBTextKind.H4));
			addAction("h5", new XHTMLTagParagraphWithControlAction(FBTextKind.H5));
			addAction("h6", new XHTMLTagParagraphWithControlAction(FBTextKind.H6));

			//addAction("ol", new XHTMLTagAction());
			//addAction("ul", new XHTMLTagAction());
			//addAction("dl", new XHTMLTagAction());
			addAction("li", new XHTMLTagItemAction());

			addAction("strong", new XHTMLTagControlAction(FBTextKind.STRONG));
			addAction("b", new XHTMLTagControlAction(FBTextKind.BOLD));
			addAction("em", new XHTMLTagControlAction(FBTextKind.EMPHASIS));
			addAction("i", new XHTMLTagControlAction(FBTextKind.ITALIC));
			final XHTMLTagAction codeControlAction = new XHTMLTagControlAction(FBTextKind.CODE);
			addAction("code", codeControlAction);
			addAction("tt", codeControlAction);
			addAction("kbd", codeControlAction);
			addAction("var", codeControlAction);
			addAction("samp", codeControlAction);
			addAction("cite", new XHTMLTagControlAction(FBTextKind.CITE));
			addAction("sub", new XHTMLTagControlAction(FBTextKind.SUB));
			addAction("sup", new XHTMLTagControlAction(FBTextKind.SUP));
			addAction("dd", new XHTMLTagControlAction(FBTextKind.DEFINITION_DESCRIPTION));
			addAction("dfn", new XHTMLTagControlAction(FBTextKind.DEFINITION));
			addAction("strike", new XHTMLTagControlAction(FBTextKind.STRIKETHROUGH));

			addAction("a", new XHTMLTagHyperlinkAction());

			addAction("img", new XHTMLTagImageAction("src"));
			addAction("object", new XHTMLTagImageAction("data"));

			//addAction("area", new XHTMLTagAction());
			//addAction("map", new XHTMLTagAction());

			//addAction("base", new XHTMLTagAction());
			//addAction("blockquote", new XHTMLTagAction());
			addAction("br", new XHTMLTagRestartParagraphAction());
			//addAction("center", new XHTMLTagAction());
			addAction("div", new XHTMLTagParagraphAction());
			//addAction("dt", new XHTMLTagAction());
			//addAction("head", new XHTMLTagAction());
			//addAction("hr", new XHTMLTagAction());
			//addAction("link", new XHTMLTagAction());
			//addAction("param", new XHTMLTagAction());
			//addAction("q", new XHTMLTagAction());
			//addAction("s", new XHTMLTagAction());

			addAction("pre", new XHTMLTagPreAction());
			//addAction("big", new XHTMLTagAction());
			//addAction("small", new XHTMLTagAction());
			//addAction("u", new XHTMLTagAction());

			//addAction("table", new XHTMLTagAction());
			addAction("td", new XHTMLTagParagraphAction());
			addAction("th", new XHTMLTagParagraphAction());
			//addAction("tr", new XHTMLTagAction());
			//addAction("caption", new XHTMLTagAction());
			//addAction("span", new XHTMLTagAction());
		}
	}

	private final BookReader myModelReader;
	private String myPathPrefix;
	String myReferenceName;
	boolean myPreformatted;

	public XHTMLReader(BookReader modelReader) {
		myModelReader = modelReader;
	}

	BookReader getModelReader() {
		return myModelReader;
	}

	String getPathPrefix() {
		return myPathPrefix;
	}

	public boolean readFile(String pathPrefix, String fileName, String referenceName) {
		myModelReader.addHyperlinkLabel(referenceName);

		fillTagTable();

		myPathPrefix = pathPrefix;
		myReferenceName = referenceName;

		myPreformatted = false;

		return read(pathPrefix + fileName);
	}

	/*
	public boolean readFile(String pathPrefix, InputStream stream, String referenceName) {
		myModelReader.addHyperlinkLabel(referenceName);

		fillTagTable();

		myPathPrefix = pathPrefix;
		myReferenceName = referenceName;

		myPreformatted = false;

		return read(stream);
	}
	*/

	public boolean startElementHandler(String tag, ZLStringMap attributes) {
		String id = attributes.getValue("id");
		if (id != null) {
			myModelReader.addHyperlinkLabel(myReferenceName + '#' + id);
		}

		XHTMLTagAction action = (XHTMLTagAction)ourTagActions.get(tag.toLowerCase());
		if (action != null) {
			action.doAtStart(this, attributes);
		}
		return false;
	}

	public boolean endElementHandler(String tag) {
		XHTMLTagAction action = (XHTMLTagAction)ourTagActions.get(tag.toLowerCase());
		if (action != null) {
			action.doAtEnd(this);
		}
		return false;
	}

	public void characterDataHandler(char[] data, int start, int len) {
		if (myPreformatted) {
			final char first = data[start]; 
			if ((first == '\r') || (first == '\n')) {
				myModelReader.addControl(FBTextKind.CODE, false);
				myModelReader.endParagraph();
				myModelReader.beginParagraph();
				myModelReader.addControl(FBTextKind.CODE, true);
			}
			int spaceCounter = 0;
cycle:
			while (spaceCounter < len) {
				switch (data[start + spaceCounter]) {
					case 0x08:
					case 0x09:
					case 0x0A:
					case 0x0B:
					case 0x0C:
					case 0x0D:
					case ' ':
						break;
					default:
						break cycle;
				}
				++spaceCounter;
			}
			myModelReader.addFixedHSpace((short)spaceCounter);
			start += spaceCounter;
			len -= spaceCounter;
		}
		if (len > 0) {
			myModelReader.addData(data, start, len);
		}
	}

	private static ArrayList ourExternalDTDs = new ArrayList();

	public ArrayList externalDTDs() {
		if (ourExternalDTDs.isEmpty()) {
			ourExternalDTDs.add(ZLibrary.JAR_DATA_PREFIX + "data/formats/xhtml/xhtml-lat1.ent");
			ourExternalDTDs.add(ZLibrary.JAR_DATA_PREFIX + "data/formats/xhtml/xhtml-special.ent");
			ourExternalDTDs.add(ZLibrary.JAR_DATA_PREFIX + "data/formats/xhtml/xhtml-symbol.ent");
		}
		return ourExternalDTDs;
	}
}
