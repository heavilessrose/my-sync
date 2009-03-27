package com.sun.j2me.blueprints.smartticket.client.midp.util;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public interface ProgressObserver {

	public boolean isStoppable();

	public void setStoppable(boolean stoppable);

	public boolean isStopped();

	public void updateProgress();

	public void setNote(String note);
}
