package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

/** 显示一个交互式UI（用户界面）屏幕，等待用户输入。
 * 当用户通过按键或从列表中选择一个条目时将产生一个UI事件，视图类的事件处理程序捕获这个事件，并将控制传递给控制器类。 */
public class AccountInfoUI extends Form implements CommandListener {

	private UIController uiController;
	private AccountInfo accountInfo;
	private Command cancelCommand;
	private Command saveCommand;
	private ChoiceGroup cardList;
	private ChoiceGroup monthList;
	private ChoiceGroup yearList;
	private StringItem expDateMessage;
	// private TextField accountNameField;
	private TextField loginField;
	private TextField creditCardNumField;
	private Ticker ticker;

	public AccountInfoUI(UIController uiController) {
		super(uiController.getString(UIConstants.ACCOUNT_INFO_TITLE));

		this.uiController = uiController;

		createItems();
		// append(accountNameField);
		append(loginField);
		append(cardList);
		append(expDateMessage);
		append(monthList);
		append(yearList);
		append(creditCardNumField);
		setTicker(ticker);

		saveCommand = new Command(uiController.getString(UIConstants.SAVE), Command.OK, 5);
		cancelCommand = new Command(uiController.getString(UIConstants.CANCEL), Command.EXIT, 5);

		addCommand(saveCommand);
		addCommand(cancelCommand);
		setCommandListener(this);
	}

	public void init(AccountInfo accountInfo) {
		this.accountInfo = accountInfo;

		creditCardNumField.setString(accountInfo.getCreditCardNumber());
		loginField.setString(accountInfo.getUserName());
		// accountNameField.setString(accountInfo.getAccountName());
		Utils.setExpirationDate(uiController, accountInfo.getCreditCardExpirationDate(), monthList,
				yearList);
		// TODO: credit cardtype is not stored into accountInfo currently.
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == cancelCommand) {
			uiController.mySettingsRequested();
		} else if (command == saveCommand) {

			/*
			 * String accountName = accountNameField.getString();
			 * if (accountName == null || accountName.length() == 0) {
			 * uiController.showErrorAlert(
			 * uiController.getString(UIConstants.ACCOUNTNAME_BAD_LENGTH));
			 * return;
			 * }
			 */
			String login = loginField.getString();

			if (login == null || login.length() < UIConstants.LOGIN_LENGTH) {
				uiController.showErrorAlert(uiController.getString(UIConstants.LOGIN_BAD_LENGTH));

				return;
			}

			String cardNum = creditCardNumField.getString();

			if (cardNum == null || cardNum.length() == 0) {
				uiController.showErrorAlert(uiController
						.getString(UIConstants.CREDITCARD_NUM_BAD_LENGTH));

				return;
			}

			// Set the information.
			// accountInfo.setAccountName(accountName);
			accountInfo.setUserName(login);

			String month = monthList.getString(monthList.getSelectedIndex()).substring(0, 3);
			;
			String year = yearList.getString(yearList.getSelectedIndex());

			accountInfo.setCreditCardExpirationDate(month + year);
			accountInfo.setCreditCardNumber(cardNum);
			uiController.accountInfoSaved(accountInfo);
		}
	}

	private void createItems() {
		cardList = Utils.newCardPopUp(uiController);
		monthList = Utils.newMonthPopUp(uiController);
		yearList = Utils.newYearPopUp(uiController);
		expDateMessage = new StringItem(uiController.getString(UIConstants.EXPIR_DATE), null);
		creditCardNumField = new TextField(uiController.getString(UIConstants.CARD_NUM), null, 19,
				TextField.NUMERIC);
		ticker = new Ticker(uiController.getString(UIConstants.SIGN_ON_TICKER));

		/*
		 * accountNameField    = new TextField(uiController.getString(
		 * UIConstants.ACCOUNT_NAME),
		 * null, 12, TextField.ANY);
		 */
		loginField = new TextField(uiController.getString(UIConstants.LOGIN), null, 12,
				TextField.ANY);
	}

}
