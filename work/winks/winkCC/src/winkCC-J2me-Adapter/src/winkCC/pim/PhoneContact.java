package winkCC.pim;

import java.util.Enumeration;
import java.util.Hashtable;

/**
 * 手机电话簿联系人封装
 * 
 * @author WangYinghua
 * 
 */
public class PhoneContact {
	/**
	 * 联系人名字
	 */
	private String _name = null;
	/**
	 * 联系人电话号码
	 */
	private Hashtable _allNumbers = null;
	/**
	 * 主要电话号码
	 */
	private String _primaryNum = null;
	/**
	 * 大头贴地址
	 */
	private String _w_path = null;
	/**
	 * 是否为该用户更新彩像
	 */
	private boolean isChosen = true;

	/** 绑定号码哪个电话号码的规则 */
	private NumberFilter _filter = null;

	/**
	 * 手机联系人封装构造方法.
	 * 
	 * @param name
	 *            名字
	 * @param nums
	 *            所有电话号码
	 */
	public PhoneContact(String name, Hashtable nums) {
		_name = name;
		_allNumbers = nums;
	}

	/**
	 * 手机联系人封装构造方法.
	 * 
	 * @param name
	 *            名字
	 * @param nums
	 *            所有电话号码
	 * @param w_path
	 *            大头贴地址
	 */
	public PhoneContact(String name, Hashtable nums, String w_path) {
		this(name, nums);
		_w_path = w_path;
	}

	/**
	 * 为该联系人设定名字.
	 * 
	 * @param name
	 */
	public void setName(String name) {
		_name = name;
	}

	/**
	 * 得到该联系人名字.
	 * 
	 * @return 联系人名字.
	 */
	public String getName() {
		return _name;
	}

	/**
	 * 设置该联系人的所有电话号码.
	 * 
	 * @param nums
	 *            包含所有联系人号码的hashtable
	 */
	public void setAllNumber(Hashtable nums) {
		_allNumbers = nums;
	}

	/**
	 * 得到该联系人的所有电话.
	 * 
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
	 * 
	 * @param path
	 *            大头贴地址
	 */
	public void setWPath(String path) {
		_w_path = path;
	}

	/**
	 * 得到该联系人大头贴地址.
	 * 
	 * @return 联系人大头贴地址.
	 */
	public String getWPath() {
		return _w_path;
	}

	/**
	 * 得到业务绑定的一个电话号码.
	 * 
	 * @param filter
	 * @return 得到绑定的电话号码.
	 */
	public String getPrimaryNum(NumberFilter filter) {
		filter.filter(_allNumbers);
		return _primaryNum;
	}

	/**
	 * 设定该绑定哪个号码的规则过滤器.
	 * 
	 * @param filter
	 */
	public void setFilter(NumberFilter filter) {
		_filter = filter;
	}

	/**
	 * 提交联系人, 更新people对象到电话簿.
	 */
	public void submit() {

	}

	//////////////////debug
	/**
	 * debug时用于得到该联系人的打印信息.
	 */
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
