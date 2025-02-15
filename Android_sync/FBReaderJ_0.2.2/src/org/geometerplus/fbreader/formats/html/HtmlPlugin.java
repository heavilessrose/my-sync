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

package org.geometerplus.fbreader.formats.html;

import org.geometerplus.fbreader.bookmodel.BookModel;
import org.geometerplus.fbreader.description.BookDescription;
import org.geometerplus.fbreader.description.BookDescription.WritableBookDescription;
import org.geometerplus.fbreader.formats.FormatPlugin;
import org.geometerplus.fbreader.formats.fb2.FB2DescriptionReader;
import org.geometerplus.fbreader.formats.fb2.FB2Reader;
import org.geometerplus.zlibrary.core.filesystem.ZLFile;


public class HtmlPlugin extends FormatPlugin {
	private final static String AUTO = "auto";
	
	@Override
	public boolean acceptsFile(ZLFile file) {
		return "htm".equals(file.getExtension()) 
			|| "html".equals(file.getExtension());
	}

	@Override
	public String getIconName() {
		final String ICON_NAME = "html";
		return ICON_NAME;
	}

	@Override
	public boolean providesMetaInfo() {
		return true;
	}

	@Override
	public boolean readDescription(String path, BookDescription description) {
		return new HtmlDescriptionReader(description).readDescription(path);
        // always true =)
		//return true;
	}

	@Override
	public boolean readModel(BookDescription description, BookModel model) {
		// TODO Auto-generated method stub
		if (!description.getEncoding().equals(AUTO)) {
			new BookDescription.BookInfo(description.FileName).EncodingOption.setValue(AUTO);
		}
		return new HtmlReader(model).readBook(description.FileName);
	}
}
