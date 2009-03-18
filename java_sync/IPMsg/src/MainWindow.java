/*
 * IP Messenger Main Window
 *		1998/01/27 (C)Copyright T.Kazawa(Digitune)
 */

import ipmsg.IPMsg;
import ipmsg.IPMEvent;
import ipmsg.IPMComEvent;
import ipmsg.IPMListener;
import ipmsg.IPMPack;
import ipmsg.IPMAddress;

import JP.digitune.gui.MsgBox;
import JP.digitune.gui.YesNoBox;
import JP.digitune.util.SortVector;
import JP.digitune.util.StringReplacer;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.Frame;
import java.awt.Button;
import java.awt.List;
import java.awt.Checkbox;
import java.awt.Choice;
import java.awt.Panel;
//import jp.kyasu.awt.Frame;
//import jp.kyasu.awt.Button;
//import jp.kyasu.awt.List;
//import jp.kyasu.awt.Checkbox;
//import jp.kyasu.awt.Choice;
//import jp.kyasu.awt.Panel;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.UnsupportedEncodingException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.MissingResourceException;

public class MainWindow extends Frame {
	public IPMsg ipmsg;
	
	private Button send, refresh, conf, exit;
	private List memberlist;
	private Checkbox absence, broadcast;
	private Choice groups;
	
	private Hashtable NAMEtoINFO = new Hashtable();
	private Hashtable ADDRtoINFO = new Hashtable();
	private boolean refreshing = false, received = false;
	
	public MainWindow() {
		ipmsg = new IPMsg();
		ipmsg.addIPMListener(new IPMListener() {
			public void eventOccured(IPMEvent ipme) {
				processEvent(ipme);
			}
		});
		createWindow();
		ipmsg.entry();
	}
	
	void createWindow() {
		setVisible(false);
		setTitle(ipmsg.getPref("appName"));
		setLayout(new BorderLayout());
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent we) {
				exitAction();
			}
			public void windowClosed(WindowEvent we) {
				System.exit(0);
			}
		});
		Panel p1 = new Panel(new FlowLayout(FlowLayout.CENTER));
		add("North", p1);
		send = new Button();
		send.setLabel(ipmsg.getPref("sendLabel"));
		send.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				sendAction();
			}
		});
		p1.add(send);
		refresh = new Button();
		refresh.setLabel(ipmsg.getPref("refreshLabel"));
		refresh.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				refreshAction();
			}
		});
		p1.add(refresh);
		conf = new Button();
		conf.setLabel(ipmsg.getPref("configLabel"));
		conf.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				confAction();
			}
		});
		p1.add(conf);
		exit = new Button();
		exit.setLabel(ipmsg.getPref("exitLabel"));
		exit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				exitAction();
			}
		});
		p1.add(exit);
		Panel p5 = new Panel(new BorderLayout());
		add("Center", p5);
		Panel p6 = new Panel(new GridLayout(1, 0, 0, 0));
		p5.add("North", p6);
		Button sortuser = new Button(ipmsg.getPref("sortUserLabel"));
		sortuser.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				sortKeyChanged("u");
			}
		});
		p6.add(sortuser);
		Button sortgroup = new Button(ipmsg.getPref("sortGroupLabel"));
		sortgroup.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				sortKeyChanged("g");
			}
		});
		p6.add(sortgroup);
		Button sorthost = new Button(ipmsg.getPref("sortHostLabel"));
		sorthost.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				sortKeyChanged("h");
			}
		});
		p6.add(sorthost);
		memberlist = new List(0, true);
		memberlist.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				sendAction();
			}
		});
		p5.add("Center", memberlist);
		Panel p2 = new Panel(new BorderLayout(5, 5));
		add("South", p2);
		Panel p3 = new Panel(new FlowLayout(FlowLayout.CENTER, 3, 3));
		p2.add("West", p3);
		broadcast = new Checkbox(ipmsg.getPref("broadcastLabel"));
		broadcast.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				memberlist.setEnabled(!broadcast.getState());
			}
		});
		p3.add(broadcast);
		groups = new Choice();
		groups.add(ipmsg.getPref("allName"));
		groups.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				received = true;
				if (!refreshing) {
					refreshing = true;
					new RefreshList().start();
				}
			}
		});
		p2.add("Center", groups);
		Panel p4 = new Panel(new FlowLayout(FlowLayout.CENTER, 3, 3));
		p2.add("East", p4);
		absence = new Checkbox(ipmsg.getPref("absenceLabel"));
		absence.setState(new Boolean(ipmsg.getPref("absenceState"))
			.booleanValue());
		absence.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				ipmsg.setPref("absenceState"
					, new Boolean(absence.getState()).toString());
				ipmsg.absenceStateChanged();
			}
		});
		p4.add(absence);
		try {
			int x = Integer.parseInt(ipmsg.getPref("mainSizeX"));
			int y = Integer.parseInt(ipmsg.getPref("mainSizeY"));
			setSize(x, y);
		} catch (MissingResourceException ex) {
			pack();
		}
		try {
			int x = Integer.parseInt(ipmsg.getPref("mainX"));
			int y = Integer.parseInt(ipmsg.getPref("mainY"));
			setLocation(x, y);
		} catch (MissingResourceException ex) {
			Dimension sc = getToolkit().getScreenSize();
			Dimension sz = getSize();
			setLocation(sc.width/2-sz.width/2, sc.height/2-sz.height/2);
		}
		setIconImage(getToolkit().getImage(
			getClass().getResource("images/ipmsg.gif")));
	}
	
	synchronized void sortKeyChanged(String ckey) {
		String tmpkey = ipmsg.getPref("sortKey");
		tmpkey = StringReplacer.replaceString(tmpkey, ckey, "");
		ipmsg.setPref("sortKey", ckey + tmpkey);
		received = true;
		if (!refreshing) {
			refreshing = true;
			new RefreshList().start();
		}
	}
	
	void sendAction() {
		IPMComEvent[] members;
		if (broadcast.getState()
			&& groups.getSelectedItem().equals(ipmsg.getPref("allName")))
			members = null;
		else if (broadcast.getState()) {
			String[] strmembers = memberlist.getItems();
			members = new IPMComEvent[strmembers.length];
			for (int i = 0; i < members.length; i++)
				members[i] = (IPMComEvent) NAMEtoINFO.get(strmembers[i]);
		} else {
			String[] strmembers = memberlist.getSelectedItems();
			if (strmembers.length == 0)
				return;
			members = new IPMComEvent[strmembers.length];
			for (int i = 0; i < members.length; i++)
				members[i] = (IPMComEvent) NAMEtoINFO.get(strmembers[i]);
		}
		SendDlg sd = new SendDlg(this, ipmsg, members);
		sd.setVisible(true);
	}
	
	void refreshAction() {
		ipmsg.refreshList();
	}
	
	void confAction() {
		ConfDlg cd = new ConfDlg(this, ipmsg);
		cd.setVisible(true);
	}
	
	void exitAction() {
		Dimension size = getSize();
		ipmsg.setPref("mainSizeX", Integer.toString(size.width));
		ipmsg.setPref("mainSizeY", Integer.toString(size.height));
		Point location = getLocation();
		ipmsg.setPref("mainX", Integer.toString(location.x));
		ipmsg.setPref("mainY", Integer.toString(location.y));
		ipmsg.exit();
		dispose();
	}
	
	String makeSortKey(IPMComEvent ipmce) {
		String tmpkey = ipmsg.getPref("sortKey");
		StringBuffer strbuf = new StringBuffer();
		for (int i = 0; i < tmpkey.length(); i++)
			switch (tmpkey.charAt(i)) {
			case 'u':
				String tmpuser;
				if (ipmce.getPack().getExtra() == null)
					tmpuser = ipmce.getPack().getUser();
				else
					tmpuser = ipmce.getPack().getExtra();
				strbuf.append(tmpuser);
				break;
			case 'g':
				if (ipmce.getPack().getGroup() != null)
					strbuf.append(ipmce.getPack().getGroup());
				else
					strbuf.append("  ");
				break;
			case 'h':
				strbuf.append(ipmce.getPack().getHost());
				break;
			}
		return new String(strbuf);
	}
	
	class RefreshList extends Thread {
		public void run() {
			String tmpgroup = Cp932.toJIS(groups.getSelectedItem());
			while (received) {
				received = false;
				try {
					sleep(500);
				} catch (InterruptedException ex) {}
				NAMEtoINFO = new Hashtable();
				ADDRtoINFO = new Hashtable();
				Hashtable groupcache = new Hashtable();
				memberlist.removeAll();
				groups.removeAll();
				groups.add(ipmsg.getPref("allName"));
				Enumeration members = ipmsg.getUserlist().elements();
				SortVector tmpvec = new SortVector();
				while (members.hasMoreElements()) {
					IPMComEvent tmpevent = (IPMComEvent) members.nextElement();
					tmpvec.addElement(makeSortKey(tmpevent), tmpevent);
				}
				members = tmpvec.elements();
				while (members.hasMoreElements()) {
					IPMComEvent tmpevent = (IPMComEvent) members.nextElement();
					IPMPack tmppack = tmpevent.getPack();
					if (tmppack.getGroup() != null
						&& groupcache.get(tmppack.getGroup()) == null) {
						groups.addItem(Cp932.toCp932(tmppack.getGroup()));
						groupcache.put(tmppack.getGroup(), tmppack.getGroup());
					}
					if (!tmpgroup.equals(ipmsg.getPref("allName")))
						if (tmppack.getGroup() == null)
							continue;
						else if (!tmppack.getGroup().equals(tmpgroup))
							continue;
					String tmpstr = Cp932.toCp932(ipmsg.makeListStr(tmppack));
					memberlist.add(tmpstr);
					NAMEtoINFO.put(tmpstr, tmpevent);
					ADDRtoINFO.put(tmpevent.getIPMAddress().toString()
						, tmpevent);
				}
				if (memberlist.getItemCount() == 0) {
					tmpgroup = ipmsg.getPref("allName");
					refreshing = true;
				} else
					groups.select(tmpgroup);
			}
			refreshing = false;
		}
	}
	
	synchronized void processEvent(IPMEvent ipme) {
		switch (ipme.getID()) {
		case IPMEvent.UPDATELIST_EVENT:
			received = true;
			if (!refreshing) {
				refreshing = true;
				new RefreshList().start();
			}
			break;
		case IPMEvent.RECEIVEMSG_EVENT:
			if (!ipmsg.lessThanReceiveMax()) {
				System.err.println("too many receive dialog.");
				break;
			}
			ipmsg.incReceiveCount();
			getToolkit().beep();
			RecvDlg rd = new RecvDlg(this, ipmsg
				, (IPMComEvent) ADDRtoINFO.get(ipme.getIPMAddress().toString())
				, ipme);
			rd.setVisible(true);
			break;
		case IPMEvent.READMSG_EVENT:
			getToolkit().beep();
			String tmpname = "";
			IPMComEvent tmpipmce
				= (IPMComEvent)ADDRtoINFO.get(ipme.getIPMAddress().toString());
			IPMPack tmppack;
			if (tmpipmce != null) {
				tmppack = tmpipmce.getPack();
				tmpname = ipmsg.makeListStr(tmppack);
			} else
				tmpname = ipme.getPack().getUser();
			StringBuffer strbuf = new StringBuffer();
			strbuf.append(ipmsg.getPref("readMsg") + "\n");
			strbuf.append(ipmsg.makeDateStr(ipme.getDate()));
			MsgBox mb = new MsgBox(this, Cp932.toCp932(tmpname)
				, Cp932.toCp932(new String(strbuf)), false);
			mb.setVisible(true);
			break;
		case IPMEvent.DELETEMSG_EVENT:
			getToolkit().beep();
			tmpname = "";
			tmpipmce = (IPMComEvent)
				ADDRtoINFO.get(ipme.getIPMAddress().toString());
			if (tmpipmce != null) {
				tmppack = tmpipmce.getPack();
				tmpname = ipmsg.makeListStr(tmppack);
			} else
				tmpname = ipme.getPack().getUser();
			strbuf = new StringBuffer();
			strbuf.append(ipmsg.getPref("deleteMsg") + "\n");
			strbuf.append(ipmsg.makeDateStr(ipme.getDate()));
			mb = new MsgBox(this, Cp932.toCp932(tmpname)
				, Cp932.toCp932(new String(strbuf)), false);
			mb.setVisible(true);
			break;
		case IPMEvent.CANTSENDMSG_EVENT:
			RetryDlg retry = new RetryDlg(this, ipmsg.getPref("appName")
				, ipmsg.getPref("retryMsg"), ipmsg, ipme);
			retry.setVisible(true);
			break;
		}
	}
}
