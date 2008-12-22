package luke.android.dbAdapter;

import android.content.Context;
import android.database.Cursor;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CursorAdapter;

public class DBAdapter extends CursorAdapter {

	public DBAdapter(Context context, Cursor c) {
		super(context, c);
	}

	@Override
	public void bindView(View view, Context context, Cursor cursor) {

	}

	@Override
	public View newView(Context context, Cursor cursor, ViewGroup parent) {
		return null;
	}

}
