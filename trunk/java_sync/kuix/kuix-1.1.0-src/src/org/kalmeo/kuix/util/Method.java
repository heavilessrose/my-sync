package org.kalmeo.kuix.util;

/**
 * @author bbeaulant
 */
public class Method {

	private static final Object[] NO_ARGUMENTS = new Object[0];

	// The method's name
	private final String name;

	// A list that contains method's arguments
	private Object[] arguments;

	/**
	 * Construct a {@link Method}
	 * 
	 * @param name
	 */
	public Method(String name) {
		this.name = name;
		this.arguments = NO_ARGUMENTS;
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return the arguments
	 */
	public Object[] getArguments() {
		return arguments;
	}

	/**
	 * @param arguments
	 *            the arguments to set
	 */
	public void setArguments(Object[] arguments) {
		this.arguments = arguments;
	}

}
