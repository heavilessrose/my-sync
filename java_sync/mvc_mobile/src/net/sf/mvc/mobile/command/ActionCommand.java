package net.sf.mvc.mobile.command;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Displayable;

import net.sf.mvc.mobile.Navigation;

public abstract class ActionCommand extends Command {

	protected Object parameter;

	public ActionCommand(String label, int commandType, int priority) {
		super(label, commandType, priority);
	}

	public ActionCommand() {
		super(null, 0, 0);
	}

	/**
	 * @return identifier of the next Displayable to show or null if no view
	 *         change is required
	 */
	public abstract Navigation execute(Displayable d) throws Exception;

	public void setParameter(Object parameter) {
		this.parameter = parameter;
	}
}
