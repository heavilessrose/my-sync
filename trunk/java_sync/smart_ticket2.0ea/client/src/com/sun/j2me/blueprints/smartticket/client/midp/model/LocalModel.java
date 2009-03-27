package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

/** 处理需要对本地设备上存储的数据进行访问的动作 */
public class LocalModel {

	/**
	 * @link aggregation
	 */
	private RMSAdapter rmsAdapter;
	private Preferences preferences = null;
	protected static ProgressObserver progressObserver;

	public LocalModel() throws ApplicationException {
		rmsAdapter = new RMSAdapter();
	}

	public static void setProgressObserver(ProgressObserver progressObserver) {
		progressObserver = progressObserver;
	}

	public void init() throws ApplicationException {
	}

	public void destroy() throws ApplicationException {
		cleanup();
		rmsAdapter.closeRecordStores();
	}

	public Theater[] getTheaters(String zipCode) throws ApplicationException {
		return getTheaters(zipCode, IndexEntry.MODE_ANY);
	}

	public Theater[] getCachedTheaters(String zipCode) throws ApplicationException {
		return getTheaters(zipCode, IndexEntry.MODE_CACHING);
	}

	public Theater[] getPersistedTheaters(String zipCode) throws ApplicationException {
		return getTheaters(zipCode, IndexEntry.MODE_PERSISTING);
	}

	private Theater[] getTheaters(String zipCode, int mode) throws ApplicationException {
		IndexEntry[] indexEntries = rmsAdapter.getIndexEntries(IndexEntry.TYPE_THEATER, mode);
		Vector theaterVector = new Vector();

		for (int i = 0; i < indexEntries.length; i++) {
			Theater theater = rmsAdapter.loadTheater(indexEntries[i].getRecordId());

			if (zipCode == null || theater.getZipCode().equals(zipCode)) {
				theaterVector.addElement(theater);
			}
		}

		Theater[] theaters = new Theater[theaterVector.size()];

		theaterVector.copyInto(theaters);

		return theaters;
	}

	public void deleteTheater(String theaterKey) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(theaterKey, IndexEntry.TYPE_THEATER,
				IndexEntry.MODE_ANY);

		if (indexEntry != null) {
			rmsAdapter.deleteTheater(indexEntry.getRecordId());
			rmsAdapter.deleteIndexEntry(indexEntry.getKey(), indexEntry.getType());
		}

		indexEntry = rmsAdapter.getIndexEntry(theaterKey, IndexEntry.TYPE_THEATER_SCHEDULE,
				IndexEntry.MODE_ANY);

		if (indexEntry != null) {
			rmsAdapter.deleteTheaterSchedule(indexEntry.getRecordId());
			rmsAdapter.deleteIndexEntry(indexEntry.getKey(), indexEntry.getType());
		}

		// Movies are to be removed (garbage collected) by an independent
		// thread using the cleanup method
		return;
	}

	public void cleanup() throws ApplicationException {
		IndexEntry[] indexEntries = rmsAdapter.getIndexEntries(IndexEntry.TYPE_THEATER_SCHEDULE,
				IndexEntry.MODE_ANY);

		for (int i = 0; i < indexEntries.length; i++) {
			if (indexEntries[i].getMode() == IndexEntry.MODE_CACHING
					&& indexEntries[i].getExpirationDate() < System.currentTimeMillis()) {
				// clean the cache entries
				deleteTheater(indexEntries[i].getKey());
			} else {
				// mark dependent movies as reachable from a theater schedule
				TheaterSchedule theaterSchedule = rmsAdapter.loadTheaterSchedule(indexEntries[i]
						.getRecordId());
				TheaterSchedule.MovieSchedule[] movieSchedules = theaterSchedule
						.getMovieSchedules();

				for (int j = 0; j < movieSchedules.length; j++) {
					Movie movie = movieSchedules[j].getMovie();
					IndexEntry indexEntry = rmsAdapter.getIndexEntry(movie.getPrimaryKey(),
							IndexEntry.TYPE_MOVIE, IndexEntry.MODE_ANY);

					if (indexEntry != null) {
						indexEntry.setMarked(true);
						rmsAdapter.addIndexEntry(indexEntry);
					}
				}
			}
		}

		indexEntries = rmsAdapter
				.getIndexEntries(IndexEntry.TYPE_MOVIE_RATING, IndexEntry.MODE_ANY);

		for (int i = 0; i < indexEntries.length; i++) {
			MovieRating movieRating = rmsAdapter.loadMovieRating(indexEntries[i].getRecordId());
			Movie movie = movieRating.getMovie();
			IndexEntry indexEntry = rmsAdapter.getIndexEntry(movie.getPrimaryKey(),
					IndexEntry.TYPE_MOVIE, IndexEntry.MODE_ANY);

			if (indexEntry != null) {
				indexEntry.setMarked(true);
				rmsAdapter.addIndexEntry(indexEntry);
			}
		}

		indexEntries = rmsAdapter.getIndexEntries(IndexEntry.TYPE_MOVIE, IndexEntry.MODE_ANY);

		for (int i = 0; i < indexEntries.length; i++) {
			// delete dependent movies which are not reachable from any theater schedule
			if (indexEntries[i].isMarked()) {
				indexEntries[i].setMarked(false);
				rmsAdapter.addIndexEntry(indexEntries[i]);
			} else {
				rmsAdapter.deleteMovie(indexEntries[i].getRecordId());
				rmsAdapter.deleteIndexEntry(indexEntries[i].getKey(), IndexEntry.TYPE_MOVIE);
			}
		}

		return;
	}

	public AccountInfo getAccountInfo() throws ApplicationException {
		AccountInfo accountInfo = null;
		IndexEntry indexEntry = rmsAdapter.getIndexEntry("*", IndexEntry.TYPE_ACCOUNT_INFO,
				IndexEntry.MODE_ANY);

		if (indexEntry != null) {
			accountInfo = rmsAdapter.loadAccountInfo(indexEntry.getRecordId());
		}

		return accountInfo;
	}

	public void setAccountInfo(AccountInfo accountInfo) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry("*", IndexEntry.TYPE_ACCOUNT_INFO,
				IndexEntry.MODE_ANY);
		int recordId = rmsAdapter.storeAccountInfo(accountInfo, indexEntry != null ? indexEntry
				.getRecordId() : -1);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_ACCOUNT_INFO, "*",
					IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		}

		return;
	}

	/*
	 * This is used as cache and is maintained up-to-date,
	 * getting the preferences always returns the same instance,
	 * setting new preferences does not replace this instance but updates the individual attributes/properties of this instance
	 */
	public Preferences getPreferences() throws ApplicationException {
		if (preferences == null) {
			IndexEntry indexEntry = rmsAdapter.getIndexEntry("*", IndexEntry.TYPE_PREFERENCES,
					IndexEntry.MODE_ANY);

			if (indexEntry != null) {
				preferences = rmsAdapter.loadPreferences(indexEntry.getRecordId());
			} else {
				preferences = new Preferences();

				int recordId = rmsAdapter.storePreferences(preferences,
						indexEntry != null ? indexEntry.getRecordId() : -1);

				indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_PREFERENCES, "*",
						IndexEntry.MODE_PERSISTING);

				rmsAdapter.addIndexEntry(indexEntry);
			}
		}

		return preferences;
	}

	// public void setPreferences() throws ApplicationException {
	public void setPreferences(Preferences preferences) throws ApplicationException {
		this.preferences.copy(preferences);

		IndexEntry indexEntry = rmsAdapter.getIndexEntry("*", IndexEntry.TYPE_PREFERENCES,
				IndexEntry.MODE_ANY);
		int recordId = rmsAdapter.storePreferences(preferences, indexEntry != null ? indexEntry
				.getRecordId() : -1);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_PREFERENCES, "*",
					IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		}

		return;
	}

	public IndexedResourceBundle getResourceBundle(String baseName) throws ApplicationException {
		try {
			IndexEntry indexEntry = rmsAdapter.getIndexEntry(baseName,
					IndexEntry.TYPE_RESOURCE_BUNDLE, IndexEntry.MODE_ANY);

			if (indexEntry != null) {
				return rmsAdapter.loadResourceBundle(indexEntry.getRecordId());
			}
		} catch (ApplicationException ae) {
		}

		return null;
	}

	public void setResourceBundle(String baseName, IndexedResourceBundle bundle)
			throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(baseName, IndexEntry.TYPE_RESOURCE_BUNDLE,
				IndexEntry.MODE_ANY);
		int recordId = rmsAdapter.storeResourceBundle(bundle, indexEntry != null ? indexEntry
				.getRecordId() : -1);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_RESOURCE_BUNDLE, baseName,
					IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		}

		return;
	}

	public void setMovieRating(MovieRating movieRating) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(movieRating.getPrimaryKey(),
				IndexEntry.TYPE_MOVIE_RATING, IndexEntry.MODE_ANY);
		int recordId = rmsAdapter.storeMovieRating(movieRating, indexEntry != null ? indexEntry
				.getRecordId() : -1);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_MOVIE_RATING, movieRating
					.getPrimaryKey(), IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
			persistRatedMovie(movieRating.getMovie());
		}

		return;
	}

	public void addMovieRating(MovieRating movieRating) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(movieRating.getPrimaryKey(),
				IndexEntry.TYPE_MOVIE_RATING, IndexEntry.MODE_ANY);
		int recordId = -1;

		if (indexEntry != null) {
			recordId = indexEntry.getRecordId();

			MovieRating rating = rmsAdapter.loadMovieRating(recordId);

			rating.setLastViewingDate(movieRating.getLastViewingDate());

			movieRating = rating;
		}

		recordId = rmsAdapter.storeMovieRating(movieRating, recordId);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_MOVIE_RATING, movieRating
					.getPrimaryKey(), IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
			persistRatedMovie(movieRating.getMovie());
		}

		return;
	}

	public void deleteMovieRating(String movieRatingKey) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(movieRatingKey,
				IndexEntry.TYPE_MOVIE_RATING, IndexEntry.MODE_ANY);

		if (indexEntry != null) {
			MovieRating movieRating = rmsAdapter.loadMovieRating(indexEntry.getRecordId());

			movieRating.setModificationStatus(Synchronizable.STATUS_DELETED);
			rmsAdapter.storeMovieRating(movieRating, indexEntry.getRecordId());
		}

		return;
	}

	public void purgeMovieRating(String movieRatingKey) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(movieRatingKey,
				IndexEntry.TYPE_MOVIE_RATING, IndexEntry.MODE_ANY);

		if (indexEntry != null) {
			rmsAdapter.deleteMovieRating(indexEntry.getRecordId());
			rmsAdapter.deleteIndexEntry(indexEntry.getKey(), indexEntry.getType());
		}

		return;
	}

	public MovieRating[] getMovieRatings() throws ApplicationException {
		IndexEntry[] indexEntries = rmsAdapter.getIndexEntries(IndexEntry.TYPE_MOVIE_RATING,
				IndexEntry.MODE_ANY);
		MovieRating[] movieRatings = new MovieRating[indexEntries.length];

		for (int i = 0; i < indexEntries.length; i++) {
			movieRatings[i] = rmsAdapter.loadMovieRating(indexEntries[i].getRecordId());
		}

		return movieRatings;
	}

	private void persistRatedMovie(Movie movie) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry(movie.getPrimaryKey(),
				IndexEntry.TYPE_MOVIE, IndexEntry.MODE_ANY);

		if (indexEntry == null) {
			int recordId = rmsAdapter.storeMovie(movie, -1);

			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_MOVIE, movie.getPrimaryKey(),
					IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		} else if (indexEntry.getMode() != IndexEntry.MODE_PERSISTING) {
			// since persisting: overwrite caching entries
			indexEntry = new IndexEntry(indexEntry.getRecordId(), indexEntry.getType(), indexEntry
					.getKey(), IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		}

		return;
	}

	public void setMovieRatingSyncAnchor(SyncAnchor syncAnchor) throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry("MOVIE_RATINGS",
				IndexEntry.TYPE_SYNC_ANCHOR, IndexEntry.MODE_ANY);
		int recordId = rmsAdapter.storeSyncAnchor(syncAnchor, indexEntry != null ? indexEntry
				.getRecordId() : -1);

		if (indexEntry == null) {
			indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_SYNC_ANCHOR, "MOVIE_RATINGS",
					IndexEntry.MODE_PERSISTING);

			rmsAdapter.addIndexEntry(indexEntry);
		}

		return;
	}

	public SyncAnchor getMovieRatingSyncAnchor() throws ApplicationException {
		IndexEntry indexEntry = rmsAdapter.getIndexEntry("MOVIE_RATINGS",
				IndexEntry.TYPE_SYNC_ANCHOR, IndexEntry.MODE_ANY);

		return indexEntry != null ? rmsAdapter.loadSyncAnchor(indexEntry.getRecordId()) : null;
	}

}
