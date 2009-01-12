package net.sf.mvc.mobile.command;

import javax.microedition.lcdui.Displayable;

import net.sf.mvc.mobile.Navigation;

public class NavigationCommand extends ActionCommand {

	private final Navigation navigation;

	public NavigationCommand(Navigation navigation, String label, int type, int priority) {
		super(label, type, priority);
		this.navigation = navigation;
	}
	public NavigationCommand(String navi, String label, int type, int priority) {
		this(new Navigation(navi, null), label, type, priority);
	}

	public Navigation execute(Displayable d) throws Exception {
		return navigation;
	}
}
