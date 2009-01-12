package net.sf.mvc.mobile;

public abstract class LazyInit {
	private Page instance;
	public abstract Page create();
	public Page get() {
		return instance == null ? instance = create() : instance;
	}
}
