/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 * 服务邮箱：  wmw0@sina.com
 */
package py.pinying;

import java.util.*;
import java.lang.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class t extends JFrame {
	JPanel jPanel2 = new JPanel();
	JLabel jLabel1 = new JLabel();
	JComboBox jComboBox1 = new JComboBox();
	JComboBox jComboBox11 = new JComboBox();
	JTextArea jTextArea2 = new JTextArea();
	JButton jbdindpy = new JButton();
	JButton jButton1 = new JButton();
	JScrollPane jScrollPane1 = new JScrollPane();
	JTextArea jTextArea1 = new JTextArea();
	JScrollPane jScrollPane2 = new JScrollPane();
	BorderLayout borderLayout1 = new BorderLayout();
	JLabel jLabel2 = new JLabel();

	public t() {
		try {
			jbInit();
			jComboBox1.addItem("中国");
			jComboBox1.addItem("东南亚");
			jComboBox1.addItem("欧洲");
			jComboBox1.addItem("南美洲");
			jComboBox1.addItem("澳洲");
			jComboBox1.addItem("非洲");
			jComboBox1.addItem("亚洲");
			jComboBox1.addItem("美国");
			jComboBox1.addItem("加拿大");
			jComboBox1.addItem("新加坡");
			jComboBox1.addItem("韩国");
			jComboBox1.addItem("日本");
			jComboBox1.addItem("上海市");
			jComboBox1.addItem("北京市");
			jComboBox1.addItem("123");
			jComboBox1.addItem("564");
			jComboBox1.addItem("天津市");
			jComboBox1.addItem("河北省");
			jComboBox1.addItem("山西省");
			jComboBox1.addItem("辽宁省");
			jComboBox1.addItem("吉林省");
			jComboBox1.addItem("江苏省");
			jComboBox1.addItem("浙江省");
			jComboBox1.addItem("安徽省");
			jComboBox1.addItem("福建省");
			jComboBox1.addItem("江西省");
			jComboBox1.addItem("山东省");
			jComboBox1.addItem("河南省");
			jComboBox1.addItem("内蒙古自治区");
			jComboBox1.addItem("黑龙江省");
			jComboBox1.addItem("湖北省");
			jComboBox1.addItem("湖南省");
			jComboBox1.addItem("广东省");
			jComboBox1.addItem("广西壮族自治区");
			jComboBox1.addItem("海南省");
			jComboBox1.addItem("四川省");
			jComboBox1.addItem("重庆市");
			jComboBox1.addItem("台湾省");
			jComboBox1.addItem("贵州省");
			jComboBox1.addItem("云南省");
			jComboBox1.addItem("西藏自治区");
			jComboBox1.addItem("陕西省");
			jComboBox1.addItem("甘肃省");
			jComboBox1.addItem("青海省");
			jComboBox1.addItem("宁夏回族自治区");
			jComboBox1.addItem("新疆维吾尔自治区");
			jComboBox1.addItem("香港特别行政区");
			jComboBox1.addItem("澳门特别行政区");
			jComboBox1.addItem("其他");
			javax.swing.plaf.basic.BasicCheckBoxUI cu;
			javax.swing.plaf.basic.BasicComboPopup cu0;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		try {

			py.wmw.ui.ToolKit.StartUI(new py.wmw.ui.UItool());
			py.wmw.ui.ToolKit.SetTimeOut(1000);
			t t = new t();

			t.setSize(450, 400);
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			t.setLocation((screenSize.width - t.getWidth()) / 2,
					(int) ((screenSize.height - t.getHeight()) / 4));
			t.show();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	private void jbInit() throws Exception {
		this.getContentPane().setLayout(borderLayout1);
		jPanel2.setBorder(BorderFactory.createEtchedBorder());
		jPanel2.setLayout(null);
		jLabel1.setText("jLabel1");
		jLabel1.setBounds(new Rectangle(0, 3, 239, 35));

		jComboBox1.setBorder(BorderFactory.createEtchedBorder());
		jComboBox1.setToolTipText("输入拼音字母，自动定位");
		jComboBox1.setBounds(new Rectangle(92, 56, 291, 31));

		jTextArea2.setBorder(BorderFactory.createEtchedBorder());
		jTextArea2.setText("");
		jTextArea2.setLineWrap(true);
		jTextArea2.setTabSize(8);
		jbdindpy.setBounds(new Rectangle(93, 174, 142, 24));
		jbdindpy.setText("查拼音");
		jbdindpy.addActionListener(new t_jbdindpy_actionAdapter(this));
		jButton1.setBounds(new Rectangle(253, 175, 121, 23));
		jButton1.setText("查声母");
		jButton1.addActionListener(new t_jButton1_actionAdapter(this));
		jTextArea1.setAlignmentY((float) 0.5);
		jTextArea1.setBorder(BorderFactory.createEtchedBorder());
		jTextArea1.setText("我是一个程序员");
		jTextArea1.setLineWrap(true);
		jTextArea1.setTabSize(8);
		jScrollPane2.setBounds(new Rectangle(93, 203, 291, 51));
		jScrollPane1.setBounds(new Rectangle(93, 124, 291, 47));
		jLabel2.setToolTipText("");
		jLabel2.setText("输入拼音字母，自动定位");
		jLabel2.setBounds(new Rectangle(98, 25, 239, 24));
		this.getContentPane().add(jPanel2, BorderLayout.CENTER);
		jPanel2.add(jScrollPane1, null);
		jScrollPane1.getViewport().add(jTextArea1, null);
		jPanel2.add(jScrollPane2, null);
		jScrollPane2.getViewport().add(jTextArea2, null);
		jPanel2.add(jButton1, null);
		jPanel2.add(jbdindpy, null);
		jPanel2.add(jComboBox1, null);
		jPanel2.add(jLabel2, null);

	}

	void jComboBox1_actionPerformed(ActionEvent e) {

	}

	void jbdindpy_actionPerformed(ActionEvent e) {
		String hanzhi = jTextArea1.getText().trim();
		String[] py = new py.pinying.pybean()
				.getSomeSM(new String[] { hanzhi });
		// new pinying.pybean().getIndexByPiny(py,"gz");
		String pingying = new py.pinying.pybean().getSomepiny(hanzhi); // 得到拼音
		// 含多音
		jTextArea2.setText(pingying);
	}

	void jButton1_actionPerformed(ActionEvent e) {
		String hanzhi = jTextArea1.getText().trim();
		String pingying = new py.pinying.pybean().getSomeSM(hanzhi); // 得到拼音 含多音
		jTextArea2.setText(pingying);

	}

}

class t_jbdindpy_actionAdapter implements java.awt.event.ActionListener {
	t adaptee;

	t_jbdindpy_actionAdapter(t adaptee) {
		this.adaptee = adaptee;
	}

	public void actionPerformed(ActionEvent e) {
		adaptee.jbdindpy_actionPerformed(e);
	}
}

class t_jButton1_actionAdapter implements java.awt.event.ActionListener {
	t adaptee;

	t_jButton1_actionAdapter(t adaptee) {
		this.adaptee = adaptee;
	}

	public void actionPerformed(ActionEvent e) {
		adaptee.jButton1_actionPerformed(e);
	}
}
