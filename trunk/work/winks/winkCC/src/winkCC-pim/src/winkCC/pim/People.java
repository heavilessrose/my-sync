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
	// 大头贴地址
	private String _w_path = null;
	// 是否为该用户更新彩像
	private boolean isChosen = true;

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

	/**
	 * 得到指定类别的电话号码.
	 * 
	 * @param attr
	 *            类别: mobile, home, work, fax, other.
	 * @return 如果指定类别的号码不存在,则返回null.
	 */
	public String getNumber(String attr) {
		String num = null;
		if ((!_numbers.isEmpty()) && _numbers.containsKey(attr)) {
			num = (String) _numbers.get(attr);
		}
		return num;
	}

	public void setWPath(String path) {
		_w_path = path;
	}

	/**
	 * 得到大头贴地址.
	 * 
	 * @return
	 */
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

	/**
	 * 提交联系人, 更新people对象到电话簿.
	 */
	public void submit() {

	}

	//////////////////debug
	public String toString() {
		return "name: " + getName() + "\n" + "numbers: " + getNumberString();
	}

	private String getNumberString() {
		StringBuffer sb = new StringBuffer();
		Enumeration keys = getNumbers().keys();
		Enumeration values = getNumbers().elements();
		for (; values.hasMoreElements();) {
			sb = sb.append((String) keys.nextElement() + ":"
					+ values.nextElement() + "\n");
		}
		return sb.toString();
	}
}
