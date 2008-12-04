package com.android.hello;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

//@SuppressWarnings("unused")
public class AndroidLifeCycle extends Activity {
	private static final String TAG = "AndroidLifeCycle";

	// ***************************************************
	// ÉúÃüÖÜÆÚ
	// ***************************************************
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// TextView tv = new TextView(this);
		// tv.setText("Hello, Android");
		// setContentView(tv);
	}

	// Called after onStart() when the current activity is being re-displayed to
	// the user (the user has navigated back to it).
	@Override
	protected void onRestart() {
		Log.e(TAG, "onRestart");
		super.onRestart();
	}

	// Called after onCreate(Bundle) or onStop() when the current activity is
	// now being displayed to the user.
	@Override
	protected void onStart() {
		Log.e(TAG, "onStart");
		super.onStart();
	}

	// Called after onRestoreInstanceState(Bundle), onRestart(), or onPause(),
	// for your activity to start interacting with the user.
	@Override
	protected void onResume() {
		super.onResume();
		Log.e(TAG, "onResume");
	}

	// Called as part of the activity lifecycle when an activity is going into
	// the background, but has not (yet) been killed.
	@Override
	protected void onPause() {
		super.onPause();
		Log.e(TAG, "onPause");
	}

	// Called when you are no longer visible to the user.
	@Override
	protected void onStop() {
		super.onStop();
		Log.e(TAG, "onStop");
	}

	// Perform any final cleanup before an activity is destroyed.
	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.e(TAG, "onDestroy");
	}

	// ***************************************************
	// 
	// ***************************************************
	// Called by the virtual machine when there are no longer any (non-weak)
	// references to the receiver.
	protected void finalize() {

	}

	// Called when an activity you launched exits, giving you the requestCode
	// you started it with, the resultCode it returned, and any additional data
	// from it.
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

	}

	// Called when activity start-up is complete (after onStart() and
	// onRestoreInstanceState(Bundle) have been called).
	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		Log.e(TAG, "onPostCreate");
	}

	// Called when activity resume is complete (after onResume() has been
	// called).
	@Override
	protected void onPostResume() {
		super.onPostResume();
		Log.e(TAG, "onPostResume");
	}

	// This method is called after onStart() when the activity is being
	// re-initialized from a previously saved state, given here in state.
	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		Log.e(TAG, "onRestoreInstanceState");
	}

	// Called to retrieve per-instance state from an activity before being
	// killed so that the state can be restored in onCreate(Bundle) or
	// onRestoreInstanceState(Bundle) (the Bundle populated by this method will
	// be passed to both).
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		Log.e(TAG, "onSaveInstanceState");
	}

	// ***************************************************
	// 
	// ***************************************************
	// This is called for activities that set launchMode to "singleTop" in their
	// package, or if a client used the FLAG_ACTIVITY_SINGLE_TOP flag when
	// calling startActivity(Intent).
	@Override
	protected void onNewIntent(Intent intent) {
		Log.e(TAG, "onNewIntent");
	}

	@Override
	protected void onTitleChanged(CharSequence title, int color) {
		Log.e(TAG, "onTitleChanged");
	}

	// Called by setTheme(int) and getTheme() to apply a theme resource to the
	// current Theme object.
	@Override
	protected void onApplyThemeResource(Resources.Theme theme, int resid,
			boolean first) {
		Log.e(TAG, "onApplyThemeResource");
	}

	@Override
	protected void onChildTitleChanged(Activity childActivity,
			CharSequence title) {
		Log.e(TAG, "onChildTitleChanged");
	}

	// Callback for creating dialogs that are managed (saved and restored) for
	// you by the activity.
	@Override
	protected Dialog onCreateDialog(int id) {
		Log.e(TAG, "onCreateDialog");
		Dialog dialog = new Dialog(this);

		return dialog;
	}

	// Provides an opportunity to prepare a managed dialog before it is being
	// shown.
	@Override
	protected void onPrepareDialog(int id, Dialog dialog) {
		Log.e(TAG, "onPrepareDialog");
	}
}