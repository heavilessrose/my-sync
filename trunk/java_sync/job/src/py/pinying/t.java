/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: ���ڳ������������ </p>
 * @author  wangsong
 * @version 1.1
 * �������䣺  wmw0@sina.com
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
			jComboBox1.addItem("�й�");
			jComboBox1.addItem("������");
			jComboBox1.addItem("ŷ��");
			jComboBox1.addItem("������");
			jComboBox1.addItem("����");
			jComboBox1.addItem("����");
			jComboBox1.addItem("����");
			jComboBox1.addItem("����");
			jComboBox1.addItem("���ô�");
			jComboBox1.addItem("�¼���");
			jComboBox1.addItem("����");
			jComboBox1.addItem("�ձ�");
			jComboBox1.addItem("�Ϻ���");
			jComboBox1.addItem("������");
			jComboBox1.addItem("123");
			jComboBox1.addItem("564");
			jComboBox1.addItem("�����");
			jComboBox1.addItem("�ӱ�ʡ");
			jComboBox1.addItem("ɽ��ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("�㽭ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("ɽ��ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("���ɹ�������");
			jComboBox1.addItem("������ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("�㶫ʡ");
			jComboBox1.addItem("����׳��������");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("�Ĵ�ʡ");
			jComboBox1.addItem("������");
			jComboBox1.addItem("̨��ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����������");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("����ʡ");
			jComboBox1.addItem("�ຣʡ");
			jComboBox1.addItem("���Ļ���������");
			jComboBox1.addItem("�½�ά���������");
			jComboBox1.addItem("����ر�������");
			jComboBox1.addItem("�����ر�������");
			jComboBox1.addItem("����");
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
		jComboBox1.setToolTipText("����ƴ����ĸ���Զ���λ");
		jComboBox1.setBounds(new Rectangle(92, 56, 291, 31));

		jTextArea2.setBorder(BorderFactory.createEtchedBorder());
		jTextArea2.setText("");
		jTextArea2.setLineWrap(true);
		jTextArea2.setTabSize(8);
		jbdindpy.setBounds(new Rectangle(93, 174, 142, 24));
		jbdindpy.setText("��ƴ��");
		jbdindpy.addActionListener(new t_jbdindpy_actionAdapter(this));
		jButton1.setBounds(new Rectangle(253, 175, 121, 23));
		jButton1.setText("����ĸ");
		jButton1.addActionListener(new t_jButton1_actionAdapter(this));
		jTextArea1.setAlignmentY((float) 0.5);
		jTextArea1.setBorder(BorderFactory.createEtchedBorder());
		jTextArea1.setText("����һ������Ա");
		jTextArea1.setLineWrap(true);
		jTextArea1.setTabSize(8);
		jScrollPane2.setBounds(new Rectangle(93, 203, 291, 51));
		jScrollPane1.setBounds(new Rectangle(93, 124, 291, 47));
		jLabel2.setToolTipText("");
		jLabel2.setText("����ƴ����ĸ���Զ���λ");
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
		String pingying = new py.pinying.pybean().getSomepiny(hanzhi); // �õ�ƴ��
		// ������
		jTextArea2.setText(pingying);
	}

	void jButton1_actionPerformed(ActionEvent e) {
		String hanzhi = jTextArea1.getText().trim();
		String pingying = new py.pinying.pybean().getSomeSM(hanzhi); // �õ�ƴ�� ������
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
