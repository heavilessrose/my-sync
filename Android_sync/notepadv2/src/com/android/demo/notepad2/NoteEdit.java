package com.android.demo.notepad2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class NoteEdit extends Activity {

	private Long mRowId;
	private EditText mTitleText;
	private EditText mBodyText;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// setup the layout
		setContentView(R.layout.note_edit);

		// Find the edit and button components we need
		mTitleText = (EditText) findViewById(R.id.title);
		mBodyText = (EditText) findViewById(R.id.body);
		Button confirmButton = (Button) findViewById(R.id.confirm);

		// initialize the title, body and mRowId from the extras Bundle in the
		// Intent.
		// pull the title and body out of the extras Bundle that was set from
		// the Intent invocation.
		mRowId = null;
		Bundle extras = getIntent().getExtras();
		if (extras != null) {
			String title = extras.getString(NotesDbAdapter.KEY_TITLE);
			String body = extras.getString(NotesDbAdapter.KEY_BODY);
			mRowId = extras.getLong(NotesDbAdapter.KEY_ROWID);

			if (title != null) {
				mTitleText.setText(title);
			}
			if (body != null) {
				mBodyText.setText(body);
			}
		}

		/** Create an onClickListener() for the button */
		confirmButton.setOnClickListener(new View.OnClickListener() {

			// This is the code that will be run when the user clicks on the
			// confirm button.
			public void onClick(View view) {
				// grab the title and body text from the edit text fields, and
				// put it into the return bundle so that they can be passed back
				// to the Activity that invoked this NoteEdit Activity.
				Bundle bundle = new Bundle();
				bundle.putString(NotesDbAdapter.KEY_TITLE, mTitleText.getText()
						.toString());
				bundle.putString(NotesDbAdapter.KEY_BODY, mBodyText.getText()
						.toString());
				// If the operation is an edit rather than a create, we also
				// want to put the mRowId into the Bundle so that the Notepadv2
				// class can save the changes back to the correct note.
				if (mRowId != null) {
					bundle.putLong(NotesDbAdapter.KEY_ROWID, mRowId);
				}

				/*
				 * Set the result information (the Bundle) in a new Intent and
				 * finish the Activity.
				 */
				// The Intent is simply our data carrier that carries our Bundle
				// (with the title, body and mRowId).
				Intent mIntent = new Intent();
				mIntent.putExtras(bundle);
				// used to set the result code and return Intent to be passed
				// back to the Intent caller.
				setResult(RESULT_OK, mIntent);
				// signal that the Activity is done (like a return call).
				// Anything set in the Result will then be returned to the
				// caller, along with execution control.
				finish();
			}
		});
	}
}
