package com.example.android.apis.view;

import android.app.TabActivity;
import android.os.Bundle;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;
import android.view.LayoutInflater;
import android.view.View;

import com.example.android.apis.R;

/**
 * An example of tabs that uses labels (
 * {@link TabSpec#setIndicator(CharSequence)}) for its indicators and views by
 * id from a layout file ({@link TabSpec#setContent(int)}).
 */
public class Tabs1 extends TabActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		TabHost tabHost = getTabHost();

		LayoutInflater.from(this).inflate(R.layout.tabs1,
				tabHost.getTabContentView(), true);

		tabHost.addTab(tabHost.newTabSpec("tab1").setIndicator("tab1")
				.setContent(R.id.view1));
		tabHost.addTab(tabHost.newTabSpec("tab3").setIndicator("tab2")
				.setContent(R.id.view2));
		tabHost.addTab(tabHost.newTabSpec("tab3").setIndicator("tab3")
				.setContent(R.id.view3));
	}
}
