package winkCC.net.http;

import java.io.IOException;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextBox;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class HttpTest extends MIDlet implements CommandListener {

	HttpUtil util = new HttpUtil();
	HttpWaitUI wait = new HttpWaitUI();

	static Display disp = null;
	static Form form = new Form("Http Test");
	TextBox text = new TextBox("response", "init texts", 100, TextField.ANY);

	TextField url = new TextField("url: ", /*"http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4762500"*/"www.sina.com", 100,
			TextField.ANY);

	Command get = new Command("get", Command.OK, 1);
	Command stop = new Command("stop", Command.OK, 1);
	Command back = new Command("back", Command.BACK, 1);
	Command exit = new Command("exit", Command.EXIT, 1);

	public HttpTest() {
		disp = Display.getDisplay(this);
	}

	//	@Override
	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {

	}

	//	@Override
	protected void pauseApp() {

	}

	//	@Override
	protected void startApp() throws MIDletStateChangeException {
		form.addCommand(get);
		form.addCommand(stop);
		form.addCommand(back);
		form.addCommand(exit);
		form.append(url);
		disp.setCurrent(form);
		form.setCommandListener(this);
	}

	public void commandAction(Command cmd, Displayable dis) {
		if (cmd == get) {
//			disp.setCurrent(wait);
			new Thread() {
				public void run() {
					try {
						util.init(url.getString(), false);
						util.post(new byte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}.start();

		} else if (cmd == stop) {
			util.cancel();
		} else if (cmd == back) {

		} else if (cmd == exit) {

		}
	}

	public static void goback() {
		disp.setCurrent(form);
	}
}
