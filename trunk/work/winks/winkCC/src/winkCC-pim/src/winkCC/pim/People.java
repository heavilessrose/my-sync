package winkCC.pim;

import java.util.Enumeration;
import java.util.Hashtable;

public class People {
	// 联系人名字
	private String _name = null;
	// 电话号码
	private Hashtable _numbers = null;
	// 主要电话号码
	private String _primaryNum = null;
	// 彩像路径
	private String _w_path = null;
	private NumberFilter _filter = null;

	public People(String name, Hashtable num) {
		_name = name;
		_numbers = num;
	}

	public People(String name, Hashtable num, String w_path) {
		this(name, num);
		_w_path = w_path;
	}

	public void setName(String name) {
		_name = name;
	}

	public String getName() {
		return _name;
	}

	public void setNumbers(Hashtable num) {
		_numbers = num;
	}

	public Hashtable getNumbers() {
		return _numbers;
	}

	public void setWPath(String path) {
		_w_path = path;
	}

	public String getWPath() {
		return _w_path;
	}

	public String getPrimaryNum(NumberFilter filter) {
		filter.filter(_numbers);
		return _primaryNum;
	}

	public void setFilter(NumberFilter filter) {
		_filter = filter;
	}

	//////////////////
	public String toString() {
		return "name: " + getName() + "\n" + "numbers: " + getNumberString();
	}

	private String getNumberString() {
		StringBuffer sb = new StringBuffer();
		Enumeration keys = getNumbers().keys();
		Enumeration values = getNumbers().elements();
		for (; values.hasMoreElements();) {
			sb = sb.append((String) keys.nextElement() + values.nextElement());
		}
		return sb.toString();
	}
}
