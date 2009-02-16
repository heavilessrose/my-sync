package bm.mvc;

import bm.core.Application;
import bm.core.ResourceManager;
import bm.core.event.ProgressEvent;
import bm.core.io.SerializationException;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.mvc.AbstractController;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.View;
import bm.db.DBException;
import bm.db.Database;
import bm.err.ErrorLog;
import bm.net.NetException;
import bm.net.rpc.RemoteCallException;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;

import javax.microedition.lcdui.Gauge;
import java.io.IOException;
import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 09-mar-2007 16:08:06
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Base Controller with functionality common to all the controllers in the
 * application.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 * @noinspection ThrowableInstanceNeverThrown
 */
public abstract class BaseController extends AbstractController {
	/** @noinspection AnalyzingVariableNaming */
	private static final Log log = LogFactory.getLog("BaseController");

	protected static final int KB = 1024;
	protected static final int MB = KB * KB;
	protected static final int MIN_FREE_DISK = 500 * KB;

	protected static Hashtable errorMessages = new Hashtable(7);
	protected static ProgressEvent waitEvent;
	static {
		waitEvent = new ProgressEvent();
		waitEvent.setAnimate(true);
		final String waitMessage = ResourceManager.getResource("global.Wait");
		waitEvent.setMessage("");
		waitEvent.setPhase(waitMessage);
		waitEvent.setTitle(waitMessage);

		errorMessages.put(new SerializationException(0).getClass(),
				"error.Serialization");
		errorMessages.put(new RemoteCallException(0).getClass(),
				"error.RemoteCall");
		errorMessages.put(new NetException(0).getClass(), "error.Network");
		errorMessages.put(new IOException().getClass(), "error.Serialization");
		errorMessages.put(new DBException(0).getClass(), "error.DB");
		errorMessages.put(new RSException(0).getClass(), "error.DB");
		errorMessages.put(new RecordStoreFullException(0).getClass(),
				"error.RSFull");
	}

	protected int versionWarningCount;
	protected int warningCount;

	/** @noinspection DuplicateThrows,JavaDoc */
	protected abstract void continueProcess(final ControllerEvent event)
			throws SerializationException, NetException,
			RecordStoreFullException, DBException, RSException,
			RemoteCallException, Exception;

	public void mildExitApplication() {
		final boolean confirmExit = Application.getManager().confirm(
				ResourceManager.getResource("global.Exit"),
				ResourceManager.getResource("main.AskExitApplication"));
		if (confirmExit) {
			exitApplication();
		}
	}

	/**
	 * Process an event. Subclasesses must implement this method to handle
	 * events.
	 * 
	 * @param event
	 *            event to handle
	 */
	protected void process(final ControllerEvent event) {
		try {
			boolean consumed = false;
			final View source = event.getSource();
			if (source instanceof ListBrowserView) {
				consumed = processListBrowserView(event);
			}
			if (!consumed) {
				if (event.getCode() == 11000) {
					event.getSource().go(
							ViewFactory.getMenu((String) event.get("view")));
				} else {
					continueProcess(event);
				}
			}
		} catch (Exception e) {
			handleError(event.getSource(), e, null);
		}
	}

	/**
	 * Handle an error.
	 * 
	 * @param source
	 *            source view that originated the error, if any.
	 * @param e
	 *            the error
	 * @param message
	 *            error message
	 */
	public void handleError(final Object source, final Exception e,
			final String message) {
		showError(e, message);
		ErrorLog.addError(source != null ? source.getClass().getName()
				: "BaseController", "unkown", null, message, e);
		checkDatabaseSanity();
		recoverView(source != null && source instanceof View ? (View) source
				: null);
	}

	/**
	 * Get access to the application database.
	 * 
	 * @return the application database
	 * @throws DBException
	 *             on database errors
	 * @throws RecordStoreFullException
	 *             if no space left
	 * @throws RSException
	 *             on storage errors
	 * @throws SerializationException
	 *             on object serialization errors
	 */
	protected abstract Database getDatabase() throws DBException,
			RecordStoreFullException, RSException, SerializationException;

	protected void checkDatabaseSanity() {
		try {
			getDatabase().sanityCheck();
			if (getDatabase().isDamaged()) {
				Application.getManager().showWarning(
						ResourceManager.getResource("warn.databaseDamaged"));
			}
		} catch (Exception e) {
			showError(e, null);
		}
	}

	protected void showError(final Exception e, final String msg) {
		log.error(e);
		final String message = (String) errorMessages.get(e.getClass());
		if (message != null) {
			Application.getManager().showError(
					ResourceManager.getResource(message)
							+ (msg != null ? ", " + msg : ""), e);
		} else if (msg != null) {
			Application.getManager().showError(msg, e);
		} else {
			Application.getManager().showError(e);
		}
	}

	protected void exitApplication() {
		Application.getManager().exitApplication();
	}

	// This method is only intented for the Native interface ListBrowsers
	private boolean processListBrowserView(final ControllerEvent event)
			throws RecordStoreFullException, DBException, RSException {
		switch (event.getCode()) {
		case ListBrowserView.PAGE_DOWN:
			((ListBrowserView) event.getSource()).pageDown();
			return true;

		case ListBrowserView.PAGE_UP:
			((ListBrowserView) event.getSource()).pageUp();
			return true;

		default:
			return false;
		}
	}

	protected void showSearchingProgress(final ControllerEvent event) {
		final String title = ResourceManager.getResource("global.Searching");
		final ProgressEvent pe = new ProgressEvent(event.getSource(),
				new Integer(Gauge.INDEFINITE), Gauge.CONTINUOUS_RUNNING);
		pe.setTitle(title);
		pe.setPhase(title);
		pe.dispatch();
	}

	protected void recoverView(final View source) {
		if (source != null) {
			source.show();
		} else {
			Application.getManager().getMainView().show();
		}
	}
}
