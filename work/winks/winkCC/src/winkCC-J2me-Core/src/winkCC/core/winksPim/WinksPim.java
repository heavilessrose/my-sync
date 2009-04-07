package winkCC.core.winksPim;

import java.util.Enumeration;
import java.util.Hashtable;

import winkCC.pim.PhoneContact;
import winkCC.pim.PhoneContactUtil;

/**
 * 业务层手机电话本操作封装.
 * 
 * @author WangYinghua
 * 
 */
public class WinksPim {

	SE_numberFilter _filter = null;

	/**
	 * 得到手机电话簿中所有联系人的名字与电话对.
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
