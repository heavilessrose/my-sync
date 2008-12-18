package luke.android.tabs.byIntent;

import java.util.ArrayList;
import java.util.List;

import android.app.ExpandableListActivity;
import android.app.TabActivity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;

public class ExpandListTabs extends TabActivity {
	// test
	Group group1 = new Group();
	Group group2 = new Group();
	Groups groups = new Groups();

	ExpandListTabs() {
		groups.add(group1);
		groups.add(group2);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	private static class ContactExpandList extends ExpandableListActivity {
		@Override
		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);

		}

		private static class ExpandbleContactListAdapter extends
				BaseExpandableListAdapter {

			public Object getChild(int groupPosition, int childPosition) {
				return null;
			}

			public long getChildId(int groupPosition, int childPosition) {
				return 0;
			}

			public int getChildrenCount(int groupPosition) {
				return 0;
			}

			public Object getGroup(int groupPosition) {
				return null;
			}

			public int getGroupCount() {
				return 0;
			}

			public long getGroupId(int groupPosition) {
				return 0;
			}

			public View getGroupView(int groupPosition, boolean isExpanded,
					View convertView, ViewGroup parent) {
				return null;
			}

			public boolean hasStableIds() {
				return false;
			}

			public boolean isChildSelectable(int groupPosition,
					int childPosition) {
				return false;
			}

			public View getChildView(int groupPosition, int childPosition,
					boolean isLastChild, View convertView, ViewGroup parent) {

				return null;
			}

		}
	}

	// test
	static class Groups {
		List<Group> groups = new ArrayList<Group>();

		public void add(Group g) {
			groups.add(g);
		}
	}

	class Group {
		List<User> groupUsers = new ArrayList<User>();

		Group() {
			for (int i = 0; i < 3; i++) {
				groupUsers.set(i, new User(i));
			}
		}
	}

	class User {
		String nickName;
		String QQ_number;
		Bitmap avatar;

		User(int i) {
			nickName = "user " + i;
			QQ_number = "" + i;
			// avatar =
		}
	}
}