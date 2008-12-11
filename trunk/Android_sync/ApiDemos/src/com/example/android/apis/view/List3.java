package com.example.android.apis.view;

import android.app.ListActivity;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.Contacts.Phones;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;

/**
 * A list view example where the data comes from a cursor, and a
 * SimpleCursorListAdapter is used to map each item to a two-line display.
 */
public class List3 extends ListActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Get a cursor with all phones
		Cursor c = getContentResolver().query(Phones.CONTENT_URI, null, null,
				null, null);
		startManagingCursor(c);

		// Map Cursor columns to views defined in simple_list_item_2.xml
		ListAdapter adapter = new SimpleCursorAdapter(this,
				android.R.layout.simple_list_item_2, c, new String[] {
						Phones.NAME, Phones.NUMBER }, new int[] {
						android.R.id.text1, android.R.id.text2 });
		setListAdapter(adapter);
	}

}
