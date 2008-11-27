package com.android.demo.notepad2;

import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class Notepadv2 extends ListActivity {
	private static final int ACTIVITY_CREATE = 0;
	private static final int ACTIVITY_EDIT = 1;

	private static final int INSERT_ID = Menu.FIRST;
	private static final int DELETE_ID = Menu.FIRST + 1;

	private NotesDbAdapter mDbHelper;
	private Cursor mNotesCursor;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.notes_list);
		mDbHelper = new NotesDbAdapter(this);
		mDbHelper.open();
		fillData();
	}

	/** 从数据库拿到数据,更新View */
	private void fillData() {
		// Get all of the rows from the database and create the item list
		mNotesCursor = mDbHelper.fetchAllNotes();
		startManagingCursor(mNotesCursor);

		// Create an array to specify the fields we want to display in the list
		// (only TITLE)
		String[] fromColumns = new String[] { NotesDbAdapter.KEY_TITLE };

		// and an array of the fields we want to bind those fields to (in this
		// case just text1)
		int[] toView = new int[] { R.id.text1 };

		/* Now create a simple cursor adapter and set it to display */
		SimpleCursorAdapter notes = new SimpleCursorAdapter(this,
				R.layout.notes_row, mNotesCursor, fromColumns, toView);
		// Provide the cursor for the list view. 
		setListAdapter(notes);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		menu.add(0, INSERT_ID, 0, R.string.menu_insert);
		menu.add(0, DELETE_ID, 0, R.string.menu_delete);
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case INSERT_ID:
			createNote();
			return true;
		case DELETE_ID:
			boolean result = mDbHelper.deleteNote(getListView().getSelectedItemId());
			Log.i("Notepadv2", "delete return: " + result);
			fillData();
			return true;
		}

		return super.onMenuItemSelected(featureId, item);
	}

	/**
	 * Create a new Intent to create a note using the NoteEdit class. (using an
	 * asynchronous Intent invocation, so We need a handler for the
	 * callback--onActivityResult())
	 */
	private void createNote() {
		/*
		 * This form of the Intent call targets a specific class in our
		 * Activity, in this case NoteEdit. Since the Intent class will need to
		 * communicate with the Android operating system to route requests, we
		 * also have to provide a Context, in this case is "this"
		 */
		// Create a new Intent to create a note using the NoteEdit class
		Intent i = new Intent(this, NoteEdit.class);
		// fire the Intent. 运行结束时返回结果, 并回调onActivityResult()方法
		startActivityForResult(i, ACTIVITY_CREATE);
	}

	/**
	 * a callback method. It is called when the user selects an item from the
	 * list. Get the data from the correct row, and bundle it up to send to the
	 * NoteEdit Activity(using an asynchronous Intent invocation, so We need a
	 * handler for the callback--onActivityResult())
	 */
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);

		// The details of the note are pulled out from our query Cursor, which
		// we move to the proper position for the element that was selected in
		// the list, with the moveToPosition() method.
		Cursor c = mNotesCursor;
		c.moveToPosition(position);

		// an Intent to edit the note using the NoteEdit class
		Intent i = new Intent(this, NoteEdit.class);

		// adds data into the extras Bundle of the Intent, which will be passed
		// to the called Activity.
		// putExtra() is the method to add items into the extras Bundle to pass
		// in to intent invocations.
		i.putExtra(NotesDbAdapter.KEY_ROWID, id);
		i.putExtra(NotesDbAdapter.KEY_TITLE, c.getString(c
				.getColumnIndexOrThrow(NotesDbAdapter.KEY_TITLE)));
		i.putExtra(NotesDbAdapter.KEY_BODY, c.getString(c
				.getColumnIndexOrThrow(NotesDbAdapter.KEY_BODY)));

		// fire the Intent.
		// With the extras added to the Intent, we invoke the Intent on the
		// NoteEdit class by passing startActivityForResult() the Intent and the
		// request code. (The request code will be returned to onActivityResult
		// as the requestCode parameter.)
		startActivityForResult(i, ACTIVITY_EDIT);
	}

	/**
	 * The combination of startActivityForResult() and onActivityResult() can be
	 * thought of as an asynchronous RPC (remote procedure call) and forms the
	 * recommended way for an Activity to invoke another and share services.
	 * 
	 * Will be called when an Activity returns with a result.(an Activity will
	 * only return a result if launched with startActivityForResult())
	 * 
	 * @param requestCode
	 *            the original request code specified in the Intent invocation
	 *            (either ACTIVITY_CREATE or ACTIVITY_EDIT for us).
	 * 
	 * @param resultCode
	 *            the result (or error code) of the call, this should be zero if
	 *            everything was OK, but may have a non-zero code indicating
	 *            that something failed.There are standard result codes
	 *            available, and you can also create your own constants to
	 *            indicate specific problems.
	 * @param intent
	 *            this is an Intent created by the Activity returning results.
	 *            It can be used to return data in the Intent "extras."
	 */
	@Override
	protected void onActivityResult(int requestCode, int resultCode,
			Intent intent) {
		super.onActivityResult(requestCode, resultCode, intent);

		Bundle extras = intent.getExtras();
		// handling both the ACTIVITY_CREATE and ACTIVITY_EDIT activity results
		switch (requestCode) {
		case ACTIVITY_CREATE:
			// pull the title and body from the extras (retrieved from the
			// returned Intent) and use them to create a new note.
			String title = extras.getString(NotesDbAdapter.KEY_TITLE);
			String body = extras.getString(NotesDbAdapter.KEY_BODY);
			mDbHelper.createNote(title, body);
			// ensures everything is up to date
			fillData();
			break;
		case ACTIVITY_EDIT:
			// pull the mRowId as well, and use that to update the note in the
			// database
			Long mRowId = extras.getLong(NotesDbAdapter.KEY_ROWID);
			if (mRowId != null) {
				String editTitle = extras.getString(NotesDbAdapter.KEY_TITLE);
				String editBody = extras.getString(NotesDbAdapter.KEY_BODY);
				mDbHelper.updateNote(mRowId, editTitle, editBody);
			}
			// ensures everything is up to date
			fillData();
			break;
		}
	}

}
