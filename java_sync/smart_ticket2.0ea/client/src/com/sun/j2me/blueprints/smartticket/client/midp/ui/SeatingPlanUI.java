package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Movie;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.TheaterSchedule;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class SeatingPlanUI extends Form implements CommandListener {

	private Command buyCommand;
	private SeatingPlanCustomItem seatingPlanItem;
	private int currentRowNumber;
	private int currentSeatNumber;
	private UIController uiController;
	private Ticker ticker;
	private SeatingPlan seatingPlan;
	private StringItem showInfo;

	public SeatingPlanUI(UIController uiController) {
		super(uiController.getString(UIConstants.SELECT_SEATS_TITLE));

		this.uiController = uiController;
		buyCommand = new Command(uiController.getString(UIConstants.BUY), Command.OK, 1);
		seatingPlanItem = new SeatingPlanCustomItem(uiController, null);

		seatingPlanItem.setLayout(Item.LAYOUT_CENTER);

		ticker = new Ticker(uiController.getString(UIConstants.SEATING_PLAN_MESSAGE));
		showInfo = new StringItem(null, null);

		append(showInfo);
		append(seatingPlanItem);
		setTicker(ticker);
		addCommand(buyCommand);
		setCommandListener(this);
	}

	/**
	 * Initialize the seating canvas variables.
	 * @exception IOException  when some type of network error occurs.
	 */
	public void init(String theaterName, String movieName, SeatingPlan myseatingPlan, int[] showTime) {
		showInfo.setText(theaterName + " - " + movieName + "\n"
				+ Utils.showTimeToDate(uiController, showTime) + " "
				+ Utils.showTimeToHourMinute(uiController, showTime));
		showInfo.setLayout(Item.LAYOUT_CENTER);

		this.seatingPlan = myseatingPlan;

		seatingPlanItem.init(myseatingPlan);
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == buyCommand) {
			if (seatingPlan == null) {
				uiController.mainMenuRequested();
			}

			if (seatingPlan.getBookedSeatsCount() > 0) {
				uiController.seatsSelected(seatingPlan.getBookedSeats());
			}
		} else {
			uiController.commandAction(command, displayable);
		}
	}

}
