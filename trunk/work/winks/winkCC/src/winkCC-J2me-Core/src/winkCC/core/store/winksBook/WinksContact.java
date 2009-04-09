package winkCC.core.store.winksBook;

/**
 * 彩像电话本中的联系人.<br>
 * 初始化时从ccBook.xml中解析出来
 * 
 * @author WangYinghua
 * 
 */
public class WinksContact {

	/**
	 * 名字
	 */
	public String _name;
	/**
	 * 号码
	 */
	public String _no;
	/**
	 * 彩像ID
	 */
	public String _ccid;
	/**
	 * 彩像类型(后缀)
	 */
	public String _ccExt;
	/**
	 * 问候语
	 */
	public String _greeting;

	/**
	 * 设置彩像ID.
	 * 
	 * @param newCcId
	 *            新的彩像ID.
	 */
	void setCcId(String newCcId) {
		_ccid = newCcId;
	}

	/**
	 * 设置彩像类型(后缀).
	 * 
	 * @param newCctype
	 */
	void setCcExt(String newCcExt) {
		_ccExt = newCcExt;
	}

	/**
	 * 获得该联系人的彩像文件名.
	 * 
	 * @return
	 */
	String getCcName() {
		return _ccid + "." + _ccExt;
	}

	/**
	 * 设置该联系人的彩像文件名.
	 * 
	 * @return
	 */
	void setCcName(String ccid, String cctype) {
		_ccid = ccid;
		_ccExt = cctype;
	}

	/**
	 * 获得电话号码.
	 * 
	 * @return
	 */
	String getNo() {
		return _no;
	}

}
