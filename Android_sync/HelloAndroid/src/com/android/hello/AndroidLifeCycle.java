package com.android.hello;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

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

	}

	@Override
	public void onResume() {

	}

	// @Override
	// public void onFreeze() {
	//
	// }

	@Override
	public void onPause() {

	}

	@Override
	public void onStop() {

	}

	@Override
	public void onDestroy() {

	}
}