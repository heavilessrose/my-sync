import java.util.Enumeration;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

import winkCC.pim.ContactUtil;
import winkCC.pim.People;

public class PimTest extends MIDlet implements CommandListener {
	Display display = null;
	Form form = null;
	final Command test = new Command("test", Command.OK, 1);
	final Command showPeople = new Command("showPeople", Command.OK, 1);
	final Command showContacts = new Command("showContacts", Command.OK, 2);
	final Command refresh = new Command("refresh", Command.OK, 2);
	final Command setWpath = new Command("setWpath", Command.OK, 2);
	final Command exit = new Command("Exit", Command.EXIT, 3);

	TextField name = new TextField("name", "王映华", 100, TextField.ANY);
	TextField attr = new TextField("attr", "mobile", 100, TextField.ANY);
	TextField num = new TextField("电话", "", 100, TextField.PHONENUMBER);
	TextField wpath = new TextField("wpath", "", 100, TextField.ANY);

	public PimTest() {
		display = Display.getDisplay(this);
		form = new Form("pim test");
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
		form.addCommand(test);
		form.addCommand(showPeople);
		form.addCommand(showContacts);
		form.addCommand(refresh);
		form.addCommand(setWpath);
		form.addCommand(exit);
		form.append(name);
		form.append(attr);
		form.append(num);
		form.append(wpath);
		form.setCommandListener(this);
		display.setCurrent(form);
	}

	ContactUtil util = new ContactUtil();

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == test) {
			new Thread() {
				public void run() {
					ContactUtil.test();
				}
			}.start();
		} else if (cmd == showContacts) {
			new Thread() {
				People people = null;

				public void run() {
					for (Enumeration peoples = util.getAllPeople().elements(); peoples
							.hasMoreElements();) {
						people = (People) peoples.nextElement();
						form.append(people.toString());
					}
				}
			}.start();

		} else if (cmd == showPeople) {
			new Thread() {
				public void run() {
					People people = util.getPeople(name.getString());
					String nums = util.getNumber(people, attr.getString());
					System.out.println(people.toString());
					num.setString(nums);

					String path = util.getWPath(people);
					wpath.setString(path);
				}
			}.start();
		} else if (cmd == refresh) {
			form.deleteAll();
			form.append(name);
			form.append(attr);
			num.setString("");
			form.append(num);
			wpath.setString("");
			form.append(wpath);
		} else if (cmd == setWpath) {
			new Thread() {
				public void run() {
					util.setWPath(name.getString(), wpath.getString());
				}
			}.start();
		} else if (cmd == exit) {
			try {
				destroyApp(true);
			} catch (MIDletStateChangeException e) {
				e.printStackTrace();
			}
			notifyDestroyed();
		}
	}
}
