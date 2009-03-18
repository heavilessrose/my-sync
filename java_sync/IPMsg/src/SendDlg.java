/*
 * IP Messenger Send Dialog Class
 *		1998/01/30 (C)Copyright T.Kazawa(Digitune)
 */

import ipmsg.IPMsg;
import ipmsg.IPMPack;
import ipmsg.IPMComEvent;
import ipmsg.IPMAddress;

import JP.digitune.gui.YesNoBox;

import java.awt.Point;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Panel;
import java.awt.Label;
import java.awt.Choice;
import java.awt.TextArea;
import java.awt.Button;
import java.awt.Checkbox;
//import jp.kyasu.awt.Frame;
//import jp.kyasu.awt.Panel;
//import jp.kyasu.awt.Label;
//import jp.kyasu.awt.Choice;
//import jp.kyasu.awt.TextArea;
//import jp.kyasu.awt.Button;
//import jp.kyasu.awt.Checkbox;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowListener;
import java.util.Date;
import java.util.MissingResourceException;

public class SendDlg extends Dialog {
	IPMsg ipmsg;
	IPMComEvent[] users;
	
	TextArea body;
	Checkbox secret, passwd;
	
	public SendDlg(Frame p, IPMsg i, IPMComEvent[] e) {
		super(p, false);
		ipmsg = i;
		users = e;
		createWindow(p);
	}
	
	void createWindow(final Frame p) {
		setVisible(false);
		setTitle(ipmsg.getPref("senddlgName"));
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				exitAction();
			}
		});
		setLayout(new BorderLayout());
		Panel p1 = new Panel(new FlowLayout(FlowLayout.CENTER));
		add("North", p1);
		if (users == null) {
			Label to = new Label(ipmsg.getPref("toBroadcastLabel"));
			p1.add(to);
		} else if (users.length == 1) {
			Label to = new Label(ipmsg.getPref("toUnicastLabel"));
			p1.add(to);
			Label user = new Label(
				Cp932.toCp932(ipmsg.makeListStr(users[0].getPack())));
			p1.add(user);
		} else {
			Label to = new Label(ipmsg.getPref("toMulticastLabel"));
			p1.add(to);
			Choice choice = new Choice();
			for (int i = 0; i < users.length; i++)
				choice.add(
					Cp932.toCp932(ipmsg.makeListStr(users[i].getPack())));
			p1.add(choice);
		}
		body = new TextArea();
		add("Center", body);
		Panel p2 = new Panel(new FlowLayout(FlowLayout.CENTER));
		add("South", p2);
		Button send = new Button(ipmsg.getPref("sendLabel"));
		send.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				exitAction();
				long flag = 0;
				String suffix = "";
				if (secret.getState()) {
					flag |= IPMsg.IPMSG_SECRETOPT;
					suffix = "(" + ipmsg.getPref("secretLogFlag") + ")";
				}
				if (passwd.getState()) {
					flag |= IPMsg.IPMSG_PASSWORDOPT;
					suffix = "(" + ipmsg.getPref("passwdLogFlag") + ")";
				}
				IPMAddress[] tmpaddrs = null;
				String tostr;
				if (users != null) {
					tmpaddrs = new IPMAddress[users.length];
					tmpaddrs[0] = users[0].getIPMAddress();
					tostr = "To: " + ipmsg.makeListStr(users[0].getPack());
					for (int i = 1; i < users.length; i++) {
						suffix = "(" + ipmsg.getPref("multicastLogFlag")
							+ ") " + suffix;
						tmpaddrs[i] = users[i].getIPMAddress();
						tostr = System.getProperty("line.separator", "\n")
							+ "To: " + ipmsg.makeListStr(users[i].getPack());
					}
				} else {
					tostr = "To: BROADCAST";
					suffix = "(" + ipmsg.getPref("broadcastLogFlag") + ") "
						+ suffix;
				}
				ipmsg.sendMsg(tmpaddrs, Cp932.toJIS(body.getText()), flag);
				ipmsg.writeLog(tostr
					, ipmsg.makeDateStr(new Date(System.currentTimeMillis()))
					+ " " + suffix
					, Cp932.toJIS(body.getText()));
			}
		});
		p2.add(send);
		secret = new Checkbox(ipmsg.getPref("secretLabel"));
		secret.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				if (secret.getState())
					passwd.setEnabled(true);
				else {
					passwd.setState(false);
					passwd.setEnabled(false);
				}
			}
		});
		secret.setState(new Boolean(ipmsg.getPref("secretState"))
			.booleanValue());
		p2.add(secret);
		passwd = new Checkbox(ipmsg.getPref("passwdLabel"));
		passwd.setState(new Boolean(ipmsg.getPref("passwdState"))
			.booleanValue());
		passwd.setEnabled(secret.getState());
		p2.add(passwd);
		Button cancel = new Button(ipmsg.getPref("cancelLabel"));
		cancel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				exitAction();
			}
		});
		p2.add(cancel);
		try {
			int x = Integer.parseInt(ipmsg.getPref("dlgSizeX"));
			int y = Integer.parseInt(ipmsg.getPref("dlgSizeY"));
			setSize(x, y);
		} catch (MissingResourceException ex) {
			pack();
		}
		try {
			int x = Integer.parseInt(ipmsg.getPref("senddlgX"));
			int y = Integer.parseInt(ipmsg.getPref("senddlgY"));
			setLocation(x, y);
		} catch (MissingResourceException ex) {
			Dimension sc = getToolkit().getScreenSize();
			Dimension sz = getSize();
			setLocation(sc.width/2-sz.width, sc.height/2-sz.height/2);
		}
	}
	
	void exitAction() {
		if (new Boolean(ipmsg.getPref("resumeState")).booleanValue()) {
			ipmsg.setPref("secretState"
				, new Boolean(secret.getState()).toString());
			ipmsg.setPref("passwdState"
				, new Boolean(passwd.getState()).toString());
		}
		Dimension size = getSize();
		ipmsg.setPref("dlgSizeX", Integer.toString(size.width));
		ipmsg.setPref("dlgSizeY", Integer.toString(size.height));
		Point location = getLocation();
		ipmsg.setPref("senddlgX", Integer.toString(location.x));
		ipmsg.setPref("senddlgY", Integer.toString(location.y));
		dispose();
	}
	
	public void setText(String text) {
		body.setText(Cp932.toCp932(text));
	}
}
