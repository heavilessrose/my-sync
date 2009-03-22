package winkCC.pim;

import java.util.Enumeration;
import java.util.Hashtable;

public class People {
	// 联系人名字
	private String _name = null;
	// 电话号码
	private Hashtable _allNumbers = null;
	// 主要电话号码
	private String _primaryNum = null;
	// 大头贴地址
	private String _w_path = null;
	// 是否为该用户更新彩像
	private boolean isChosen = true;

	private NumberFilter _filter = null;

	public People(String name, Hashtable num) {
		_name = name;
		_allNumbers = num;
	}

	public People(String name, Hashtable num, String w_path) {
		this(name, num);
		_w_path = w_path;
	}

	/**
	 * 为该联系人设定名字.
	 * @param name
	 */
	public void setName(String name) {
		_name = name;
	}

	/**
	 * 得到该联系人名字.
	 * @return
	 */
	public String getName() {
		return _name;
	}

	/**
	 * 设置该联系人的所有电话号码.
	 * @param numTable 包含所有联系人号码的hashtable
	 */
	public void setAllNumber(Hashtable numTable) {
		_allNumbers = numTable;
	}

	/**
	 * 得到该联系人的所有电话.
	 * @return 返回包含所有类别电话的hashtable: 如mobile:1580000
	 */
	public Hashtable getAllNumber() {
		return _allNumbers;
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
		if ((!_allNumbers.isEmpty()) && _allNumbers.containsKey(attr)) {
			num = (String) _allNumbers.get(attr);
		}
		return num;
	}

	/**
	 * 设置该联系人的大头贴地址.
	 * @param path 大头贴地址
	 */
	public void setWPath(String path) {
		_w_path = path;
	}

	/**
	 * 得到该联系人大头贴地址.
	 * 
	 * @return
	 */
	public String getWPath() {
		return _w_path;
	}

	/**
	 * 得到与业务相关的一个电话号码.
	 * @param filter
	 * @return
	 */
	public String getPrimaryNum(NumberFilter filter) {
		filter.filter(_allNumbers);
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
		Enumeration keys = getAllNumber().keys();
		Enumeration values = getAllNumber().elements();
		for (; values.hasMoreElements();) {
			sb = sb.append((String) keys.nextElement() + ":"
					+ values.nextElement() + "\n");
		}
		return sb.toString();
	}
}
