package py.wmw.ui;

import java.beans.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.Serializable;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import java.io.IOException;
import py.pinying.*;

public class WComBoxKeySelectManager implements JComboBox.KeySelectionManager,
		Serializable {
	public JComboBox jComboBox;
	String Skey = "";
	long intime = 0;
	String[] OldItem = null;
	String[] Itempy = null;
	py.pinying.pybean pybean = new py.pinying.pybean(); // 第一步: 得到处理对象

	public WComBoxKeySelectManager(JComboBox jComboBox) {
		this.jComboBox = jComboBox;
	}

	// this code is hard to you for read
	public String GetFirstselection(char aKey, ComboBoxModel aModel) {
		int i = selectionForKey(aKey, aModel);
		if (aModel.getElementAt(i) != null) {
			return aModel.getElementAt(i).toString();
		}
		return "";
	}

	public int selectionForKey(char aKey, ComboBoxModel aModel) {
		try {// System.out.println(aModel);
			if (aModel.getSize() == 0) {
				return -1;
			}
			if (aKey == '\b' || aKey == '\n' || aKey == ' ' || aKey == '\uFFFF') {
				Skey = "";
				// jComboBox.setPopupVisible(false);
				return -1;
			} else if (System.currentTimeMillis() - intime > py.wmw.ui.ToolKit
					.GetTimeOut()) {
				Skey = "" + aKey;
				intime = System.currentTimeMillis();
			} else {
				Skey = Skey + aKey;
				intime = System.currentTimeMillis();
			}
			String tin = "";
			OldItem = new String[aModel.getSize()];
			for (int i = 0; i < aModel.getSize(); i++) {
				Object elem = aModel.getElementAt(i);
				if (elem != null) {
					tin = py.wmw.ui.ToolKit.UItoolinterface
							.ObjectToString(elem);
				} else {
					tin = "";
				}
				OldItem[i] = tin;
			}

			// }
			this.getClass().getDeclaringClass();
			// Itempy = pybean.getSomeSM(OldItem); //第二步 得到拼音首字符

			int intlist0[] = pybean.getIndexByHanzi(OldItem, Skey); // 第三步
			// 得到排序后的字符数组
			Object elemchanc;
			java.util.Vector v = new java.util.Vector();
			for (int i = 0; i < aModel.getSize(); i++) {
				v.add(aModel.getElementAt(intlist0[i]));
			}
			jComboBox.setModel(new DefaultComboBoxModel(v));
			jComboBox.showPopup();

		} catch (Exception e) {
			System.out.println("orientation err");
			e.printStackTrace();
			return selectionForKeyold(aKey, aModel);
		}
		return 0;
	}

	public int selectionForKeyold(char aKey, ComboBoxModel aModel) {
		int i, c;
		int currentSelection = -1;
		Object selectedItem = aModel.getSelectedItem();
		String v;
		String pattern;

		if (selectedItem != null) {
			for (i = 0, c = aModel.getSize(); i < c; i++) {
				if (selectedItem == aModel.getElementAt(i)) {
					currentSelection = i;
					break;
				}
			}
		}

		pattern = ("" + aKey).toLowerCase();
		aKey = pattern.charAt(0);

		for (i = ++currentSelection, c = aModel.getSize(); i < c; i++) {
			Object elem = aModel.getElementAt(i);
			if (elem != null && elem.toString() != null) {
				v = elem.toString().toLowerCase();
				if (v.length() > 0 && v.charAt(0) == aKey)
					return i;
			}
		}

		for (i = 0; i < currentSelection; i++) {
			Object elem = aModel.getElementAt(i);
			if (elem != null && elem.toString() != null) {
				v = elem.toString().toLowerCase();
				if (v.length() > 0 && v.charAt(0) == aKey)
					return i;
			}
		}
		return -1;
	}
}
