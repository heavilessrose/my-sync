package com.sun.j2me.blueprints.smartticket.client.midp.model;

import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

/** 具体的handler类扩展了RemoteModelRequestHandler类。嵌套的构造函数建立一个处理程序链 */
abstract public class RemoteModelRequestHandler implements RequestHandler, RemoteModel {

	/**
	 * @link aggregation
	 */
	private RemoteModelRequestHandler nextHandler;
	private Preferences preferences;
	protected static ProgressObserver progressObserver;

	public RemoteModelRequestHandler(RemoteModelRequestHandler nextHandler) {
		this.nextHandler = nextHandler;
	}

	public RequestHandler getNextHandler() {
		return nextHandler;
	}

	public void init() throws ApplicationException {
		if (nextHandler != null) {
			nextHandler.init();
		}

		return;
	}

	public void destroy() throws ApplicationException {
		if (nextHandler != null) {
			nextHandler.destroy();
		}

		return;
	}

	public static void setProgressObserver(ProgressObserver progressObserver) {
		RemoteModelRequestHandler.progressObserver = progressObserver;
	}

	protected static void updateProgress() throws ApplicationException {
		if (progressObserver != null) {
			if (!progressObserver.isStopped()) {
				progressObserver.updateProgress();

				return;
			}

			throw new ApplicationException(ErrorMessageCodes.ERROR_OPERATION_INTERRUPTED);
		}
	}

	private RemoteModelRequestHandler getRemoteModelRequestHandler() throws ApplicationException {
		if (nextHandler != null) {
			return nextHandler;
		}

		throw new ApplicationException(ErrorMessageCodes.ERROR_BROKEN_HANDLER_CHAIN);
	}

	public void setPreferences(Preferences preferences) {
		this.preferences = preferences;

		if (nextHandler != null) {
			nextHandler.setPreferences(preferences);
		}

		return;
	}

	public Preferences getPreferences() {
		return preferences;
	}

	public boolean isOffline() throws ApplicationException {
		return getRemoteModelRequestHandler().isOffline();
	}

	public void setOffline(boolean offline) throws ApplicationException {
		getRemoteModelRequestHandler().setOffline(offline);

		return;
	}

	public void login(String userName, String password) throws ModelException, ApplicationException {
		getRemoteModelRequestHandler().login(userName, password);

		return;
	}

	public void createAccount(AccountInfo accountInfo) throws ModelException, ApplicationException {
		getRemoteModelRequestHandler().createAccount(accountInfo);

		return;
	}

	public void updateAccount(AccountInfo accountInfo) throws ModelException, ApplicationException {
		getRemoteModelRequestHandler().updateAccount(accountInfo);

		return;
	}

	public Theater[] getTheaters(String zipCode) throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getTheaters(zipCode);
	}

	public TheaterSchedule getTheaterSchedule(Theater theater) throws ModelException,
			ApplicationException {
		return getRemoteModelRequestHandler().getTheaterSchedule(theater);
	}

	public TheaterSchedule downloadTheaterSchedule(Theater theater) throws ModelException,
			ApplicationException {
		return getRemoteModelRequestHandler().downloadTheaterSchedule(theater);
	}

	public TheaterSchedule getTheaterSchedule(String theaterKey) throws ModelException,
			ApplicationException {
		return getRemoteModelRequestHandler().getTheaterSchedule(theaterKey);
	}

	public Movie getMovie(String movieKey) throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getMovie(movieKey);
	}

	public byte[] getMoviePoster(String movieKey) throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getMoviePoster(movieKey);
	}

	public int[][] getMovieShowTimes(String theaterKey, String movieKey) throws ModelException,
			ApplicationException {
		return getRemoteModelRequestHandler().getMovieShowTimes(theaterKey, movieKey);
	}

	public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, int[] showTime)
			throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getSeatingPlan(theaterKey, movieKey, showTime);
	}

	public Reservation reserveSeats(String theaterKey, String movieKey, int[] showTime, Seat[] seats)
			throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().reserveSeats(theaterKey, movieKey, showTime, seats);
	}

	public void purchaseTickets(String reservationId) throws ModelException, ApplicationException {
		getRemoteModelRequestHandler().purchaseTickets(reservationId);
	}

	public void cancelSeatReservation(String reservationId) throws ModelException,
			ApplicationException {
		getRemoteModelRequestHandler().cancelSeatReservation(reservationId);
	}

	public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime)
			throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().initiateSynchronization(syncAnchor, clientTime);
	}

	public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings,
			int conflictResolutionStrategyId) throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().synchronizeMovieRatings(movieRatings,
				conflictResolutionStrategyId);
	}

	public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) throws ModelException,
			ApplicationException {
		return getRemoteModelRequestHandler().commitMovieRatings(movieRatings);
	}

	public String[] getLocales() throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getLocales();
	}

	public IndexedResourceBundle getResourceBundle(String baseName, String locale)
			throws ModelException, ApplicationException {
		return getRemoteModelRequestHandler().getResourceBundle(baseName, locale);
	}

}
