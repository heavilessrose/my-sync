package com.example.android.apis.view;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;

import com.example.android.apis.R;

import java.util.ArrayList;

/**
 * Demonstrates the using a list view in transcript mode
 * 
 */
public class List12 extends ListActivity implements OnClickListener,
		OnKeyListener {

	private EditText mUserText;

	private ArrayAdapter<String> mAdapter;

	private ArrayList<String> mStrings = new ArrayList<String>();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.list_12);

		mAdapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, mStrings);

		setListAdapter(mAdapter);

		mUserText = (EditText) findViewById(R.id.userText);

		mUserText.setOnClickListener(this);
		mUserText.setOnKeyListener(this);
	}

	public void onClick(View v) {
		sendText();
	}

	private void sendText() {
		String text = mUserText.getText().toString();
		mAdapter.add(text);
		mUserText.setText(null);
	}

	public boolean onKey(View v, int keyCode, KeyEvent event) {
		if (event.getAction() == KeyEvent.ACTION_DOWN) {
			switch (keyCode) {
			case KeyEvent.KEYCODE_DPAD_CENTER:
			case KeyEvent.KEYCODE_ENTER:
				sendText();
				return true;
			}
		}
		return false;
	}

}
