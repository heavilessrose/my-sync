package luke.android.tabs.byIntent;

import luke.android.adapter.extendsCursorAdapter.PeopleListActivity;
import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TabHost;

public class MainTabs extends TabActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		final TabHost tabHost = getTabHost();

		tabHost.addTab(tabHost.newTabSpec("tab1").setIndicator("list")
				.setContent(new Intent(this, PeopleListActivity.class)));
		tabHost
				.addTab(tabHost
						.newTabSpec("tab2")
						.setIndicator("list2")
						.setContent(
								new Intent(
										this,
										luke.android.adapter.extendsBaseAdapter.PeopleListActivity.class)));
	}

}
