package bm.core;

import java.util.Timer;

/*
 * File Information
 *
 * Created on       : 12-dic-2005 17:31:05
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Entry point for ApplicationManagers and other Application wide settings and
 * things that can vary from one platform to another.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public final class Application {
	private static ApplicationManager manager;
	private static CommandFactory commandFactory;
	private static Timer timer = new Timer();

	public static ApplicationManager getManager() {
		return manager;
	}

	public static synchronized void setManager(final ApplicationManager manager) {
		Application.manager = manager;
	}

	public static CommandFactory getCommandFactory() {
		return commandFactory;
	}

	public static synchronized void setCommandFactory(
			final CommandFactory commandFactory) {
		Application.commandFactory = commandFactory;
	}

	public static Timer getTimer() {
		return timer;
	}
}
