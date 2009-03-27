package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public abstract class ModelObjectLoader {

	private static ModelObjectLoader instance = null;

	protected ModelObjectLoader() {
		instance = this;

		return;
	}

	public static ModelObjectLoader getInstance() {
		return instance;
	}

	public abstract TheaterSchedule getTheaterSchedule(Theater theater) throws ModelException,
			ApplicationException;

	public abstract Movie getMovie(String movieKey) throws ModelException, ApplicationException;

	public abstract Movie getMovie(Movie movie) throws ModelException, ApplicationException;

	public abstract byte[] getMoviePoster(String movieKey) throws ModelException,
			ApplicationException;

	public abstract int[][] getMovieShowTimes(String theaterKey, String movieKey)
			throws ModelException, ApplicationException;

	public abstract SeatingPlan getSeatingPlan(String theaterKey, String movieKey, int[] showTime)
			throws ModelException, ApplicationException;
}
