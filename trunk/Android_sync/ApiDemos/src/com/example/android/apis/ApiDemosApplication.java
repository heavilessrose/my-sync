package com.example.android.apis;

import com.example.android.apis.app.DefaultValues;

import android.app.Application;
import android.preference.PreferenceManager;

/**
 * 作为一个中心仓库提供信息, 而这些信息是需要被多个activity共享的
 * This is an example of a {@link android.app.Application} class. Ordinarily you
 * would use a class like this as a central repository for information that
 * might be shared between multiple activities.
 * 
 * In this case, we have not defined any specific work for this Application.
 * 
 * See samples/ApiDemos/tests/src/com.example.android.apis/
 * ApiDemosApplicationTests for an example of how to perform unit tests on an
 * Application object.
 */
public class ApiDemosApplication extends Application {

	public void onCreate() {
		/*
		 * This populates the default values from the preferences XML file. See
		 * {@link DefaultValues} for more details.
		 */
		PreferenceManager.setDefaultValues(this, R.xml.default_values, false);
	}

	public void onTerminate() {
	}
}
