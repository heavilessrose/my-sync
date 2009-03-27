package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class ModelException extends Exception {

	public static final byte CAUSE_GENERAL = 0;
	public static final byte CAUSE_UNAVAILABLE_SEATS = 1;
	public static final byte CAUSE_USER_ALREADY_EXISTS = 2;
	public static final byte CAUSE_USER_NOT_FOUND = 3;
	public static final byte CAUSE_INCORRECT_PASSWORD = 4;
	public static final byte CAUSE_USER_NOT_LOGGED_IN = 5;
	public static final byte CAUSE_THEATERS_NOT_FOUND = 6;
	public static final byte CAUSE_THEATER_SCHEDULE_NOT_FOUND = 7;
	public static final byte CAUSE_SHOW_TIMES_NOT_FOUND = 8;
	public static final byte CAUSE_SHOW_NOT_FOUND = 9;
	public static final byte CAUSE_SEATING_PLAN_NOT_FOUND = 10;
	public static final byte CAUSE_RESERVATION_NOT_FOUND = 11;
	public static final byte CAUSE_MOVIE_NOT_FOUND = 12;
	public static final byte CAUSE_MOVIE_POSTER_NOT_FOUND = 12;
	private int causeCode = CAUSE_GENERAL;

	public ModelException(int causeCode) {
		this.causeCode = causeCode;

		return;
	}

	public int getCauseCode() {
		return causeCode;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeInt(causeCode);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static ModelException deserialize(DataInputStream dataStream)
			throws ApplicationException {
		try {
			return new ModelException(dataStream.readInt());
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

}
