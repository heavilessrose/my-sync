package winkCC.core.midp;

import javax.microedition.lcdui.Command;

public interface CommandFactory {

	Command backCommand();

	Command helpCommand();

	Command exitCommand();

	Command cancelCommand();

	Command okCommand();

	Command stopCommand();

	Command homeCommand();

	Command getCommand(final int id);
}
