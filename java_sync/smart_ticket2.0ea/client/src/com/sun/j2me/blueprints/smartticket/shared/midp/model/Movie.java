package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class Movie {

	private String primaryKey;
	private String title;
	private String summary;
	private String rating;
	private boolean alreadySeen = false;
	transient private byte[] poster = null;

	public Movie(String primaryKey, String title, String summary, String rating) {
		this.primaryKey = primaryKey;
		this.title = title;
		this.summary = summary;
		this.rating = rating;

		return;
	}

	private Movie() {
	}

	public String getPrimaryKey() {
		return primaryKey;
	}

	public String getTitle() {
		return title;
	}

	public String getSummary() {
		return summary;
	}

	public String getRating() {
		return rating;
	}

	public void setAlreadySeen(boolean alreadySeen) {
		this.alreadySeen = alreadySeen;

		return;
	}

	public boolean isAlreadySeen() {
		return alreadySeen;
	}

	public byte[] getPoster() throws ApplicationException {
		if (poster == null) {
			try {
				poster = ModelObjectLoader.getInstance().getMoviePoster(primaryKey);
			} catch (ModelException me) {
				throw new ApplicationException();
			}
		}

		return poster;
	}

	public boolean equals(Object object) {
		return object != null
				&& (object instanceof Movie && (object == this || ((Movie) object).getPrimaryKey()
						.equals(primaryKey)));
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeUTF(primaryKey);
			dataStream.writeUTF(title);
			dataStream.writeUTF(summary);
			dataStream.writeUTF(rating);
			dataStream.writeBoolean(alreadySeen);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static Movie deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			Movie movie = new Movie();

			movie.primaryKey = dataStream.readUTF();
			movie.title = dataStream.readUTF();
			movie.summary = dataStream.readUTF();
			movie.rating = dataStream.readUTF();

			try {
				movie.alreadySeen = dataStream.readBoolean();
			} catch (IOException ioe) {
				movie.alreadySeen = false;
			}

			try {
				return ModelObjectLoader.getInstance().getMovie(movie);
			} catch (ModelException me) {
				throw new ApplicationException();
			}
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

}
