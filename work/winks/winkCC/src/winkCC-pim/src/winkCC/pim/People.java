package winkCC.pim;

import java.util.Enumeration;
import java.util.Hashtable;

public class People {
	// ��ϵ������
	private String _name = null;
	// �绰����
	private Hashtable _allNumbers = null;
	// ��Ҫ�绰����
	private String _primaryNum = null;
	// ��ͷ����ַ
	private String _w_path = null;
	// �Ƿ�Ϊ���û����²���
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
	 * Ϊ����ϵ���趨����.
	 * @param name
	 */
	public void setName(String name) {
		_name = name;
	}

	/**
	 * �õ�����ϵ������.
	 * @return
	 */
	public String getName() {
		return _name;
	}

	/**
	 * ���ø���ϵ�˵����е绰����.
	 * @param numTable ����������ϵ�˺����hashtable
	 */
	public void setAllNumber(Hashtable numTable) {
		_allNumbers = numTable;
	}

	/**
	 * �õ�����ϵ�˵����е绰.
	 * @return ���ذ����������绰��hashtable: ��mobile:1580000
	 */
	public Hashtable getAllNumber() {
		return _allNumbers;
	}

	/**
	 * �õ�ָ�����ĵ绰����.
	 * 
	 * @param attr
	 *            ���: mobile, home, work, fax, other.
	 * @return ���ָ�����ĺ��벻����,�򷵻�null.
	 */
	public String getNumber(String attr) {
		String num = null;
		if ((!_allNumbers.isEmpty()) && _allNumbers.containsKey(attr)) {
			num = (String) _allNumbers.get(attr);
		}
		return num;
	}

	/**
	 * ���ø���ϵ�˵Ĵ�ͷ����ַ.
	 * @param path ��ͷ����ַ
	 */
	public void setWPath(String path) {
		_w_path = path;
	}

	/**
	 * �õ�����ϵ�˴�ͷ����ַ.
	 * 
	 * @return
	 */
	public String getWPath() {
		return _w_path;
	}

	/**
	 * �õ���ҵ����ص�һ���绰����.
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
	 * �ύ��ϵ��, ����people���󵽵绰��.
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
