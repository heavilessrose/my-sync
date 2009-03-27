package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import com.sun.j2me.blueprints.smartticket.shared.midp.model.MovieRating;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Movie;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Synchronizable;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.client.midp.ui.UIController.EventIds;
import javax.microedition.lcdui.*;
import java.io.IOException;

public class SyncMovieRatingsUI extends Form implements CommandListener {

	private MovieRating[][] movieRatingPairs;
	private UIController uiController;
	private Command okCommand;

	public SyncMovieRatingsUI(UIController uiController) {
		super(uiController.getString(UIConstants.CONFLICTING_RATINGS));

		this.uiController = uiController;
		okCommand = new Command(uiController.getString(UIConstants.SAVE), Command.OK, 1);

		addCommand(okCommand);
		setCommandListener(this);
	}

	public void init(MovieRating[][] movieRatingPairs) {
		this.movieRatingPairs = movieRatingPairs;

		for (int i = size(); i > 0; i--) {
			delete(i - 1);
		}

		for (int i = 0; i != movieRatingPairs.length; i++) {
			try {
				MovieRating rating1 = movieRatingPairs[i][0];
				MovieRating rating2 = movieRatingPairs[i][1];
				ChoiceGroup ratingChoiceGroup = new ChoiceGroup(rating1.getMovie().getTitle(),
						Choice.EXCLUSIVE);

				ratingChoiceGroup.append(
						(rating1.getModificationStatus() != Synchronizable.STATUS_DELETED) ? String
								.valueOf(rating1.getStarNumber()) : uiController
								.getString(UIConstants.DELETED), null);
				ratingChoiceGroup.append(
						(rating2.getModificationStatus() != Synchronizable.STATUS_DELETED) ? String
								.valueOf(rating2.getStarNumber()) : uiController
								.getString(UIConstants.DELETED), null);
				append(ratingChoiceGroup);
			} catch (ApplicationException ae) {
				ae.printStackTrace();
			}
		}
	}

	public void commandAction(Command command, Displayable displayable) {
		if (command == okCommand) {
			MovieRating[] movieRatings = new MovieRating[size()];

			for (int i = 0; i != size(); i++) {
				ChoiceGroup ratingChoiceGroup = (ChoiceGroup) get(i);

				movieRatings[i] = movieRatingPairs[i][ratingChoiceGroup.getSelectedIndex()];
			}

			uiController.rateMyMoviesResolutionRequested(movieRatings);
		}
	}

}
