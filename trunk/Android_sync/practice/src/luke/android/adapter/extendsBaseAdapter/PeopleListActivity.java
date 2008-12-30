package luke.android.adapter.extendsBaseAdapter;

import java.util.ArrayList;

import luke.android.R;
import android.app.ListActivity;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.Contacts.People;
import android.provider.Contacts.Phones;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class PeopleListActivity extends ListActivity {

	private static final String TAG = "PeopleListActivity";
	private static Cursor _c = null;
	private static ArrayList<ContactItem> _contacts = new ArrayList<ContactItem>();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.main);
		
		_c = getContentResolver().query(Phones.CONTENT_URI, null, null, null,
				null);
		startManagingCursor(_c);
		int pplId = _c.getColumnIndex(People._ID);
		if (_c.moveToFirst()) {
			long id = 0;
			do {
				id = _c.getLong(pplId);
				Log.d(TAG, "pplId: " + pplId);
				Log.d(TAG, "id: " + id);
				Bitmap bitmap = People.loadContactPhoto(this, ContentUris
						.withAppendedId(People.CONTENT_URI, id),
						R.drawable.default_image, null);
				ContactItem temp = new ContactItem(_c.getPosition(), _c
						.getString(_c.getColumnIndex(Phones.NAME)), _c
						.getString(_c.getColumnIndex(Phones.NUMBER)), bitmap,
						"online");
				_contacts.add(temp);
			} while (_c.moveToNext());
			// debug
			Log.d(TAG, "contacts ------------------------ ");
			for (ContactItem pp : _contacts) {
				Log
						.w(TAG, "" + pp.position + ", " + pp.name + ", "
								+ pp.number);
			}

			setListAdapter(new PeopleAdapter(this));
		}
	}

	private static class PeopleAdapter extends BaseAdapter {
		private LayoutInflater _inflater;

		public PeopleAdapter(Context context) {
			_inflater = LayoutInflater.from(context);
		}

		public int getCount() {
			return _c.getCount();
		}

		public Object getItem(int position) {
			return _contacts.get(position);
		}

		public long getItemId(int position) {
			return _contacts.get(position).position;
		}

		public View getView(int position, View convertView, ViewGroup parent) {
			ViewHolder holder = null;
			if (convertView == null) {
				convertView = _inflater.inflate(R.layout.image_list_item, null);

				holder = new ViewHolder();
				holder.name = (TextView) convertView.findViewById(R.id.name);
				holder.number = (TextView) convertView
						.findViewById(R.id.number);
				holder.icon = (ImageView) convertView.findViewById(R.id.icon);
				holder.presence = (TextView) convertView
						.findViewById(R.id.presence);

				convertView.setTag(holder);
			} else {
				holder = (ViewHolder) convertView.getTag();
			}

			holder.name.setText(_contacts.get(position).name);
			holder.number.setText(_contacts.get(position).number);
			holder.icon.setImageBitmap(_contacts.get(position).icon);
			holder.presence.setText(_contacts.get(position).presence);
			return convertView;
		}

		static class ViewHolder {
			TextView name;
			TextView number;
			ImageView icon;
			TextView presence;
		}
	}

	private static class ContactItem {
		int position = 0;
		String name = null;
		String number = null;
		Bitmap icon = null;
		String presence = null;

		ContactItem(int position, String name, String number, Bitmap icon,
				String presence) {
			this.position = position;
			this.name = name;
			this.number = number;
			this.icon = icon;
			this.presence = presence;
		}

		// @Override
		public int compareTo(ContactItem other) {
			if (this.name != null)
				return this.name.compareTo(other.name);
			else
				throw new IllegalArgumentException();
		}
	}
}
