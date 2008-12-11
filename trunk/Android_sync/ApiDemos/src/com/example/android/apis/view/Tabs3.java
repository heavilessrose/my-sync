package com.example.android.apis.view;

import android.app.TabActivity;
import android.os.Bundle;
import android.widget.TabHost;
import android.content.Intent;

/**
 * An example of tab content that launches an activity via
 * {@link android.widget.TabHost.TabSpec#setContent(android.content.Intent)}
 */
public class Tabs3 extends TabActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		final TabHost tabHost = getTabHost();

		tabHost.addTab(tabHost.newTabSpec("tab1").setIndicator("list")
				.setContent(new Intent(this, List1.class)));

		tabHost.addTab(tabHost.newTabSpec("tab2").setIndicator("photo list")
				.setContent(new Intent(this, List8.class)));

		// This tab sets the intent flag so that it is recreated each time
		// the tab is clicked.
		tabHost.addTab(tabHost.newTabSpec("tab3").setIndicator("destroy")
				.setContent(
						new Intent(this, Controls2.class)
								.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)));
	}
}
