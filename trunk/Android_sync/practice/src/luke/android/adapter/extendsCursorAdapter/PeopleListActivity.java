package luke.android.adapter.extendsCursorAdapter;

import luke.android.R;
import android.app.ListActivity;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.Contacts.People;
import android.provider.Contacts.Phones;
import android.view.View;
import android.widget.ImageView;
import android.widget.SimpleCursorAdapter;

public class PeopleListActivity extends ListActivity {

	private static final String TAG = "PeopleListActivity";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Cursor c = getContentResolver().query(Phones.CONTENT_URI, null, null,
				null, null);
		startManagingCursor(c);
		// Map Cursor columns to views defined in simple_list_item_2.xml
		PeopleAdapter adapter = new PeopleAdapter(this,
				R.layout.cursor_image_list_item, c,
				new String[] { Phones.NAME }, new int[] { R.id.contacts });
		/*
		 * ListAdapter adapter = new SimpleCursorAdapter(this,
		 * 
		 * android.R.layout.simple_list_item_2, c, new String[] { Phones.NAME,
		 * Phones.NUMBER }, new int[] { android.R.id.text1, android.R.id.text2
		 * }); setListAdapter(adapter);
		 */
		setListAdapter(adapter);
	}

	private static class PeopleAdapter extends SimpleCursorAdapter {
		private static Cursor _c = null;
		private static Context context = null;

		public PeopleAdapter(Context context, int layout, Cursor c,
				String[] from, int[] to) {
			super(context, layout, c, from, to);
			this._c = c;
			this.context = context;
		}

		/*
		 * (non-Javadoc)
		 * 
		 * @see android.widget.SimpleCursorAdapter#bindView(android.view.View,
		 * android.content.Context, android.database.Cursor)
		 */
		@Override
		public void bindView(View view, Context context, Cursor cursor) {
			// TODO Auto-generated method stub
			ImageView imageView = (ImageView) view
					.findViewById(R.id.contact_image);

			// int id = this._c.getColumnIndex(Phones._ID);
			int id = this._c
					.getColumnIndex(android.provider.Contacts.Phones.PERSON_ID);
			Uri uri = ContentUris.withAppendedId(People.CONTENT_URI, this._c
					.getLong(id));

			Bitmap bitmap = People.loadContactPhoto(this.context, uri,
					R.drawable.icon, null);

			imageView.setImageBitmap(bitmap);

			super.bindView(view, context, cursor);
		}
	}
}
