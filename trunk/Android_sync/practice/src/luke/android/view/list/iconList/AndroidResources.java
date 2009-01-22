package luke.android.view.list.iconList;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Collections;
import java.util.SortedMap;
import java.util.TreeMap;

import android.util.Log;

/**
 * Using reflection, name maps of the resources available in android.R
 */
public class AndroidResources {
	private static final String TAG = "AndroidResources";

	/** SortedMap of android.R.id constants to their values. */
	public static SortedMap<String, Integer> IDS_NAMES;
	/** SortedMap of android.R.drawable resource id constants to their values. */
	public static SortedMap<String, Integer> DRAWABLE_NAMES;

	static {
		// Populate the map when this class is first referenced
		IDS_NAMES = mapIdConstants(android.R.id.class);
		DRAWABLE_NAMES = mapIdConstants(android.R.drawable.class);
	}

	/**
	 * For a given class, build a map of all public int constants (i.e., static
	 * fields).
	 * 
	 * Uses the Java reflection APIs.
	 */
	private static SortedMap<String, Integer> mapIdConstants(
			Class<? extends Object> c) {
		SortedMap<String, Integer> map = new TreeMap<String, Integer>();

		Field fields[] = c.getFields(); // A constant is a type of field
		for (Field f : fields) {
			// Look for the fields matching prototype "public static int"
			int modifiers = f.getModifiers();
			if (Modifier.isPublic(modifiers) && Modifier.isStatic(modifiers)
					&& f.getType().equals(Integer.TYPE)) // Integer.TYPE is the
			// class for the int
			// primitive
			{
				String name = f.getName(); // Get the field name
				try {
					int value = f.getInt(null); // And its value

					// Add it to the map
					map.put(name, value);
				} catch (Exception error) {
					Log.e(TAG, "Failed to access id constant " + c.getName()
							+ "." + name + '\n' + error);
				}
			}
		}

		return Collections.unmodifiableSortedMap(map);
	}
}