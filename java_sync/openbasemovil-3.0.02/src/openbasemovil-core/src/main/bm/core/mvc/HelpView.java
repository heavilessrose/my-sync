package bm.core.mvc;

import bm.core.Application;
import bm.core.CoreConstants;
import bm.core.HelpAssisted;
import bm.core.ResourceManager;

import javax.microedition.lcdui.*;
import java.io.IOException;

/*
 * File Information
 *
 * Created on       : 24-may-2005 19:00:51
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Common Help View.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class HelpView extends AbstractView implements CommandListener {
	private static HelpView instance = new HelpView();
	private static HelpProvider helpProvider = new ResourceHelpProvider();

	public static HelpProvider getHelpProvider() {
		return helpProvider;
	}

	public static synchronized void setHelpProvider(
			final HelpProvider helpProvider) {
		HelpView.helpProvider = helpProvider;
	}

	private HelpView() {
		super(null);
		final Form form = new Form(ResourceManager
				.getResource("helpView.Title"));
		focusedItem = new StringItem("", "");
		form.append(focusedItem);
		form.addCommand(Application.getCommandFactory().backCommand());
		form.setCommandListener(this);
		this.displayable = form;
	}

	/**
	 * Add a command to the view.
	 * 
	 * @param command
	 *            command to add
	 */
	public void addCommand(final Command command) {
		// Does nothing, no commands can be added to this view
	}

	/**
	 * Each view must have an id that identifies it.
	 * 
	 * @return view id
	 */
	public int getId() {
		return CoreConstants.HELP_VIEW_ID;
	}

	public static HelpView getInstance() {
		return instance;
	}

	public static synchronized void reload() {
		instance = new HelpView();
	}

	public void setText(final String text) {
		((StringItem) focusedItem).setText(ResourceManager.getResource(text));
	}

	/**
	 * Default implementation does nothing.
	 * 
	 * @param command
	 *            command
	 */
	public synchronized void removeCommand(Command command) {
		// Does nothing, no commands can be removed from this view
	}

	public void commandAction(final Command command,
			final Displayable displayable) {
		if (command.getCommandType() == Command.BACK) {
			HelpView.getInstance().getCaller().show();
		}
	}

	/**
	 * The view is about to hide and we shuld free up resources used by it.
	 */
	public void release() {
		((StringItem) focusedItem).setText(null);
	}

	/**
	 * Fetch localized help for a given help assisted item.
	 * 
	 * @param target
	 *            help assisted item
	 * @return help text or null if not found
	 * @throws IOException
	 *             on errors
	 */
	public static String getHelp(final HelpAssisted target) throws IOException {
		return helpProvider.getHelp(target.getHelpId());
	}

	/**
	 * Show help for a given HelpAssisted item.
	 * 
	 * @param target
	 *            help assited item
	 */
	public static void showHelp(final HelpAssisted target) {
		try {
			final String help = getHelp(target);
			if (help != null) {
				final HelpView instance = HelpView.instance;
				instance.setText(help);
				instance
						.setTitle(ResourceManager.getResource("helpView.Title"));
				if (target instanceof View) {
					instance.setCaller((View) target);
				}
				instance.show();
			} else {
				Application
						.getManager()
						.showWarning(
								ResourceManager
										.getResource("helpManager.warn.noHelpAvailable"));
			}
		} catch (IOException e) {
			Application.getManager().showError(
					ResourceManager
							.getResource("helpManager.error.cantReadHelp"), e);
		}
	}

	public void setFont(final Font font) {
		((StringItem) focusedItem).setFont(font);
	}
}
