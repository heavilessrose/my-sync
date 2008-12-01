package com.android.demo.notepad1;

import android.app.ListActivity;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.SimpleCursorAdapter;

public class Notepadv1 extends ListActivity {
	public static final int INSERT_ID = Menu.FIRST;

	private int mNoteNumber = 1;
	private NotesDbAdapter mDbHelper;

	private static final String TAG = "Notepadv1";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.notepad_list);
		// Activity实现Context抽象类
		mDbHelper = new NotesDbAdapter(this);
		mDbHelper.open();
		fillData();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		Log.w(TAG, "onCreateOptionsMenu");
		boolean result = super.onCreateOptionsMenu(menu);
		menu.add(0, INSERT_ID, 0, R.string.menu_insert);
		return result;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.w(TAG, "onOptionsItemSelected");
		switch (item.getItemId()) {
		case INSERT_ID:
			createNote();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	private void createNote() {
		Log.w(TAG, "createNote");
		String noteName = "Note " + mNoteNumber++;
		mDbHelper.addNote(noteName, "");
		fillData();
	}

	/** 填充list */
	private void fillData() {
		Log.w(TAG, "fillData");
		// Get all of the notes from the database and create the item list
		Cursor c = mDbHelper.fetchAllNotes();
		startManagingCursor(c);

		String[] from = new String[] { NotesDbAdapter.KEY_TITLE };
		int[] to = new int[] { R.id.text1 };

		// Now create an array adapter and set it to display using our row
		SimpleCursorAdapter notes = new SimpleCursorAdapter(this,
				R.layout.notes_row, c, from, to);
		setListAdapter(notes);
	}
}
