package bm.mvc;

import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;

import bm.core.mvc.AbstractView;
import bm.core.mvc.AbstractController;
import bm.core.mvc.ControllerEvent;
import bm.core.Application;
import bm.core.ResourceManager;

/*
 * File Information
 *
 * Created on       : 24-jul-2006 13:52:19
 * Created by       : javier
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Prompt View.
 * 
 * @author <a href="mailto:javier.chacon@elondra.com">Javier Chacon</a>
 * @version $Revision$
 */
public class PromptView extends AbstractView {
	private String oldText;
	private TextField textField;

	Boolean cancelled;

	public PromptView(final String title, final int maxSize,
			final int constraints) {
		super(null);
		setController(new PromptController());

		textField = new TextField(null, null, maxSize, constraints);

		Form form = new Form(ResourceManager.getResource(title));
		form.append(textField);

		form.addCommand(Application.getCommandFactory().okCommand());
		form.addCommand(Application.getCommandFactory().cancelCommand());
		form.setCommandListener(this);

		displayable = form;
	}

	public void setText(final String text) {
		oldText = text;
		textField.setString(text);
	}

	public String getText() {
		return textField.getString();
	}

	public String getOldText() {
		return oldText;
	}

	public boolean isCancelled() {
		return cancelled != null && cancelled.booleanValue();
	}

	public class PromptController extends AbstractController {
		protected void process(final ControllerEvent event) {
			cancelled = new Boolean(event.getCode() == ControllerEvent.CANCEL);
		}
	}

	private class Waiter extends Thread {
		public void run() {
			while (cancelled == null) {
				try {
					sleep(500);
				} catch (InterruptedException e) {
				}
			}
		}
	}

	public String prompt() {
		final Waiter waiter = new Waiter();
		final Display display = Application.getManager().getDisplay();
		final Displayable current = display.getCurrent();
		display.setCurrent(displayable);
		waiter.start();
		try {
			waiter.join();
		} catch (InterruptedException e) {
		}
		if (current != null) {
			display.setCurrent(current);
		}
		return cancelled.booleanValue() ? null : getText();
	}
}
