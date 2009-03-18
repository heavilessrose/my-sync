/*
 * IP Messenger Config Dialog
 *		1998/2/7 (C)Copyright T.Kazawa(Digitune)
 */

import ipmsg.IPMsg;
import ipmsg.IPMComEvent;
import ipmsg.IPMPack;

import JP.digitune.util.MessageDigester;

import java.awt.*; //import jp.kyasu.awt.*;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.FileDialog;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.StringTokenizer;
import java.util.MissingResourceException;

public class ConfDlg extends Dialog {
	IPMsg ipmsg;

	public ConfDlg(Frame parent, IPMsg argipm) {
		super(parent, true);
		ipmsg = argipm;
		createWindow(parent);
	}

	void createWindow(final Frame parent) {
		GridBagLayout gridBagLayout = new GridBagLayout();
		setLayout(gridBagLayout);
		Label label1 = new Label(ipmsg.getPref("setnickLabel"), Label.RIGHT);
		GridBagConstraints gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label1, gbc);
		add(label1);
		final TextField textField1 = new TextField();
		try {
			textField1.setText(Cp932.toCp932(ipmsg.getPref("nickName")));
		} catch (MissingResourceException ex) {
		}
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 0;
		gbc.gridwidth = 2;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(textField1, gbc);
		add(textField1);
		Label label2 = new Label(ipmsg.getPref("setgroupLabel"), Label.RIGHT);
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label2, gbc);
		add(label2);
		final TextField textField2 = new TextField(12);
		try {
			textField2.setText(Cp932.toCp932(ipmsg.getPref("groupName")));
		} catch (MissingResourceException ex) {
		}
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 1;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(textField2, gbc);
		add(textField2);
		final Choice choice1 = new Choice();
		Hashtable groupcache = new Hashtable();
		Enumeration members = ipmsg.getUserlist().elements();
		while (members.hasMoreElements()) {
			IPMComEvent tmpevent = (IPMComEvent) members.nextElement();
			IPMPack tmppack = tmpevent.getPack();
			if (tmppack.getGroup() != null
					&& groupcache.get(tmppack.getGroup()) == null) {
				choice1.addItem(Cp932.toCp932(tmppack.getGroup()));
				groupcache.put(tmppack.getGroup(), tmppack.getGroup());
			}
		}
		choice1.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				textField2.setText(choice1.getSelectedItem());
			}
		});
		gbc = new GridBagConstraints();
		gbc.gridx = 2;
		gbc.gridy = 1;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(choice1, gbc);
		add(choice1);
		Label label3 = new Label(ipmsg.getPref("setabsmsgLabel"), Label.RIGHT);
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 2;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label3, gbc);
		add(label3);
		final TextField textField3 = new TextField();
		try {
			textField3.setText(Cp932.toCp932(ipmsg.getPref("absenceMsg")));
		} catch (MissingResourceException ex) {
		}
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 2;
		gbc.gridwidth = 2;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(textField3, gbc);
		add(textField3);
		Label label4 = new Label(ipmsg.getPref("setbroadcastLabel"),
				Label.RIGHT);
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 3;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label4, gbc);
		add(label4);
		final TextField textField4 = new TextField();
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 3;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(textField4, gbc);
		add(textField4);
		final List list1 = new List(0, false);
		try {
			StringTokenizer st1 = new StringTokenizer(ipmsg
					.getPref("broadcastAddr"), ",");
			while (st1.hasMoreTokens())
				list1.addItem(st1.nextToken());
		} catch (MissingResourceException ex) {
		}
		gbc = new GridBagConstraints();
		gbc.gridx = 2;
		gbc.gridy = 3;
		gbc.gridheight = 3;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.BOTH;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(list1, gbc);
		add(list1);
		Button button1 = new Button();
		button1.setLabel(ipmsg.getPref("addbuttonLabel"));
		button1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				if (!textField4.getText().equals("")) {
					list1.addItem(textField4.getText());
					textField4.setText("");
				}
			}
		});
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 4;
		gbc.weightx = 1.0;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(button1, gbc);
		add(button1);
		Button button2 = new Button();
		button2.setLabel(ipmsg.getPref("removebuttonLabel"));
		button2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				if (list1.getSelectedItem() != null) {
					textField4.setText(list1.getSelectedItem());
					list1.remove(list1.getSelectedIndex());
				}
			}
		});
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 5;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(button2, gbc);
		add(button2);
		Label label5 = new Label(ipmsg.getPref("setpasswdLabel"), Label.RIGHT);
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 6;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label5, gbc);
		add(label5);
		Panel panel1 = new Panel();
		panel1.setLayout(new GridLayout(1, 0, 0, 0));
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 6;
		gbc.gridwidth = 2;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(panel1, gbc);
		add(panel1);
		final TextField textField5 = new TextField();
		textField5.setEchoChar('*');
		panel1.add(textField5);
		final TextField textField6 = new TextField();
		textField6.setEchoChar('*');
		panel1.add(textField6);
		final TextField textField7 = new TextField();
		textField7.setEchoChar('*');
		panel1.add(textField7);
		Label label6 = new Label(ipmsg.getPref("setLogFilenameLabel"),
				Label.RIGHT);
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 7;
		gbc.anchor = GridBagConstraints.EAST;
		gbc.fill = GridBagConstraints.NONE;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(label6, gbc);
		add(label6);
		final TextField textField8 = new TextField();
		try {
			textField8.setText(ipmsg.getPref("logFilename"));
		} catch (MissingResourceException ex) {
		}
		gbc = new GridBagConstraints();
		gbc.gridx = 1;
		gbc.gridy = 7;
		gbc.gridwidth = 1;
		gbc.weightx = 2.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(textField8, gbc);
		add(textField8);
		Button bbutton = new Button();
		bbutton.setLabel(ipmsg.getPref("browseLabel"));
		bbutton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				FileDialog fd = new FileDialog(parent);
				fd.setVisible(true);
				if (fd.getFile() != null)
					textField8.setText(fd.getDirectory() + fd.getFile());
			}
		});
		gbc = new GridBagConstraints();
		gbc.gridx = 2;
		gbc.gridy = 7;
		gbc.gridwidth = 1;
		gbc.weightx = 0.5;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(bbutton, gbc);
		add(bbutton);
		Panel panel2 = new Panel();
		panel2.setLayout(new FlowLayout(FlowLayout.CENTER));
		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 8;
		gbc.gridwidth = 3;
		gbc.weightx = 1.0;
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.insets = new Insets(2, 2, 2, 2);
		gridBagLayout.setConstraints(panel2, gbc);
		add(panel2);
		Button button3 = new Button();
		button3.setLabel(ipmsg.getPref("okLabel"));
		button3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				if (!textField1.getText().equals(""))
					ipmsg
							.setPref("nickName", Cp932.toJIS(textField1
									.getText()));
				else
					ipmsg.setPref("nickName", "");
				if (!textField2.getText().equals(""))
					ipmsg.setPref("groupName", Cp932
							.toJIS(textField2.getText()));
				else
					ipmsg.setPref("groupName", "");
				if (!textField3.getText().equals(""))
					ipmsg.setPref("absenceMsg", Cp932.toJIS(textField3
							.getText()));
				else
					ipmsg.setPref("absenceMsg", "");
				if (list1.getItemCount() != 0) {
					StringBuffer strbuf = new StringBuffer();
					strbuf.append(list1.getItem(0));
					for (int i = 1; i < list1.getItemCount(); i++)
						strbuf.append("," + list1.getItem(i));
					ipmsg.setPref("broadcastAddr", new String(strbuf));
				} else
					ipmsg.setPref("broadcastAddr", "");
				try {
					String tmppass = ipmsg.getPref("password");
					if (textField5.getText() != null)
						if (MessageDigester.getMD5(textField5.getText())
								.equals(tmppass))
							if (textField6.getText() != null
									&& textField7.getText() != null)
								if (textField6.getText().equals(
										textField7.getText()))
									ipmsg.setPref("password", MessageDigester
											.getMD5(textField6.getText()));
				} catch (MissingResourceException ex) {
					if (textField6.getText() != null
							&& textField7.getText() != null)
						if (textField6.getText().equals(textField7.getText()))
							ipmsg.setPref("password", MessageDigester
									.getMD5(textField6.getText()));
				}
				if (!textField8.getText().equals(""))
					ipmsg.setPref("logFilename", textField8.getText());
				else
					ipmsg.setPref("logFilename", "");
				ipmsg.refreshList();
				dispose();
			}
		});
		panel2.add(button3);
		Button button4 = new Button();
		button4.setLabel(ipmsg.getPref("cancelLabel"));
		button4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				dispose();
			}
		});
		panel2.add(button4);
		pack();
		setTitle(ipmsg.getPref("confdlgName"));
		setResizable(false);
		Dimension sc = getToolkit().getScreenSize();
		Dimension sz = getSize();
		setLocation(sc.width / 2 - sz.width / 2, sc.height / 2 - sz.height / 2);

		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				dispose();
			}
		});
	}
}
