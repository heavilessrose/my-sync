package com.android.demo.notepad3;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class NoteEdit extends Activity {

	private EditText mTitleText;
	private EditText mBodyText;
	private Long mRowId;

	private NotesDbAdapter mDbHelper;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mDbHelper = new NotesDbAdapter(this);
		mDbHelper.open();

		setContentView(R.layout.note_edit);

		mTitleText = (EditText) findViewById(R.id.title);
		mBodyText = (EditText) findViewById(R.id.body);

		Button confirmButton = (Button) findViewById(R.id.confirm);

		// mRowId = null;
		// Bundle extras = getIntent().getExtras();
		// if (extras != null) {
		// // String title = extras.getString(NotesDbAdapter.KEY_TITLE);
		// // String body = extras.getString(NotesDbAdapter.KEY_BODY);
		// mRowId = extras.getLong(NotesDbAdapter.KEY_ROWID);
		// // if (title != null) {
		// // mTitleText.setText(title);
		// // }
		// // if (body != null) {
		// // mBodyText.setText(body);
		// // }
		// }
		mRowId = savedInstanceState != null ? savedInstanceState
				.getLong(NotesDbAdapter.KEY_ROWID) : null;
		if (mRowId == null) {
			Bundle extras = getIntent().getExtras();
			mRowId = extras != null ? extras.getLong(NotesDbAdapter.KEY_ROWID)
					: null;
		}
		// populate the fields based on the mRowId if we have it
		populateFields();

		confirmButton.setOnClickListener(new View.OnClickListener() {

			public void onClick(View view) {
				// Bundle bundle = new Bundle();
				//
				// bundle.putString(NotesDbAdapter.KEY_TITLE,
				// mTitleText.getText()
				// .toString());
				// bundle.putString(NotesDbAdapter.KEY_BODY, mBodyText.getText()
				// .toString());
				// if (mRowId != null) {
				// bundle.putLong(NotesDbAdapter.KEY_ROWID, mRowId);
				// }
				//
				// Intent mIntent = new Intent();
				// mIntent.putExtras(bundle);
				// setResult(RESULT_OK, mIntent);
				// finish();
				setResult(RESULT_OK);
				finish();
			}

		});
	}

	private void populateFields() {
		if (mRowId != null) {
			// find the right note to edit
			Cursor note = mDbHelper.fetchNote(mRowId);
			startManagingCursor(note);
			mTitleText.setText(note.getString(note
					.getColumnIndexOrThrow(NotesDbAdapter.KEY_TITLE)));
			mBodyText.setText(note.getString(note
					.getColumnIndexOrThrow(NotesDbAdapter.KEY_BODY)));
		}
	}

	/* life-cycle method */
	/**
	 * called by Android if the Activity is being stopped and may be killed
	 * before it is resumed! This means it should store any state necessary to
	 * re-initialize to the same condition when the Activity is restarted. It is
	 * the counterpart to the onCreate() method, and in fact the
	 * savedInstanceState Bundle passed in to onCreate() is the same Bundle that
	 * you construct as outState in the onSaveInstanceState() method.
	 */
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		outState.putLong(NotesDbAdapter.KEY_ROWID, mRowId);
	}

	@Override
	protected void onPause() {
		super.onPause();
		saveState();
	}

	private void saveState() {
		String title = mTitleText.getText().toString();
		String body = mBodyText.getText().toString();

		if (mRowId == null) {
			long id = mDbHelper.createNote(title, body);
			if (id > 0) {
				mRowId = id;
			}
		} else {
			mDbHelper.updateNote(mRowId, title, body);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		populateFields();
	}
}
