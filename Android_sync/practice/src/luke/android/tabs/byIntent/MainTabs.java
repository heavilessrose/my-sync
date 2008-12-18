package luke.android.tabs.byIntent;

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
				.setContent(new Intent(this, ExpandListTabs.class)));
	}

}
