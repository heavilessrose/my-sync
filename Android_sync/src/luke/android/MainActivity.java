package luke.android;

import java.text.Collator;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends ListActivity {
	private static final String TAG = "practice";
	private static final String CATEGORY = "luke.android.categoryShow";
	private static final String ACTION = Intent.ACTION_MAIN;

	// private String[] mStrings = { "Abbaye de Belloc",
	// "Abbaye du Mont des Cats", "Abertam", "bondance", "ckawi", "corn",
	// "delost", "ffidelice au Chablis", "fuega'l Pitu", "irag",
	// "iredale", };

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// 不使用默认的screen layout
		setContentView(R.layout.main);

		// Return the intent that started this activity.
		Intent intent = getIntent();
		String path = intent.getStringExtra("luke.android.Path");

		if (path == null) {
			path = "";
		}

		// 为ListActivity中的ListView绑定数据, 和row layout
		setListAdapter(new SimpleAdapter(this, getData(path),
				R.layout.main_item, new String[] { "title" },
				new int[] { R.id.text1 }));
		// setListAdapter(new ArrayAdapter<String>(this, R.layout.main_item,
		// mStrings));

		// 得到在此ListActivity中的ListView
		ListView listView = getListView();
		// 设置过滤器
		listView.setTextFilterEnabled(true);
	}

	protected List getData(String prefix) {
		// 显示在列表中的activity
		List<Map> mainShowActivity = new ArrayList<Map>();

		// 返回所有可处理指定Intent的activity
		Intent mainIntent = new Intent(ACTION, null);
		mainIntent.addCategory(CATEGORY);
		PackageManager pm = getPackageManager();
		List<ResolveInfo> list = pm.queryIntentActivities(mainIntent, 0);

		if (null == list)
			return mainShowActivity;

		String[] prefixPath;
		if (prefix.equals("")) {
			prefixPath = null;
		} else {
			prefixPath = prefix.split("/");
		}

		Log.i(TAG, "list.size: " + list.size());
		int listLen = list.size();
		Map<String, Boolean> entries = new HashMap<String, Boolean>();
		for (int i = 0; i < listLen; i++) {
			ResolveInfo info = list.get(i);
			CharSequence labelSeq = info.loadLabel(pm);
			String label = labelSeq != null ? labelSeq.toString()
					: info.activityInfo.name;

			Log.e(TAG, "label: " + label);

			if (prefix.length() == 0 || label.startsWith(prefix)) {
				String[] labelPath = label.split("/");

				// 如果是最外的activity则为其名字, 如果不是最外的activity则为其目录名
				String nextLabel = prefixPath == null ? labelPath[0]
						: labelPath[prefixPath.length];
				
				Log.e(TAG, "nextLabel: " + nextLabel);

				if ((prefixPath != null ? prefixPath.length : 0) == labelPath.length - 1) {
					addItem(mainShowActivity, nextLabel, activityIntent(
							info.activityInfo.applicationInfo.packageName,
							info.activityInfo.name));
					Log.e(TAG, "packageName: "
							+ info.activityInfo.applicationInfo.packageName);
					Log.e(TAG, "componentName: " + info.activityInfo.name);
				} else {
					if (entries.get(nextLabel) == null) {
						addItem(mainShowActivity, nextLabel,
								browseIntent(prefix.equals("") ? nextLabel
										: prefix + "/" + nextLabel));
						entries.put(nextLabel, true);
					}
				}
			}
		}

		return mainShowActivity;
	}

	private final static Comparator<Map> sDisplayNameComparator = new Comparator<Map>() {
		private final Collator collator = Collator.getInstance();

		public int compare(Map map1, Map map2) {
			return collator.compare(map1.get("title"), map2.get("title"));
		}
	};

	/** 已到达目录最末端, */
	protected Intent activityIntent(String packageName, String componentName) {
		Intent result = new Intent();
		result.setClassName(packageName, componentName);
		return result;
	}

	/** 没有到目录最末端(末端是activity), 继续在此listActivity中显示 */
	protected Intent browseIntent(String path) {
		Intent result = new Intent();
		result.setClass(this, MainActivity.class);
		result.putExtra("luke.android.Path", path);
		return result;
	}

	/** 在ListView上添加显示item */
	protected void addItem(List<Map> data, String name, Intent intent) {
		Map<String, Object> temp = new HashMap<String, Object>();
		temp.put("title", name);
		temp.put("intent", intent);
		data.add(temp);
	}

	/** 如果是目录则进入下一层目录,如果已经在目录最末端则执行该activity */
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Map map = (Map) l.getItemAtPosition(position);

		Intent intent = (Intent) map.get("intent");
		startActivity(intent);
	}
}