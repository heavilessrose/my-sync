package com.example.android.apis;

import android.test.ApplicationTestCase;

/**
 * This is a simple framework for a test of an Application. See
 * {@link android.test.ApplicationTestCase ApplicationTestCase} for more
 * information on how to write and extend Application tests.
 * 
 * To run this test, you can type: adb shell am instrument -w \ -e class
 * com.example.android.apis.ApiDemosApplicationTests \
 * com.example.android.apis.tests/android.test.InstrumentationTestRunner
 */
public class ApiDemosApplicationTests extends
		ApplicationTestCase<ApiDemosApplication> {

	public ApiDemosApplicationTests() {
		super(ApiDemosApplication.class);
	}

	@Override
	protected void setUp() throws Exception {
		super.setUp();
	}

	/**
	 * The name 'test preconditions' is a convention to signal that if this test
	 * doesn't pass, the test case was not set up properly and it might explain
	 * any and all failures in other tests. This is not guaranteed to run before
	 * other tests, as junit uses reflection to find the tests.
	 */
	public void testPreconditions() {
	}

	/**
	 * Test basic startup/shutdown of Application
	 */
	public void testSimpleCreate() {
		createApplication();
	}

}
