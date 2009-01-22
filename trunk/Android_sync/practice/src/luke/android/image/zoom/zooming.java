package luke.android.image.zoom;

import android.app.Activity;
import android.os.Bundle;

public class zooming extends Activity {
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(new Zoom(this));
	}
}