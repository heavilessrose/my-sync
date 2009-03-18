/*
 *	Retry Dialog Box
 *		1998/02/13 (C) Copyright T.Kazawa(Digitune)
 */

import JP.digitune.gui.MsgBox;

import ipmsg.IPMsg;
import ipmsg.IPMEvent;
import ipmsg.IPMAddress;

import java.awt.Frame;
import java.awt.Dialog;
import java.awt.Label;
import java.awt.Button;
import java.awt.Panel; //import jp.kyasu.awt.Frame;
//import jp.kyasu.awt.Dialog;
//import jp.kyasu.awt.Label;
//import jp.kyasu.awt.Button;
//import jp.kyasu.awt.Panel;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ResourceBundle;

public class RetryDlg extends MsgBox {
	private Label label1;
	private Button yes, no;

	private IPMsg ipmsg;
	private IPMEvent ipme;

	public RetryDlg(Frame parent, String title, String msg, IPMsg aipmsg,
			IPMEvent aipme) {
		super(parent, title, msg, false);
		ipmsg = aipmsg;
		ipme = aipme;
	}

	protected void createButton() {
		yes = new Button(rc.getString("yesLabel"));
		no = new Button(rc.getString("noLabel"));
		yes.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				long flag = ipme.getPack().getCommand() & IPMsg.IPMSG_OPTMASK;
				IPMAddress[] tmpaddr = new IPMAddress[1];
				tmpaddr[0] = ipme.getIPMAddress();
				ipmsg.sendMsg(tmpaddr, ipme.getPack().getExtra(), flag);
				dispose();
			}
		});
		no.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				dispose();
			}
		});
		regbutton.add(yes, "Center");
		regbutton.add(no, "Center");
	}
}
