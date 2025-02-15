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

package org.geometerplus.zlibrary.ui.android.image;

import android.graphics.BitmapFactory;

import org.geometerplus.zlibrary.core.image.*;

public final class ZLAndroidImageManager extends ZLImageManager {
	public ZLAndroidImageData getImageData(ZLImage image) {
		if (image instanceof ZLSingleImage) {
			ZLSingleImage singleImage = (ZLSingleImage)image;
			if ("image/palm".equals(singleImage.mimeType())) {
				return null;
			}
			byte[] array = singleImage.byteData();
			return new ZLAndroidImageData(BitmapFactory.decodeByteArray(array, 0, array.length));
		} else {
			//TODO
			return null;
		}
	}
}
