package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.io.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Synchronizable;
import com.sun.j2me.blueprints.smartticket.client.midp.ui.UIConstants;

/**　用户设置 */
public class Preferences {

	public static final int ORDERING_NONE = 0;
	public static final int ORDERING_COLLABORATIVE_RATING = 1;
	public static final int ORDERING_CRITICS_RATING = 21;
	public static final int FILTERING_NONE = 0;
	public static final int FILTERING_ALREADY_SEEN = 1;
	private String defaultZipCode = "";
	private String secondZipCode = "";
	private int favoriteDay = UIConstants.PREF_SUN;
	private int favoriteSeat = UIConstants.PREF_CENTER_M;
	private int updateMovieSchedule = UIConstants.AUTOMATICALLY;
	private int listMovie = UIConstants.SUGGESTED_ORDER;
	private int defaultTicketNumber = 1;
	private int conflictResolutionStrategyId = Synchronizable.STRATEGY_LAST_MODIFIED_WINS;
	private boolean caching = true;
	private boolean silentlyLoggingIn = false;
	private boolean silentlyUpdating = false;
	private boolean silentlySynchronizing = false;
	private Hashtable updatableTheaterScheduleKeys = new Hashtable();

	public Preferences() {
	}

	public String getDefaultZipCode() {
		return defaultZipCode;
	}

	public void setDefaultZipCode(String defaultZipCode) {
		this.defaultZipCode = defaultZipCode;
	}

	public String getSecondZipCode() {
		return secondZipCode;
	}

	public void setSecondZipCode(String secondZipCode) {
		this.secondZipCode = secondZipCode;
	}

	public int getDefaultTicketNumber() {
		return defaultTicketNumber;
	}

	public void setDefaultTicketNumber(int defaultTicketNumber) {
		this.defaultTicketNumber = defaultTicketNumber;
	}

	public int getFavoriteDay() {
		return favoriteDay;
	}

	public void setFavoriteDay(int favoriteDay) {
		this.favoriteDay = favoriteDay;
	}

	public int getUpdateMovieSchedule() {
		return updateMovieSchedule;
	}

	public void setUpdateMovieSchedule(int updateMovieSchedule) {
		this.updateMovieSchedule = updateMovieSchedule;
	}

	public int getConflictResolutionStrategyId() {
		return conflictResolutionStrategyId;
	}

	public void setConflictResolutionStrategyId(int conflictResolutionStrategyId) {
		this.conflictResolutionStrategyId = conflictResolutionStrategyId;
	}

	public int getListMovie() {
		return listMovie;
	}

	public void setListMovie(int listMovie) {
		this.listMovie = listMovie;
	}

	public int getFavoriteSeat() {
		return favoriteSeat;
	}

	public void setFavoriteSeat(int favoriteSeat) {
		this.favoriteSeat = favoriteSeat;
	}

	public boolean isCaching() {
		return caching;
	}

	public void setCaching(boolean caching) {
		this.caching = caching;
	}

	public boolean isSilentlyLoggingIn() {
		return silentlyLoggingIn;
	}

	public void setSilentlyLoggingIn(boolean silentlyLoggingIn) {
		this.silentlyLoggingIn = silentlyLoggingIn;
	}

	public boolean isSilentlyUpdating() {
		return silentlyUpdating;
	}

	public void setSilentlyUpdating(boolean silentlyUpdating) {
		this.silentlyUpdating = silentlyUpdating;
	}

	public boolean isSilentlySynchronizing() {
		return silentlySynchronizing;
	}

	public void setSilentlySynchronizing(boolean silentlySynchronizing) {
		this.silentlySynchronizing = silentlySynchronizing;
	}

	public Enumeration getUpdatableTheaterScheduleKeys() {
		return updatableTheaterScheduleKeys.elements();
	}

	public void addUpdatableTheaterScheduleKey(String theaterScheduleKey) {
		updatableTheaterScheduleKeys.put(theaterScheduleKey, theaterScheduleKey);

		return;
	}

	public void removeUpdatableTheaterScheduleKey(String theaterScheduleKey) {
		updatableTheaterScheduleKeys.remove(theaterScheduleKey);

		return;
	}

	public boolean isUpdatable(String theaterScheduleKey) {
		return updatableTheaterScheduleKeys.contains(theaterScheduleKey);
	}

	public void copy(Preferences preferences) {
		this.defaultZipCode = preferences.defaultZipCode;
		this.secondZipCode = preferences.secondZipCode;
		this.defaultTicketNumber = preferences.defaultTicketNumber;
		this.conflictResolutionStrategyId = preferences.conflictResolutionStrategyId;
		this.favoriteDay = preferences.favoriteDay;
		this.favoriteSeat = preferences.favoriteSeat;
		this.updateMovieSchedule = preferences.updateMovieSchedule;
		this.listMovie = preferences.listMovie;
		this.caching = preferences.caching;
		this.silentlyLoggingIn = preferences.silentlyLoggingIn;
		this.silentlyUpdating = preferences.silentlyUpdating;
		this.silentlySynchronizing = preferences.silentlySynchronizing;
		this.updatableTheaterScheduleKeys = preferences.updatableTheaterScheduleKeys;

		return;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeUTF(defaultZipCode);
			dataStream.writeUTF(secondZipCode);
			dataStream.writeByte(defaultTicketNumber);
			dataStream.writeByte(conflictResolutionStrategyId);
			dataStream.writeByte(favoriteDay);
			dataStream.writeByte(favoriteSeat);
			dataStream.writeByte(updateMovieSchedule);
			dataStream.writeByte(listMovie);
			dataStream.writeBoolean(caching);
			dataStream.writeBoolean(silentlyLoggingIn);
			dataStream.writeBoolean(silentlyUpdating);
			dataStream.writeBoolean(silentlySynchronizing);
			dataStream.writeInt(updatableTheaterScheduleKeys.size());

			for (Enumeration theaterScheduleKeys = getUpdatableTheaterScheduleKeys(); theaterScheduleKeys
					.hasMoreElements();) {
				dataStream.writeUTF((String) theaterScheduleKeys.nextElement());
			}

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static Preferences deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			Preferences preferences = new Preferences();

			preferences.defaultZipCode = dataStream.readUTF();
			preferences.secondZipCode = dataStream.readUTF();
			preferences.defaultTicketNumber = dataStream.readByte();
			preferences.conflictResolutionStrategyId = dataStream.readByte();
			preferences.favoriteDay = dataStream.readByte();
			preferences.favoriteSeat = dataStream.readByte();
			preferences.updateMovieSchedule = dataStream.readByte();
			preferences.listMovie = dataStream.readByte();
			preferences.caching = dataStream.readBoolean();
			preferences.silentlyLoggingIn = dataStream.readBoolean();
			preferences.silentlyUpdating = dataStream.readBoolean();
			preferences.silentlySynchronizing = dataStream.readBoolean();

			int theaterScheduleKeyCount = dataStream.readInt();

			for (int i = 0; i < theaterScheduleKeyCount; i++) {
				preferences.addUpdatableTheaterScheduleKey(dataStream.readUTF());
			}

			return preferences;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

}
