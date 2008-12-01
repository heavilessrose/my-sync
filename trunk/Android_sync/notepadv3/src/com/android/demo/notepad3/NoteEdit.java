package com.android.demo.notepad3;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class NoteEdit extends Activity {

	private static final String TAG = "NoteEdit";
	
	private EditText _titleText;
	private EditText _bodyText;
	private Long _rowId;

	private NotesDbAdapter _dbAdapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		_dbAdapter = new NotesDbAdapter(this);
		_dbAdapter.open();

		setContentView(R.layout.note_edit);

		_titleText = (EditText) findViewById(R.id.titleView);
		_bodyText = (EditText) findViewById(R.id.bodyView);
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
		_rowId = savedInstanceState != null ? savedInstanceState
				.getLong(NotesDbAdapter.KEY_ROWID) : null;
		if (_rowId == null) {
			Bundle extras = getIntent().getExtras();
			_rowId = extras != null ? extras.getLong(NotesDbAdapter.KEY_ROWID)
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
		if (_rowId != null) {
			// find the right note to edit
			Cursor note = _dbAdapter.fetchNote(_rowId);
			startManagingCursor(note);
			_titleText.setText(note.getString(note
					.getColumnIndexOrThrow(NotesDbAdapter.KEY_TITLE)));
			_bodyText.setText(note.getString(note
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
		Log.e(TAG, "_rowId: " + _rowId);
		outState.putLong(NotesDbAdapter.KEY_ROWID, _rowId);
	}

	/**
	 * 当系统在准备resume 另一个之前的activity时执行. This is typically used to commit unsaved
	 * changes to persistent data, stop animations and other things that may be
	 * consuming CPU, etc. Implementations of this method must be very quick
	 * because the next activity will not be resumed until this method returns.
	 */
	@Override
	protected void onPause() {
		super.onPause();
		saveState();
		// 数据库如何关闭以及resume之后如何回复状态??
//		_dbAdapter.close();
	}

	/** 保存当前没有保存的Note */
	private void saveState() {
		String title = _titleText.getText().toString();
		String body = _bodyText.getText().toString();

		if (_rowId == null) {
			long id = _dbAdapter.createNote(title, body);
			if (id > 0) {
				_rowId = id;
			}
		} else {
			_dbAdapter.updateNote(_rowId, title, body);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		populateFields();
	}
}
