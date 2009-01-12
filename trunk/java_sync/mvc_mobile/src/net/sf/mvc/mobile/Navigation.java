package net.sf.mvc.mobile;

public class Navigation {

	private final String name;
	private Object parameter;

	public Navigation(String name, Object parameter) {
		this.name = name;
		this.parameter = parameter;
	}

	public Navigation(String name, Object param1, Object param2) {
		this(name, new Object[] { param1, param2 });
	}

	public Navigation(String name, Object param1, Object param2, Object param3) {
		this(name, new Object[] { param1, param2, param3 });
	}
	
	public String getName() {
		return name;
	}

	public Object getParameter() {
		return parameter;
	}

	public void setParameter(Object parameter) {
		this.parameter = parameter;
	}
}
