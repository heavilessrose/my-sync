package com.sun.j2me.blueprints.smartticket.shared.midp;

public final class MessageConstants {

	public static final byte OPERATION_LOGIN_USER = 0;
	public static final byte OPERATION_CREATE_ACCOUNT = 1;
	public static final byte OPERATION_UPDATE_ACCOUNT = 2;
	public static final byte OPERATION_GET_THEATERS = 3;
	public static final byte OPERATION_GET_THEATER_SCHEDULE = 4;
	public static final byte OPERATION_GET_MOVIE = 5;
	public static final byte OPERATION_GET_MOVIE_POSTER = 6;
	public static final byte OPERATION_GET_MOVIE_SHOWTIMES = 7;
	public static final byte OPERATION_GET_SEATING_PLAN = 8;
	public static final byte OPERATION_RESERVE_SEATS = 9;
	public static final byte OPERATION_PURCHASE_TICKETS = 10;
	public static final byte OPERATION_CANCEL_SEAT_RESERVATION = 11;
	public static final byte OPERATION_GET_LOCALES = 12;
	public static final byte OPERATION_GET_RESOURCE_BUNDLE = 13;
	public static final byte OPERATION_INITIATE_SYNCHRONIZATION = 14;
	public static final byte OPERATION_SYNCHRONIZE_MOVIE_RATINGS = 15;
	public static final byte OPERATION_COMMIT_MOVIE_RATINGS = 16;
	public static final byte ERROR_NONE = 0;
	public static final byte ERROR_UNKNOWN_OPERATION = 1;
	public static final byte ERROR_SERVER_ERROR = 2;
	public static final byte ERROR_MODEL_EXCEPTION = 3;
	public static final byte ERROR_REQUEST_FORMAT = 4;

	private MessageConstants() {
	}

}
