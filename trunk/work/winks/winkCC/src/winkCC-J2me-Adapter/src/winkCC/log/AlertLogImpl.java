package winkCC.log;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Display;

public class AlertLogImpl implements ILog, ILogConfigurator {
	private String shortName;

	// static Display display;
	// public AlertLogImpl(Display display){
	// this.display = display;
	// }
	//
	// public AlertLogImpl(){
	// this(display);
	// }

	private void doLog(final String level, final String message,
			final Throwable e) {
		final StringBuffer msg = new StringBuffer();
		msg.append("[").append(shortName).append("] ").append(level).append(
				" - ").append(message);

		Alert alert = new Alert("Log", msg.toString(), null, AlertType.ERROR);
		// if (e != null) {
		// e.printStackTrace();
		// }
		alert.setTimeout(2000);
		// display.setCurrent(alert);
	}

	public void debug(String message) {
		doLog(DEBUG, message, null);
	}

	public void debug(String message, Throwable e) {
		doLog(DEBUG, message, e);
	}

	public void debug(Throwable e) {
		doLog(DEBUG, null, e);
	}

	public void error(String message) {
		doLog(ERROR, message, null);
	}

	public void error(String message, Throwable e) {
		doLog(ERROR, message, e);
	}

	public void error(Throwable e) {
		doLog(ERROR, null, e);
	}

	public void fatal(String message) {
		doLog(FATAL, message, null);
	}

	public void fatal(String message, Throwable e) {
		doLog(FATAL, message, e);
	}

	public void fatal(Throwable e) {
		doLog(FATAL, null, e);
	}

	public void info(String message) {
		doLog(INFO, message, null);
	}

	public void warn(String message) {
		doLog(WARN, message, null);
	}

	public void warn(String message, Throwable e) {
		doLog(WARN, message, e);
	}

	public void warn(Throwable e) {
		doLog(WARN, null, e);
	}

	public void setName(String name) {
		this.shortName = name;
	}

	public void clear() {
		// TODO Auto-generated method stub

	}

	public String getContent() {
		// TODO Auto-generated method stub
		return null;
	}

}
