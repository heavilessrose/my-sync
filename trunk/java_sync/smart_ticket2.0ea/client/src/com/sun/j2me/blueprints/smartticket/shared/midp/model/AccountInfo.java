package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class AccountInfo {

	private String userName = "";
	private String password = "";
	private String creditCardNumber = "";
	private String creditCardHolderName = "";
	private String creditCardExpirationDate = "";

	public AccountInfo() {
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getUserName() {
		return userName;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getCreditCardNumber() {
		return creditCardNumber;
	}

	public void setCreditCardNumber(String creditCardNumber) {
		this.creditCardNumber = creditCardNumber;
	}

	public String getCreditCardHolderName() {
		return creditCardHolderName;
	}

	public void setCreditCardHolderName(String creditCardHolderName) {
		this.creditCardHolderName = creditCardHolderName;
	}

	public String getCreditCardExpirationDate() {
		return creditCardExpirationDate;
	}

	public void setCreditCardExpirationDate(String creditCardExpirationDate) {
		this.creditCardExpirationDate = creditCardExpirationDate;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeUTF(userName);
			dataStream.writeUTF(password);
			dataStream.writeUTF(creditCardNumber);
			dataStream.writeUTF(creditCardHolderName);
			dataStream.writeUTF(creditCardExpirationDate);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static AccountInfo deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			AccountInfo accountInfo = new AccountInfo();

			accountInfo.userName = dataStream.readUTF();
			accountInfo.password = dataStream.readUTF();
			accountInfo.creditCardNumber = dataStream.readUTF();
			accountInfo.creditCardHolderName = dataStream.readUTF();
			accountInfo.creditCardExpirationDate = dataStream.readUTF();

			return accountInfo;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

}
