package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import java.io.IOException;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Theater;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class ManageTheaterSchedulesUI extends List implements CommandListener {

	private UIController uiController;
	private Theater[] theaters;
	private Command downloadCommand;
	private Command deleteCommand;
	private ChoiceGroup theatersCG;

	public ManageTheaterSchedulesUI(UIController uiController) {
		super(uiController.getString(UIConstants.MANAGE_THEATER_SCHEDULES), List.IMPLICIT);

		this.uiController = uiController;
		downloadCommand = new Command(uiController.getString(UIConstants.UPDATE_SCHEDULE),
				Command.ITEM, 5);
		deleteCommand = new Command(uiController.getString(UIConstants.DELETE_SCHEDULE),
				Command.ITEM, 5);

		setCommandListener(this);
	}

	public void init(Theater[] theaters, Theater[] downloadedTheaters) {
		this.theaters = theaters;

		/* need to clean up the list */
		deleteAll();
		Utils.appendTheaterList(uiController, this, theaters, downloadedTheaters);
		addCommand(downloadCommand);
		addCommand(deleteCommand);
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == downloadCommand) {
			Theater selectedTheater = theaters[getSelectedIndex()];

			uiController.downloadTheaterSchedule(selectedTheater);
		} else if (command == deleteCommand) {
			Theater selectedTheater = theaters[getSelectedIndex()];

			uiController.theaterScheduleDeletionRequested(selectedTheater);
		} else {
			uiController.commandAction(command, displayable);
		}
	}

}
