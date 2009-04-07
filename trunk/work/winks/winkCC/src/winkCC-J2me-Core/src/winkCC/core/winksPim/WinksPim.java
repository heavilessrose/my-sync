package winkCC.core.winksPim;

import java.util.Enumeration;
import java.util.Hashtable;

import winkCC.pim.PhoneContact;
import winkCC.pim.PhoneContactUtil;

/**
 * ҵ����ֻ��绰��������װ.
 * 
 * @author WangYinghua
 * 
 */
public class WinksPim {

	SE_numberFilter _filter = null;

	/**
	 * �õ��ֻ��绰����������ϵ�˵�������绰��.
	 * 
	 * @return key:name, value:no
	 */
	public Hashtable getAllPhoneContacts() {
		Hashtable pair = new Hashtable();
		Hashtable phoneContact = PhoneContactUtil.getAllPhoneContacts();
		Enumeration contactEnum = phoneContact.elements();
		while (contactEnum.hasMoreElements()) {
			PhoneContact p = (PhoneContact) contactEnum.nextElement();
			pair.put(p.getName(), p.getPrimaryNum(_filter));
		}
		return pair;
	}

}
