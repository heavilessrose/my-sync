package winkCC.persistence;

/**
 * ����绰���е���ϵ��.<br>
 * ��ʼ��ʱ��cc_contacts.xml�н�������
 * 
 * @author WangYinghua
 * 
 */
public class People {

	/**
	 * ����
	 */
	public String name;
	/**
	 * ����
	 */
	public String no;
	/**
	 * ����ID
	 */
	public String ccid;
	/**
	 * ��������(��׺)
	 */
	public String cctype;
	/**
	 * �ʺ���
	 */
	public String greeting;

	/**
	 * ���²���ID.
	 * 
	 * @param newCCID
	 *            �µĲ���ID.
	 */
	void updateCCID(String newCCID) {
		ccid = newCCID;
	}

	/**
	 * ���²�������(��׺).
	 * 
	 * @param newCctype
	 */
	void updateCCType(String newCCType) {
		cctype = newCCType;
	}

	/**
	 * ɾ������ID.
	 */
	void deleteCCID() {
		ccid = null;
	}

	/**
	 * ɾ����������.
	 */
	void deleteCCType() {
		cctype = null;
	}

	/**
	 * ��ø���ϵ�˵Ĳ����ļ���.
	 * 
	 * @return
	 */
	String getCCName() {
		return ccid + "." + cctype;
	}

	/**
	 * ���µ�xml�ļ���.
	 */
	void submit() {

	}

}
