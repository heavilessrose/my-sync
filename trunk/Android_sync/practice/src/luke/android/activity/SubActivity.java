package luke.android.activity;

import luke.android.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class SubActivity extends Activity {
	private static int RESULT_CODE = 1;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.subactivity);
		CharSequence tip = "sub activity fired";
		Toast.makeText(this, tip, 5000).show();
		Log.i("practice", "sub toast show");

		// 设置返回的数据
		Intent requestIntent = getIntent();
//		if (requestIntent.getExtras().get(
//				"luke.android.activity.CallingActivity") == "Hello, Luke!")
			setResult(RESULT_CODE, new Intent("sub activity action return"));

		Log.i("practice", "sub return");
		finish();
	}

}
