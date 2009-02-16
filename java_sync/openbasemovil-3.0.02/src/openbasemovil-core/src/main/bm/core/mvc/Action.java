package bm.core.mvc;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Item;

/*
 * File Information
 *
 * Created on       : 11-mar-2007 2:20:36
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * An action is a command with an action code and a name, that make it easy to
 * handle and allows automatic controller event dispatching.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class Action {
	private String name;
	private Command command;
	private int code;
	private Item item;
	private boolean defaultAction;

	public Action() {
	}

	public Action(final String name, final Command command, final int code) {
		this.name = name;
		this.command = command;
		this.code = code;
	}

	public Action(final String name, final Command command, final int code,
			final Item item) {
		this.name = name;
		this.command = command;
		this.code = code;
		this.item = item;
	}

	public Action(final String name, final Command command, final int code,
			final Item item, final boolean defaultAction) {
		this.name = name;
		this.command = command;
		this.code = code;
		this.item = item;
		this.defaultAction = defaultAction;
	}

	public String getName() {
		return name;
	}

	public void setName(final String name) {
		this.name = name;
	}

	public Command getCommand() {
		return command;
	}

	public void setCommand(final Command command) {
		this.command = command;
	}

	public int getCode() {
		return code;
	}

	public void setCode(final int code) {
		this.code = code;
	}

	public Item getItem() {
		return item;
	}

	public void setItem(final Item item) {
		this.item = item;
	}

	public boolean isDefaultAction() {
		return defaultAction;
	}

	public void setDefaultAction(final boolean defaultAction) {
		this.defaultAction = defaultAction;
	}

	public boolean equals(final Object o) {
		if (this == o) {
			return true;
		}
		if (o == null || getClass() != o.getClass()) {
			return false;
		}

		final Action action = (Action) o;

		if (code != action.code) {
			return false;
		}
		if (!name.equals(action.name)) {
			return false;
		}

		return true;
	}

	public int hashCode() {
		int result;
		result = name.hashCode();
		result = 31 * result + code;
		return result;
	}
}
