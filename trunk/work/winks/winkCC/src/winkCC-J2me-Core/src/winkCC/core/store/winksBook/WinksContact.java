package winkCC.core.store.winksBook;

/**
 * ����绰���е���ϵ��.<br>
 * ��ʼ��ʱ��ccBook.xml�н�������
 * 
 * @author WangYinghua
 * 
 */
public class WinksContact {

	/**
	 * ����
	 */
	public String _name;
	/**
	 * ����
	 */
	public String _no;
	/**
	 * ����ID
	 */
	public String _ccid;
	/**
	 * ��������(��׺)
	 */
	public String _ccExt;
	/**
	 * �ʺ���
	 */
	public String _greeting;

	/**
	 * ���ò���ID.
	 * 
	 * @param newCcId
	 *            �µĲ���ID.
	 */
	void setCcId(String newCcId) {
		_ccid = newCcId;
	}

	/**
	 * ���ò�������(��׺).
	 * 
	 * @param newCctype
	 */
	void setCcExt(String newCcExt) {
		_ccExt = newCcExt;
	}

	/**
	 * ��ø���ϵ�˵Ĳ����ļ���.
	 * 
	 * @return
	 */
	String getCcName() {
		return _ccid + "." + _ccExt;
	}

	/**
	 * ���ø���ϵ�˵Ĳ����ļ���.
	 * 
	 * @return
	 */
	void setCcName(String ccid, String cctype) {
		_ccid = ccid;
		_ccExt = cctype;
	}

	/**
	 * ��õ绰����.
	 * 
	 * @return
	 */
	String getNo() {
		return _no;
	}

}
