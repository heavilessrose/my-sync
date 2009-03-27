package com.sun.j2me.blueprints.smartticket.shared.midp.model;

public interface Synchronizable {

	int STRATEGY_FIRST_MODIFIED_WINS = 0;
	int STRATEGY_LAST_MODIFIED_WINS = 1;
	int STRATEGY_ASK_USER = 2;
	int STATUS_UNCHANGED = 0;
	int STATUS_INSERTED = 1;
	int STATUS_MODIFIED = 2;
	int STATUS_DELETED = 3;

	long getLastModificationDate();

	void setModificationStatus(int modificationStatus);

	int getModificationStatus();

	/*
	 * interface ConflictResolver {
	 * Synchronizable resolveConflict(Synchronizable item1, Synchronizable item2);
	 * }
	 */
}
