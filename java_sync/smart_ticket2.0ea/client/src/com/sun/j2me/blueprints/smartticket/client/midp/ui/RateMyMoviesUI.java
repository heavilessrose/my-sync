package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.MovieRating;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Movie;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class RateMyMoviesUI extends Form implements CommandListener, ItemStateListener {

	private Command deleteCommand, syncCommand, saveCommand, cancelCommand;
	private Gauge[] movieRatesUI; // should use a custom item
	private int[] movieRatesNum;
	private MovieRating[] movieRatings;
	private boolean[] modifiedMovieRatings;
	private StringItem[] movieNames;
	private StringItem[] lastViewDates;
	private UIController uiController;

	public RateMyMoviesUI(UIController uiController) {
		super(uiController.getString(UIConstants.RATE_MY_MOVIES_TITLE));

		this.uiController = uiController;

		/*
		 * deleteCommand
		 * = new Command(uiController.getString(UIConstants.DELETE),
		 * Command.OK, 2);
		 */
		syncCommand = new Command(uiController.getString(UIConstants.SYNC), Command.OK, 2);

		// addCommand(deleteCommand);
		addCommand(syncCommand);

		saveCommand = new Command(uiController.getString(UIConstants.SAVE), Command.OK, 1);

		addCommand(saveCommand);

		cancelCommand = new Command(uiController.getString(UIConstants.CANCEL), Command.CANCEL, 1);

		addCommand(cancelCommand);
		setCommandListener(this);
		setItemStateListener(this);
	}

	/**
	 * Initialize the seating canvas variables.
	 * @exception IOException  when some type of network error occurs.
	 */
	public void init(MovieRating[] movieRatings) throws ApplicationException {
		this.movieRatings = movieRatings;

		int numOfRatings = movieRatings.length;

		deleteAll();

		// create items
		movieRatesUI = new Gauge[numOfRatings];
		movieRatesNum = new int[numOfRatings];
		modifiedMovieRatings = new boolean[numOfRatings];
		movieNames = new StringItem[numOfRatings];
		lastViewDates = new StringItem[numOfRatings];

		for (int i = 0; i < numOfRatings; i++) {
			movieRatesNum[i] = movieRatings[i].getStarNumber();
			movieNames[i] = new StringItem(null, movieRatings[i].getMovie().getTitle());
			// need to parse the date value

			/*
			 * lastViewDates[i] =  new StringItem(null,
			 * movieRatings[i].getLastViewingDate()+"");
			 */

			/*
			 * if (movieRatesNum[i] == MovieRating.RATING_UNRATED) {
			 * movieRatesUI[i].setLabel(uiController.getString(UIConstants.UNRATED));
			 * }
			 */
			movieRatesUI[i] = new Gauge(null, true, MovieRating.RATING_FIVE_STARS, movieRatesNum[i]);
			modifiedMovieRatings[i] = false;

			movieNames[i].setLayout(Item.LAYOUT_CENTER);
			// lastViewDates[i].setLayout(Item.LAYOUT_CENTER);
			movieRatesUI[i].setLayout(Item.LAYOUT_CENTER);
			append(movieRatesUI[i]);
			append(movieNames[i]);
			// append(lastViewDates[i]);
		}
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == syncCommand) {
			uiController.rateMyMoviesSyncRequested();
		} else if (command == saveCommand) {
			MovieRating[] tmp = new MovieRating[movieRatings.length];
			int j = 0;

			for (int i = 0; i < movieRatings.length; i++) {
				if (modifiedMovieRatings[i]) {
					tmp[j++] = movieRatings[i];
					modifiedMovieRatings[i] = false;
				}
			}

			if (j > 0) {
				System.out.println("j:" + j);

				MovieRating[] modifiedList = new MovieRating[j];

				System.arraycopy(tmp, 0, modifiedList, 0, j);
				uiController.rateMyMoviesSaveRequested(modifiedList);
			}
		} else if (command == cancelCommand) {
			uiController.mainMenuRequested();
		} else {
			uiController.commandAction(command, displayable);
		}
	}

	public void itemStateChanged(Item item) {
		for (int i = 0; i < movieRatesUI.length; i++) {
			if (item == movieRatesUI[i]) {
				movieRatings[i].setStarNumber(movieRatesUI[i].getValue());

				modifiedMovieRatings[i] = true;
			}
		}
	}

}
