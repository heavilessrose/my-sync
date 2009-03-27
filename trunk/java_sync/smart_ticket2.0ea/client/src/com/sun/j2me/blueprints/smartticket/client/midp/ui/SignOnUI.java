package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

public class SignOnUI extends Form implements CommandListener {

	/**
	 * The runnable to execute if the sign-on succeeds.
	 */
	private Runnable authorizedRunnable;
	private UIController uiController;
	private TextField userNameField;
	private TextField passwordField;
	private Command signOnCommand;

	public SignOnUI(UIController uiController) {
		super( // uiController.getString(UIMessageCodes.SIGN_IN)
				"Sign In");

		this.uiController = uiController;
		userNameField = new TextField( // uiController.getString(UIMessageCodes.LOGIN),
				"Login:", null, 12, TextField.ANY);
		passwordField = new TextField( // uiController.getString(UIMessageCodes.PASSWORD),
				"Password:", null, 20, TextField.PASSWORD);
		signOnCommand = new Command( // uiController.getString(UIMessageCodes.SIGN_IN),
				"Signing in...", Command.OK, 5);

		append(userNameField);
		append(passwordField);
		addCommand(signOnCommand);
		setCommandListener(this);
	}

	public void init(Runnable authorizedRunnable) {
		this.authorizedRunnable = authorizedRunnable;

		userNameField.setString(null);
		passwordField.setString(null);
	}

	public void commandAction(Command command, Displayable displayable) {
		uiController.signOn(authorizedRunnable, userNameField.getString(), passwordField
				.getString());
	}

}
