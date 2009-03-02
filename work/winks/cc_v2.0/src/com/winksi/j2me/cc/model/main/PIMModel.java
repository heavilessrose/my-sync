package com.winksi.j2me.cc.model.main;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

import javax.microedition.pim.Contact;
import javax.microedition.pim.Event;
import javax.microedition.pim.PIM;
import javax.microedition.pim.PIMException;
import javax.microedition.pim.PIMItem;
import javax.microedition.pim.PIMList;
import javax.microedition.pim.ToDo;

/** 电话簿操作 */
public class PIMModel extends Thread {

	private GUIController controller;
	private PimListener listener;
	private Hashtable uidHt = new Hashtable();
	private Hashtable fullNameHt = new Hashtable();
	/** 保存电话号码及对应彩像 */
	private Hashtable ht = new Hashtable();
	private int event = 0;
	/** 所有通讯录的名字 */
	private String[] listsNames;
	private int listsIndex = 0;
	private String listsitem;
	
	private PIM pim;
	/** 某个通讯录 */
	private PIMList list;
	//    private Vector itemsname = new Vector();
	/** 所有通讯录 */
	private Vector[] contactLists;
	//    private Vector[] contractlist;
	/** 联系人 */
	private PIMItem item;

	private boolean running = true;
	private boolean complete = false;

	public PIMModel(GUIController controller, PimListener listener) {
		this.controller = controller;
		this.listener = listener;
		pim = PIM.getInstance();
	}

	public void running() {
		running = true;
	}

	public void stop() {
		running = false;
	}

	public boolean isComplete() {
		return complete;
	}

	/*public PIMModel getInstances(){
	 instances=new PIMModel();
	 return instances;
	 }*/
	public void run() {
		if (running) {
			try {
				//                lists = pim.listPIMLists(PIM.CONTACT_LIST);
				int count = 0;
				while (true) {
					count++;
					// 得到所有通讯录的名字
					listsNames = pim.listPIMLists(PIM.CONTACT_LIST);
					if (count > 3) {
						break;
					}
				}

				contactLists = new Vector[listsNames.length];
				//        contractlist = new Vector[lists.length];
				for (int i = 0; i < listsNames.length; i++) {
					//                    System.out.println("pim lists length : " + lists.length );
					contactLists[i] = new Vector();
					//            contractlist[i] = new Vector();
					count = 0;
					while (true) {
						count++;
						// 根据通讯录名字 打开指定通讯录
						list = pim.openPIMList(PIM.CONTACT_LIST, PIM.READ_WRITE, listsNames[i]);
						if (count > 3) { // 为什么要做4次?
							break;
						}
					}

					Enumeration e = list.items();

					//                    item = null;                    
					count = 0;
					while (e.hasMoreElements()) {
						count++;
						//                        System.out.println("count : " + count );
						item = (PIMItem) e.nextElement();
						contactLists[i].addElement(item);
						populate(item);
					}
				}
			} catch (PIMException e) {
				controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
				//                e.printStackTrace();
			} catch (SecurityException se) {
				controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
			}
			controller.initPim();
		}
	}

	/** 准备每个联系人的数据 */
	private void populate(PIMItem item)/* throws PIMException */{
		int[] fields = item.getPIMList().getSupportedFields();
		//        boolean allFieldsUsed = true;

		// 该item的所有field 的值
		Vector[] fieldsValue = new Vector[fields.length];

		for (int i = 0; i < fields.length; i++) {
			fieldsValue[i] = new Vector();
			int curField = fields[i];

			if (isClassField(curField)) {
				continue;
			}

			// curField的 数据值的个数
			int curFieldValueCount = item.countValues(curField);

			// curField的数据类型
			int dataType = item.getPIMList().getFieldDataType(curField);
			// curField的label
			String label = item.getPIMList().getFieldLabel(curField);
			///////////debug
//			System.out.println("curFieldValueCount: " + curFieldValueCount);
//			System.out.println("dataType: " + dataType);
//			System.out.println("curField lable: " + label);
			//////////
			//            String uid="";
			//            System.out.println("label : "+label);
			fieldsValue[i].addElement(label);
			
			if (curFieldValueCount == 0) {
				//                allFieldsUsed = false;
				continue;
			}

			for (int j = 0; j < curFieldValueCount; j++) {
				switch (dataType) {
				case PIMItem.STRING: {
					String sValue = item.getString(curField, j).toLowerCase();

					if (sValue == null) {
						sValue = "";
					}
					
					////////////////debug
//					System.out.println("sValue: " + sValue);
					//////////////
					
					fieldsValue[i].addElement(sValue);
					break;
				}
				default:
					break;

				//                    case PIMItem.BOOLEAN: {
				////                        formItem = new StringItem(label, item.getBoolean(field, j) ? "yes" : "no");
				////                        formItem.setDefaultCommand(editBooleanCommand);
				//
				//                        break;
				//                    }

				//                    case PIMItem.STRING_ARRAY: {
				//                        String[] a = item.getStringArray(field, j);
				//
				//                        if (a != null) {
				////                            formItem = new StringItem(label, joinStringArray(a));
				////                            formItem.setDefaultCommand(editArrayCommand);
				//                        }
				//
				//                        break;
				//                    }

				//                    case PIMItem.DATE: {
				//                        long time = item.getDate(field, j);
				//                        break;
				//                    }

				//                    case PIMItem.INT: {
				////                        formItem = new TextField(label, String.valueOf(item.getInt(field, j)), 64,
				////                                TextField.DECIMAL);
				//
				//                        break;
				//                    }

				//                    case PIMItem.BINARY: {
				////                        System.out.println("field=" + field);
				//                        byte[] data = item.getBinary(field, j);
				//
				//                        if (data != null) {
				////                            formItem = new StringItem(label, data.length + " bytes");
				//                        }
				//
				//                        break;
				//                    }
				}
			}
		}
		parseContact(fieldsValue);
		//        return v;
	}

	/** 屏蔽CLASS常量, CLASS常量表示此联系人的可访问级别 */
	private boolean isClassField(int field) {
		return (item instanceof Contact && (field == Contact.CLASS))
				|| (item instanceof Event && (field == Event.CLASS))
				|| (item instanceof ToDo && (field == ToDo.CLASS));
	}

	public void setEvent(int event) {
		this.event = event;
	}

	public void setLists(String lists) {
		this.listsitem = lists;
	}

	public String[] getPIMLists() {
		return listsNames;
	}

	public Vector[] getItems() {
		return contactLists;
	}

	public Hashtable getContactHashtable() {
		return ht;
	}

	public Hashtable getUIDHashtable() {
		return uidHt;
	}

	public Hashtable getFullHashtable() {
		return fullNameHt;
	}

	/** 为联系人匹配彩像 */
	public void parseContact(Vector[] fieldsValue) {
		String uid = fieldsValue[CPProperty.UID].elementAt(1).toString();
		//        System.out.println("uid : "+uid);
		String imageurl = "";
		String fullname = "";

		if (fieldsValue[CPProperty.FULLNAME].size() > 1) {
			fullname = fieldsValue[CPProperty.FULLNAME].elementAt(1).toString();
			// 将名字实时显示到屏幕
			listener.setMessage(fullname);
			//            System.out.println("fullname="+fullname);
		}

		if (fieldsValue[CPProperty.PICTURE].size() > 1) {
			imageurl = fieldsValue[CPProperty.PICTURE].elementAt(1).toString();
		}

		// 至少有一个号码才绑定号码与图片
		if (fieldsValue[CPProperty.NUMBER].size() > 1) {
			fullNameHt.put(fullname, fieldsValue[CPProperty.NUMBER]);
			// 多个号码绑定同一个图片
			for (int y = 1; y < fieldsValue[CPProperty.NUMBER].size(); y++) {
				String num = fieldsValue[CPProperty.NUMBER].elementAt(y).toString();
//				System.out.println("UID: " + uid + " number: " + num + "pic: " + imageurl);
				uidHt.put(uid, num);
				ht.put(num, imageurl);
			}
		}
	}

	public void changeImage(String number, String imgname, int mode) {
		//        System.out.println("change number : "+number+" image : "+imgname+" mode : "+mode);
		boolean ischange = false;
		int len = contactLists[0].size();
		//        PIMItem item = null;
		String path = "file:///" + CPProperty.SAVEPATH + "/" + imgname;
		//        path="file:///c:/pictures/Pearlz.gif";
		for (int i = 0; i < len; i++) {
			PIMItem pimItem = (PIMItem) contactLists[0].elementAt(i);

			int field = Contact.TEL;

			int count = pimItem.countValues(field);
			for (int j = 0; j < count; j++) {
				String ss = pimItem.getString(field, j);
				int numberlen = number.length();
				int sslen = ss.length();
				if (mode == 1) {
					//                    System.out.println("change pim : " + ss);
					if (numberlen >= CPProperty.MATCH_MAX) {
						if (sslen >= CPProperty.MATCH_MAX) {
							String ss_sub = ss.substring(sslen - CPProperty.MATCH_MAX, sslen);
							//                            System.out.println("ss_sub : " + ss_sub);
							String number_sub = number.substring(numberlen - CPProperty.MATCH_MAX,
									numberlen);
							//                            System.out.println("number_sub : " + number_sub);
							if (ss_sub.equals(number_sub)) {
								ischange = true;
								break;
							}
						}
					} else {
						if (ss.equals(number)) {
							ischange = true;
							break;
						}
					}
				} else if (mode == 2) {
					//                    System.out.println("number : "+number+" ss: "+ss);
					//                    if(ss.length()>number.length()){
					//                        String ss_sub=ss.substring(sslen-numberlen, sslen);
					//                        if(ss_sub.equals(number)){
					//                            ischange = true;
					//                            break;
					//                        }
					//                    } else {
					if (ss.equals(number)) {
						ischange = true;
						break;
					}
					//                    }
				}
			}

			if (ischange) {
				field = Contact.PHOTO_URL;
				try {
					int dataType = pimItem.getPIMList().getFieldDataType(field);
					if (dataType == PIMItem.STRING) {
						try {
							pimItem.addString(Contact.PHOTO_URL, 0, path);
						} catch (Exception e) {
							pimItem.setString(Contact.PHOTO_URL, 0, PIMItem.ATTR_NONE, path);
						}
						//                        System.out.println("commit : "+path);
						// 数据更新后提交
						pimItem.commit();
						break;
					}
				} catch (PIMException ex) {
					ex.printStackTrace();
				} catch (IllegalArgumentException e) {
					// this was a read-only field (UID)
				} catch (Exception e) {
					//                    System.out.println("ee: "+e.toString());
				}
			}
		}
	}

	public void delImage(String number) {
		boolean ischange = false;
		int len = contactLists[0].size();

		for (int i = 0; i < len; i++) {
			PIMItem pimItem = (PIMItem) contactLists[0].elementAt(i);

			int field = Contact.TEL;

			int count = pimItem.countValues(field);
			for (int j = 0; j < count; j++) {
				String ss = pimItem.getString(field, j);
				if (ss.equals(number)) {
					ischange = true;
					break;
				}
			}

			if (ischange) {
				//                field = fields[CPProperty.PICTURE];
				field = Contact.PHOTO_URL;
				try {
					int dataType = pimItem.getPIMList().getFieldDataType(field);
					if (dataType == PIMItem.STRING) {
						try {
							pimItem.removeValue(field, 0);
						} catch (Exception e) {
						}
						pimItem.commit();
					}
				} catch (PIMException ex) {
					ex.printStackTrace();
				} catch (IllegalArgumentException e) {
					// this was a read-only field (UID)
				}
			}
		}
	}
	//    public static Vector matchPim(Hashtable ht1, Hashtable ht2) {
	//        Vector vector = new Vector();
	////        ht1.put("010", "");
	//        Enumeration e = ht1.keys();
	//        while (e.hasMoreElements()) {
	//            String key = e.nextElement().toString();
	//            if (!ht2.containsKey(key)) {
	//                vector.addElement(key);
	//            }
	//        }
	//        return vector;
	//    }
}
