package com.example.android.apis.view;

import android.app.TabActivity;
import android.os.Bundle;
import android.widget.TabHost;
import android.widget.TextView;
import android.view.View;
import com.example.android.apis.R;

/**
 * Example of using a tab content factory for the content via {@link TabHost.TabSpec#setContent(android.widget.TabHost.TabContentFactory)}
 *
 * It also demonstrates using an icon on one of the tabs via {@link TabHost.TabSpec#setIndicator(CharSequence, android.graphics.drawable.Drawable)}
 *
 */
public class Tabs2 extends TabActivity implements TabHost.TabContentFactory {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final TabHost tabHost = getTabHost();
        tabHost.addTab(tabHost.newTabSpec("tab1")
                .setIndicator("tab1", getResources().getDrawable(R.drawable.star_big_on))
                .setContent(this));
        tabHost.addTab(tabHost.newTabSpec("tab2")
                .setIndicator("tab2")
                .setContent(this));
        tabHost.addTab(tabHost.newTabSpec("tab3")
                .setIndicator("tab3")
                .setContent(this));
    }

    /** {@inheritDoc} */
    public View createTabContent(String tag) {
        final TextView tv = new TextView(this);
        tv.setText("Content for tab with tag " + tag);
        return tv;
    }
}
