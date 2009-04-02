package winkCC.persistence;

import java.util.Hashtable;

/**
 * ����绰��.
 * 
 * @author WangYinghua
 * 
 */
public class CcBook {

	/**
	 * �ڴ��л���Ĳ���绰��. key: name, value: CcPeople����.
	 */
	private Hashtable _allCcPeoples = new Hashtable();

	/**
	 * ��ȡ����绰����������ϵ��.
	 * 
	 * @return
	 */
	public Hashtable getAllCcPeoples() {
		return _allCcPeoples;
	}

	/**
	 * ���ݵ绰����ȡ��һ����ϵ��.
	 * 
	 * @param no
	 *            �绰����.
	 * @return
	 */
	public CcPeople getCcPeople(String no) {
		return null;
	}

	/**
	 * ��ʼ��. �������������绰��,���ò���绰��������������Լ���.<br>
	 * 1> �����ֻ����к���, ��˺��벻��������CcBook��ɾ������ϵ��.<br>
	 * 2> �����ʧ����ϵ�˵Ĳ����ļ���ɾ������ϵ�˰󶨵Ĳ���ID, �ȴ��´θ������ز���ʱ�������ض�ʧ�Ĳ����ļ�.
	 */
	public void init() {
		parseAndLoadCcBook();
	}

	/**
	 * ��֤����绰����һ����ϵ�˵�����������.<br>
	 * 1> �����ֻ����к���, ��˺��벻��������CcBook��ɾ������ϵ��.<br>
	 * 2> �����ʧ����ϵ�˵Ĳ����ļ���ɾ������ϵ�˰󶨵Ĳ���ID, �ȴ��´θ������ز���ʱ�������ض�ʧ�Ĳ����ļ�.
	 * 
	 * 
	 * @param people
	 *            ����绰���е���ϵ��.
	 * @return
	 */
	private boolean verifyCcPeople(CcPeople people) {
		return false;
	}

	/**
	 * ��֤��������绰��.
	 */
	private void verifyIntegrality() {

	}

	/**
	 * ��������绰��.<br>
	 * �����һ���������û�ѡ�����ɲ���绰�� ��У�������Ժ��������ɲ���绰��.
	 */
	public void createCcBook(Hashtable ccPeoples) {

		writeCcBookFile();
	}

	/**
	 * ������绰���������ϵ��.<br>
	 * ʵ����ֻ�Ǽ��뵽�����в�δ�������Ĳ���绰���ļ�. Ӧ���ʵ���ʱ�򽫸���д���ļ�.
	 * 
	 * @param people
	 */
	public void addCcPeople(CcPeople people) {

	}

	/**
	 * ɾ������绰����һ����ϵ��.<br>
	 * ʵ����ֻ�Ǽ��뵽�����в�δ�������Ĳ���绰���ļ�. Ӧ���ʵ���ʱ�򽫸���д���ļ�.
	 */
	public void deleteCcPeople(CcPeople people) {

	}

	/**
	 * ��������ϵ���б�д�뵽����绰��xml�ļ�.
	 */
	public void writeCcBookFile() {

	}

	/**
	 * ������������绰�������ڴ�.
	 */
	private void parseAndLoadCcBook() {

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

		writeCcBookFile();
	}

}
