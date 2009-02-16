package bm.mvc;

import bm.core.mvc.AbstractView;
import bm.core.mvc.Controller;
import bm.core.ResourceManager;
import bm.core.Application;

import javax.microedition.lcdui.TextBox;

/*
 * File Information
 *
 * Created on       : 09-mar-2007 16:57:00
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A simple textbox screen for editing text.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class TextBoxView extends AbstractView {
	private int viewId;

	public TextBoxView(final int viewId, final Controller controller,
			final String titleResource, final int maxSize, final int constraints) {
		super(controller);
		this.viewId = viewId;
		final TextBox txtNote = new TextBox(ResourceManager
				.getResource(titleResource), "", maxSize, constraints);
		txtNote.addCommand(Application.getCommandFactory().okCommand());
		txtNote.setCommandListener(this);
		displayable = txtNote;
	}

	public int getId() {
		return viewId;
	}

	public String getText() {
		return ((TextBox) displayable).getString();
	}

	public void setText(final String text) {
		((TextBox) displayable).setString(text != null ? text : "");
	}

	public void release() {
		super.release();
		((TextBox) displayable).setString("");
	}
}
