package py.wmw.ui;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: ���ڳ������������ </p>
 * @author  wangsong
 * @version 1.1
 */

import javax.swing.*;

public class ToolKit {
	private ToolKit() {
	}

	private static long TimeOut = 1000;// ���¼��� ��ʱ��

	public static long GetTimeOut() {
		return TimeOut;
	}

	public static void SetTimeOut(long timeout) {
		thisTool.TimeOut = timeout;
	}

	static ToolKit thisTool;
	static {
		thisTool = new ToolKit();
		if (thisTool.UItoolinterface == null) {
			thisTool.UItoolinterface = new UItool();
		}
	}

	public static void StartUI(UItoolinterface UItoolinterface) {
		if (UItoolinterface != null) {
			thisTool.UItoolinterface = UItoolinterface;
		}
		thisTool.UItoolinterface.SetUI();
	}

	public static UItoolinterface UItoolinterface = null;

	public static void main(String[] args) {
		if (args == null || args.length == 0) {
			System.out.println("�밴��JAVA   wmw.ui.ToolKit  yourclass���ķ�ʽʹ�� ");
		}
		py.wmw.ui.ToolKit.StartUI(new py.wmw.ui.UItool());
		py.wmw.ui.ToolKit.SetTimeOut(1000);
		try {
			Class c = Class.forName(args[0]);
			Object obj = null;
			// obj = c.newInstance();
			String[] args0 = new String[args.length - 1];
			System.arraycopy(args, 0, args0, 0, args.length - 1);
			Class c0[] = { (new String[0]).getClass() };
			for (int k = 0; k < args.length - 1; k++) {
				c0[k] = Class.forName("java.lang.String");
			}
			java.lang.reflect.Method m = c.getMethod("main", c0);
			Object r = m.invoke(obj, new Object[] { args0 });
		} catch (Exception eer) {
			eer.printStackTrace();
		}
	}

}
