package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

/** 用本地数据同步化远程服务器端数据 -- 只有评级实现了同步 */
public class SynchronizationAgent {

	/**
	 * @link aggregationByValue
	 */
	private RemoteModel remoteModel;

	/**
	 * @link aggregationByValue
	 */
	private LocalModel localModel;
	private SyncAnchor nextSyncAnchor;

	public SynchronizationAgent(LocalModel localModel, RemoteModel remoteModel)
			throws ApplicationException {
		this.localModel = localModel;
		this.remoteModel = remoteModel;

		return;
	}

	public void synchronizeMovieRatings(int conflictResolutionStrategyId) throws ModelException,
			ApplicationException {
		SyncAnchor lastSyncAnchor = localModel.getMovieRatingSyncAnchor();

		nextSyncAnchor = remoteModel.initiateSynchronization(
				lastSyncAnchor != null ? lastSyncAnchor : new SyncAnchor(), new Date().getTime());

		MovieRating[] movieRatings = localModel.getMovieRatings();
		Vector changes = new Vector();

		// filter out the unchanged movie ratings
		for (int i = 0; i < movieRatings.length; i++) {
			if (nextSyncAnchor.isOutOfSync()
					|| movieRatings[i].getModificationStatus() != Synchronizable.STATUS_UNCHANGED) {
				changes.addElement(movieRatings[i]);
			}
		}

		MovieRating[] changedMovieRatings = new MovieRating[changes.size()];

		changes.copyInto(changedMovieRatings);

		MovieRating[] updatedMovieRatings = remoteModel.synchronizeMovieRatings(
				changedMovieRatings, conflictResolutionStrategyId);
		// consolidate the updated movie ratings and the initial movie ratings
		Hashtable consolidatedMovieRatings = new Hashtable();

		for (int i = 0; i < movieRatings.length; i++) {
			consolidatedMovieRatings.put(movieRatings[i].getMovie().getPrimaryKey(),
					movieRatings[i]);
		}

		for (int i = 0; i < updatedMovieRatings.length; i++) {
			consolidatedMovieRatings.put(updatedMovieRatings[i].getMovie().getPrimaryKey(),
					updatedMovieRatings[i]);
		}

		for (Enumeration elements = consolidatedMovieRatings.elements(); elements.hasMoreElements();) {
			MovieRating movieRating = (MovieRating) elements.nextElement();

			if (movieRating.getModificationStatus() != Synchronizable.STATUS_DELETED) {
				movieRating.setModificationStatus(Synchronizable.STATUS_UNCHANGED);
				localModel.setMovieRating(movieRating);
			} else {
				localModel.purgeMovieRating(movieRating.getPrimaryKey());
			}
		}

		localModel.setMovieRatingSyncAnchor(nextSyncAnchor);

		return;
	}

	/**　同步评级 */
	public MovieRating[][] synchronizeMovieRatings() throws ModelException, ApplicationException {
		SyncAnchor lastSyncAnchor = localModel.getMovieRatingSyncAnchor();

		nextSyncAnchor = remoteModel.initiateSynchronization(
				lastSyncAnchor != null ? lastSyncAnchor : new SyncAnchor(), new Date().getTime());

		MovieRating[] movieRatings = localModel.getMovieRatings();
		Hashtable changes = new Hashtable();

		// filter out the unchanged movie ratings
		for (int i = 0; i < movieRatings.length; i++) {
			if (nextSyncAnchor.isOutOfSync()
					|| movieRatings[i].getModificationStatus() != Synchronizable.STATUS_UNCHANGED) {
				changes.put(movieRatings[i].getMovie().getPrimaryKey(), movieRatings[i]);
			}
		}

		MovieRating[] changedMovieRatings = new MovieRating[changes.size()];
		Enumeration elements = changes.elements();

		for (int i = 0; i < changedMovieRatings.length; i++) {
			changedMovieRatings[i] = (MovieRating) elements.nextElement();
		}

		MovieRating[] conflictingMovieRatings = remoteModel.synchronizeMovieRatings(
				changedMovieRatings, Synchronizable.STRATEGY_ASK_USER);
		MovieRating[][] conflictingMovieRatingPairs = new MovieRating[conflictingMovieRatings.length][2];

		for (int i = 0; i < conflictingMovieRatings.length; i++) {
			conflictingMovieRatingPairs[i][0] = (MovieRating) changes
					.get(conflictingMovieRatings[i].getMovie().getPrimaryKey());
			conflictingMovieRatingPairs[i][1] = conflictingMovieRatings[i];
		}

		return conflictingMovieRatingPairs;
	}

	/** 向后台提交已分析的同步请求，并更新本地存储的内容 */
	public void commitMovieRatings(MovieRating[] resolvedMovieRatings) throws ModelException,
			ApplicationException {
		MovieRating[] movieRatings = localModel.getMovieRatings();
		MovieRating[] updatedMovieRatings = remoteModel.commitMovieRatings(resolvedMovieRatings);
		// consolidate the updated movie ratings and the initial movie ratings
		Hashtable consolidatedMovieRatings = new Hashtable();

		for (int i = 0; i < movieRatings.length; i++) {
			consolidatedMovieRatings.put(movieRatings[i].getMovie().getPrimaryKey(),
					movieRatings[i]);
		}

		for (int i = 0; i < updatedMovieRatings.length; i++) {
			consolidatedMovieRatings.put(updatedMovieRatings[i].getMovie().getPrimaryKey(),
					updatedMovieRatings[i]);
		}

		for (Enumeration elements = consolidatedMovieRatings.elements(); elements.hasMoreElements();) {
			MovieRating movieRating = (MovieRating) elements.nextElement();

			if (movieRating.getModificationStatus() != Synchronizable.STATUS_DELETED) {
				movieRating.setModificationStatus(Synchronizable.STATUS_UNCHANGED);
				localModel.setMovieRating(movieRating);
			} else {
				localModel.purgeMovieRating(movieRating.getPrimaryKey());
			}
		}

		localModel.setMovieRatingSyncAnchor(nextSyncAnchor);

		return;
	}

}
