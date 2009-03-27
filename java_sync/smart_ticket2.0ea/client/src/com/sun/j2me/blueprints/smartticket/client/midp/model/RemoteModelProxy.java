package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

/**　实现了RemoteModel接口，处理需要对J2EE服务器进行访问的动作，比如购票。
 * RemoteModelProxy中的方法对服务器端界面进行远程过程调用（remote procedure call，RPC). 
 * 
 * RemoteModelProxy类把每一个请求动作都委托给handler类链，以便透明地解决RMS串行化和 HTTP连接的异常管道。
 * 链接的处理程序体系结构基于实现它的RequestHandler接口和RemoteModelRequestHandler抽象类*/
public class RemoteModelProxy extends ModelObjectLoader implements RemoteModel {

	/**
	 * @link dependency
	 */
	/* # RMSCacheHandler lnkRMSCacheHandler; */
	private RemoteModelRequestHandler requestHandlerChain;
	private Preferences preferences = null;
	private Hashtable movies = new Hashtable();

	/** 链接装配 */
	public RemoteModelProxy(String serviceURL) throws ApplicationException {
		requestHandlerChain = new RMSCacheHandler(new HTTPCommunicationHandler(null, serviceURL));

		return;
	}

	public void setProgressObserver(ProgressObserver progressObserver) {
		RemoteModelRequestHandler.setProgressObserver(progressObserver);
	}

	public void init() throws ApplicationException {
		requestHandlerChain.init();

		return;
	}

	public void destroy() throws ApplicationException {
		requestHandlerChain.destroy();

		return;
	}

	public boolean isOffline() throws ApplicationException {
		return requestHandlerChain.isOffline();
	}

	public void setOffline(boolean offline) throws ApplicationException {
		requestHandlerChain.setOffline(offline);

		return;
	}

	public void setPreferences(Preferences preferences) {
		this.preferences = preferences;

		requestHandlerChain.setPreferences(preferences);

		return;
	}

	public void login(String userName, String password) throws ModelException, ApplicationException {
		requestHandlerChain.login(userName, password);

		return;
	}

	public void createAccount(AccountInfo accountInfo) throws ModelException, ApplicationException {
		requestHandlerChain.createAccount(accountInfo);

		return;
	}

	public void updateAccount(AccountInfo accountInfo) throws ModelException, ApplicationException {
		requestHandlerChain.updateAccount(accountInfo);

		return;
	}

	public Theater[] getTheaters(String zipCode) throws ModelException, ApplicationException {
		return requestHandlerChain.getTheaters(zipCode);
	}

	public TheaterSchedule getTheaterSchedule(Theater theater) throws ModelException,
			ApplicationException {
		return requestHandlerChain.getTheaterSchedule(theater);
	}

	public TheaterSchedule downloadTheaterSchedule(Theater theater) throws ModelException,
			ApplicationException {
		return requestHandlerChain.downloadTheaterSchedule(theater);
	}

	public TheaterSchedule getTheaterSchedule(String theaterKey) throws ModelException,
			ApplicationException {
		return requestHandlerChain.getTheaterSchedule(theaterKey);
	}

	/** 如果一个RemoteModelProxy类调用一个动作方法不能通过前面的handler类完成，那么基类RemoteModelRequestHandler类能够确保这个调用被传递给链中的下一处理程序。
	 * 如果链中的一个处理程序判定它已经完成一个动作的处理，它会直接返回。否则，它再调用在超类中的同一动作方法，并将它传递给链中的下一处理程序。 */
	public Movie getMovie(String movieKey) throws ModelException, ApplicationException {
		Movie movie = (Movie) movies.get(movieKey);

		if (movie == null) {
			movie = requestHandlerChain.getMovie(movieKey);

			movies.put(movieKey, movie);
		}

		return movie;
	}

	public Movie getMovie(Movie movie) throws ModelException, ApplicationException {
		Movie instance = (Movie) movies.get(movie.getPrimaryKey());

		if (instance == null) {
			movies.put(movie.getPrimaryKey(), movie);

			instance = movie;
		}

		return instance;
	}

	public byte[] getMoviePoster(String movieKey) throws ModelException, ApplicationException {
		return requestHandlerChain.getMoviePoster(movieKey);
	}

	public int[][] getMovieShowTimes(String theaterKey, String movieKey) throws ModelException,
			ApplicationException {
		return requestHandlerChain.getMovieShowTimes(theaterKey, movieKey);
	}

	public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, int[] showTime)
			throws ModelException, ApplicationException {
		return requestHandlerChain.getSeatingPlan(theaterKey, movieKey, showTime);
	}

	public Reservation reserveSeats(String theaterKey, String movieKey, int[] showTime, Seat[] seats)
			throws ModelException, ApplicationException {
		return requestHandlerChain.reserveSeats(theaterKey, movieKey, showTime, seats);
	}

	public void purchaseTickets(String reservationId) throws ModelException, ApplicationException {
		requestHandlerChain.purchaseTickets(reservationId);

		return;
	}

	public void cancelSeatReservation(String reservationId) throws ModelException,
			ApplicationException {
		requestHandlerChain.cancelSeatReservation(reservationId);

		return;
	}

	public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime)
			throws ModelException, ApplicationException {
		return requestHandlerChain.initiateSynchronization(syncAnchor, clientTime);
	}

	public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings,
			int conflictResolutionStrategyId) throws ModelException, ApplicationException {
		return requestHandlerChain.synchronizeMovieRatings(movieRatings,
				conflictResolutionStrategyId);
	}

	public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) throws ModelException,
			ApplicationException {
		return requestHandlerChain.commitMovieRatings(movieRatings);
	}

	public String[] getLocales() throws ModelException, ApplicationException {
		return requestHandlerChain.getLocales();
	}

	public IndexedResourceBundle getResourceBundle(String baseName, String locale)
			throws ModelException, ApplicationException {
		return requestHandlerChain.getResourceBundle(baseName, locale);
	}

}
