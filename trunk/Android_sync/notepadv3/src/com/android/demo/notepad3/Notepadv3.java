package com.android.demo.notepad3;

import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class Notepadv3 extends ListActivity {
	private static final int ACTIVITY_CREATE = 0;
	private static final int ACTIVITY_EDIT = 1;

	private static final int INSERT_ID = Menu.FIRST;
	private static final int DELETE_ID = Menu.FIRST + 1;

	private NotesDbAdapter mDbHelper;

	// private Cursor mNotesCursor;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.notes_list);
		mDbHelper = new NotesDbAdapter(this);
		mDbHelper.open();
		fillData();
	}

	private void fillData() {
		// Get all of the rows from the database and create the item list
		// mNotesCursor = mDbHelper.fetchAllNotes();
		Cursor notesCursor = mDbHelper.fetchAllNotes();
		startManagingCursor(/* mNotesCursor */notesCursor);

		// Create an array to specify the fields we want to display in the list
		// (only TITLE)
		String[] from = new String[] { NotesDbAdapter.KEY_TITLE };

		// and an array of the fields we want to bind those fields to (in this
		// case just text1)
		int[] to = new int[] { R.id.text1 };

		// Now create a simple cursor adapter and set it to display
		SimpleCursorAdapter notes = new SimpleCursorAdapter(this,
				R.layout.notes_row, /* mNotesCursor */notesCursor, from, to);
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
			mDbHelper.deleteNote(getListView().getSelectedItemId());
			fillData();
			return true;
		}

		return super.onMenuItemSelected(featureId, item);
	}

	private void createNote() {
		Intent i = new Intent(this, NoteEdit.class);
		startActivityForResult(i, ACTIVITY_CREATE);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);
		// Cursor c = mNotesCursor;
		// c.moveToPosition(position);
		Intent i = new Intent(this, NoteEdit.class);
		i.putExtra(NotesDbAdapter.KEY_ROWID, id);
		// i.putExtra(NotesDbAdapter.KEY_TITLE, c.getString(c
		// .getColumnIndexOrThrow(NotesDbAdapter.KEY_TITLE)));
		// i.putExtra(NotesDbAdapter.KEY_BODY, c.getString(c
		// .getColumnIndexOrThrow(NotesDbAdapter.KEY_BODY)));
		startActivityForResult(i, ACTIVITY_EDIT);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode,
			Intent intent) {
		// super.onActivityResult(requestCode, resultCode, intent);
		// Bundle extras = intent.getExtras();
		// switch(requestCode) {
		// case ACTIVITY_CREATE:
		// String title = extras.getString(NotesDbAdapter.KEY_TITLE);
		// String body = extras.getString(NotesDbAdapter.KEY_BODY);
		// mDbHelper.createNote(title, body);
		// fillData();
		// break;
		// case ACTIVITY_EDIT:
		// Long rowId = extras.getLong(NotesDbAdapter.KEY_ROWID);
		// if (rowId != null) {
		// String editTitle = extras.getString(NotesDbAdapter.KEY_TITLE);
		// String editBody = extras.getString(NotesDbAdapter.KEY_BODY);
		// mDbHelper.updateNote(rowId, editTitle, editBody);
		// }
		// fillData();
		// break;
		// }
		super.onActivityResult(requestCode, resultCode, intent);
		fillData();
	}
}
