package passkeep.control;

import bm.mvc.BaseController;
import bm.core.*;
import bm.core.event.Event;
import bm.core.event.EventListener;
import bm.core.event.ProgressEvent;
import bm.core.fs.FSException;
import bm.core.io.SerializationException;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.math.FixedPoint;
import bm.core.mvc.ControllerEvent;
import bm.core.mvc.HelpView;
import bm.core.mvc.View;
import bm.core.tools.DeviceInfo;
import bm.db.*;
import bm.err.ErrorLog;
import bm.mvc.*;
import bm.net.NetException;
import bm.net.rpc.RemoteCall;
import bm.net.rpc.RemoteCallException;
import bm.storage.InvalidRecordIDException;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.storage.Store;
import bm.ui.ProgressView;
import bm.ui.Ui;
import bm.util.Util;
import passkeep.AppState;
import passkeep.Model;

import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Gauge;
import javax.microedition.rms.RecordStoreException;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 27-jul-2005 12:03:31
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2005/11/30 10:21:30 $
 * Revision         : $Revision: 1.9 $
 */

/**
 * Controller for general view.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 1.9 $
 */
public class MainController extends BaseController implements EventListener {
	private static Log log = LogFactory.getLog("MainController");

	public static final int VIEW_ID_MAIN = 1;
	public static final int VIEW_ID_LANGUAGE = 2;
	public static final int VIEW_ID_BROWSE_CATEGORIES = 3;
	public static final int VIEW_ID_EDIT_CATEGORY = 4;
	public static final int VIEW_ID_SEARCH_ENTRIES = 5;
	public static final int VIEW_ID_BROWSE_ENTRIES = 6;
	public static final int VIEW_ID_EDIT_ENTRY = 7;
	public static final int VIEW_ID_TOOLS = 15;
	public static final int VIEW_ID_PREFERENCES = 16;
	public static final int VIEW_ID_FONTSIZE = 18;
	public static final int VIEW_ID_DIAGNOSIS = 19;

	private static final int MAINM_CATEGORIES = 100;
	private static final int MAINM_SEARCH_ENTRIES = 101;
	private static final int MAINM_ABOUT = 105;

	public static final int TOOLSMENU_DELETE_DATA = 204;

	public static final int DIAGMENU_INFO = 101;
	public static final int DIAGMENU_CHEK_MEM = 102;
	public static final int DIAGMENU_DEVICE_INFO = 106;
	public static final int DIAGMENU_FORCE_DEVICE_INFO = 107;
	public static final int DIAGMENU_VIEW_LOG = 109;

	private static final int ACTION_FTS_SMALL = 201;
	private static final int ACTION_FTS_MEDIUM = 202;
	private static final int ACTION_FTS_LARGE = 203;

	private static final int ACTION_CATB_NEW_CAT = 1;
	private static final int ACTION_CATB_DEL_CAT = 2;
	private static final int ACTION_CATB_EDIT_CAT = 3;
	private static final int ACTION_CATB_NEW_KEY = 4;

	private static final int ACTION_ENTB_NEW_ENT = 1;
	private static final int ACTION_ENTB_DEL_ENT = 2;

	private static final int LANG_ES = 201;
	private static final int LANG_EN = 202;

	// Cache for custom view
	private static Hashtable views = new Hashtable(40);
	// List of view that should not have a back action
	private static Vector noBack = new Vector(3);
	// List of view that should have an exit option
	private static Vector exitPoints = new Vector(3);
	// Event for triggering when creating a new instance of a custom view
	// private static ProgressEvent waitEvent;

	private EventListener progressListener = new ProgressView();

	// -------------------------------------------------------------------------
	// Public methods
	// -------------------------------------------------------------------------

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
	protected Database getDatabase() throws DBException,
			RecordStoreFullException, RSException, SerializationException {
		return Model.getDatabase();
	}

	/**
	 * Handle language change events.
	 * 
	 * @param event
	 *            the event
	 */
	public void handleEvent(Event event) {
		switch (event.getType().intValue()) {
		case Event.ET_LANGUAGE_CHANGE:
			// Create a new progress view when the language changes, so it
			// gets built with the appropriate resources
			progressListener = new ProgressView();
			break;

		case 1000: // ViewFactory.EVENT_VIEW_CREATED:
			final View view = (View) event.getAttachment("view");
			addView((String) event.getAttachment("viewName"), view, false);
			if (view instanceof DataboundView) {
				((DataboundView) view).setResolver(DefaultFieldResolver
						.getInstance());
			}
			break;
		}
	}

	/**
	 * Release views.
	 * 
	 * @param view
	 *            view released
	 */
	public void onRelease(final View view) {
		view.detachAll();
	}

	// <Application Startup> ---------------------------------------------------
	/**
	 * Start application.
	 */
	public void startApplication() {
		// Local variable to improve speed on CDC
		final Log log = MainController.log;
		bm.core.ErrorHandler.setHandler(new ErrorHandler());
		Event.register(this, Event.LANGUAGE_CHANGE);
		ProgressEvent pe = new ProgressEvent(this, new Integer(4), 0);
		pe.setTitle(ResourceManager.getResource("main.StartApplication"));
		pe.dispatch();
		try {
			init();
			pe.increment();
			pe.dispatch();

			log.debug("Starting application");
			if (!checkConfig()) {
				exitApplication();
				return;
			}
			setup();
			pe.increment();
			pe.dispatch();

			if (LoginManager.getEncryptedPassword() == null) {
				createPin();
			} else {
				promptPin();
			}

			View nextView = startDatabase();
			checkDatabaseSanity();
			pe.increment();
			pe.dispatch();
			final Database database = Model.getDatabase();
			DefaultFieldResolver.setDatabase(database);
			if (nextView == null) {
				nextView = restoreLastView();
			}
			if (nextView == null) {
				nextView = ViewFactory.getMenu("mainMenu");
			}
			Event.unregisterAll(Event.PROGRESS);
			Event.register(progressListener, Event.PROGRESS);
			nextView.show();
		} catch (RuntimeException e) {
			e.printStackTrace();
			log.error(e);
			Application.getManager().showError(
					ResourceManager.getResource("error.cantStartApp"), e);
			exitApplication();
		} catch (Throwable e) {
			e.printStackTrace();
			log.error(e);
			Application.getManager().showError(
					ResourceManager.getResource("error.cantStartApp") + ": "
							+ e.toString());
			exitApplication();
		}
	}

	private void promptPin() throws DBException {
		final String pin = Application.getManager().promptPassword(
				ResourceManager.getResource("pin.AskPin"));
		if (pin == null || pin.trim().length() == 0
				|| !LoginManager.checkOfflinePassword(pin)) {
			Application.getManager().showError(
					ResourceManager.getResource("pin.WrongPin"));
			exitApplication();
		} else {
			AppState.setPassword(pin);
		}
	}

	private void createPin() throws DBException {
		boolean done = false;
		while (!done) {
			final String pin = Application.getManager().promptPassword(
					ResourceManager.getResource("pin.CreatePin"));
			if (pin == null || pin.trim().length() == 0) {
				Application.getManager().showError(
						ResourceManager.getResource("pin.CantBeEmpty"));
				continue;
			}
			final String pin2 = Application.getManager().promptPassword(
					ResourceManager.getResource("pin.ConfirmPin"));
			if (pin2 == null || pin2.trim().length() == 0 || !pin.equals(pin2)) {
				Application.getManager().showError(
						ResourceManager.getResource("pin.DoNotMatch"));
			} else {
				LoginManager.storeEncryptedPassword(pin);
				AppState.setPassword(pin);
				done = true;
			}
		}
	}

	private View restoreLastView() {
		try {
			// ToDo: if there's something to restore do it here and return the
			// view that should be shown next
			return null;
		} catch (Exception e) {
			log.error(e);
			return null;
		}
	}

	/**
	 * Basic setup needed by anything else.
	 * 
	 * @throws IOException
	 *             on errors
	 * @throws DBException
	 *             on errors
	 */
	private void init() throws IOException, DBException {
		final String locale = Settings.getProperty(Model.CONFIG_ACTIVE_LOCALE);
		if (locale != null) {
			ResourceManager.setActiveLocale(locale);
		}
		DefaultFieldResolver.registerDefinition("/app.resolver");
		Event.register(this, ViewFactory.EVENT_VIEW_CREATED);
		// waitEvent = new ProgressEvent();
		// waitEvent.setAnimate( true );
		// final String waitMessage = ResourceManager.getResource( "global.Wait"
		// );
		// waitEvent.setMessage( "" );
		// waitEvent.setPhase( waitMessage );
		// waitEvent.setTitle( waitMessage );

		// ToDo: add the id's of the views that should have an exit command
		exitPoints.addElement(new Integer(MainController.VIEW_ID_MAIN));

		// ToDo: add the id's of the views that shouldn't have a back action
		// noBack.addElement( new Integer( VIEW_ID ) );
	}

	/**
	 * Check that the needed configuration values are set.
	 * 
	 * @return true if so
	 */
	private boolean checkConfig() {
		// ToDo: check for any configuration setting that must be present
		// if( Application.getManager().getProperty( "server.url" ) == null )
		// {
		// Application.getManager().showError(
		// ResourceManager.getResource(
		// "error.missconfigured"
		// )
		// );
		// return false;
		// }
		return true;
	}

	/**
	 * Perform the extended setup
	 * 
	 * @throws DBException
	 *             on errors
	 */
	private void setup() throws DBException {
		Application.getManager().setMainView(ViewFactory.getMenu("mainMenu"));
		// The server.url property is the default for remote calls if defined
		final String serverURL = Application.getManager().getProperty(
				"server.url");
		if (serverURL != null) {
			RemoteCall.setDefaultURL(serverURL);
		}
		// #ifdef bluetooth
		if (Util.hasBluetoothAPI()) {
			ViewFactory.setFlag("bluetooth");
		}
		// #endif
		// #ifdef cldc11
		ViewFactory.setFlag("cldc11");
		// #endif
	}

	/**
	 * Start the database.
	 * 
	 * @return the next view, that will be login if no database or main view if
	 *         downloaded
	 * @throws SerializationException
	 *             on errors
	 * @throws IOException
	 *             on errors
	 * @throws NetException
	 *             on errors
	 * @throws RemoteCallException
	 *             on errors
	 * @throws RSException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             on errors
	 * @throws DBException
	 *             on errors
	 */
	private View startDatabase() throws SerializationException, IOException,
			NetException, RemoteCallException, RSException,
			RecordStoreFullException, DBException {
		final Database database = Model.getDatabase();
		database.start();
		return ViewFactory.getMenu("mainMenu");
	}

	/**
	 * Entry point for all events.
	 * 
	 * @param event
	 *            event
	 * @throws SerializationException
	 *             on errors
	 * @throws NetException
	 *             on errors
	 * @throws RecordStoreFullException
	 *             on errors
	 * @throws DBException
	 *             on errors
	 * @throws RSException
	 *             on errors
	 * @throws RemoteCallException
	 *             on errors
	 * @throws Exception
	 *             on errors
	 * @noinspection DuplicateThrows
	 */
	protected void continueProcess(final ControllerEvent event)
			throws SerializationException, NetException,
			RecordStoreFullException, DBException, RSException,
			RemoteCallException, Exception {
		switch (event.getSource().getId()) {
		case VIEW_ID_MAIN:
			processMainView(event);
			break;

		case VIEW_ID_BROWSE_CATEGORIES:
			processBrowseCategories(event);
			break;

		case VIEW_ID_EDIT_CATEGORY:
			processEditCategory(event);
			break;

		case VIEW_ID_SEARCH_ENTRIES:
			processSearchEntries(event);
			break;

		case VIEW_ID_BROWSE_ENTRIES:
			processBrowseEntries(event);
			break;

		case VIEW_ID_EDIT_ENTRY:
			processEditEntry(event);
			break;

		case VIEW_ID_TOOLS:
			processToolsView(event);
			break;

		case VIEW_ID_LANGUAGE:
			processLanguageView(event);
			break;

		case VIEW_ID_FONTSIZE:
			processFontSizeView(event);
			break;

		case VIEW_ID_PREFERENCES:
			processPreferencesView(event);
			break;

		case VIEW_ID_DIAGNOSIS:
			processDiagnosisView(event);
			break;
		}

	}

	private void processEditEntry(final ControllerEvent event)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ControllerEvent.ACCEPT:
			saveEntry(event);
			break;
		}
	}

	private void saveEntry(final ControllerEvent event) throws DBException,
			SerializationException, RecordStoreFullException, RSException {
		final DataFormView editEntry = (DataFormView) event.getSource();
		final String entryTitle = editEntry.getTextField("title").getString();
		final String entryPassword = editEntry.getTextField("password")
				.getString();
		final String entryPassword2 = editEntry.getTextField("repeat_password")
				.getString();

		if (entryTitle == null || entryTitle.trim().length() == 0) {
			Application.getManager().showError(
					ResourceManager
							.getResource("entries.error.TitleCantBeEmpty"));
			editEntry.setFocusAt("title");
			editEntry.show();
		} else if (entryPassword == null || entryPassword.trim().length() == 0) {
			Application.getManager().showError(
					ResourceManager
							.getResource("entries.error.PasswordCantBeEmpty"));
			editEntry.setFocusAt("password");
			editEntry.show();
		} else if (entryPassword2 == null
				|| entryPassword2.trim().length() == 0) {
			Application
					.getManager()
					.showError(
							ResourceManager
									.getResource("entries.error.RepeatPasswordCantBeEmpty"));
			editEntry.setFocusAt("password2");
			editEntry.show();
		} else if (!entryPassword.equals(entryPassword2)) {
			Application.getManager().showError(
					ResourceManager
							.getResource("entries.error.PasswordsDontMatch"));
			editEntry.setFocusAt("password2");
			editEntry.show();
		} else if (editEntry.getRow() != null) {
			Model.saveEntry(editEntry.getRow(), editEntry.getTextField(
					"password").getString());
			goBack(editEntry);
		} else {
			final ListBrowser browseCategories = ViewFactory
					.getListBrowser("browseCategories");
			final Row selected = browseCategories.getSelected();
			long categoryId = selected != null ? selected.getId()
					: ((Long) browseCategories.getAttachment("parentId"))
							.longValue();
			Model.createEntry(entryTitle, entryPassword, categoryId, editEntry
					.getTextField("user_name").getString(), editEntry
					.getTextField("url").getString(), editEntry.getTextField(
					"comment").getString(), editEntry.getDateField("expires")
					.getDate());
			if (selected != null) {
				// It comes from the browserEntries, so reload it and go back
				reloadEntries(event);
				goBack(editEntry);
			} else {
				// It comes from an empty categoriesBrowser, so fetch the
				// entries
				// (the one just added) and show them as if comming from the
				// categoryBrowser (the caller of the editEntry form)
				final RowSet entries = Model.findEntriesByCategory(categoryId);
				final ListBrowser entriesBrowser = ViewFactory
						.getListBrowser("browseEntries");
				entriesBrowser.setScrollSet(entries);
				editEntry.getCaller().go(entriesBrowser);
			}
		}
	}

	private void processBrowseEntries(final ControllerEvent event)
			throws DBException, RecordStoreFullException, RSException,
			SerializationException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ACTION_ENTB_DEL_ENT:
			deleteKey(event);
			break;

		case ACTION_ENTB_NEW_ENT:
			createKeyFromBrowser(event);
			break;

		default:
			editKey(event);
			break;
		}
	}

	private void deleteKey(final ControllerEvent event) throws DBException,
			RecordStoreFullException, RSException, SerializationException {
		final ListBrowser browseEntries = (ListBrowser) event.getSource();
		if (browseEntries.getSelected() != null) {
			final boolean confirm = Application.getManager().confirm(
					ResourceManager.getResource("entries.remove.Title"),
					ResourceManager.getResource("entries.remove.message"));
			if (confirm) {
				browseEntries.getSelected().remove();
				reloadEntries(event);
			} else {
				event.getSource().show();
			}
		} else {
			Application.getManager().showWarning(
					ResourceManager.getResource("entries.MustSelectEntry"));
			event.getSource().show();
		}
	}

	private void reloadEntries(final ControllerEvent event) throws DBException,
			RecordStoreFullException, RSException, SerializationException {
		final DataFormView searchEntries = ViewFactory
				.getDataForm("searchEntries");
		if (comesFrom(event.getSource(), searchEntries)) {
			final String searchText = searchEntries.getTextField("search")
					.getString();
			final RowSet entries = Model.findEntries(searchText);
			if (entries.size() == 0) {
				Application.getManager().showWarning(
						ResourceManager.getResource("global.search.NoResults"));
				goBack(event.getSource());
			} else {
				final ListBrowser browseEntries = ViewFactory
						.getListBrowser("browseEntries");
				browseEntries.setScrollSet(entries);
				browseEntries.show();
			}
		} else {
			final ListBrowser browseCategories = ViewFactory
					.getListBrowser("browseCategories");
			final Row selected = browseCategories.getSelected();
			final RowSet entries = Model
					.findEntriesByCategory(selected.getId());
			if (entries.size() > 0) {
				// There are child entries (passwords) so show them
				final ListBrowser entriesBrowser = ViewFactory
						.getListBrowser("browseEntries");
				entriesBrowser.setScrollSet(entries);
				entriesBrowser.show();
			} else {
				goBack(event.getSource());
			}
		}
	}

	private void editKey(final ControllerEvent event) throws DBException,
			RecordStoreFullException, RSException {
		final ListBrowser browseEntries = (ListBrowser) event.getSource();
		if (browseEntries.getSelected() != null) {
			final DataFormView form = ViewFactory.getDataForm("editEntry");
			form.bind(browseEntries.getSelected());
			final String unencryptedPassword = Model
					.decryptPassword(browseEntries.getSelected().getBlob(
							"password"));
			form.getTextField("password").setString(unencryptedPassword);
			form.getTextField("repeat_password").setString(unencryptedPassword);
			event.getSource().go(form);
		} else {
			Application.getManager().showWarning(
					ResourceManager.getResource("entries.MustSelectEntry"));
			event.getSource().show();
		}
	}

	private void createKeyFromBrowser(final ControllerEvent event) {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		if (comesFrom(event.getSource(), browseCategories)) {
			final Long parentId = (Long) browseCategories
					.getAttachment("parentId");
			final DataFormView form = ViewFactory.getDataForm("editEntry");
			form.bind((Row) null);
			form.getTextField("title").setString("");
			form.getTextField("password").setString("");
			form.getTextField("repeat_password").setString("");
			form.getTextField("user_name").setString("");
			form.getTextField("url").setString("");
			form.getTextField("comment").setString("");
			form.attach("parentId", parentId == null
					|| parentId.longValue() == 0 ? null : parentId);
			event.getSource().go(form);
		} else {
			Application
					.getManager()
					.showWarning(
							ResourceManager
									.getResource("entries.CanCreateOnlyFromCategories"));
			event.getSource().show();
		}
	}

	private void processSearchEntries(final ControllerEvent event)
			throws DBException, RecordStoreFullException, RSException,
			SerializationException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ControllerEvent.ACCEPT:
			final DataFormView searchEntries = (DataFormView) event.getSource();
			final String searchText = searchEntries.getTextField("search")
					.getString();
			if (searchText == null || searchText.trim().length() == 0) {
				Application.getManager().showWarning(
						ResourceManager
								.getResource("global.search.CantBeEmpty"));
				event.getSource().show();
			} else {
				final RowSet entries = Model.findEntries(searchText);
				if (entries.size() == 0) {
					Application.getManager().showWarning(
							ResourceManager
									.getResource("global.search.NoResults"));
					event.getSource().show();
				} else {
					final ListBrowser browseEntries = ViewFactory
							.getListBrowser("browseEntries");
					browseEntries.setScrollSet(entries);
					searchEntries.go(browseEntries);
				}
			}
			break;
		}
	}

	private void processEditCategory(final ControllerEvent event)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ControllerEvent.ACCEPT:
			saveCategory(event);
			break;
		}
	}

	private void saveCategory(final ControllerEvent event) throws DBException,
			SerializationException, RecordStoreFullException, RSException {
		final DataFormView editCategory = (DataFormView) event.getSource();
		final Row row = editCategory.getRow();
		if (row != null) {
			row.setField(Model.FLD_CAT_NAME, editCategory.getTextField("name")
					.getString());
			row.setField(Model.FLD_CAT_DESCRIPTION, editCategory.getTextField(
					"description").getString());
			row.save();
		} else {
			Model.createCategory((Long) editCategory.getAttachment("parentId"),
					editCategory.getTextField("name").getString(), editCategory
							.getTextField("description").getString());
		}
		reloadCategories((ListBrowser) editCategory.getCaller());
		goBack(editCategory);
	}

	private void processBrowseCategories(final ControllerEvent event)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			categoriesBack(event);
			break;

		case ACTION_CATB_NEW_CAT:
			createCategory(event);
			break;

		case ACTION_CATB_DEL_CAT:
			removeCategory(event);
			break;

		case ACTION_CATB_EDIT_CAT:
			editCategory(event);
			break;

		case ACTION_CATB_NEW_KEY:
			createKey(event);
			break;

		default:
			categoriesForward(event);
		}
	}

	private void editCategory(final ControllerEvent event) throws DBException,
			RecordStoreFullException, RSException {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		final Row selected = browseCategories.getSelected();
		if (selected != null) {
			final DataFormView form = ViewFactory.getDataForm("editCategory");
			form.bind(selected);
			event.getSource().go(form);
		} else {
			Application.getManager().showWarning(
					ResourceManager.getResource("warn.SelectACategory"));
			event.getSource().show();
		}

	}

	private void createKey(final ControllerEvent event) {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		if (browseCategories.getScrollSet().size() > 0) {
			Application.getManager().showWarning("categories.NotALeaf");
			event.getSource().show();
		} else {
			final Long parentId = (Long) browseCategories
					.getAttachment("parentId");
			if (parentId == null || parentId.longValue() == 0) {
				Application.getManager().showWarning(
						"categories.CanCreateAtRoot");
				event.getSource().show();
			} else {
				final DataFormView form = ViewFactory.getDataForm("editEntry");
				form.bind((Row) null);
				form.getTextField("title").setString("");
				form.getTextField("password").setString("");
				form.getTextField("repeat_password").setString("");
				form.getTextField("user_name").setString("");
				form.getTextField("url").setString("");
				form.getTextField("comment").setString("");
				form.attach("parentId", parentId);
				event.getSource().go(form);
			}
		}
	}

	private void removeCategory(final ControllerEvent event)
			throws DBException, RecordStoreFullException, RSException,
			SerializationException {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		final Row selected = browseCategories.getSelected();
		if (selected != null) {
			final boolean confirm = Application.getManager().confirm(
					ResourceManager.getResource("categories.remove.Title"),
					ResourceManager.getResource("categories.remove.message"));
			if (confirm) {
				Model.removeCategoryTree(selected);
				reloadCategories(browseCategories);
			} else {
				event.getSource().show();
			}
		} else {
			Application.getManager().showWarning(
					ResourceManager.getResource("warn.SelectACategory"));
			event.getSource().show();
		}
	}

	private void reloadCategories(final ListBrowser browseCategories)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		// Refresh the list
		final Long parentId = (Long) browseCategories.getAttachment("parentId");
		final RowSet children = Model.getChildCategories(parentId);
		browseCategories.setScrollSet(children);
		browseCategories.show();
	}

	private void createCategory(final ControllerEvent event) {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		final Long parentId = (Long) browseCategories.getAttachment("parentId");
		final DataFormView form = ViewFactory.getDataForm("editCategory");
		form.bind((Row) null);
		form.getTextField("name").setString("");
		form.getTextField("description").setString("");
		form
				.attach("parentId", parentId == null
						|| parentId.longValue() == 0 ? null : parentId);
		event.getSource().go(form);
	}

	private void categoriesForward(final ControllerEvent event)
			throws SerializationException, RecordStoreFullException,
			RSException, DBException {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		final Row selected = browseCategories.getSelected();
		if (selected != null) {
			final Long parentId = new Long(selected.getId());
			final RowSet children = Model.getChildCategories(parentId);
			if (children.size() > 0) {
				browseCategories.setScrollSet(children);
				browseCategories.attach("parentId", parentId);
				browseCategories.show();
			} else {
				final RowSet entries = Model.findEntriesByCategory(selected
						.getId());
				if (entries.size() > 0) {
					// There are child entries (passwords) so show them
					final ListBrowser entriesBrowser = ViewFactory
							.getListBrowser("browseEntries");
					entriesBrowser.setScrollSet(entries);
					event.getSource().go(entriesBrowser);
				} else {
					// Show an empty category browser
					browseCategories.setScrollSet(children);
					browseCategories.attach("parentId", parentId);
					browseCategories.show();
				}
			}
		} else {
			event.getSource().show();
		}
	}

	private void categoriesBack(final ControllerEvent event)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		final ListBrowser browseCategories = ViewFactory
				.getListBrowser("browseCategories");
		final Long parentId = (Long) browseCategories.getAttachment("parentId");
		if (parentId == null || parentId.longValue() == 0) {
			goBack(event.getSource());
		} else {
			final Row previous = Model.findCategoryById(parentId);
			browseCategories.setScrollSet(Model
					.getChildCategories((Long) previous
							.getField(Model.FLD_CAT_PARENT_ID)));
			browseCategories.attach("parentId", previous
					.getField(Model.FLD_CAT_PARENT_ID));
			browseCategories.show();
		}
	}

	private void processDiagnosisView(final ControllerEvent event)
			throws SerializationException, InvalidRecordIDException,
			RecordStoreFullException, DBException, RSException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case DIAGMENU_INFO:
			showSystemInfo(event);
			break;

		case DIAGMENU_CHEK_MEM:
			try {
				checkMemory(event);
			} catch (RecordStoreException e) {
				Application.getManager().showError(e);
			}
			break;

		case DIAGMENU_DEVICE_INFO:
			showDeviceInfo(event);
			break;

		case DIAGMENU_FORCE_DEVICE_INFO:
			Store.safeDeleteRecordStore("sys_devinfo");
			DeviceInfo.setDeviceInfo(null);
			showDeviceInfo(event);
			break;

		case DIAGMENU_VIEW_LOG:
			try {
				final String logTxt = ErrorLog.getErrors();
				HelpView v = HelpView.getInstance();
				v.setText(logTxt);
				event.getSource().go(v);
			} catch (J2MEException e) {
				Application.getManager().showError(e);
			}
			break;

		case ControllerEvent.HOME:
			ViewFactory.getMenu("main").show();
			break;
		}
	}

	private void showDeviceInfo(final ControllerEvent event) {
		final StringBuffer buffer = new StringBuffer();

		final DeviceInfo deviceInfo = DeviceInfo.getDeviceInfo();
		buffer.append(ResourceManager.getResource("deviceInfo.speed")).append(
				": ").append(deviceInfo.getmHz()).append("MHz").append('\n')
				.append(ResourceManager.getResource("deviceInfo.performance"))
				.append(": ").append(deviceInfo.getComputationPerformance())
				.append('\n').append(
						ResourceManager.getResource("deviceInfo.memSpeed"))
				.append(": ").append(deviceInfo.getMemoryReadSpeed()).append(
						"/").append(deviceInfo.getMemoryWriteSpeed()).append(
						"/").append(deviceInfo.getMemoryCopySpeed()).append(
						'\n').append(
						ResourceManager.getResource("deviceInfo.rsSizeLimit"))
				.append(": ");

		final long rsSizeLimit = deviceInfo.getRsSizeLimit();
		if (rsSizeLimit == DeviceInfo.RS_LIMIT_UNKOWN) {
			buffer.append(ResourceManager
					.getResource("deviceInfo.rslimit.unknown"));
		} else if (rsSizeLimit == DeviceInfo.RS_LIMIT_NONE) {
			buffer.append(ResourceManager
					.getResource("deviceInfo.rslimit.none"));
		} else {
			buffer.append((rsSizeLimit / 1024)).append("Kb");
		}
		buffer.append('\n').append(
				ResourceManager.getResource("deviceInfo.allocHandling"))
				.append(": ");
		switch (deviceInfo.getAllocHandling()) {
		case DeviceInfo.AH_UNKNOWN:
			buffer.append(ResourceManager.getResource("deviceInfo.ah.unknown"));
			break;

		case DeviceInfo.AH_OK:
			buffer.append(ResourceManager.getResource("deviceInfo.ah.ok"));
			break;

		case DeviceInfo.AH_FREES_ON_RECORD_DELETE:
			buffer.append(ResourceManager
					.getResource("deviceInfo.ah.freesOnRecord"));
			break;

		case DeviceInfo.AH_FREES_ON_RS_DELETE:
			buffer.append(ResourceManager
					.getResource("deviceInfo.ah.freesOnRS"));
			break;

		case DeviceInfo.AH_BOGUS:
			buffer.append(ResourceManager.getResource("deviceInfo.ah.bogus"));
			break;
		}
		buffer.append('\n').append(
				ResourceManager.getResource("deviceInfo.hasAllocBug")).append(
				": ").append(
				deviceInfo.hasAllocBug() ? ResourceManager
						.getResource("global.Yes") : ResourceManager
						.getResource("global.No")).append('\n').append(
				ResourceManager.getResource("deviceInfo.growingHeap")).append(
				": ").append(
				deviceInfo.isGrowingHeap() ? ResourceManager
						.getResource("global.Yes") : ResourceManager
						.getResource("global.No")).append('\n').append(
				ResourceManager.getResource("deviceInfo.bluetooth")).append(
				": ").append(
				Util.hasBluetoothAPI() ? ResourceManager
						.getResource("global.Yes") : ResourceManager
						.getResource("global.No")).append('\n').append(
				ResourceManager.getResource("deviceInfo.fileApi")).append(": ")
				.append(
						Util.hasFileAPI() ? ResourceManager
								.getResource("global.Yes") : ResourceManager
								.getResource("global.No")).append('\n').append(
						ResourceManager.getResource("deviceInfo.location"))
				.append(": ").append(
						Util.hasLocationAPI() ? ResourceManager
								.getResource("global.Yes") : ResourceManager
								.getResource("global.No"));

		buffer.append('\n').append("Error de tipo de letra en listas: ")
				.append(deviceInfo.hasListFontBug() ? "si" : "no");

		final HelpView view = HelpView.getInstance();
		view.setTitle(ResourceManager.getResource("diagnostics.deviceInfo"));
		view.setText(buffer.toString());
		view.setCaller(event.getSource());
		view.setFont(Util.SMALL_FONT);
		view.show();
	}

	private void checkMemory(final ControllerEvent event)
			throws RecordStoreException {
		final String report = Util.buildMemoryReport();
		final HelpView view = HelpView.getInstance();
		view.setTitle(ResourceManager.getResource("diagnostics.memoryReport"));
		view.setText(report);
		view.setCaller(event.getSource());
		view.show();
	}

	private void showSystemInfo(final ControllerEvent event)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		final ProgressEvent pe = new ProgressEvent();
		pe.setAnimate(true);
		pe.setTitle(ResourceManager.getResource("sysinfo.Title"));
		pe.dispatch();

		final StringBuffer buffer = new StringBuffer();
		final long totalMemory = Runtime.getRuntime().totalMemory() / KB;
		final long freeMemory = Runtime.getRuntime().freeMemory() / KB;
		final Database db = Model.getDatabase();
		final long dbTotalSize = db.getTotalSize();
		// final int freeDisk = Settings.getFreeDisk();
		buffer.append(ResourceManager.getResource("sysinfo.totalMemory"))
				.append(": ").append(totalMemory).append(" Kb").append("\n")
				.append(ResourceManager.getResource("sysinfo.freeMemory"))
				.append(": ").append(freeMemory).append(" Kb").append("\n")
				.append(ResourceManager.getResource("sysinfo.databaseSize"))
				.append(": ").append(sizeIn(dbTotalSize, KB)).append(" Kb (")
				.append(sizeIn(dbTotalSize, MB)).append(" Mb)");
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.freeDisk" ) )
		// .append( ": " )
		// .append( sizeIn( freeDisk, KB ) )
		// .append( " Kb (" )
		// .append( sizeIn( freeDisk, MB ) )
		// .append( " Mb)" );

		// FixedPoint hitRatio = new FixedPoint( SimpleCache.getWideHits() );
		// hitRatio = hitRatio.setPrecision( 2 );
		// final long totalCacheMoves = SimpleCache.getWideHits() +
		// SimpleCache.getWideMisses();
		// if( totalCacheMoves > 0 )
		// {
		// hitRatio = hitRatio.mult( 100 );
		// hitRatio = hitRatio.div( totalCacheMoves );
		// }

		// FixedPoint rsHitRatio = new FixedPoint( Store.getHits() );
		// rsHitRatio = rsHitRatio.setPrecision( 2 );
		// final long totalRsCacheMoves = Store.getHits() +
		// Store.getMisses();
		// if( totalRsCacheMoves > 0 )
		// {
		// rsHitRatio = rsHitRatio.mult( 100 );
		// rsHitRatio = rsHitRatio.div( totalRsCacheMoves );
		// }
		//
		// final RecordStoreStats stats = Store.getWideStats();
		// buffer.append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.avgDeleteTime" ) )
		// .append( ": " )
		// .append( stats.getAverageDeleteTime() )
		// .append( " ms." )
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.avgFindTime" ) )
		// .append( ": " )
		// .append( Index.getAverageWideFindTime() )
		// .append( " ms." )
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.avgReadTime" ) )
		// .append( ": " )
		// .append( stats.getAverageReadTime() )
		// .append( " ms." )
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.avgUpdateTime" ) )
		// .append( ": " )
		// .append( stats.getAverageUpdateTime() )
		// .append( " ms." )
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.avgWriteTime" ) )
		// .append( ": " )
		// .append( stats.getAverageAddTime() )
		// .append( " ms." )
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.hitRatio" ) )
		// .append( ": " )
		// .append( SimpleCache.getWideHits() )
		// .append( "/" )
		// .append( totalCacheMoves )
		// .append( " (" )
		// .append( hitRatio.toString() )
		// .append( "%)")
		// .append( "\n" )
		// .append( ResourceManager.getResource( "sysinfo.rsCache" ) )
		// .append( ": " )
		// .append( Store.getHits() )
		// .append( "/" )
		// .append( totalRsCacheMoves )
		// .append( " (" )
		// .append( rsHitRatio.toString() )
		// .append( "%)");

		final HelpView helpView = HelpView.getInstance();
		helpView.setText(buffer.toString());
		helpView.setTitle(ResourceManager.getResource("sysinfo.Title"));
		helpView.setCaller(event.getSource());
		helpView.show();
	}

	private String sizeIn(final long size, final int divisor) {
		FixedPoint f = new FixedPoint(size);
		f = f.setPrecision(5);
		f = f.div(divisor);
		f = f.setPrecision(1);
		return f.toString();
	}

	private void processPreferencesView(final ControllerEvent event)
			throws SerializationException, DBException, NetException,
			IOException, RemoteCallException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ControllerEvent.HOME:
			ViewFactory.getMenu("mainMenu").show();
			event.getSource().release();
			break;
		}
	}

	private void processFontSizeView(final ControllerEvent event) {
		switch (event.getCode()) {
		case ACTION_FTS_SMALL:
			setFont(event, Font.SIZE_SMALL);
			break;

		case ACTION_FTS_MEDIUM:
			setFont(event, Font.SIZE_MEDIUM);
			break;

		case ACTION_FTS_LARGE:
			setFont(event, Font.SIZE_LARGE);
			break;

		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case ControllerEvent.HOME:
			Application.getManager().getMainView().show();
			break;
		}
	}

	private void setFont(final ControllerEvent event, final int size) {
		final ProgressEvent e = new ProgressEvent(this);
		e.setAnimate(true);
		final String changing = ResourceManager
				.getResource("fontSize.changing");
		e.setPhase(changing);
		e.setTitle(changing);
		e.setMessage("");
		e.dispatch();
		Ui.setFontSize(size);
		event.getSource().show();
	}

	// <ToolsView> -------------------------------------------------------------
	private void processToolsView(final ControllerEvent event)
			throws SerializationException, NetException, RemoteCallException,
			DBException, RSException, RecordStoreFullException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		case TOOLSMENU_DELETE_DATA:
			tools_clearData();
			break;
		}
	}

	private void tools_clearData() {
		final boolean doIt = Application.getManager().confirm(
				ResourceManager.getResource("tools.eraseData.Title"),
				ResourceManager.getResource("tools.eraseData.Message"));
		if (doIt) {
			try {
				Model.getDatabase().drop();
				Application.getManager().showInfo(
						ResourceManager.getResource("tools.eraseData.Done"));
			} catch (Exception e) {
				log.error(e);
				Application.getManager().showError(e);
			}
			exitApplication();
		}
	}

	/**
	 * Utility method to check if a view has another one in the path, that is,
	 * if the user comes from that view.
	 * 
	 * @param source
	 *            current view
	 * @param view
	 *            target view
	 * @return true if so
	 */
	private boolean comesFrom(final View source, final View view) {
		final View caller = source.getCaller();
		return caller != null
				&& (caller.getId() == view.getId() || comesFrom(caller, view));
	}

	// <MainView> --------------------------------------------------------------
	private void processMainView(final ControllerEvent event)
			throws DBException, NetException, RecordStoreFullException,
			SerializationException, RemoteCallException, FSException,
			IOException, RSException {
		AppState.clearState();
		switch (event.getCode()) {
		case MAINM_CATEGORIES:
			browseCategories(event);
			break;

		case MAINM_SEARCH_ENTRIES:
			event.getSource().go(ViewFactory.getDataForm("searchEntries"));
			break;

		case ControllerEvent.EXIT:
			mildExitApplication();
			break;

		case ControllerEvent.HELP:
			mildExitApplication();
			break;

		case MAINM_ABOUT:
			showAboutInfo(event);
			break;
		}
	}

	private void browseCategories(final ControllerEvent event)
			throws DBException, RecordStoreFullException, RSException,
			SerializationException {
		// Find the root categories
		final RowSet set = Model.getRootCategories();
		// Fetch the browser view
		final ListBrowser browser = ViewFactory
				.getListBrowser("browseCategories");
		// Feed the browser view
		browser.setScrollSet(set);
		// And show it
		event.getSource().go(browser);
	}

	private void showAboutInfo(final ControllerEvent event) {
		final StringBuffer buffer = new StringBuffer();
		buffer.append("BaseMovil ").append(Model.APPLICATION_NAME).append(" v")
				.append(
						Application.getManager().getProperty(
								"basemovil.version"));
		buffer.append('\n').append(
				ResourceManager.getResource("about.licensedTo")).append(":\n")
				.append(
						LoginManager.getCompanyName() != null ? LoginManager
								.getCompanyName() : "").append('\n').append(
						LoginManager.getUserFullName()).append('\n');
		buffer.append("id: ").append(LoginManager.getUserCustomId());
		buffer.append('\n').append(ResourceManager.getResource("about.text"));

		final HelpView view = HelpView.getInstance();
		view.setCaller(event.getSource());
		view.setText(buffer.toString());
		view.setTitle(ResourceManager.getResource("about.Title"));
		view.show();
	}

	// </MainView> -------------------------------------------------------------

	// <LanguageView> ----------------------------------------------------------
	private void processLanguageView(final ControllerEvent event)
			throws DBException {
		switch (event.getCode()) {
		case ControllerEvent.BACK:
			goBack(event.getSource());
			break;

		default:
			setLanguage(event);
			break;
		}
	}

	private void setLanguage(final ControllerEvent event) throws DBException {
		final ProgressEvent pe = new ProgressEvent(event.getSource(),
				new Integer(Gauge.INDEFINITE), Gauge.CONTINUOUS_RUNNING);
		pe
				.setTitle(ResourceManager
						.getResource("languageView.LoadingLanguage"));
		pe.dispatch();
		final String locale;
		switch (event.getCode()) {
		case LANG_ES:
			locale = "es";
			break;

		default:
			locale = "en";
		}
		if (!locale.equals(ResourceManager.getLocale())) {
			ResourceManager.setActiveLocale(locale);
			Settings.setProperty(Model.CONFIG_ACTIVE_LOCALE, locale);
			Settings.setProperty(CoreConstants.PROP_RESOURCE_LOCALE, locale);
			views.clear();
			ViewFactory.clear();
			System.gc();
		}
		final MenuView mainView = ViewFactory.getMenu("mainMenu");
		Application.getManager().setMainView(mainView);
		mainView.show();
	}

	// </LanguageView> ---------------------------------------------------------

	/**
	 * Configure default actions and update custom view cache.
	 * 
	 * @param key
	 *            view key
	 * @param view
	 *            view
	 * @param cache
	 *            add it to the cache
	 */
	private static void addView(final String key, final View view,
			final boolean cache) {
		final int viewId = view.getId();
		if (exitPoints.contains(new Integer(viewId))) {
			view.addCommand(Application.getCommandFactory().exitCommand());
		} else if (!noBack.contains(new Integer(viewId))) {
			view.addCommand(Application.getCommandFactory().backCommand());
		}

		if (view instanceof HelpAssisted
				&& ((HelpAssisted) view).getHelpId() != null) {
			view.addCommand(Application.getCommandFactory().helpCommand());
		}

		if (cache) {
			views.put(key, view);
		}
	}
}
