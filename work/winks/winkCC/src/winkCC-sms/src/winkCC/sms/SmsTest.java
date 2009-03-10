/**
 * 
 */
package winkCC.sms;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;

/**
 * 短信模块测试.
 * 
 * 测试发送与接收.
 * 
 * @author WangYinghua
 * 
 */
public class SmsTest extends MIDlet implements CommandListener, MessageListener {
	Display display = null;
	Form form = null;
	final Command send = new Command("Send", Command.OK, 1);
	final Command back = new Command("Back", Command.BACK, 2);
	final Command exit = new Command("Exit", Command.EXIT, 3);

	TextField numField = new TextField("电话", "15801630382", 100,
			TextField.PHONENUMBER);
	TextField contentField = new TextField("内容", "test123测试", 100,
			TextField.ANY);

	String address = "sms://15801630382:5050";
	String smsMonitorPort = "5050";
	MessageConnection mc = null;
	SmsMessage sms = SmsMessage.getInstance();

	public SmsTest() {
		display = Display.getDisplay(this);
		form = new Form("sms test");
		mc = sms.newMessageConnection("sms://:" + smsMonitorPort, this);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#destroyApp(boolean)
	 */
	//	@Override
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#pauseApp()
	 */
	//	@Override
	protected void pauseApp() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see javax.microedition.midlet.MIDlet#startApp()
	 */
	//	@Override
	protected void startApp() throws MIDletStateChangeException {
		form.addCommand(send);
		form.addCommand(back);
		form.addCommand(exit);
		form.append(numField);
		form.append(contentField);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	public void commandAction(Command cmd, Displayable disp) {
		System.out.println("cmd = " + cmd.toString());
		if (cmd == send) {
			String destNum = numField.getString();
			if (destNum != null && !destNum.equals(""))
				address = "sms://" + destNum + ":5050";
			sms.sendTextMessage(mc, contentField.getString(), address);
		} else if (cmd == back) {
			display.setCurrent(form);
		} else if (cmd == exit) {
			disposeSms();
			try {
				destroyApp(true);
			} catch (MIDletStateChangeException e) {
				e.printStackTrace();
			}
			notifyDestroyed();
		}
	}

	public void disposeSms() {
		sms.closeConnection(mc);
		if (sms != null)
			sms = null;
		if (mc != null)
			mc = null;
	}

	public void notifyIncomingMessage(MessageConnection mc) {
		System.out.println("msssage received:");
		//		sms.readMessageThreaded(mc);
		sms.readMessage(mc);
		// 如何在多线程时保证receives != null ?, 此处未使用多线程
		Item[] receives = sms.getReceives();
		for (int i = 0; i < receives.length; i++) {
			form.append(receives[i]);
		}
	}
}
