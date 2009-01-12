package net.sf.mvc.mobile;

import javax.microedition.lcdui.Displayable;

public class Page {

	private final Model model;
	private final Displayable view;

	public Page(Model model, Displayable display) {
		this.model = model;
		this.view = display;
		model.setView(view);
	}

	public Displayable getView() {
		return view;
	}

	public Model getModel() {
		return model;
	}
}
