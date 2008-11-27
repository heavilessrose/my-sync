package com.android.demo.notepad1;

import android.app.ListActivity;
import android.database.Cursor;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.SimpleCursorAdapter;

public class Notepadv1 extends ListActivity {
	private int mNoteNumber = 1;

	private NotesDbAdapter mDbHelper;

	public static final int INSERT_ID = Menu.FIRST;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.notepad_list);
		mDbHelper = new NotesDbAdapter(this);
		// 打开数据库
		mDbHelper.open();
		fillData();
	}

	/** 从数据库得到数据, 并映射到View */
	private void fillData() {

		// Get all of the notes from the database
		Cursor cursor = mDbHelper.fetchAllNotes();
		// let Android to take care of the Cursor lifecycle
		startManagingCursor(cursor);

		/* create the item list */
		// declare columns we want
		String[] fromColumns = new String[] { NotesDbAdapter.KEY_TITLE };
		// defines the View(s) to which we'd like to bind the columns
		int[] toView = new int[] { R.id.text1 };

		// 从Cursor中映射columns到View(在xml文件中定义)
		SimpleCursorAdapter notes = new SimpleCursorAdapter(this,
				R.layout.notes_row, cursor, fromColumns, toView);
		setListAdapter(notes);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		boolean result = super.onCreateOptionsMenu(menu);
		menu.add(0, INSERT_ID, 0, R.string.menu_insert);
		return result;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case INSERT_ID:
			createNote();
			return true;
		}

		return super.onOptionsItemSelected(item);
	}

	private void createNote() {
		String noteName = "Note " + mNoteNumber++;
		mDbHelper.createNote(noteName, "");
		fillData();
	}
}
