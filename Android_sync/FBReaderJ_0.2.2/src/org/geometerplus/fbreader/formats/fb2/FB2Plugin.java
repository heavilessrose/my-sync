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

package org.geometerplus.fbreader.formats.fb2;

import org.geometerplus.fbreader.bookmodel.BookModel;
import org.geometerplus.fbreader.description.BookDescription;
import org.geometerplus.fbreader.formats.FormatPlugin;
import org.geometerplus.zlibrary.core.filesystem.ZLFile;

public class FB2Plugin extends FormatPlugin {
	public boolean providesMetaInfo() {
		 return true;
	}
	
	public boolean acceptsFile(ZLFile file) {
		return "fb2".equals(file.getExtension());
	}
	
	public boolean readDescription(String path, BookDescription description) {
		return new FB2DescriptionReader(description).readDescription(path);
	}
	
	public boolean readModel(BookDescription description, BookModel model) {
		return new FB2Reader(model).readBook(description.FileName);
	}
	
	public String getIconName() {
		return "fb2";
	}
}
