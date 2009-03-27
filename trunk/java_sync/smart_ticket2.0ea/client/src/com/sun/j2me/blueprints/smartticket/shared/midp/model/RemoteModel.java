package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan;

public interface RemoteModel {

	void login(String userName, String password) throws ModelException, ApplicationException;

	void createAccount(AccountInfo accountInfo) throws ModelException, ApplicationException;

	void updateAccount(AccountInfo accountInfo) throws ModelException, ApplicationException;

	Theater[] getTheaters(String zipCode) throws ModelException, ApplicationException;

	TheaterSchedule getTheaterSchedule(String theaterKey) throws ModelException,
			ApplicationException;

	Movie getMovie(String movieKey) throws ModelException, ApplicationException;

	byte[] getMoviePoster(String movieKey) throws ModelException, ApplicationException;

	int[][] getMovieShowTimes(String theaterKey, String movieKey) throws ModelException,
			ApplicationException;

	SeatingPlan getSeatingPlan(String theaterKey, String movieKey, int[] showTime)
			throws ModelException, ApplicationException;

	Reservation reserveSeats(String theaterKey, String movieKey, int[] showTime,
			SeatingPlan.Seat[] seats) throws ModelException, ApplicationException;

	void purchaseTickets(String reservationId) throws ModelException, ApplicationException;

	void cancelSeatReservation(String reservationId) throws ModelException, ApplicationException;

	SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime)
			throws ModelException, ApplicationException;

	MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings,
			int conflictResolutionStrategyId) throws ModelException, ApplicationException;

	MovieRating[] commitMovieRatings(MovieRating[] movieRatings) throws ModelException,
			ApplicationException;

	String[] getLocales() throws ModelException, ApplicationException;

	IndexedResourceBundle getResourceBundle(String baseName, String locale) throws ModelException,
			ApplicationException;
}
