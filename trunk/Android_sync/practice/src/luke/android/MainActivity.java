package luke.android;

import java.text.Collator;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.ListActivity;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.Contacts.People;
import android.provider.Contacts.Phones;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CursorAdapter;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.SimpleCursorAdapter;

public class MainActivity extends ListActivity {
	private static final String TAG = "practice";
	private static final String CATEGORY = "luke.android.categoryShow";
	private static final String ACTION = Intent.ACTION_MAIN;

	private String[] mStrings = { "Abbaye de Belloc",
			"Abbaye du Mont des Cats", "Abertam", "bondance", "ckawi", "corn",
			"delost", "ffidelice au Chablis", "fuega'l Pitu", "irag",
			"iredale", };

	/** 每个contact人的信息 (icon,name,majorNumber) */
	private ArrayList peopleInfos = new ArrayList();

	private Map<String, Object> peopleRowInfo = new HashMap<String, Object>();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);
		// Return the intent that started this activity.
		Intent intent = getIntent();
		String path = intent.getStringExtra("luke.android.Path");

		if (path == null) {
			path = "";
		}

		// 为ListActivity中的ListView绑定数据
		// 1.
		setListAdapter(new SimpleAdapter(this, getData(path),
				android.R.layout.simple_list_item_1, new String[] { "title" },
				new int[] { android.R.id.text1 }));
		// 2.
		// setListAdapter(new ArrayAdapter<String>(this, R.layout.main_item,
		// mStrings));
		// 3.
		// Get a cursor with all phones
		// Cursor c = getContentResolver().query(Phones.CONTENT_URI, null, null,
		// null, null);
		// startManagingCursor(c);
		// int pplId = c.getColumnIndex(People._ID);
		// if (c.moveToFirst()) {
		// long id = 0;
		// do {
		// id = c.getLong(pplId);
		// Log.d(TAG, "pplId: " + pplId);
		// Log.d(TAG, "icon_id: " + id);
		// Bitmap bitmap = People.loadContactPhoto(this, ContentUris
		// .withAppendedId(People.CONTENT_URI, id),
		// R.drawable.default_image, null);
		// Log.d(TAG, "id: " + id);
		// } while (c.moveToNext());
		// }
		// Map Cursor columns to views defined in simple_list_item_2.xml
		// ListAdapter adapter = new SimpleCursorAdapter(this,
		// R.layout.image_list, c, new String[] { Phones._ID, Phones.NAME,
		// Phones.NUMBER }, new int[] { R.id.presence, R.id.name,
		// R.id.number });
		// setListAdapter(adapter);

		// 得到在此ListActivity中的ListView
		ListView listView = getListView();
		// 设置过滤器
		listView.setTextFilterEnabled(true);
	}

	private static class ContactInfoAdapter extends CursorAdapter {

		public ContactInfoAdapter(Context context, Cursor c) {
			super(context, c);
		}

		@Override
		public void bindView(View view, Context context, Cursor cursor) {
			// LayoutInflater inflater = (LayoutInflater) context
			// .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			// View v = inflater.inflate(R.layout.image_list, view, false);
			// ImageView imageView = (ImageView) v.findViewById(R.id.icon);
			// imageView.setImage(contactImage);
		}

		@Override
		public View newView(Context context, Cursor cursor, ViewGroup parent) {
			return null;
		}

	}

	protected List getData(String prefix) {
		List<Map> myData = new ArrayList<Map>();

		Intent mainIntent = new Intent(ACTION, null);
		mainIntent.addCategory(CATEGORY);

		PackageManager pm = getPackageManager();
		List<ResolveInfo> list = pm.queryIntentActivities(mainIntent, 0);

		if (null == list)
			return myData;

		String[] prefixPath;

		if (prefix.equals("")) {
			prefixPath = null;
			// debug
			// Log.d(TAG, "prefixPath: Null");
		} else {
			prefixPath = prefix.split("/");

			// debug
			// for (int i = 0; i < prefixPath.length; i++) {
			// Log.d(TAG, "prefixPath: " + prefixPath[i]);
			// }
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

				// 如果是最外的activity则显示其名字, 如果不是最外的activity则显示其目录名
				String nextLabel = ((prefixPath == null) ? labelPath[0]
						: labelPath[prefixPath.length]);
				// debug
				// Log.d(TAG, "nextLabel: " + nextLabel);

				if ((prefixPath != null ? prefixPath.length : 0) == labelPath.length - 1) {
					addItem(myData, nextLabel, activityIntent(
							info.activityInfo.applicationInfo.packageName,
							info.activityInfo.name));
					Log.e(TAG, "packageName: "
							+ info.activityInfo.applicationInfo.packageName);
					Log.e(TAG, "componentName: " + info.activityInfo.name);
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