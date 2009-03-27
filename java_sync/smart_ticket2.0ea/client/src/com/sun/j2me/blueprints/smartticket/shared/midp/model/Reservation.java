package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class Reservation {

	private String id;
	private String movieId;
	private long showTime;

	private Reservation() {
	}

	public Reservation(String id, String movieId, long showTime) {
		this.id = id;
		this.movieId = movieId;
		this.showTime = showTime;

		return;
	}

	public String getId() {
		return id;
	}

	public String getMovieId() {
		return movieId;
	}

	public long getShowTime() {
		return showTime;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeUTF(id);
			dataStream.writeUTF(movieId);
			dataStream.writeLong(showTime);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static Reservation deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			Reservation reservation = new Reservation();

			reservation.id = dataStream.readUTF();
			reservation.movieId = dataStream.readUTF();
			reservation.showTime = dataStream.readLong();

			return reservation;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

}
