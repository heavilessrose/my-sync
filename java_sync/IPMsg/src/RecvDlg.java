/*
 * IP Messenger Recv Dialog Class
 *		1998/02/01 (C)Copyright T.Kazawa(Digitune)
 */

import ipmsg.IPMsg;
import ipmsg.IPMPack;
import ipmsg.IPMEvent;
import ipmsg.IPMComEvent;
import ipmsg.IPMAddress;

import JP.digitune.util.MessageDigester;
import JP.digitune.util.StringReplacer;

import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Panel;
import java.awt.Label;
import java.awt.TextField;
import java.awt.TextArea;
import java.awt.Button;
import java.awt.Checkbox; //import jp.kyasu.awt.Frame;
//import jp.kyasu.awt.Panel;
//import jp.kyasu.awt.Label;
//import jp.kyasu.awt.TextField;
//import jp.kyasu.awt.TextArea;
//import jp.kyasu.awt.Button;
//import jp.kyasu.awt.Checkbox;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowListener;
import java.util.Random;
import java.util.MissingResourceException;

public class RecvDlg extends Dialog {
	IPMsg ipmsg;
	IPMEvent ipme;
	IPMComEvent ipmce;

	String suffix = "";

	Label user;
	TextArea body;
	Checkbox quote;

	public RecvDlg(Frame p, IPMsg i, IPMComEvent c, IPMEvent e) {
		super(p, false);
		ipmsg = i;
		ipmce = c;
		ipme = e;
		createWindow(p);
	}

	void createWindow(final Frame p) {
		setVisible(false);
		setTitle(ipmsg.getPref("recvdlgName"));
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				exitAction();
			}
		});
		setLayout(new BorderLayout());
		Panel p1 = new Panel(new FlowLayout(FlowLayout.CENTER));
		add("North", p1);
		Label to = new Label(ipmsg.getPref("msgFromLabel"));
		p1.add(to);
		String tmpuser = null;
		if (ipmce != null)
			tmpuser = ipmsg.makeListStr(ipmce.getPack());
		else
			tmpuser = ipme.getPack().getUser();
		user = new Label(Cp932.toCp932(tmpuser));
		p1.add(user);
		body = new TextArea();
		body.setEditable(false);
		body.setText(Cp932.toCp932(ipme.getPack().getExtra()));
		add("Center", body);
		Panel p2 = new Panel(new BorderLayout());
		add("South", p2);
		Label time = new Label(ipmsg.makeDateStr(ipme.getDate()), Label.CENTER);
		p2.add("North", time);
		Panel p3 = new Panel(new FlowLayout(FlowLayout.CENTER));
		p2.add("South", p3);
		final Button reply = new Button(ipmsg.getPref("replyLabel"));
		reply.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				IPMComEvent[] tmpipmce = new IPMComEvent[1];
				if (ipmce != null)
					tmpipmce[0] = ipmce;
				else {
					tmpipmce[0] = ipme;
					tmpipmce[0].getPack().setExtra(null);
				}
				SendDlg sd = new SendDlg(p, ipmsg, tmpipmce);
				if (quote.getState()) {
					//	String cr = (String) System.getProperty("line.separator");
					String cr = "\n";
					String tmpstr = ipmsg.getPref("quoter")
							+ Cp932.toJIS(body.getText());
					tmpstr = StringReplacer
							.replaceString(tmpstr, cr, cr + "> ");
					sd.setText(tmpstr);
				}
				sd.setVisible(true);
			}
		});
		p3.add(reply);
		quote = new Checkbox(ipmsg.getPref("quoteLabel"));
		quote.setState(new Boolean(ipmsg.getPref("quoteState")).booleanValue());
		p3.add(quote);
		Button close = new Button(ipmsg.getPref("closeLabel"));
		close.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				exitAction();
			}
		});
		p3.add(close);
		try {
			int x = Integer.parseInt(ipmsg.getPref("dlgSizeX"));
			int y = Integer.parseInt(ipmsg.getPref("dlgSizeY"));
			setSize(x, y);
		} catch (MissingResourceException ex) {
			pack();
		}
		Dimension sc = getToolkit().getScreenSize();
		Dimension sz = getSize();
		Random random = new Random();
		setLocation(sc.width / 2 + (random.nextInt() % 64), sc.height / 2
				- sz.height / 2 + (random.nextInt() % 64));
		if ((ipme.getPack().getCommand() & IPMsg.IPMSG_MULTICASTOPT) != 0)
			suffix = " (" + ipmsg.getPref("multicastLogFlag") + ")";
		else if ((ipme.getPack().getCommand() & IPMsg.IPMSG_BROADCASTOPT) != 0)
			suffix = " (" + ipmsg.getPref("broadcastLogFlag") + ")";
		if ((ipme.getPack().getCommand() & IPMsg.IPMSG_PASSWORDOPT) != 0) {
			suffix = suffix + " (" + ipmsg.getPref("passwdLogFlag") + ")";
			try {
				final String strpass = ipmsg.getPref("password");
				final Panel p4 = new Panel(new FlowLayout());
				add("Center", p4);
				Label passwdlabel = new Label(ipmsg.getPref("inputPasswdLabel"));
				p4.add(passwdlabel);
				final TextField input = new TextField(20);
				input.setEchoChar('*');
				p4.add(input);
				Button open = new Button(ipmsg.getPref("openLabel"));
				open.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent ae) {
						if (strpass.equals(MessageDigester.getMD5(input
								.getText()))) {
							ipmsg.sendReadMsg(ipme);
							remove(p4);
							add("Center", body);
							reply.setEnabled(true);
							quote.setEnabled(true);
							validate();
							ipmsg.writeLog("From: "
									+ Cp932.toJIS(user.getText()), ipmsg
									.makeDateStr(ipme.getDate())
									+ suffix, ipme.getPack().getExtra());
						} else
							getToolkit().beep();
					}
				});
				p4.add(open);
				reply.setEnabled(false);
				quote.setEnabled(false);
				return;
			} catch (MissingResourceException ex) {
			}
		}
		if ((ipme.getPack().getCommand() & IPMsg.IPMSG_SECRETOPT) != 0) {
			suffix = suffix + " (" + ipmsg.getPref("secretLogFlag") + ")";
			final Button open = new Button(ipmsg.getPref("openLabel"));
			open.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent ae) {
					ipmsg.sendReadMsg(ipme);
					remove(open);
					add("Center", body);
					reply.setEnabled(true);
					quote.setEnabled(true);
					validate();
					ipmsg.writeLog("From: " + Cp932.toJIS(user.getText()),
							ipmsg.makeDateStr(ipme.getDate()) + suffix, ipme
									.getPack().getExtra());
				}
			});
			add("Center", open);
			reply.setEnabled(false);
			quote.setEnabled(false);
			return;
		}
		ipmsg.writeLog("From: " + Cp932.toJIS(user.getText()), ipmsg
				.makeDateStr(ipme.getDate())
				+ suffix, ipme.getPack().getExtra());
	}

	void exitAction() {
		if (!quote.isEnabled())
			ipmsg.sendDeleteMsg(ipme);
		if (new Boolean(ipmsg.getPref("resumeState")).booleanValue())
			ipmsg.setPref("quoteState", new Boolean(quote.getState())
					.toString());
		Dimension size = getSize();
		ipmsg.setPref("dlgSizeX", Integer.toString(size.width));
		ipmsg.setPref("dlgSizeY", Integer.toString(size.height));
		ipmsg.decReceiveCount();
		dispose();
	}
}
