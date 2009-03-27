package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class ConfirmTicketUI extends Form implements CommandListener, ItemCommandListener {

	private UIController uiController;
	private Command cancelCommand;
	private Command confirmCommand;
	private StringItem theater;
	private StringItem movie;
	private StringItem showTimeStr;
	private StringItem seatsStr;
	private StringItem cost;
	private StringItem totalCost;
	private StringItem placeOrderBtn;

	public ConfirmTicketUI(UIController uiController) {
		super(uiController.getString(UIConstants.CONFIRM_TITLE));

		this.uiController = uiController;

		createItems();
		append(theater);
		append(movie);
		append(showTimeStr);
		append(seatsStr);
		append(cost);
		append(totalCost);
		append(placeOrderBtn);

		confirmCommand = new Command(uiController.getString(UIConstants.CONFIRM), Command.OK, 5);
		cancelCommand = new Command(uiController.getString(UIConstants.CANCEL), Command.EXIT, 5);

		addCommand(confirmCommand);
		addCommand(cancelCommand);
		setCommandListener(this);
		placeOrderBtn.setDefaultCommand(confirmCommand);
		placeOrderBtn.setItemCommandListener(this);
	}

	public void init(String theaterName, String movieName, int[] showTime, Seat[] seats) {
		if (seats.length > 0) {
			theater.setText(theaterName + " - ");
			movie.setText(movieName);
			// TODO: the date is temporary hardcoded, should query current date
			showTimeStr.setText(Utils.showTimeToDate(uiController, showTime) + " 3/17/2003 "
					+ Utils.showTimeToHourMinute(uiController, showTime));

			StringBuffer seatString = new StringBuffer(uiController
					.getString(UIConstants.SEATS_COLON)
					+ "\n");

			for (int i = 0; i < seats.length; i++) {
				seatString.append(" - " + uiController.getString(UIConstants.ROW)
						+ seats[i].getRowNumber() + ", " + uiController.getString(UIConstants.SEAT)
						+ seats[i].getSeatNumber() + "\n");
			}

			seatsStr.setText(seatString.toString());
			cost.setText(seats.length + " " + uiController.getString(UIConstants.TICKET_PRICE_MSG));
			totalCost.setText(uiController.getString(UIConstants.TOTAL_COST)
					+ (seats.length * UIConstants.TICKET_PRICE)
					+ uiController.getString(UIConstants.HANDLING));
		}
	}

	public void commandAction(Command command, Item item) {
		if (command == confirmCommand) {
			uiController.purchaseRequested();
		}
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == cancelCommand) {
			uiController.mainMenuRequested();
		} else if (command == confirmCommand) {
			uiController.purchaseRequested();
		}
	}

	private void createItems() {
		theater = new StringItem(null, null);
		movie = new StringItem(null, null);
		showTimeStr = new StringItem(null, null);
		seatsStr = new StringItem(null, null);
		cost = new StringItem(null, null);
		totalCost = new StringItem(null, null);
		placeOrderBtn = new StringItem(null, uiController.getString(UIConstants.PLACE_ORDER),
				Item.BUTTON);

		placeOrderBtn.setLayout(Item.LAYOUT_CENTER);
	}

}
