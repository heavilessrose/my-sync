package luke.android.phone.callLog;

import java.util.ArrayList;

import luke.android.R;
import luke.android.app.fileBrowser.iconifiedlist.IconifiedText;
import luke.android.app.fileBrowser.iconifiedlist.IconifiedTextListAdapter;

import android.app.ListActivity;
import android.database.Cursor;
import android.graphics.drawable.Drawable;
import android.os.Bundle;

public class CallLogDisplayer extends ListActivity {
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		// Querying for a cursor is like querying for any SQL-Database
		Cursor c = getContentResolver().query(
				android.provider.CallLog.Calls.CONTENT_URI, null, null, null,
				android.provider.CallLog.Calls.DATE + " DESC");
		startManagingCursor(c);

		// Retrieve the column-indixes of phoneNumber, date and calltype
		int numberColumn = c
				.getColumnIndex(android.provider.CallLog.Calls.NUMBER);
		int dateColumn = c.getColumnIndex(android.provider.CallLog.Calls.DATE);
		// type can be: Incoming, Outgoing or Missed
		int typeColumn = c.getColumnIndex(android.provider.CallLog.Calls.TYPE);

		// Will hold the calls, available to the cursor
		ArrayList<IconifiedText> callList = new ArrayList<IconifiedText>();

		// Loop through all entries the cursor provides to us.
		if (c.isFirst()) {
			do {
				String callerPhoneNumber = c.getString(numberColumn);
				int callDate = c.getInt(dateColumn);
				int callType = c.getInt(typeColumn);

				Drawable currentIcon = null;
				switch (callType) {
				case android.provider.CallLog.Calls.INCOMING_TYPE:
					currentIcon = getResources().getDrawable(
							R.drawable.calllog_in);
					break;
				case android.provider.CallLog.Calls.MISSED_TYPE:
					currentIcon = getResources().getDrawable(
							R.drawable.calllog_missed);
					break;
				case android.provider.CallLog.Calls.OUTGOING_TYPE:
					currentIcon = getResources().getDrawable(
							R.drawable.calllog_out);
					break;
				}
				// Convert the unix-timestamp to a readable datestring
				// String dateString =
				// DateUtils.dateString(callDate).toString();

				callList.add(new IconifiedText("@ " + /* dateString */callDate
						+ "  | # " + callerPhoneNumber, currentIcon));
			} while (c.isLast());
		}

		// Create an ListAdapter that manages to display out 'callList'
		IconifiedTextListAdapter itla = new IconifiedTextListAdapter(this);
		itla.setListItems(callList);
		this.setListAdapter(itla);
		// Done =)
	}
}