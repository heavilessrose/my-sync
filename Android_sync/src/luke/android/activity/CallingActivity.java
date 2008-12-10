package luke.android.activity;

import luke.android.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class CallingActivity extends Activity {
	private static final int REQUEST_CODE = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.callingactivity);
		Intent subIntent = new Intent();
		subIntent.setClassName("luke.android",
				"luke.android.activity.SubActivity");
		// key/value pair, where key needs current package prefix.
		subIntent.putExtra("luke.android.activity.CallingActivity",
				"Hello, Luke!");
		Log.i("practice", "start sub");
		// startActivity(subIntent);
		startActivityForResult(subIntent, REQUEST_CODE);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		Log.i("practice", "got sub return");
		CharSequence tip = data.getAction();
		if (resultCode == 1) {
			Toast.makeText(this, tip, 5000).show();
		}
	}
}
