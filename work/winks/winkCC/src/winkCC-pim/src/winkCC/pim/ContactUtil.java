/**
 * 
 */

package winkCC.pim;

import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.pim.Contact;
import javax.microedition.pim.ContactList;
import javax.microedition.pim.PIM;
import javax.microedition.pim.PIMException;
import javax.microedition.pim.PIMItem;

public class ContactUtil {

	/**
	 * 所有通讯录, 可能有一个或多个通讯录.
	 */
	ContactList[] contactLists = null;
	/**
	 * 所有联系人
	 */
	Hashtable allPeople = new Hashtable();

	private static final int ATTR_MOBILE = 144;

	//	private static final int ATTR_HOME = 8;
	//	private static final int ATTR_WORK = 512;
	//	private static final int ATTR_FAX = 4;
	//	private static final int ATTR_OTHER = 32;

	/**
	 * 测试
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
					| Contact.ATTR_PREFERRED, "jqpublic@xyz.dom1.com");
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
	 * 打开通讯录
	 * 
	 * @return
	 */
	private ContactList[] getContactList() {
		if (contactLists != null) {
			return contactLists;
		} else {
			// 所有通讯录的名字
			String[] contactListNames = PIM.getInstance().listPIMLists(
					PIM.CONTACT_LIST);
			contactLists = new ContactList[contactListNames.length];
			try { // 打开一个通讯录
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
	 * 得到联系人的显示姓名
	 * 
	 * @param contact
	 * @return
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
	 * 整理字段的显示值
	 * 
	 * @param contact
	 */
	private void fixDisplayName(Contact contact) {

		// FORMATTED_NAME字段是否已经定义
		boolean defined = false;

		if (contact.countValues(Contact.FORMATTED_NAME) != 0) {
			String s = contact.getString(Contact.FORMATTED_NAME, 0);

			if ((s != null) && (s.trim().length() > 0)) {
				defined = true;
			}
		}

		// 如果FORMATTED_NAME字段没有定义
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
						// 给联系人添加FORMATTED_NAME字段
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
	 * 得到一个联系人的所有号码.
	 * 
	 * @param contact
	 * @return
	 */
	private Hashtable getNumbers(Contact contact) {
		Hashtable numbers = null;
		if (contact.getPIMList().isSupportedField(Contact.TEL)) {
			numbers = new Hashtable();
			int count = contact.countValues(Contact.TEL);
			for (int i = 0; i < count; i++) {
				String num = contact.getString(Contact.TEL, i);
				//				numbers.put("" + i, num);

				int attr = contact.getAttributes(Contact.TEL, i);
				if (attr == ATTR_MOBILE) {
					numbers.put("mobile", num);
				} else if (attr == Contact.ATTR_HOME) {
					numbers.put("home", num);
				} else if (attr == Contact.ATTR_WORK) {
					numbers.put("work", num);
				} else if (attr == Contact.ATTR_FAX) {
					numbers.put("fax", num);
				} else if (attr == Contact.ATTR_OTHER) {
					numbers.put("other", num);
				} else {// 非以上分类
					numbers.put("" + attr, num);
				}

				//////debug
				//				System.out.println(i + ">> " + attr + " >> " + num);
			}

			//			String mobile = contact.getString(Contact.TEL, 0);
			//			String home = contact.getString(Contact.TEL, Contact.ATTR_HOME);
			//			String work = contact.getString(Contact.TEL, Contact.ATTR_WORK);
			//			String other = contact.getString(Contact.TEL, Contact.ATTR_OTHER);
			//			String fax = contact.getString(Contact.TEL, Contact.ATTR_FAX);
			//			if (mobile != null && !mobile.equals(""))
			//				numbers.put("mobile", mobile);
			//			if (home != null && !home.equals(""))
			//				numbers.put("home", home);
			//			if (work != null && !work.equals(""))
			//				numbers.put("work", work);
			//			if (other != null && !other.equals(""))
			//				numbers.put("other", other);
			//			if (fax != null && !fax.equals(""))
			//				numbers.put("fax", fax);
		}
		return numbers;
	}

	public String getNumber(People people, String attr) {
		return people.getNumber(attr);
	}

	/**
	 * 根据姓名得到People对象.
	 * 
	 * @param name
	 * @return
	 */
	public People getPeople(String name) {
		People people = null;
		if (getAllPeople().containsKey(name)) {
			people = (People) allPeople.get(name);
		}
		return people;
	}

	//	/**
	//	 * 得到主要电话号码.
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
	 * 得到联系人的大头贴地址字符串.
	 * @param contact
	 */
	private String getWPath(Contact contact) {
		String wpath = null;
		if (contact.getPIMList().isSupportedField(Contact.PHOTO_URL)) {
			int count = contact.countValues(Contact.PHOTO_URL);
			if (count > 0)
				for (int i = 0; i < count; i++) {
					wpath = contact.getString(Contact.PHOTO_URL, 0);
				}
		}
		if (wpath == null || wpath == "")
			wpath = "";
		return wpath;
	}

	/**
	 * 得到手机话簿中所有联系人.
	 */
	public Hashtable getAllPeople() {
		if (allPeople != null && allPeople.size() > 0) {
			return allPeople;
		}
		synchronized (ContactUtil.this) {
			try {
				// 清空items
				if (!allPeople.isEmpty())
					allPeople.clear();

				String showName = null;
				Hashtable numbers = new Hashtable();
				String wPath = null;
				contactLists = getContactList();
				for (int i = 0; i < contactLists.length; i++) {
					// 遍历所有通讯录, 得到所有联系人
					for (Enumeration items = contactLists[i].items(); items
							.hasMoreElements();) {
						Contact item = (Contact) items.nextElement();

						showName = getDisplayName(item);
						numbers = getNumbers(item);
						wPath = getWPath(item);
						if (showName == null) {
							showName = "<Incomplete data>";
						}
						allPeople.put(showName, new People(showName, numbers,
								wPath));
					}
				}
			} catch (PIMException e) {
			}
		}
		return allPeople;
	}
}
