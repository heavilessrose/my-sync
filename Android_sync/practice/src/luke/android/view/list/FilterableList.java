package luke.android.view.list;

import android.app.ListActivity;
import android.widget.Filter;
import android.widget.Filterable;

/** 实现过滤器, 可快速定位到列表的指定行 */
public class FilterableList extends ListActivity implements Filterable {

	public Filter getFilter() {
		return null;
	}

}
