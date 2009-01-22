package luke.android.view.list.iconList;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;

import android.os.Bundle;
import android.util.Log;
import android.widget.SimpleAdapter;
import android.app.ListActivity;

/**
 * Demonstrates the built-in Android layout activity_list_item.
 * 
 * Utilizes the built-in icons as source material for the demo list. These are
 * collected via reflection.
 */
public class DemoActivityListItem extends ListActivity {
	// /////////////////////////////////////////////////////////////////////
	// Private Constants

	/** Tag for logging */
	private static final String TAG = "DemoActivityListItem";

	/** Attribute key for the list item text. */
	private static final String LABEL = "LABEL";
	/** Attribute key for the list item icon's drawable resource. */
	private static final String ICON = "ICON";

	// /////////////////////////////////////////////////////////////////////
	// Public Methods
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(TAG, "Entered onCreate(..)");

		// Format the data for the SimpleListAdapter:
		// Each item in the list represents one list entry.
		// The attributes of this item are represented in a Map,
		// with the names of the attributes as the keys.
		// Our keys are LABEL and ICON.
		List<Map<String, Object>> drawables = buildListForSimpleAdapter();

		// Now build the list adapter
		SimpleAdapter adapter = new SimpleAdapter(
		// the Context
				this,
				// the data to display
				drawables,
				// The layout to use for each item
				android.R.layout.activity_list_item,
				// The list item attributes to display
				new String[] { LABEL, ICON },
				// And the ids of the views where they should be displayed (same
				// order)
				new int[] { android.R.id.text1, android.R.id.icon });

		setListAdapter(adapter);

		Log.i(TAG, "Exiting onCreate(..)");
	}

	/**
	 * @return
	 */
	private List<Map<String, Object>> buildListForSimpleAdapter() {
		// Data source...
		SortedMap<String, Integer> drawables = AndroidResources.DRAWABLE_NAMES;
		// Resulting list...
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>(
				drawables.size());

		for (String name : drawables.keySet()) {
			// For each item in the source data
			int id = drawables.get(name);

			// Build a map for the attributes
			Map<String, Object> map = new HashMap<String, Object>();
			map.put(LABEL, name);
			map.put(ICON, id);
			list.add(map);
		}

		return list;
	}
}