package org.kalmeo.demo.kuix.model;

import org.kalmeo.kuix.core.model.DataProvider;

/**
 * @author omarino
 */
public class Calculator extends DataProvider {

	private final static String VALUE1_PROPERTY = "vVal1";
	private final static String VALUE2_PROPERTY = "vVal2";
	private final static String OPERATOR_PROPERTY = "cOp";
	private final static String RESULT_PROPERTY = "vResult";

	private short value1 = 0;
	private short value2 = 0;
	private char operator = ' ';
	private int result = 0;

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * org.kalmeo.kuix.core.model.DataProvider#getUserDefinedValue(java.lang
	 * .String)
	 */
	protected Object getUserDefinedValue(String property) {
		if (VALUE1_PROPERTY.equals(property)) {
			return Integer.toString(value1);
		}
		if (VALUE2_PROPERTY.equals(property)) {
			return Integer.toString(value2);
		}
		if (OPERATOR_PROPERTY.equals(property)) {
			return String.valueOf(operator);
		}
		if (RESULT_PROPERTY.equals(property)) {
			return Long.toString(result);
		}
		return null;
	}

	/**
	 * Calculates the result
	 */
	public void calculates() {
		if (operator == '+') {
			result = value1 + value2;
		} else if (operator == '-') {
			result = value1 - value2;
		}

		dispatchUpdateEvent(RESULT_PROPERTY);
	}

	/**
	 * Set the value1
	 * 
	 * @param value
	 *            the value to set
	 */
	public void setValue1(short value) {
		value1 = value;
		dispatchUpdateEvent(VALUE1_PROPERTY);
	}

	/**
	 * Set the value2
	 * 
	 * @param value
	 *            the value to set
	 */
	public void setValue2(short value) {
		value2 = value;
		dispatchUpdateEvent(VALUE2_PROPERTY);
	}

	/**
	 * Set op to + or -
	 * 
	 * @param op
	 *            the op to set
	 */
	public void setOperator(char op) {
		this.operator = op;
		dispatchUpdateEvent(OPERATOR_PROPERTY);
	}
}
