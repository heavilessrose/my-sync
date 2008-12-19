package luke.android.tabs.byIntent;

import luke.android.view.list.ExpandableList1;
import luke.android.view.list.FriendList;
import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TabHost;

public class ExpandListTabs extends TabActivity {

	// ExpandListTabs() {
	// }

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// groups.add(group1);
		// groups.add(group2);

		final TabHost tabHost = getTabHost();

		tabHost.addTab(tabHost.newTabSpec("tab1").setIndicator("list")
				.setContent(new Intent(this, FriendList.class)));
		tabHost.addTab(tabHost.newTabSpec("tab2").setIndicator("list2")
				.setContent(new Intent(this, ExpandableList1.class)));
	}

}