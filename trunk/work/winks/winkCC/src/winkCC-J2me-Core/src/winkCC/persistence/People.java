package winkCC.persistence;

/**
 * 彩像电话本中的联系人.<br>
 * 初始化时从cc_contacts.xml中解析出来
 * 
 * @author WangYinghua
 * 
 */
public class People {

	/**
	 * 名字
	 */
	public String name;
	/**
	 * 号码
	 */
	public String no;
	/**
	 * 彩像ID
	 */
	public String ccid;
	/**
	 * 彩像类型(后缀)
	 */
	public String cctype;
	/**
	 * 问候语
	 */
	public String greeting;

	/**
	 * 更新彩像ID.
	 * 
	 * @param newCCID
	 *            新的彩像ID.
	 */
	void updateCCID(String newCCID) {
		ccid = newCCID;
	}

	/**
	 * 更新彩像类型(后缀).
	 * 
	 * @param newCctype
	 */
	void updateCCType(String newCCType) {
		cctype = newCCType;
	}

	/**
	 * 删除彩像ID.
	 */
	void deleteCCID() {
		ccid = null;
	}

	/**
	 * 删除彩像类型.
	 */
	void deleteCCType() {
		cctype = null;
	}

	/**
	 * 获得该联系人的彩像文件名.
	 * 
	 * @return
	 */
	String getCCName() {
		return ccid + "." + cctype;
	}

	/**
	 * 更新到xml文件中.
	 */
	void submit() {

	}

}
