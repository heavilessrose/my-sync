/*
 * Created on 30-abr-2005
 */
package passkeep;

import bm.core.Application;
import bm.core.ControlledTask;
import bm.core.ErrorHandler;
import bm.core.ResourceManager;
import bm.core.event.Event;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.db.DBException;
import bm.db.Settings;
import bm.midp.MidpApplication;
import bm.mvc.ViewFactory;
import bm.storage.Store;
import bm.ui.ProgressView;
import bm.util.IndexResourceProvider;
import bm.util.Util;
import passkeep.control.MainController;

import javax.microedition.lcdui.Image;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * @author narciso
 */
public class PasswordKeeper extends MidpApplication {
	private static Log log = LogFactory.getLog("PasswordKeeper");

	public PasswordKeeper() {
		super(240);
	}

	/**
	 * Register resource bundles during application startup.<br/>
	 * It registers a single resource bundle called &quot;language&quot; which
	 * should include all the resources needed for the application.<br/>
	 * You should pack all the resources in that bundle and not override this
	 * method.<br/>
	 * It uses the bundled resource method of delivering languages and views.
	 */
	protected void registerResourceBoundles() {
		// Create the index based resource provider
		final IndexResourceProvider provider = new IndexResourceProvider(
				"rix_language", 20, new String[] { "es", "en" });
		boolean rebuild;
		try {
			// Check if application is just installed or version has changed
			// in that case we should copy again the bundled resources to the
			// appropriate stores
			final String version = Settings.getProperty("resource.appVersion");
			rebuild = version == null
					|| !version.equals(getProperty("MIDlet-Version"));
		} catch (DBException e) {
			rebuild = true;
		}
		if (rebuild) {
			// If we need to rebuild the indexes, copy them from the bundled
			// resources, and set the new application version afterwards
			try {
				Util.copyIndex("rix_language", "/language.index");
				Util.copyIndex(ViewFactory.COMPILED_VIEWS, "/views.index");
				Settings.setProperty("resource.appVersion",
						getProperty("MIDlet-Version"));
			} catch (Exception e) {
				log.error(e);
				ErrorHandler.handleError(this, e);
			}
		}
		// Configure the ResourceManager, and pre-open it to enable faster
		// searches of resources
		ResourceManager.addProvider(provider);
		ResourceManager.open();
		// Initialize the ViewFactory
		ViewFactory.init(30, "/views.index");
	}

	/**
	 * Show the splash screen.
	 * 
	 * @param bigSplashSize
	 *            horizontal resolution that triggers the use of the big splash
	 *            image
	 */
	protected void showSplash(final int bigSplashSize) {
		// The basic splash screen defined at MidpApplication is just a form
		// with an image item
		// We will use a much more elegant and user friendly one
		// This might be well included in the MidpApplication and thus this
		// method will be removed from here
		try {
			String splash;
			splash = defaultProperties.getProperty("splash.image");
			final ProgressView form = new ProgressView();
			if (splash != null) {
				form.setFullScreenMode(true);
				AppState.setScreenWidth(form.getWidth());
				AppState.setScreenHeight(form.getHeight());
				final Image image = Image.createImage(getClass()
						.getResourceAsStream(splash));
				form.setImage(image);
				form.setMeterPlacement(ProgressView.BOTTOM);
			}
			display.setCurrent(form);
			Event.unregisterAll(Event.PROGRESS);
			Event.register(form, Event.PROGRESS);
		} catch (Exception e) {
			Event.unregisterAll(Event.PROGRESS);
			Event.register(new ProgressView(), Event.PROGRESS);
		}
	}

	protected void startApp() throws MIDletStateChangeException {
		log.info("startApp()");
		try {
			// If the last time the application exited it set the install.url
			// setting, it means that we should install a new version of the
			// midlet and exit again
			final String installURL = Settings.getProperty("install.url");
			if (installURL != null) {
				Settings.setProperty("install.url", null);
				platformRequest(installURL);
				destroyApp(true);
				notifyDestroyed();
			} else {
				((MainController) ViewFactory.getController("main"))
						.startApplication();
			}
		} catch (Exception e) {
			log.error(e);
			throw new MIDletStateChangeException(e.getMessage());
		}
	}

	protected void pauseApp() {
	}

	protected void destroyApp(final boolean mandatory)
			throws MIDletStateChangeException {
		if (!mandatory) {
			final boolean confirmExit = Application.getManager().confirm(
					ResourceManager.getResource("global.Exit"),
					ResourceManager.getResource("main.AskExitApplication"));
			if (!confirmExit) {
				throw new MIDletStateChangeException();
			}
		}
		log.debug("called destroyApp: " + mandatory);
		// Close EVERYTHING before exiting
		ControlledTask.cancelAll();
		Application.getTimer().cancel();
		Event.unregisterAll(Event.PROGRESS);
		Event.register(new ProgressView(), Event.PROGRESS);
		ResourceManager.close();
		ViewFactory.shutdown();
		Store.smartShutdown();
	}
}
