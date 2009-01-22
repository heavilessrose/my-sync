package luke.android.app.todoList;

import luke.android.R;
import android.database.Cursor;
import android.graphics.Paint;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

public class CrossBinder implements SimpleCursorAdapter.ViewBinder {

	public boolean setViewValue(View view, Cursor cursor, int columnIndex) {

		switch (view.getId()) {
		case android.R.id.content:
			// binding to parent container should set the crossed value
			ImageView icon = (ImageView) view.findViewById(android.R.id.icon);
			TextView text1 = (TextView) view.findViewById(android.R.id.text1),
			text2 = (TextView) view.findViewById(android.R.id.text2);

			// read crossed status and set text flags for strikethrough
			boolean crossed = Boolean.valueOf(cursor.getString(columnIndex));
			if (crossed) {
				icon.setImageState(new int[] { android.R.attr.state_checked },
						true);
				text1.setPaintFlags(text1.getPaintFlags()
						| Paint.STRIKE_THRU_TEXT_FLAG);
				text2.setPaintFlags(text2.getPaintFlags()
						| Paint.STRIKE_THRU_TEXT_FLAG);
			} else {
				icon.setImageState(new int[] {}, true);
				text1.setPaintFlags(text1.getPaintFlags()
						& ~Paint.STRIKE_THRU_TEXT_FLAG);
				text2.setPaintFlags(text2.getPaintFlags()
						& ~Paint.STRIKE_THRU_TEXT_FLAG);
			}
			return true;

		case android.R.id.text2:
			// binding to second textview should format time nicely
			long created = cursor.getLong(columnIndex);
			long now = System.currentTimeMillis() / 1000;

			int minutes = (int) ((now - created) / 60);
			String nice = view.getContext().getString(
					R.string.todolist_bind_minutes, minutes);
			if (minutes >= 60) {
				int hours = (minutes / 60);
				nice = view.getContext().getString(
						R.string.todolist_bind_hours, hours);
				if (hours >= 24) {
					int days = (hours / 24);
					nice = view.getContext().getString(
							R.string.todolist_bind_days, days);
				}
			}

			((TextView) view).setText(nice);

			return true;
		}

		// otherwise fall through to other binding methods
		return false;

	}

}
