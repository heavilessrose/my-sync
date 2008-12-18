package luke.android.phone.call;

import luke.android.R;
import android.app.ListActivity;
import android.content.ContentUris;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.Contacts.People;
import android.view.View;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class PhoneCall extends ListActivity {

	private ListAdapter mAdapter;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Cursor c = getContentResolver().query(People.CONTENT_URI, null, null,
				null, null);
		startManagingCursor(c);

		String[] columns = new String[] { People.NAME }; // Comment
		int[] names = new int[] { R.id.row_entry };

		mAdapter = new SimpleCursorAdapter(this, R.layout.phone_call, c, columns,
				names);

		this.setListAdapter(mAdapter);
//		setContentView(R.layout.phone_call);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);

		Intent i = new Intent(Intent.ACTION_CALL);

		Cursor c = (Cursor) mAdapter.getItem(position);
		long phoneID = c.getLong(c.getColumnIndex(People.PRIMARY_PHONE_ID));

		i.setData(ContentUris.withAppendedId(
				android.provider.Contacts.Phones.CONTENT_URI, phoneID));

		this.startActivity(i);
	}
}