package luke.android.view.list;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import luke.android.R;
import android.app.ExpandableListActivity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.ImageView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

public class FriendList extends ExpandableListActivity {

	ExpandableListAdapter mAdapter;

	private static final String NAME = "NAME";
	private static final String INFO = "INFO";
	private Resources res;
	private Drawable p1, p2;
	private List<Drawable> pps;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.friendlist_tab_layout);

		// test
		res = this.getResources();
		p1 = (Drawable) res.getDrawable(R.drawable.p1);
		p2 = (Drawable) res.getDrawable(R.drawable.p2);
		pps = new ArrayList<Drawable>();
		pps.add(p1);
		pps.add(p2);

		List<Map<String, String>> groupData = new ArrayList<Map<String, String>>();
		List<List<Map<String, String>>> childData = new ArrayList<List<Map<String, String>>>();

		Map<String, String> curGroupMap = new HashMap<String, String>();
		groupData.add(curGroupMap);
		curGroupMap.put(NAME, "nananana");
		curGroupMap.put(INFO, "");

		List<Map<String, String>> children = new ArrayList<Map<String, String>>();
		Map<String, String> curChildMap = new HashMap<String, String>();
		children.add(curChildMap);

		curChildMap.put(NAME, "ccccc1");
		curChildMap.put(INFO, "cc");

		curChildMap = new HashMap<String, String>();
		children.add(curChildMap);

		curChildMap.put(NAME, "ccccccc2");
		curChildMap.put(INFO, "cc2");

		childData.add(children);

		curGroupMap = new HashMap<String, String>();
		groupData.add(curGroupMap);

		curGroupMap.put(NAME, "ccc");
		curGroupMap.put(INFO, "");

		children = new ArrayList<Map<String, String>>();

		curChildMap = new HashMap<String, String>();
		children.add(curChildMap);

		curChildMap.put(NAME, "ssssss");
		curChildMap.put(INFO, "sss");

		childData.add(children);

		mAdapter = new FriendExpandableListAdapter(this, groupData,
				R.layout.friendlist_group, new String[] { NAME, INFO },
				new int[] { R.id.rowText1, R.id.rowText2, R.id.icon },
				childData, R.layout.friendlist_child,
				new String[] { NAME, INFO }, new int[] { R.id.rowText1,
						R.id.rowText2, R.id.avatar });
		setListAdapter(mAdapter);
		registerForContextMenu(getExpandableListView());
	}

	public class FriendExpandableListAdapter extends
			SimpleExpandableListAdapter {

		private List<? extends List<? extends Map<String, ?>>> _childData;
		private String[] _childFrom;
		private int[] _childTo;

		public FriendExpandableListAdapter(Context context,
				List<? extends Map<String, ?>> groupData, int groupLayout,
				String[] groupFrom, int[] groupTo,
				List<? extends List<? extends Map<String, ?>>> childData,
				int childLayout, String[] childFrom, int[] childTo) {
			super(context, groupData, groupLayout, groupFrom, groupTo,
					childData, childLayout, childFrom, childTo);

			_childData = childData;
			_childFrom = childFrom;
			_childTo = childTo;

		}

		public View getChildView(int groupPosition, int childPosition,
				boolean isLastChild, View convertView, ViewGroup parent) {

			View v;
			if (convertView == null) {
				v = newChildView(isLastChild, parent);
			} else {
				v = convertView;
			}
			bindView(v, _childData.get(groupPosition).get(childPosition),
					_childFrom, _childTo, groupPosition, childPosition);
			return v;
		}

		private void bindView(View view, Map<String, ?> data, String[] from,
				int[] to, int groupPosition, int childPosition) {
			int len = to.length - 1;

			for (int i = 0; i < len; i++) {
				TextView v = (TextView) view.findViewById(to[i]);
				if (v != null) {
					v.setText((String) data.get(from[i]));
				}

			}

			// 这里要更改得到图片的方法
			ImageView imgV = (ImageView) view.findViewById(to[2]);
			if (imgV != null) {
				if (childPosition % 2 == 0)
					imgV.setImageDrawable(pps.get(0));
				else
					imgV.setImageDrawable(pps.get(1));
			}
		}
	}

}