package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import java.io.IOException;
import java.util.Calendar;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.client.midp.model.Preferences;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Theater;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.TheaterSchedule;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Movie;

public class ChooseMovieUI extends Form implements CommandListener, ItemStateListener,
		ItemCommandListener {

	private UIController uiController;
	private Preferences preferences;
	private final static int THEATERLIST_ITEM_NUM = 1;
	private final static int MOVIESDATE_ITEM_NUM = THEATERLIST_ITEM_NUM + 1;
	private final static int MOVIELIST_ITEM_NUM = MOVIESDATE_ITEM_NUM + 1;
	private final static int SHOWDATELIST_ITEM_NUM = MOVIELIST_ITEM_NUM + 1;
	// num count of monday - sunday, should be 7
	private final static int SHOWDATE_NUM = UIConstants.PREF_DATE_END - UIConstants.PREF_DATE_START
			+ 1;
	private Command selectSeatsCommand;
	private Theater[] theaters;
	private TheaterSchedule.MovieSchedule[] movieSchedules;
	private Movie[] movies;
	private int[][] showTimes;
	private String[][] showTimebyDate;
	private ChoiceGroup theaterList;
	private ChoiceGroup movieList;
	private ChoiceGroup showDateList;
	private ChoiceGroup showTimesList;
	private ChoiceGroup seatingPref;
	private TextField numOfTickets;
	private StringItem theaterListLabel;
	private StringItem moviesDate;
	private StringItem movieTimeLabel;
	private StringItem selectSeats;

	public ChooseMovieUI(UIController uiController) {
		super(uiController.getString(UIConstants.CHOOSE_MOVIE_TITLE));

		this.uiController = uiController;

		createItems();

		selectSeatsCommand = new Command(uiController.getString(UIConstants.SELECT_SEATS),
				Command.ITEM, 1);

		// the order of items is important for layout
		append(theaterListLabel);
		append(theaterList);
		selectSeats.setDefaultCommand(selectSeatsCommand);
		selectSeats.setItemCommandListener(this);
		setCommandListener(this);
		setItemStateListener(this);
	}

	public void init(Preferences preferences) {
		this.preferences = preferences;

		numOfTickets.setString(String.valueOf(preferences.getDefaultTicketNumber()));
		Utils.setChoiceGroupEntry(uiController, preferences.getFavoriteDay(), showDateList);
		Utils.setChoiceGroupEntry(uiController, preferences.getFavoriteSeat(), seatingPref);
		// remove all the items in the form except theater list
		cleanItemAfter(THEATERLIST_ITEM_NUM);
	}

	public void setTheaters(Theater[] allTheaters, Theater[] downloadedTheaters) {
		if (allTheaters == null && downloadedTheaters == null) {
			uiController.showErrorAlert(uiController.getString(UIConstants.NO_THEATER_FOUND));
			uiController.mainMenuRequested();

			return;
		}

		this.theaters = allTheaters;

		theaterList.deleteAll();
		theaterList.append(uiController.getString(UIConstants.SELECT_THEATER), null);
		Utils.appendTheaterList(uiController, theaterList, theaters, downloadedTheaters);
	}

	public void itemStateChanged(Item item) {
		if (item == theaterList) {
			// the first one is 'select theater'
			int selection = theaterList.getSelectedIndex();

			if (theaterList.getString(0).equals(uiController.getString(UIConstants.SELECT_THEATER))) {
				// remove the first 'select theater option"
				if (selection == 0) {
					return;
				} else {
					theaterList.delete(0);

					selection--;
				}
			}

			theaterChanged(selection);
		} else if (item == movieList) {
			resetDayList();
			cleanItemAfter(SHOWDATELIST_ITEM_NUM);
		} else if (item == showDateList) {
			try {
				setShowTimes(movieSchedules[movieList.getSelectedIndex()].getShowTimes(),
						showDateList.getSelectedIndex() + 1);
			} catch (ApplicationException e) {
				System.err.println("Exception " + e);
			}
		}
	}

	public void theaterScheduleDownloadSuccess(TheaterSchedule theaterSchedule, int selection) {
		String theaterName = theaters[selection].getName() + " ["
				+ theaters[selection].getZipCode() + "]";

		// we only update the image for expired entries, even though the remote
		// theater schedule is in cache, we still want to show it as remote to
		// indicate possible chance to download more information in the future
		if (theaterList.getImage(selection) == uiController
				.getImage(UIConstants.ICON_IDX_SCHEDULE_EXPIRED)) {
			theaterList.set(selection, theaterName, uiController
					.getImage(UIConstants.ICON_IDX_SCHEDULE_LOCAL));
		}

		movieSchedules = theaterSchedule.getMovieSchedules();
		movies = new Movie[movieSchedules.length];

		movieList.deleteAll();

		for (int i = 0; i < movieSchedules.length; i++) {
			movies[i] = movieSchedules[i].getMovie();

			String rating = movies[i].getRating();
			byte imageIndex = UIConstants.ICON_IDX_RATING_UNRATED;

			if ("g".equals(rating)) {
				imageIndex = UIConstants.ICON_IDX_RATING_G;
			} else if ("pg".equals(rating)) {
				imageIndex = UIConstants.ICON_IDX_RATING_PG;
			} else if ("r".equals(rating)) {
				imageIndex = UIConstants.ICON_IDX_RATING_R;
			}

			movieList.append(movies[i].getTitle(), uiController.getImage(imageIndex));
		}

		resetDayList();
		cleanItemAfter(THEATERLIST_ITEM_NUM);
		append(moviesDate);
		append(movieList);
		append(showDateList);
	}

	private void resetDayList() {
		// add the choose a day option
		if (showDateList.size() <= SHOWDATE_NUM) {
			showDateList.insert(0, uiController.getString(UIConstants.CHOOSE_A_DAY), null);
			showDateList.setSelectedIndex(0, true);
		}
	}

	private void theaterChanged(int selection) {
		String theaterName = theaters[selection].getName() + " ["
				+ theaters[selection].getZipCode() + "]";

		// remove all items first
		// this method will call theaterScheduleDownloadSuccess later
		uiController.lookupTheaterSchedule(theaters[selection], selection);
	}

	private int[] getShowTimes() {
		int[] showTime = new int[3];

		showTime[0] = showDateList.getSelectedIndex() + 1;

		String showTimeStr = showTimesList.getString(showTimesList.getSelectedIndex());
		int colonPos = showTimeStr.indexOf(':');

		showTime[1] = Integer.parseInt(showTimeStr.substring(0, colonPos));
		showTime[2] = Integer.parseInt(showTimeStr
				.substring(colonPos + 1, showTimeStr.indexOf(' ')));

		if (showTimeStr.indexOf(uiController.getString(UIConstants.PREF_PM)) > 0
				&& showTime[1] != 12) {
			showTime[1] = showTime[1] + 12;
		}

		return showTime;
	}

	private void setShowTimes(int[][] showTimes, int date) {
		movieTimeLabel.setText(movieList.getString(movieList.getSelectedIndex()));

		this.showTimes = showTimes;

		showTimesList.deleteAll();

		for (int i = 0; i < showTimes.length; i++) {
			if (showTimes[i][0] == date) {
				showTimesList.append(Utils.showTimeToHourMinute(uiController, showTimes[i]), null);
			}
		}

		// add back the choose a day option
		if (showDateList.size() > SHOWDATE_NUM) {
			showDateList.delete(0);
		}

		cleanItemAfter(SHOWDATELIST_ITEM_NUM);
		append(movieTimeLabel);
		append(showTimesList);
		append(seatingPref);
		append(numOfTickets);
		append(selectSeats);
	}

	public void commandAction(Command command, Item item) {
		if (command == selectSeatsCommand) {
			if (numOfTickets.getString().length() == 0
					|| Integer.parseInt(numOfTickets.getString()) < 1) {
				uiController.showErrorAlert(uiController.getString(UIConstants.NUM_OF_TICKET_ERR));
			} else {
				uiController.selectSeatsSelected(movieSchedules[movieList.getSelectedIndex()],
						getShowTimes());
			}
		}
	}

	public void commandAction(Command command, Displayable displayable) {
		uiController.commandAction(command, displayable);
	}

	private void createItems() {
		theaterList = new ChoiceGroup(null, Choice.POPUP);
		movieList = new ChoiceGroup(null, Choice.EXCLUSIVE);
		showTimesList = new ChoiceGroup(null, Choice.EXCLUSIVE);
		seatingPref = new ChoiceGroup(null, Choice.POPUP);
		seatingPref = Utils.newPopUp(uiController,
				uiController.getString(UIConstants.SEATING_PREF), UIConstants.PREF_SEAT_START,
				UIConstants.PREF_SEAT_END);
		showDateList = Utils.newPopUp(uiController, uiController.getString(UIConstants.DAY_COLON),
				UIConstants.PREF_DATE_START, UIConstants.PREF_DATE_END);
		moviesDate = new StringItem(uiController.getString(UIConstants.MOVIES_FOR), null);
		movieTimeLabel = new StringItem(uiController.getString(UIConstants.MOVIE_TIMES_FOR), null);
		theaterListLabel = new StringItem(uiController.getString(UIConstants.MOVIE_SCHEDULE_FOR),
				null);
		selectSeats = new StringItem(null, uiController.getString(UIConstants.SELECT_SEATS),
				Item.BUTTON);

		selectSeats.setLayout(Item.LAYOUT_CENTER);

		numOfTickets = new TextField(uiController.getString(UIConstants.NUMBER_OF_TICKETS), "", 3,
				TextField.NUMERIC);
	}

	private void cleanItemAfter(int itemNum) {
		if (size() > itemNum) {
			for (int i = size() - 1; i > itemNum; i--) {
				delete(i);
			}
		}
	}

}
