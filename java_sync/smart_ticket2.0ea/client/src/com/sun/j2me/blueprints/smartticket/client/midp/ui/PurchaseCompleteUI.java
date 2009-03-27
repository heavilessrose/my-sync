package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class PurchaseCompleteUI extends Form implements CommandListener {

	private UIController uiController;
	private Command confirmCommand;
	private StringItem theater;
	private StringItem movie;
	private StringItem showTimeStr;
	private StringItem msg;
	private StringItem purchaseNumStr;

	public PurchaseCompleteUI(UIController uiController) {
		super(uiController.getString(UIConstants.PURCHASE_COMPLETE_TITLE));

		this.uiController = uiController;

		createItems();
		append(msg);
		append(purchaseNumStr);
		append(theater);
		append(movie);
		append(showTimeStr);

		confirmCommand = new Command(uiController.getString(UIConstants.DONE), Command.OK, 5);

		addCommand(confirmCommand);
		setCommandListener(this);
	}

	public void init(String purchaseNum, String theaterName, String movieName, int[] showTime) {
		theater.setText(theaterName + " - ");
		movie.setText(movieName);
		// TODO: the date is temporary hardcoded, should query current date
		showTimeStr.setText(Utils.showTimeToDate(uiController, showTime) + " 4/17/2003 "
				+ Utils.showTimeToHourMinute(uiController, showTime));
		purchaseNumStr.setText(uiController.getString(UIConstants.PURCHASE_NUM) + purchaseNum);
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == confirmCommand) {
			uiController.mainMenuRequested();
		}
	}

	private void createItems() {
		theater = new StringItem(null, null);
		movie = new StringItem(null, null);
		showTimeStr = new StringItem(null, null);
		purchaseNumStr = new StringItem(null, null);
		msg = new StringItem(null, uiController.getString(UIConstants.PURCHASE_COMPLETE_MSG));
	}

}
