package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;

public class ChangePasswordUI extends Form implements CommandListener {

	private UIController uiController;
	private AccountInfo accountInfo;
	private Command saveCommand;
	private Command cancelCommand;
	private StringItem accountNameField;
	private StringItem loginField;
	private TextField passwordField;
	private TextField verifyPasswordField;

	public ChangePasswordUI(UIController uiController) {
		super(uiController.getString(UIConstants.CHANGE_PASSWORD_TITLE));

		this.uiController = uiController;

		createItems();
		append(accountNameField);
		append(loginField);
		append(passwordField);
		append(verifyPasswordField);

		saveCommand = new Command(uiController.getString(UIConstants.SAVE), Command.OK, 5);
		cancelCommand = new Command(uiController.getString(UIConstants.CANCEL), Command.CANCEL, 5);

		addCommand(saveCommand);
		addCommand(cancelCommand);
		setCommandListener(this);
	}

	public void init(AccountInfo accountInfo) {
		this.accountInfo = accountInfo;

		// accountNameField.setText(accountInfo.getAccountName());
		loginField.setText(accountInfo.getUserName());
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == cancelCommand) {
			uiController.mySettingsRequested();
		} else if (command == saveCommand) {
			String password = null;
			String verifyPassword = null;

			password = passwordField.getString();

			if (password == null || password.length() < UIConstants.PASSWORD_LENGTH) {
				uiController
						.showErrorAlert(uiController.getString(UIConstants.PASSWORD_BAD_LENGTH));

				return;
			}

			verifyPassword = verifyPasswordField.getString();

			if (verifyPassword == null || !verifyPassword.equals(password)) {
				uiController.showErrorAlert(uiController
						.getString(UIConstants.PASSWORDS_DO_NOT_MATCH));

				return;
			}

			accountInfo.setPassword(password);
			uiController.accountInfoSaved(accountInfo);
		}
	}

	private void createItems() {
		accountNameField = new StringItem(uiController.getString(UIConstants.ACCOUNT_NAME), null);
		loginField = new StringItem(uiController.getString(UIConstants.LOGIN), null);
		passwordField = new TextField(uiController.getString(UIConstants.PASSWORD), null, 20,
				TextField.PASSWORD);
		verifyPasswordField = new TextField(uiController.getString(UIConstants.RE_ENTER_PASSWORD),
				null, 20, TextField.PASSWORD);
	}

}
