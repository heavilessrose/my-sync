package luke.android.image.zoom;

import luke.android.R;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.view.KeyEvent;
import android.view.View;

public class Zoom extends View {
	private Drawable image;
	private int zoomControler = 20;

	public Zoom(Context context) {
		super(context);
		image = context.getResources().getDrawable(R.drawable.icon);
		setFocusable(true);

	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		// here u can control the width and height of the images........ this
		// line is very important
		image.setBounds((getWidth() / 2) - zoomControler, (getHeight() / 2)
				- zoomControler, (getWidth() / 2) + zoomControler,
				(getHeight() / 2) + zoomControler);
		image.draw(canvas);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		if (keyCode == KeyEvent.KEYCODE_DPAD_UP)// zoom in
			zoomControler += 10;
		if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) // zoom out
			zoomControler -= 10;
		if (zoomControler < 10)
			zoomControler = 10;

		invalidate();
		return true;
	}
}