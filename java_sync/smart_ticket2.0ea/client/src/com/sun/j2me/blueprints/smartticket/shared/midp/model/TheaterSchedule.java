package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

/*
 * Usage:
 * TheaterSchedule theaterSchedule = new TheaterSchedule(primaryKey, expirationDate);
 * MovieSchedule movieSchedules = new MovieSchedule[] {
 * theaterSchedule.new MovieSchedule(movie1, showTimes1),
 * theaterSchedule.new MovieSchedule(movie2, showTimes2)
 * };
 * theaterSchedule.setMovieSchedules(movieSchedules);
 */
public class TheaterSchedule {

	private String primaryKey; // Same as the theater primary key
	private long expirationDate;
	private MovieSchedule[] movieSchedules;

	public class MovieSchedule {

		private Movie movie;
		private int[][] showTimes = null;

		public MovieSchedule(Movie movie, int[][] showTimes) {
			this.movie = movie;
			this.showTimes = showTimes != null ? expandShowTimes(showTimes) : new int[0][0];

			return;
		}

		private MovieSchedule() {
		}

		public Movie getMovie() {
			return movie;
		}

		public int[][] getShowTimes() throws ApplicationException {
			if (showTimes.length == 0) {
				if (ModelObjectLoader.getInstance() != null) {
					try {
						showTimes = expandShowTimes(ModelObjectLoader.getInstance()
								.getMovieShowTimes(primaryKey, movie.getPrimaryKey()));
					} catch (ModelException me) {
						throw new ApplicationException();
					}
				}
			}

			return showTimes;
		}

		public SeatingPlan getSeatingPlan(int[] showTime) throws ApplicationException {
			try {
				return ModelObjectLoader.getInstance().getSeatingPlan(primaryKey,
						movie.getPrimaryKey(), showTime); // SeatingPlan are not cached
			} catch (ModelException me) {
				throw new ApplicationException();
			}
		}

		private int[][] expandShowTimes(int[][] showTimePatterns) {
			int count = 0;

			for (int i = 0; i < showTimePatterns.length; i++) {
				count += showTimePatterns[i][0] == -1 ? 7 : 1;
			}

			int[][] showTimes = new int[count][3];
			int j = 0;

			for (int i = 0; i < showTimePatterns.length; i++) {
				if (showTimePatterns[i][0] == -1) {
					showTimes[j++] = new int[] { Calendar.SUNDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.MONDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.TUESDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.WEDNESDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.THURSDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.FRIDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
					showTimes[j++] = new int[] { Calendar.SATURDAY, showTimePatterns[i][1],
							showTimePatterns[i][2] };
				} else {
					showTimes[j++] = showTimePatterns[i];
				}
			}

			return showTimes;
		}

	}

	public TheaterSchedule(String primaryKey, long expirationDate) {
		this.primaryKey = primaryKey;
		this.expirationDate = expirationDate;

		return;
	}

	private TheaterSchedule() {
	}

	public String getPrimaryKey() {
		return primaryKey;
	}

	public long getExpirationDate() {
		return expirationDate;
	}

	public MovieSchedule[] getMovieSchedules() {
		return movieSchedules;
	}

	public MovieSchedule getMovieSchedule(Movie movie) {
		for (int i = 0; i < movieSchedules.length; i++) {
			if (movie.equals(movieSchedules[i].movie)) {
				return movieSchedules[i];
			}
		}

		return null;
	}

	public void setMovieSchedules(MovieSchedule[] movieSchedules) {
		this.movieSchedules = movieSchedules;

		return;
	}

	public void serialize(DataOutputStream dataStream, boolean complete)
			throws ApplicationException {
		try {
			dataStream.writeUTF(primaryKey);
			dataStream.writeLong(expirationDate);
			dataStream.writeInt(movieSchedules.length);
			dataStream.writeBoolean(complete);

			for (int i = 0; i < movieSchedules.length; i++) {
				if (complete) {
					movieSchedules[i].movie.serialize(dataStream);
				} else {
					dataStream.writeUTF(movieSchedules[i].movie.getPrimaryKey());
				}

				serializeShowTimes(movieSchedules[i].getShowTimes(), dataStream);
			}

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static TheaterSchedule deserialize(DataInputStream dataStream)
			throws ApplicationException {
		try {
			TheaterSchedule theaterSchedule = new TheaterSchedule();

			theaterSchedule.primaryKey = dataStream.readUTF();
			theaterSchedule.expirationDate = dataStream.readLong();
			theaterSchedule.movieSchedules = new MovieSchedule[dataStream.readInt()];

			boolean complete = dataStream.readBoolean();

			for (int i = 0; i < theaterSchedule.movieSchedules.length; i++) {
				theaterSchedule.movieSchedules[i] = theaterSchedule.new MovieSchedule();

				if (complete) {
					theaterSchedule.movieSchedules[i].movie = Movie.deserialize(dataStream);
				} else {
					try {
						theaterSchedule.movieSchedules[i].movie = ModelObjectLoader.getInstance()
								.getMovie(dataStream.readUTF());
					} catch (ModelException me) {
						throw new ApplicationException();
					}
				}

				theaterSchedule.movieSchedules[i].showTimes = deserializeShowTimes(dataStream);
			}

			return theaterSchedule;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static void serializeShowTimes(int[][] showTimes, DataOutputStream dataStream)
			throws ApplicationException {
		try {
			dataStream.writeInt(showTimes.length);

			for (int j = 0; j < showTimes.length; j++) {
				for (int k = 0; k < showTimes[j].length; k++) {
					dataStream.writeInt(showTimes[j][k]);
				}
			}

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static int[][] deserializeShowTimes(DataInputStream dataStream)
			throws ApplicationException {
		try {
			int[][] showTimes = new int[dataStream.readInt()][3];

			for (int j = 0; j < showTimes.length; j++) {
				for (int k = 0; k < showTimes[j].length; k++) {
					showTimes[j][k] = dataStream.readInt();
				}
			}

			return showTimes;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static boolean matchShowtime(int[] time, int[] showtimes) {
		return (showtimes[0] == -1 || time[0] == showtimes[0]) && time[1] == showtimes[1]
				&& time[2] == showtimes[2];
	}

}
