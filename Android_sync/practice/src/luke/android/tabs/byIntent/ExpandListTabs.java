package luke.android.tabs.byIntent;

import android.app.ExpandableListActivity;
import android.app.TabActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;

public class ExpandListTabs extends TabActivity {
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

			public View getChildView(int groupPosition, int childPosition,
					boolean isLastChild, View convertView, ViewGroup parent) {
				return null;
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
			
			
//			public View getChildrenView(){
//				
//			}

			public boolean hasStableIds() {
				return false;
			}

			public boolean isChildSelectable(int groupPosition,
					int childPosition) {
				return false;
			}

		}
	}
}