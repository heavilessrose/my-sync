package com.example.android.apis.view;

import com.example.android.apis.R;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.Contacts;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AutoCompleteTextView;
import android.widget.CursorAdapter;
import android.widget.Filterable;
import android.widget.TextView;

public class AutoComplete4 extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.autocomplete_4);

		ContentResolver content = getContentResolver();
		Cursor cursor = content.query(Contacts.People.CONTENT_URI,
				PEOPLE_PROJECTION, null, null,
				Contacts.People.DEFAULT_SORT_ORDER);
		ContactListAdapter adapter = new ContactListAdapter(this, cursor);

		AutoCompleteTextView textView = (AutoCompleteTextView) findViewById(R.id.edit);
		textView.setAdapter(adapter);
	}

	// XXX compiler bug in javac 1.5.0_07-164, we need to implement Filterable
	// to make compilation work
	public static class ContactListAdapter extends CursorAdapter implements
			Filterable {
		public ContactListAdapter(Context context, Cursor c) {
			super(context, c);
			mContent = context.getContentResolver();
		}

		@Override
		public View newView(Context context, Cursor cursor, ViewGroup parent) {
			final LayoutInflater inflater = LayoutInflater.from(context);
			final TextView view = (TextView) inflater.inflate(
					android.R.layout.simple_dropdown_item_1line, parent, false);
			view.setText(cursor.getString(5));
			return view;
		}

		@Override
		public void bindView(View view, Context context, Cursor cursor) {
			((TextView) view).setText(cursor.getString(5));
		}

		@Override
		public String convertToString(Cursor cursor) {
			return cursor.getString(5);
		}

		@Override
		public Cursor runQueryOnBackgroundThread(CharSequence constraint) {
			if (getFilterQueryProvider() != null) {
				return getFilterQueryProvider().runQuery(constraint);
			}

			StringBuilder buffer = null;
			String[] args = null;
			if (constraint != null) {
				buffer = new StringBuilder();
				buffer.append("UPPER(");
				buffer.append(Contacts.ContactMethods.NAME);
				buffer.append(") GLOB ?");
				args = new String[] { constraint.toString().toUpperCase() + "*" };
			}

			return mContent.query(Contacts.People.CONTENT_URI,
					PEOPLE_PROJECTION, buffer == null ? null : buffer
							.toString(), args,
					Contacts.People.DEFAULT_SORT_ORDER);
		}

		private ContentResolver mContent;
	}

	private static final String[] PEOPLE_PROJECTION = new String[] {
			Contacts.People._ID, Contacts.People.PRIMARY_PHONE_ID,
			Contacts.People.TYPE, Contacts.People.NUMBER,
			Contacts.People.LABEL, Contacts.People.NAME, };
}
