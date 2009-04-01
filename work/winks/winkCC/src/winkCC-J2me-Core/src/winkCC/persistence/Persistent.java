package winkCC.persistence;

import java.util.Hashtable;

public class Persistent {
	/**
	 * 彩像文件保存地址, 默认为SD卡winks_cc目录下.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * 彩像联系人文件cc_contacts.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/cc_contacts.xml";

	/**
	 * 内存中的彩像电话簿Hash表. key: name, value: People对象.
	 */
	private Hashtable ccContacts = new Hashtable();

	/**
	 * 获取彩像电话簿.
	 * 
	 * @return
	 */
	public Hashtable getCCContacts() {
		return ccContacts;
	}

	/**
	 * 初始化. 载入彩像电话簿及利用彩像电话簿完成数据完整性检查等.
	 */
	public void init() {
	}

	/**
	 * 验证数据完整性, 如果丢失该联系人的彩像文件则删除该联系人绑定的彩像ID, 等待下次更新下载彩像时重新下载丢失的彩像文件.
	 * 
	 * @param people
	 * @return
	 */
	public boolean verifyPeople(People people) {
		return false;
	}

	/**
	 * 设置保存彩像的路径.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	/**
	 * 创建彩像电话簿.
	 */
	public void createCCContacts() {

		submitCCContactsFile();
	}

	/**
	 * 给彩像电话簿中添加联系人.
	 * 
	 * @param people
	 */
	public void addPeople(People people) {

	}

	/**
	 * 修改彩像电话簿中联系人.
	 * 
	 * @param people
	 */
	public void modifyPeople(People people) {

	}

	/**
	 * 删除彩像电话簿中一个联系人.
	 */
	public void deletePeople(People people) {

		submitCCContactsFile();
	}

	/**
	 * 将彩像联系人列表提交到文件cc_contacts.xml
	 */
	private void submitCCContactsFile() {

	}

	/**
	 * 载入彩像电话簿.
	 */
	public void loadCCContacts() {

	}

	/**
	 * 删除彩像电话簿中指定联系人的彩像字段, 则下次下载更新彩像时可重新下载该联系人彩像及设置关联.
	 * 
	 * @param people
	 */
	public void deleteCCBinding(People people) {
		people.deleteCCID();
		people.deleteCCType();
	}

	/**
	 * 更新彩像绑定.
	 * 
	 * @param people
	 * @param newCCID
	 * @param newCCType
	 */
	public void updateCCBinding(People people, String newCCID, String newCCType) {
		people.updateCCID(newCCID);
		people.updateCCType(newCCType);
	}

	/**
	 * 获取指定联系人ccid, 用于比较是否需要更新彩像.
	 * 
	 * @param people
	 * @return
	 */
	public String getCCID(People people) {

		return null;
	}

	/**
	 * 添加保存一个彩像文件.
	 * 
	 * @param data
	 */
	public void addCC(byte[] data) {

	}

	/**
	 * 删除一个彩像文件
	 * 
	 * @param ccname
	 */
	public void deleteCC(String ccname) {

	}

	/**
	 * 更改系统设置.
	 * 
	 * @param propertyName
	 * @param propertyValue
	 */
	public void setProperty(String propertyName, String propertyValue) {

	}

	/**
	 * 释放资源.并提交改变到彩像电话簿文件cc_contacts.xml
	 */
	public void dispose() {

		submitCCContactsFile();
	}

}
