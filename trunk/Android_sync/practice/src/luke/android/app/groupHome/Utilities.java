package luke.android.app.groupHome;

import android.graphics.drawable.Drawable;
import android.graphics.drawable.PaintDrawable;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.Canvas;
import android.graphics.PaintFlagsDrawFilter;
import android.graphics.Paint;
import android.graphics.Rect;
import android.content.res.Resources;
import android.content.Context;

public class Utilities {

	public static Drawable createIconThumbnail(Drawable icon, int size) {
		// code adapted from packages/apps/Launcher

		int sourceWidth = icon.getIntrinsicWidth(), sourceHeight = icon
				.getIntrinsicHeight();

		int destWidth = size, destHeight = size;

		// only resize if actually needed
		if (sourceWidth != destWidth || sourceHeight != destHeight) {
			float ratio = (float) sourceWidth / sourceHeight;
			if (sourceWidth > sourceHeight) {
				destHeight = (int) (destWidth / ratio);
			} else if (sourceHeight > sourceWidth) {
				destWidth = (int) (destHeight * ratio);
			}

			final Bitmap thumb = Bitmap.createBitmap(size, size,
					Bitmap.Config.ARGB_8888);
			final Canvas canvas = new Canvas(thumb);

			icon.setBounds((size - destWidth) / 2, (size - destHeight) / 2,
					destWidth, destHeight);
			icon.draw(canvas);
			icon = new BitmapDrawable(thumb);

		}

		return icon;
	}
}
