package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class BillingInfoUI extends Form implements CommandListener, ItemCommandListener {

	private UIController uiController;
	private AccountInfo accountInfo;
	private Command exitCommand;
	private Command doneCommand;
	private ChoiceGroup cardList;
	private ChoiceGroup monthList;
	private ChoiceGroup yearList;
	private ChoiceGroup fillOutLater;
	private StringItem done;
	private StringItem expDateMessage;
	private TextField creditCardNumField;

	public BillingInfoUI(UIController uiController) {
		super(uiController.getString(UIConstants.BILLING_INFORMATION));

		this.uiController = uiController;

		createItems();
		append(cardList);
		append(expDateMessage);
		append(monthList);
		append(yearList);
		append(creditCardNumField);
		append(fillOutLater);
		append(done);

		exitCommand = new Command(uiController.getString(UIConstants.EXIT), Command.EXIT, 5);
		doneCommand = new Command(uiController.getString(UIConstants.DONE), Command.ITEM, 5);

		done.setDefaultCommand(doneCommand);
		done.setItemCommandListener(this);
		addCommand(exitCommand);
		addCommand(doneCommand);
		setCommandListener(this);
	}

	public void init(AccountInfo accountInfo) {
		this.accountInfo = accountInfo;
	}

	public void commandAction(Command command, Item item) {
		if (command == doneCommand) {
			checkDoneCommand();
		}
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == exitCommand) {
			uiController.exitRequested();
		} else if (command == doneCommand) {
			checkDoneCommand();
		}
	}

	private void checkDoneCommand() {
		if (!fillOutLater.isSelected(0)) {
			String cardNum = creditCardNumField.getString();

			if (cardNum == null || cardNum.length() == 0) {
				uiController.showErrorAlert(uiController
						.getString(UIConstants.CREDITCARD_NUM_BAD_LENGTH));

				return;
			}

			// Set the information.
			String month = monthList.getString(monthList.getSelectedIndex()).substring(0, 3);
			;
			String year = yearList.getString(yearList.getSelectedIndex());

			accountInfo.setCreditCardExpirationDate(month + year);
			accountInfo.setCreditCardNumber(cardNum);
			uiController.billingInfoSaved(accountInfo);
		} else {
			uiController.mainMenuRequested();
		}
	}

	private void createItems() {
		cardList = Utils.newCardPopUp(uiController);
		monthList = Utils.newMonthPopUp(uiController);
		yearList = Utils.newYearPopUp(uiController);
		fillOutLater = new ChoiceGroup(null, Choice.MULTIPLE);

		fillOutLater.append(uiController.getString(UIConstants.FILL_OUT_LATER), null);

		expDateMessage = new StringItem(uiController.getString(UIConstants.EXPIR_DATE), null);
		done = new StringItem(uiController.getString(UIConstants.DONE), null, Item.BUTTON);

		done.setLayout(Item.LAYOUT_CENTER);

		creditCardNumField = new TextField(uiController.getString(UIConstants.CARD_NUM), null, 19,
				TextField.NUMERIC);
	}

}
