package com.winksi.j2me.cc.tools;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/**
 * 电话簿数据比较
 * 
 * @author Jery
 */
public class PimCompareTools {

	public PimCompareTools() {

	}

	/**
	 * 得到新添加的号码vector(无法反映删除的号码,或用户是否手动改过联系人大头贴) 
	 * 
	 * @param newHt
	 *            当前号码簿中号码与图片的对应关系hashtable
	 * 
	 * @param rmsHt
	 *            rms中已保存的号码与图片的对应关系hashtable
	 * @return
	 */
	public static Vector compareHashtable(Hashtable newHt, Hashtable rmsHt) {
		Vector v = new Vector();
		Enumeration e = newHt.keys();
		while (e.hasMoreElements()) {
			Object pair = e.nextElement();
			if (!rmsHt.containsKey(pair)) {
				v.addElement(pair);
				//				System.out.println("PimCompareTools:compareHashtable(): new num: " + obj.toString());
			}
		}
		//		System.out.println("PimCompareTools:compareHashtable(): new num size: " + v.size());
		return v;
	}

}
