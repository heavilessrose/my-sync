package com.example.android.apis;

import android.test.ActivityInstrumentationTestCase;

/**
 * Make sure that the main launcher activity opens up properly, which will be
 * verified by {@link ActivityInstrumentationTestCase#testActivityTestCaseSetUpProperly}.
 */
public class ApiDemosTest extends ActivityInstrumentationTestCase<ApiDemos> {

    public ApiDemosTest() {
        super("com.example.android.apis", ApiDemos.class);
    }

}
