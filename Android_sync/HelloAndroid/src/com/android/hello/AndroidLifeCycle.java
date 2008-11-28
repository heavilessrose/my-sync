package com.android.hello;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

//@SuppressWarnings("unused")
public class AndroidLifeCycle extends Activity {
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// TextView tv = new TextView(this);
		// tv.setText("Hello, Android");
		// setContentView(tv);
	}

	@Override
	public void onStart() {
		super.onStart();
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	// @Override
	// public void onFreeze() {
	//
	// }

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onStop() {
		super.onStop();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
	}
}