package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

public class MovieRating implements Synchronizable {

	public static final int RATING_UNRATED = 0;
	public static final int RATING_ONE_STAR = 1;
	public static final int RATING_TWO_STARS = 2;
	public static final int RATING_THREE_STARS = 3;
	public static final int RATING_FOUR_STARS = 4;
	public static final int RATING_FIVE_STARS = 5;

	/**
	 * @link aggregation
	 */
	private String primaryKey;
	private Movie movie = null;
	private int starNumber;
	private long lastViewingDate;
	private long lastModificationDate;
	private int modificationStatus;
	private boolean modifiedOnClient;

	public MovieRating(String primaryKey, int starNumber, long lastViewingDate,
			long lastModificationDate, int modificationStatus, boolean modifiedOnClient) {
		this.primaryKey = primaryKey;
		this.starNumber = starNumber;
		this.lastViewingDate = lastViewingDate;
		this.lastModificationDate = lastModificationDate;
		this.modificationStatus = modificationStatus;
		this.modifiedOnClient = modifiedOnClient;

		return;
	}

	public MovieRating(Movie movie, long lastViewingDate) {
		this(movie, RATING_UNRATED, lastViewingDate, System.currentTimeMillis(),
				Synchronizable.STATUS_INSERTED, true);

		return;
	}

	private MovieRating(Movie movie, int starNumber, long lastViewingDate,
			long lastModificationDate, int modificationStatus, boolean modifiedOnClient) {
		this.movie = movie;
		this.primaryKey = movie.getPrimaryKey();
		this.starNumber = starNumber;
		this.lastViewingDate = lastViewingDate;
		this.lastModificationDate = lastModificationDate;
		this.modificationStatus = modificationStatus;
		this.modifiedOnClient = modifiedOnClient;

		return;
	}

	private MovieRating() {
	}

	public String getPrimaryKey() {
		return primaryKey;
	}

	public Movie getMovie() throws ApplicationException {
		if (movie == null) {
			try {
				movie = ModelObjectLoader.getInstance().getMovie(primaryKey);
			} catch (ModelException me) {
				throw new ApplicationException();
			}
		}

		return movie;
	}

	public int getStarNumber() {
		return starNumber;
	}

	public void setStarNumber(int starNumber) { // Should assert the validity of starNumber
		this.starNumber = starNumber;
		this.lastModificationDate = System.currentTimeMillis();
		;
		this.modificationStatus = Synchronizable.STATUS_MODIFIED;

		return;
	}

	public long getLastViewingDate() {
		return lastViewingDate;
	}

	public void setLastViewingDate(long viewingDate) {
		this.lastViewingDate = lastViewingDate;
		this.lastModificationDate = System.currentTimeMillis();
		;
		this.modificationStatus = Synchronizable.STATUS_MODIFIED;

		return;
	}

	public long getLastModificationDate() {
		return lastModificationDate;
	}

	public int getModificationStatus() {
		return modificationStatus;
	}

	public void setModificationStatus(int modificationStatus) { // Should
		// assert the validity of modificationStatus
		this.lastModificationDate = System.currentTimeMillis();
		;
		this.modificationStatus = modificationStatus;
		modifiedOnClient = true;

		return;
	}

	public boolean isModifiedOnClient() {
		return modifiedOnClient;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeByte(modificationStatus);
			dataStream.writeLong(lastModificationDate);
			dataStream.writeUTF(primaryKey);
			dataStream.writeByte(starNumber);
			dataStream.writeLong(lastViewingDate);
			dataStream.writeBoolean(modifiedOnClient);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static MovieRating deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			MovieRating movieRating = new MovieRating();

			movieRating.modificationStatus = dataStream.readByte();
			movieRating.lastModificationDate = dataStream.readLong();

			try {
				movieRating.primaryKey = dataStream.readUTF();

				if (ModelObjectLoader.getInstance() != null) { // Because server-side model does not provide a ModelObjectLoader
					movieRating.movie = ModelObjectLoader.getInstance().getMovie(
							movieRating.primaryKey);
				}
			} catch (ModelException me) {
				throw new ApplicationException();
			}

			movieRating.starNumber = dataStream.readByte();
			movieRating.lastViewingDate = dataStream.readLong();
			movieRating.modifiedOnClient = dataStream.readBoolean();

			return movieRating;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public boolean equals(Object object) {
		return object != null
				&& (object instanceof MovieRating && (object == this || ((MovieRating) object)
						.getPrimaryKey().equals(primaryKey)));
	}

	// For debugging only (downsizing candidate)
	public String toString() {
		return modificationStatus + ":" + lastModificationDate + ":" + getPrimaryKey() + ":"
				+ starNumber + ":" + lastViewingDate;
	}

}
