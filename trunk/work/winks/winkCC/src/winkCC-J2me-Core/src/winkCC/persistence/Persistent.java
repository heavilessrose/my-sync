package winkCC.persistence;

import java.util.Hashtable;

public class Persistent {
	/**
	 * �����ļ������ַ, Ĭ��ΪSD��winks_ccĿ¼��.
	 */
	public String CC_PATH = "e:/winks_cc/";
	/**
	 * ������ϵ���ļ�cc_contacts.xml.
	 */
	public String CC_CONTACTS_FILE = "c:/winks_property/cc_contacts.xml";

	/**
	 * �ڴ��еĲ���绰��Hash��. key: name, value: People����.
	 */
	private Hashtable ccContacts = new Hashtable();

	/**
	 * ��ȡ����绰��.
	 * 
	 * @return
	 */
	public Hashtable getCCContacts() {
		return ccContacts;
	}

	/**
	 * ��ʼ��. �������绰�������ò���绰��������������Լ���.
	 */
	public void init() {
	}

	/**
	 * ��֤����������, �����ʧ����ϵ�˵Ĳ����ļ���ɾ������ϵ�˰󶨵Ĳ���ID, �ȴ��´θ������ز���ʱ�������ض�ʧ�Ĳ����ļ�.
	 * 
	 * @param people
	 * @return
	 */
	public boolean verifyPeople(People people) {
		return false;
	}

	/**
	 * ���ñ�������·��.
	 * 
	 * @param path
	 */
	public void setCCPath(String path) {
		CC_PATH = path;
	}

	/**
	 * ��������绰��.
	 */
	public void createCCContacts() {

		submitCCContactsFile();
	}

	/**
	 * ������绰���������ϵ��.
	 * 
	 * @param people
	 */
	public void addPeople(People people) {

	}

	/**
	 * �޸Ĳ���绰������ϵ��.
	 * 
	 * @param people
	 */
	public void modifyPeople(People people) {

	}

	/**
	 * ɾ������绰����һ����ϵ��.
	 */
	public void deletePeople(People people) {

		submitCCContactsFile();
	}

	/**
	 * ��������ϵ���б��ύ���ļ�cc_contacts.xml
	 */
	private void submitCCContactsFile() {

	}

	/**
	 * �������绰��.
	 */
	public void loadCCContacts() {

	}

	/**
	 * ɾ������绰����ָ����ϵ�˵Ĳ����ֶ�, ���´����ظ��²���ʱ���������ظ���ϵ�˲������ù���.
	 * 
	 * @param people
	 */
	public void deleteCCBinding(People people) {
		people.deleteCCID();
		people.deleteCCType();
	}

	/**
	 * ���²����.
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
	 * ��ȡָ����ϵ��ccid, ���ڱȽ��Ƿ���Ҫ���²���.
	 * 
	 * @param people
	 * @return
	 */
	public String getCCID(People people) {

		return null;
	}

	/**
	 * ��ӱ���һ�������ļ�.
	 * 
	 * @param data
	 */
	public void addCC(byte[] data) {

	}

	/**
	 * ɾ��һ�������ļ�
	 * 
	 * @param ccname
	 */
	public void deleteCC(String ccname) {

	}

	/**
	 * ����ϵͳ����.
	 * 
	 * @param propertyName
	 * @param propertyValue
	 */
	public void setProperty(String propertyName, String propertyValue) {

	}

	/**
	 * �ͷ���Դ.���ύ�ı䵽����绰���ļ�cc_contacts.xml
	 */
	public void dispose() {

		submitCCContactsFile();
	}

}
