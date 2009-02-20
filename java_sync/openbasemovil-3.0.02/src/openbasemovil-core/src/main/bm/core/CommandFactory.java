package bm.core;

import javax.microedition.lcdui.Command;

/*
 * File Information
 *
 * Created on       : 12-dic-2005 18:00:35
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Handles creation and/or storage for Commands.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface CommandFactory {
	/**
	 * Get the command for Command.BACK / ControllerEvent.BACK.
	 * 
	 * @return back command
	 */
	Command backCommand();

	/**
	 * Get the command for Command.HELP / ControllerEvent.HELP.
	 * 
	 * @return help command
	 */
	Command helpCommand();

	/**
	 * Get the command for Command.EXIT / ControllerEvent.EXIT.
	 * 
	 * @return exit command
	 */
	Command exitCommand();

	/**
	 * Get the command for Command.CANCEL / ControllerEvent.CANCEL.
	 * 
	 * @return cancel command
	 */
	Command cancelCommand();

	/**
	 * Get the command for Command.OK / ControllerEvent.ACCEPT.
	 * 
	 * @return ok command
	 */
	Command okCommand();

	/**
	 * Get the command for Command.STOP / ControllerEvent.STOP.
	 * 
	 * @return stop command
	 */
	Command stopCommand();

	/**
	 * Get the command for ControllerEvent.HOME.
	 * 
	 * @return home command
	 */
	Command homeCommand();

	/**
	 * Get a command for the given id.
	 * 
	 * @param id
	 *            command id
	 * @return command or null if not found
	 */
	Command getCommand(final int id);
}
