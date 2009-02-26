package winkCC.net;

import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

public class FormShow extends Form implements CommandListener {
	ILog log = LogFactory.getLog("FormShow");

	public static Command exitCommand = new Command("Exit", Command.EXIT, 1);
	public static Command scheduleCmd = new Command("Schedule", Command.SCREEN,
			1);

	private static FormShow instance;

	public FormShow(String title) {
		super(title);
		// TODO Auto-generated constructor stub

		 addCommand(exitCommand);
		// addCommand(scheduleCmd);
		setCommandListener(this);
		instance = this;
	}

	public static FormShow getInstance() {
		return instance;
	}

	public void commandAction(Command cmd, Displayable disp) {
		if (cmd == exitCommand) {
			new Thread(){
				public void run(){
					HttpPushTestMidlet.getInstance().exit();
				}
			}.start();
		} else if (cmd == scheduleCmd) { // 设置一个按时间间隔运行的任务
			/* Thread th = */new Thread() {
				public void run() {
					Timer aTimer = new Timer();
					MyTask myTask = new MyTask();
					aTimer.schedule(myTask, 0, 5000);
				}
			}.start();
		}
	}

}

class MyTask extends TimerTask {
	ILog log = LogFactory.getLog("MyTask");

	public MyTask() {
	}

	public void run() {
		log.info("Cycling underground task");
	}
}
