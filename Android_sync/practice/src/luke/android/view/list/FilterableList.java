package luke.android.view.list;

import android.app.ListActivity;
import android.widget.Filter;
import android.widget.Filterable;

/** ʵ�ֹ�����, �ɿ��ٶ�λ���б��ָ���� */
public class FilterableList extends ListActivity implements Filterable {

	public Filter getFilter() {
		return null;
	}

}
