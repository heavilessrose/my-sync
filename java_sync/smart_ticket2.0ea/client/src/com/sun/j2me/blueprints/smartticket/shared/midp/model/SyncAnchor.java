package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class SyncAnchor {

	private int clientId = -1;
	private long lastSyncTime = 0L;
	private boolean outOfSync = false;

	public SyncAnchor() {
	}

	public SyncAnchor(int clientId, long lastSyncTime, boolean outOfSync) {
		this.clientId = clientId;
		this.lastSyncTime = lastSyncTime;
		this.outOfSync = outOfSync;

		return;
	}

	public int getClientId() {
		return clientId;
	}

	public long getLastSyncTime() {
		return lastSyncTime;
	}

	public boolean isOutOfSync() {
		return outOfSync;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeInt(clientId);
			dataStream.writeLong(lastSyncTime);
			dataStream.writeBoolean(outOfSync);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static SyncAnchor deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			SyncAnchor syncAnchor = new SyncAnchor();

			syncAnchor.clientId = dataStream.readInt();
			syncAnchor.lastSyncTime = dataStream.readLong();
			syncAnchor.outOfSync = dataStream.readBoolean();

			return syncAnchor;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	// For debugging only (downsizing candidate)
	public String toString() {
		return new StringBuffer("SyncAnchor: ").append(clientId).append(" ").append(lastSyncTime)
				.append(" ").append(outOfSync).toString();
	}

}
