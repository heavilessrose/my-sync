package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

public class MainMenuUI extends List implements CommandListener {

	private UIController uiController;
	private Command exitCommand;

	public MainMenuUI(UIController uiController) {
		super(uiController.getString(UIConstants.SMART_TICKET), List.IMPLICIT);

		this.uiController = uiController;

		append(uiController.getString(UIConstants.CHOOSE_MOVIE), null);
		append(uiController.getString(UIConstants.RATE_MOVIES), null);
		append(uiController.getString(UIConstants.MY_SETTINGS_TITLE), null);
		append(uiController.getString(UIConstants.MANAGE_THEATER_SCHEDULES), null);

		exitCommand = new Command(uiController.getString(UIConstants.EXIT), Command.EXIT, 1);

		addCommand(exitCommand);
		setCommandListener(this);
	}

	public void commandAction(Command command, Displayable displayable) {
		String label = getString(getSelectedIndex());

		if (label.equals(uiController.getString(UIConstants.CHOOSE_MOVIE))) {
			uiController.chooseMovieRequested();
		} else if (label.equals(uiController.getString(UIConstants.MY_SETTINGS_TITLE))) {
			uiController.mySettingsRequested();
		} else if (label.equals(uiController.getString(UIConstants.RATE_MOVIES))) {
			uiController.rateMyMoviesRequested();
		} else if (label.equals(uiController.getString(UIConstants.MANAGE_THEATER_SCHEDULES))) {
			uiController.manageTheaterSchedulesRequested();
		} else {
			if (command == exitCommand) {
				uiController.exitRequested();
			} else {
				uiController.commandAction(command, displayable);
			}
		}
	}

}
