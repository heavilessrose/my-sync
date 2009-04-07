package winkCC.pim;

import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.pim.Contact;
import javax.microedition.pim.ContactList;
import javax.microedition.pim.PIM;
import javax.microedition.pim.PIMException;
import javax.microedition.pim.PIMItem;

/**
 * �����ֻ��绰���ķ�����װ.
 * 
 * @author WangYinghua
 * 
 */
public class ContactUtil {

	/**
	 * ����ͨѶ¼, ������һ������ͨѶ¼.
	 */
	ContactList[] contactLists = null;
	/**
	 * ������ϵ��
	 */
	Hashtable allContacts = new Hashtable();

	/*
	 * ����ƽ̨����ӵ�ж���������ϵ��, ��default number�������(ԭ������128ȡ��).
	 */
	private static final int ATTR_DEFAULT_MOBILE = Contact.ATTR_MOBILE | 128;
	private static final int ATTR_DEFAULT_HOME = Contact.ATTR_HOME | 128;
	private static final int ATTR_DEFAULT_WORK = Contact.ATTR_WORK | 128;
	private static final int ATTR_DEFAULT_FAX = Contact.ATTR_FAX | 128;
	private static final int ATTR_DEFAULT_OTHER = Contact.ATTR_OTHER | 128;

	/**
	 * ������ϵ���ֶ�֧�����.
	 */
	public static void test() {
		ContactList contacts = null;
		try {
			contacts = (ContactList) PIM.getInstance().openPIMList(
					PIM.CONTACT_LIST, PIM.READ_WRITE);
		} catch (PIMException e) {
			// An error occurred
			return;
		}
		Contact contact = contacts.createContact();
		String[] addr = new String[contacts.stringArraySize(Contact.ADDR)];
		String[] name = new String[contacts.stringArraySize(Contact.NAME)];

		if (contacts.isSupportedField(Contact.FORMATTED_NAME)) {
			System.out.println("support FORMATTED_NAME");
			contact.addString(Contact.FORMATTED_NAME, PIMItem.ATTR_NONE,
					"formatted name");
		}
		if (contacts.isSupportedArrayElement(Contact.NAME, Contact.NAME_FAMILY)) {
			System.out.println("support NAME_FAMILY");
			name[Contact.NAME_FAMILY] = "familyName";
		}
		if (contacts.isSupportedArrayElement(Contact.NAME, Contact.NAME_GIVEN)) {
			System.out.println("support NAME_GIVEN");
			name[Contact.NAME_GIVEN] = "givenName";
		}
		contact.addStringArray(Contact.NAME, PIMItem.ATTR_NONE, name);
		if (contacts.isSupportedField(Contact.TEL)) {
			System.out.println("support TEL");
			contact.addString(Contact.TEL, Contact.ATTR_MOBILE, "15801630382");
			contact.addString(Contact.TEL, Contact.ATTR_HOME, "2222");
			contact.addString(Contact.TEL, Contact.ATTR_WORK, "58111");
			contact.addString(Contact.TEL, Contact.ATTR_OTHER, "1111");
			contact.addString(Contact.TEL, Contact.ATTR_FAX, "2222");
		}
		if (contacts.isSupportedField(Contact.PHOTO)) {
			System.out.println("support PHOTO");
		}
		if (contacts.isSupportedField(Contact.PHOTO_URL)) {
			System.out.println("support PHOTO_URL");
			try {
				contact.addString(Contact.PHOTO_URL, Contact.ATTR_NONE,
						"file:///e:/other/sss.bmp");
			} catch (Exception e) {
				e.printStackTrace();
				contact.setString(Contact.PHOTO_URL, 0, Contact.ATTR_NONE,
						"file:///e:/other/sss.bmp");
			}
		}
		///////////////////////////////
		if (contacts
				.isSupportedArrayElement(Contact.ADDR, Contact.ADDR_COUNTRY)) {
			System.out.println("support ADDR_COUNTRY");
			addr[Contact.ADDR_COUNTRY] = "country";
		}
		if (contacts.isSupportedArrayElement(Contact.ADDR,
				Contact.ADDR_LOCALITY)) {
			System.out.println("support ADDR_LOCALITY");
			addr[Contact.ADDR_LOCALITY] = "locality";
		}
		if (contacts.isSupportedArrayElement(Contact.ADDR,
				Contact.ADDR_POSTALCODE)) {
			System.out.println("support ADDR_POSTALCODE");
			addr[Contact.ADDR_POSTALCODE] = "91921-1234";
		}
		if (contacts.isSupportedArrayElement(Contact.ADDR, Contact.ADDR_STREET)) {
			System.out.println("support ADDR_STREET");
			addr[Contact.ADDR_STREET] = "street";
		}
		if (contacts.isSupportedField(Contact.ADDR)) {
			System.out.println("support ADDR");
			contact.addStringArray(Contact.ADDR, Contact.ATTR_HOME, addr);
		}
		try {
			if (contacts.maxCategories() != 0) {
				System.out.println("support Categories");
				if (contacts.isCategory("Friends")) {
					System.out.println("support Categories Friends");
					contact.addToCategory("Friends");
				}
			}
		} catch (PIMException e1) {
			e1.printStackTrace();
		}
		if (contacts.isSupportedField(Contact.BIRTHDAY)) {
			System.out.println("support BIRTHDAY");
			contact.addDate(Contact.BIRTHDAY, PIMItem.ATTR_NONE, new Date()
					.getTime());
		}
		if (contacts.isSupportedField(Contact.EMAIL)) {
			System.out.println("support EMAIL");
			contact.addString(Contact.EMAIL, Contact.ATTR_HOME
					| Contact.ATTR_PREFERRED, "hit456@gmail.com");
		}
		try {
			contact.commit();
		} catch (PIMException e) {
			e.printStackTrace();
		}
		try {
			contacts.close();
		} catch (PIMException e) {
		}

	}

	/**
	 * �õ��ֻ��е�����ͨѶ¼.
	 * 
	 * @return ContactList[] �ֻ�����ͨѶ¼����.
	 */
	private ContactList[] getPhoneBooks() {
		if (contactLists != null) {
			return contactLists;
		} else {
			// ����ͨѶ¼������
			String[] contactListNames = PIM.getInstance().listPIMLists(
					PIM.CONTACT_LIST);
			contactLists = new ContactList[contactListNames.length];
			try { // ��һ��ͨѶ¼
				for (int i = 0; i < contactListNames.length; i++) {
					contactLists[i] = (ContactList) PIM.getInstance()
							.openPIMList(PIM.CONTACT_LIST, PIM.READ_WRITE,
									contactListNames[i]);
				}
			} catch (PIMException e) {
				e.printStackTrace();
			}
			return contactLists;
		}
	}

	/**
	 * �õ���ϵ�˵���ʾ����
	 * 
	 * @param contact
	 *            ��ϵ��.
	 * @return showName ��ϵ�˵�����.
	 * @see winkCC.pim.ContactUtil#fixDisplayName(Contact)
	 */
	private String getDisplayName(Contact contact) {
		fixDisplayName(contact);

		String showName = null;

		if (contact.countValues(Contact.FORMATTED_NAME) != 0) {
			String s = contact.getString(Contact.FORMATTED_NAME, 0);

			if ((s != null) && (s.trim().length() != 0)) {
				showName = s;
			}
		}

		return showName;
	}

	/**
	 * ���������ֶε���ʾֵ. <br>
	 * Ĭ��ʹ��Contact.FORMATTED_NAME, ������ֶβ����������ϵ�˽������ֶ�: Contact.NAME_GIVEN + " "
	 * + Contact.NAME_FAMILY
	 * 
	 * @param contact
	 */
	private void fixDisplayName(Contact contact) {

		// FORMATTED_NAME�ֶ��Ƿ��Ѿ�����
		boolean defined = false;

		if (contact.countValues(Contact.FORMATTED_NAME) != 0) {
			String s = contact.getString(Contact.FORMATTED_NAME, 0);

			if ((s != null) && (s.trim().length() > 0)) {
				defined = true;
			}
		}

		// ���FORMATTED_NAME�ֶ�û�ж���
		if (!defined) {
			if (contact.countValues(Contact.NAME) != 0) {
				String[] name = contact.getStringArray(Contact.NAME, 0);

				if (name != null) {
					StringBuffer sb = new StringBuffer();

					if (name[Contact.NAME_GIVEN] != null) {
						sb.append(name[Contact.NAME_GIVEN]);
					}

					if (name[Contact.NAME_FAMILY] != null) {
						if (sb.length() > 0) {
							sb.append(" ");
						}

						sb.append(name[Contact.NAME_FAMILY]);
					}

					String s = sb.toString().trim();

					if (s.length() > 0) {
						// ����ϵ�����FORMATTED_NAME�ֶ�
						if (contact.countValues(Contact.FORMATTED_NAME) == 0) {
							contact.addString(Contact.FORMATTED_NAME,
									Contact.ATTR_NONE, s);
						} else {
							contact.setString(Contact.FORMATTED_NAME, 0,
									Contact.ATTR_NONE, s);
						}
					}
				}
			}
		}
	}

	/**
	 * �õ�һ����ϵ�˵����к���.
	 * 
	 * @param contact
	 * @return ������ϵ�����к����Hashtable, ��:mobile:1580000
	 */
	private Hashtable getAllNumbers(Contact contact) {
		Hashtable numbers = null;
		if (contact.getPIMList().isSupportedField(Contact.TEL)) {
			numbers = new Hashtable();
			int count = contact.countValues(Contact.TEL);
			String num = null;
			int attr = 0;
			for (int i = 0; i < count; i++) {
				num = contact.getString(Contact.TEL, i);
				attr = contact.getAttributes(Contact.TEL, i);
				if (attr == Contact.ATTR_MOBILE || attr == ATTR_DEFAULT_MOBILE) {
					numbers.put("mobile", num);
				}
				if (attr == Contact.ATTR_HOME || attr == ATTR_DEFAULT_HOME) {
					numbers.put("home", num);
				}
				if (attr == Contact.ATTR_WORK || attr == ATTR_DEFAULT_WORK) {
					numbers.put("work", num);
				}
				if (attr == Contact.ATTR_FAX || attr == ATTR_DEFAULT_FAX) {
					numbers.put("fax", num);
				}
				if (attr == Contact.ATTR_OTHER || attr == ATTR_DEFAULT_OTHER) {
					numbers.put("other", num);
				}

				//////debug
				//				System.out.println(i + ">> " + attr + " >> " + num);
			}
		}
		return numbers;
	}

	/**
	 * �õ�ָ����ϵ�˵�ָ�����Եĵ绰����.
	 * 
	 * @param people
	 *            ָ����ϵ��.
	 * @param attr
	 *            ָ���绰����,��: mobile, home, work, fax, other.
	 * @return ��ϵ�˺���string
	 */
	public String getNumber(PhoneContact people, String attr) {
		return people.getNumber(attr);
	}

	/**
	 * ����ָ������ȡ�ø���ϵ�� ָ�����ĵ绰����.
	 * 
	 * @param name
	 *            ��ϵ������
	 * @param attr
	 *            Ҫ�õ��ĵ绰���
	 * @return ��ϵ�˺���String
	 */
	public String getNumber(String name, String attr) {
		return getNumber(getPhoneContact(name), attr);
	}

	/**
	 * �õ�ָ����ϵ�˵Ĵ�ͷ����ַ.
	 * 
	 * @param people
	 * @return ��ϵ�˴�ͷ����ַString.
	 */
	public String getWPath(PhoneContact people) {
		return people.getWPath();
	}

	/**
	 * ������ϵ�����ֵõ�����ϵ�˵Ĵ�ͷ����ַ.
	 * 
	 * @param name
	 * @return ��ϵ�˴�ͷ��String.
	 */
	public String getWPath(String name) {
		return getWPath(getPhoneContact(name));
	}

	/**
	 * �趨ָ����ϵ�˵Ĵ�ͷ����ַ.
	 * 
	 * @param name
	 * @param wpath
	 */
	public void setWPath(String name, String wpath) {

		contactLists = getPhoneBooks();
		for (int i = 0; i < contactLists.length; i++) {
			// ��������ͨѶ¼, �õ�������ϵ��
			try {
				for (Enumeration items = contactLists[i].items(); items
						.hasMoreElements();) {
					Contact item = (Contact) items.nextElement();

					String showName = getDisplayName(item);
					if (showName.equals(name)) {
						if (contactLists[i].isSupportedField(Contact.PHOTO_URL)) {
							try {
								item.addString(Contact.PHOTO_URL,
										Contact.ATTR_NONE, wpath);
							} catch (Exception e) {
								//								e.printStackTrace();
								item.setString(Contact.PHOTO_URL, 0,
										Contact.ATTR_NONE, wpath);
							}
						}
						try {
							item.commit();
						} catch (PIMException e) {
							e.printStackTrace();
						}
					}
				}
			} catch (PIMException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		PhoneContact phonecontact = getPhoneContact(name);
		phonecontact.setWPath(wpath);
		// ���� allPeople
		if (allContacts.size() <= 0)
			getAllPhoneContacts();
		allContacts.remove(name);
		allContacts.put(name, phonecontact);

		System.out.println("set wpath success");
	}

	/**
	 * �������ֵõ�PhoneContact����.
	 * 
	 * @param name
	 * @return ��ϵ��People����
	 */
	public PhoneContact getPhoneContact(String name) {
		PhoneContact phoneContact = null;
		if (getAllPhoneContacts().containsKey(name)) {
			phoneContact = (PhoneContact) allContacts.get(name);
		}
		return phoneContact;
	}

	//	/**
	//	 * �õ���Ҫ�绰����.
	//	 * 
	//	 * @param contact
	//	 * @param filter
	//	 * @return
	//	 */
	//	private String getPrimaryNumber(Contact contact) {
	//		String num = null;
	//		if (contact.countValues(Contact.TEL) != 0) {
	//			num = contact.getString(Contact.TEL, Contact.ATTR_MOBILE);
	//			System.out.println("num: " + num);
	//		}
	//		return num;
	//	}

	//	private void getField(Contact contact, int field) {
	//		switch (contact.getPIMList().getFieldDataType(field)) {
	//		case Contact.STRING:
	//			contact.getString(field, 0);
	//			break;
	//		case Contact.INT:
	//			contact.getInt(field, 0);
	//			break;
	//		case Contact.STRING_ARRAY:
	//			contact.getStringArray(field, 0);
	//			break;
	//		case Contact.BINARY:
	//			contact.getBinary(field, 0);
	//			break;
	//		case Contact.BOOLEAN:
	//			contact.getBoolean(field, 0);
	//			break;
	//		case Contact.DATE:
	//			contact.getDate(field, 0);
	//			break;
	//		}
	//	}

	/**
	 * �õ���ϵ�˵Ĵ�ͷ����ַ�ַ���.
	 * 
	 * @param contact
	 */
	private String getPicPath(Contact contact) {
		String path = null;
		if (contact.getPIMList().isSupportedField(Contact.PHOTO_URL)) {
			int count = contact.countValues(Contact.PHOTO_URL);
			if (count > 0)
				for (int i = 0; i < count; i++) {
					path = contact.getString(Contact.PHOTO_URL, 0);
				}
		}
		if (path == null || path == "")
			path = "";
		return path;
	}

	/**
	 * �õ��ֻ�������������ϵ��.
	 * 
	 * @return ��������PhoneContact�����Hashtable
	 */
	public Hashtable getAllPhoneContacts() {
		if (allContacts != null && allContacts.size() > 0) {
			return allContacts;
		}
		synchronized (ContactUtil.this) {
			try {
				// ���items
				if (!allContacts.isEmpty())
					allContacts.clear();

				String showName = null;
				Hashtable allNumber = new Hashtable();
				String wPath = null;
				contactLists = getPhoneBooks();
				for (int i = 0; i < contactLists.length; i++) {
					// ��������ͨѶ¼, �õ�������ϵ��
					for (Enumeration items = contactLists[i].items(); items
							.hasMoreElements();) {
						Contact item = (Contact) items.nextElement();

						showName = getDisplayName(item);
						allNumber = getAllNumbers(item);
						wPath = getPicPath(item);
						if (showName == null) {
							showName = "<Incomplete data>";
						}
						allContacts.put(showName, new PhoneContact(showName,
								allNumber, wPath));
					}
					System.out.println("contact count: " + allContacts.size());
				}
			} catch (PIMException e) {
			}
		}
		return allContacts;
	}
}
