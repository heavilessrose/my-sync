package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

/** 进度等待观察者界面 */
public class ProgressObserverUI extends Form implements ProgressObserver, CommandListener {

	private UIController uiController;
	private static final int GAUGE_MAX = 8;
	private static final int GAUGE_LEVELS = 4;
	int current = 0;
	Gauge gauge;
	Command stopCommand;
	boolean stoppable;
	boolean stopped;

	/** 等待界面构造方法 */
	public ProgressObserverUI(UIController uiController) {
		super("");

		gauge = new Gauge("", false, GAUGE_MAX, 0);
		stopCommand = new Command(uiController.getString(UIConstants.STOP), Command.STOP, 10);

		append(gauge);
		setCommandListener(this);
	}

	public void init(String note, boolean stoppable) {
		gauge.setValue(0);
		setNote(note);
		setStoppable(stoppable);

		stopped = false;
	}

	public void setNote(String note) {
		setTitle(note);
	}

	public boolean isStoppable() {
		return stoppable;
	}

	public void setStoppable(boolean stoppable) {
		this.stoppable = stoppable;

		if (stoppable) {
			addCommand(stopCommand);
		} else {
			removeCommand(stopCommand);
		}
	}

	/** Indicates whether the user has stopped the progress. This message should be called before calling update. */
	public boolean isStopped() {
		return stopped;
	}

	public void updateProgress() {
		current = (current + 1) % GAUGE_LEVELS;

		gauge.setValue(current * GAUGE_MAX / GAUGE_LEVELS);
	}

	public void commandAction(Command c, Displayable d) {
		if (c == stopCommand) {
			stopped = true;
		}
	}

}
