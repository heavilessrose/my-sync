package com.sun.j2me.blueprints.smartticket.shared.midp;

/**
 * 
 */
public class ApplicationException extends Exception {

	private static IndexedResourceBundle bundle;
	private int code = -1;
	private Exception exception = null;

	public static void setResourceBundle(IndexedResourceBundle bundle) {
		ApplicationException.bundle = bundle;
	}

	public ApplicationException() {
	}

	public ApplicationException(String msg) {
		super(msg);

		return;
	}

	public ApplicationException(int code) {
		this.code = code;

		return;
	}

	public ApplicationException(Exception exception) {
		this.exception = exception;

		return;
	}

	public int getCode() {
		return code;
	}

	public Exception getException() {
		return exception;
	}

	public String getMessage() {
		if (bundle != null) {
			return bundle.getString(code);
		}

		if (exception != null) {
			return exception.getMessage();
		}

		return super.getMessage();
	}

}
