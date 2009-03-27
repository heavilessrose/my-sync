package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class PurchaseTicketsUI extends Form implements CommandListener {

	private UIController uiController;
	private AccountInfo accountInfo;
	private Command cancelCommand;
	private Command nextCommand;
	private ChoiceGroup cardList;
	private ChoiceGroup monthList;
	private ChoiceGroup yearList;
	private StringItem signOnMessage;
	private StringItem expDateMessage;
	private TextField cardNumField;
	// private TextField passwordField;
	private Ticker ticker;

	public PurchaseTicketsUI(UIController uiController) {
		super(uiController.getString(UIConstants.SIGN_ON_TITLE));

		this.uiController = uiController;

		createItems();
		append(signOnMessage);
		append(cardList);
		append(expDateMessage);
		append(monthList);
		append(yearList);
		append(cardNumField);
		// append(passwordField);
		setTicker(ticker);

		nextCommand = new Command(uiController.getString(UIConstants.NEXT), Command.OK, 5);
		cancelCommand = new Command(uiController.getString(UIConstants.CANCEL), Command.EXIT, 5);

		addCommand(nextCommand);
		addCommand(cancelCommand);
		setCommandListener(this);
	}

	public void init(AccountInfo accountInfo) {
		this.accountInfo = accountInfo;

		cardNumField.setString(accountInfo.getCreditCardNumber());
		Utils.setExpirationDate(uiController, accountInfo.getCreditCardExpirationDate(), monthList,
				yearList);
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == cancelCommand) {
			uiController.mainMenuRequested();
		} else if (command == nextCommand) {
			if (cardNumField.getString().length() == 0) {
				uiController.showErrorAlert(uiController
						.getString(UIConstants.CREDITCARD_NUM_BAD_LENGTH));

				return;
			}

			/*
			 * String password = passwordField.getString();
			 * if (!password.equals(accountInfo.getPassword())) {
			 * uiController.showErrorAlert(
			 * uiController.getString(UIConstants.PASSWORD_INCORRECT));
			 * return;
			 * }
			 */
			uiController.signOnConfirmed();
		}
	}

	private void createItems() {
		cardList = Utils.newCardPopUp(uiController);
		yearList = Utils.newYearPopUp(uiController);
		monthList = Utils.newMonthPopUp(uiController);
		signOnMessage = new StringItem(uiController.getString(UIConstants.SIGN_ON_MESSAGE), null);
		expDateMessage = new StringItem(uiController.getString(UIConstants.EXPIR_DATE), null);
		cardNumField = new TextField(uiController.getString(UIConstants.CARD_NUM), null, 19,
				TextField.NUMERIC);

		/*
		 * passwordField = new TextField(
		 * uiController.getString(UIConstants.PASSWORD),
		 * null, 10, TextField.PASSWORD);
		 */
		ticker = new Ticker(uiController.getString(UIConstants.SIGN_ON_TICKER));
	}

}
