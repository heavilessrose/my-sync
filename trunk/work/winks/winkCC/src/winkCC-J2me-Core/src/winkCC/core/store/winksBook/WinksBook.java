package winkCC.core.store.winksBook;

import java.util.Hashtable;

/**
 * ����绰��.
 * 
 * @author WangYinghua
 * 
 */
public class WinksBook {

	/**
	 * �ڴ��л���Ĳ���绰��. key: name, value: WinksContacts����.
	 */
	private Hashtable _allWinksContacts = new Hashtable();

	/**
	 * ��ȡ����绰����������ϵ��.
	 * 
	 * @return
	 */
	public Hashtable getAllWinksContacts() {
		return _allWinksContacts;
	}

	/**
	 * ���ݵ绰����ȡ��һ����ϵ��.
	 * 
	 * @param no
	 *            �绰����.
	 * @return
	 */
	public WinksContact getWinksContact(String no) {
		return null;
	}

	/**
	 * ��ʼ��. �������������绰��,���ò���绰��������������Լ���.<br>
	 * 1> �����ֻ����к���, ��˺��벻�������ڲ���绰����ɾ������ϵ��.<br>
	 * 2> �����ʧ����ϵ�˵Ĳ����ļ���ɾ������ϵ�˰󶨵Ĳ���ID, �ȴ��´θ������ز���ʱ�������ض�ʧ�Ĳ����ļ�.
	 */
	public void init() {
		parseAndLoadWinksBook();
	}

	/**
	 * ��֤����绰����һ����ϵ�˵�����������.<br>
	 * 1> �����ֻ����к���, ��˺��벻�������ڲ���绰����ɾ������ϵ��.<br>
	 * 2> �����ʧ����ϵ�˵Ĳ����ļ���ɾ������ϵ�˰󶨵Ĳ���ID, �ȴ��´θ������ز���ʱ�������ض�ʧ�Ĳ����ļ�.
	 * 
	 * 
	 * @param wContact
	 *            ����绰���е���ϵ��.
	 * @return
	 */
	public boolean verifyWinksContact(WinksContact wContact) {
		return false;
	}

	/**
	 * ��֤��������绰��.
	 */
	public void verifyIntegrality() {

	}

	/**
	 * ��������绰��.<br>
	 * �����һ���������û�ѡ�����ɲ���绰�� ��У�������Ժ��������ɲ���绰��.
	 */
	public void createWinksBook(Hashtable wContact) {

		writeWinksBookFile();
	}

	/**
	 * ������绰���������ϵ��.<br>
	 * ʵ����ֻ�Ǽ��뵽�����в�δ�������Ĳ���绰���ļ�. Ӧ���ʵ���ʱ�򽫸���д���ļ�.
	 * 
	 * @param wContact
	 */
	public void addWinksContact(WinksContact wContact) {

	}

	/**
	 * ɾ������绰����һ����ϵ��.<br>
	 * ʵ����ֻ�Ǽ��뵽�����в�δ�������Ĳ���绰���ļ�. Ӧ���ʵ���ʱ�򽫸���д���ļ�.
	 */
	public void deleteWinksContact(WinksContact wContact) {

	}

	/**
	 * ��������ϵ���б�д�뵽����绰��xml�ļ�.
	 */
	public void writeWinksBookFile() {

	}

	/**
	 * ������������绰�������ڴ�.
	 */
	public void parseAndLoadWinksBook() {

	}

	//-------------------------------------------------------
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
	 * �ͷ���Դ.���ύ�ı䵽����绰���ļ�.
	 */
	public void dispose() {

		writeWinksBookFile();
	}

}
