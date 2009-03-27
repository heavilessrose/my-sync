package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

public class MySettingsUI extends List implements CommandListener {

	private UIController uiController;
	private Command backCommand;

	public MySettingsUI(UIController uiController) {
		super(uiController.getString(UIConstants.MY_SETTINGS_TITLE), List.IMPLICIT);

		this.uiController = uiController;

		append(uiController.getString(UIConstants.DEFAULTS), null);
		append(uiController.getString(UIConstants.ACCOUNT_INFO), null);
		append(uiController.getString(UIConstants.CHANGE_PASSWORD), null);

		backCommand = new Command(uiController.getString(UIConstants.BACK), Command.BACK, 1);

		addCommand(backCommand);
		setCommandListener(this);
	}

	public void commandAction(Command command, Displayable displayable) {
		String label = getString(getSelectedIndex());

		if (command == List.SELECT_COMMAND) {
			if (label.equals(uiController.getString(UIConstants.DEFAULTS))) {
				uiController.defaultsRequested();
			} else if (label.equals(uiController.getString(UIConstants.ACCOUNT_INFO))) {
				uiController.accountInfoRequested();
			} else if (label.equals(uiController.getString(UIConstants.CHANGE_PASSWORD))) {
				uiController.changePasswordRequested();
			}
		} else {
			if (command == backCommand) {
				uiController.mainMenuRequested();
			} else {
				uiController.commandAction(command, displayable);
			}
		}
	}

}
