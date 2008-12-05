package com.example.android.apis;

import android.app.ListActivity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import java.text.Collator;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ApiDemos extends ListActivity {
	private static final String TAG = "ApiDemos";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Return the intent that started this activity.
		Intent intent = getIntent();
		String path = intent.getStringExtra("com.example.android.apis.Path");

		if (path == null) {
			path = "";
		}

		// 为ListActivity中的ListView绑定数据
		setListAdapter(new SimpleAdapter(this, getData(path),
				android.R.layout.simple_list_item_1, new String[] { "title" },
				new int[] { android.R.id.text1 }));
		// 得到在此ListActivity中的ListView
		ListView listView = getListView();
		// 设置过滤器
		listView.setTextFilterEnabled(true);
	}

	protected List getData(String prefix) {
		List<Map> myData = new ArrayList<Map>();

		Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
		mainIntent.addCategory(Intent.CATEGORY_SAMPLE_CODE);

		PackageManager pm = getPackageManager();
		List<ResolveInfo> list = pm.queryIntentActivities(mainIntent, 0);

		if (null == list)
			return myData;

		String[] prefixPath;

		if (prefix.equals("")) {
			prefixPath = null;
			// debug
			Log.d(TAG, "prefixPath: Null");
		} else {
			prefixPath = prefix.split("/");

			// debug
			for (int i = 0; i < prefixPath.length; i++) {
				Log.d(TAG, "prefixPath: " + prefixPath[i]);
			}
		}

		int len = list.size();

		Map<String, Boolean> entries = new HashMap<String, Boolean>();

		// 绑定所有显示项及其Intent(指定进入的activity)
		for (int i = 0; i < len; i++) {
			ResolveInfo info = list.get(i);
			CharSequence labelSeq = info.loadLabel(pm);
			String label = labelSeq != null ? labelSeq.toString()
					: info.activityInfo.name;
			// debug
			Log.d(TAG, "label: " + label);

			if (prefix.length() == 0 || label.startsWith(prefix)) {

				String[] labelPath = label.split("/");

				String nextLabel = prefixPath == null ? labelPath[0]
						: labelPath[prefixPath.length];
				// debug
				Log.d(TAG, "nextLabel: " + nextLabel);

				if ((prefixPath != null ? prefixPath.length : 0) == labelPath.length - 1) {
					addItem(myData, nextLabel, activityIntent(
							info.activityInfo.applicationInfo.packageName,
							info.activityInfo.name));
				} else {
					if (entries.get(nextLabel) == null) {
						addItem(myData, nextLabel, browseIntent(prefix
								.equals("") ? nextLabel : prefix + "/"
								+ nextLabel));
						entries.put(nextLabel, true);
					}
				}
			}
		}

		Collections.sort(myData, sDisplayNameComparator);

		return myData;
	}

	private final static Comparator<Map> sDisplayNameComparator = new Comparator<Map>() {
		private final Collator collator = Collator.getInstance();

		public int compare(Map map1, Map map2) {
			return collator.compare(map1.get("title"), map2.get("title"));
		}
	};

	/** Implicit Intents-- 由系统决定用那个component来处理该Intent */
	protected Intent activityIntent(String packageName, String componentName) {
		Intent result = new Intent();
		result.setClassName(packageName, componentName);
		return result;
	}

	/** Explicit Intents-- 明确指定component来处理该Intent */
	protected Intent browseIntent(String path) {
		Intent result = new Intent();
		result.setClass(this, ApiDemos.class);
		result.putExtra("com.example.android.apis.Path", path);
		return result;
	}

	/** 在ListView上添加显示item */
	protected void addItem(List<Map> data, String name, Intent intent) {
		Map<String, Object> temp = new HashMap<String, Object>();
		temp.put("title", name);
		temp.put("intent", intent);
		data.add(temp);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Map map = (Map) l.getItemAtPosition(position);

		Intent intent = (Intent) map.get("intent");
		startActivity(intent);
	}

}
