package bm.net.bind;

import bm.net.NetException;
import bm.core.log.Log;
import bm.core.log.LogFactory;

import java.io.*;
import java.util.Vector;
import java.util.Enumeration;

/*
 * File Information
 *
 * Created on       : 25-nov-2006 18:32:33
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * A Binding represents a plugable interface to an HttpConnection, so it can be
 * performed directly or through other means, such as a Bluetooth connection.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public abstract class Binding {
	private static final Log log = LogFactory.getLog("Binding");

	/**
	 * @noinspection InstantiatingObjectToGetClassObject
	 * 
	 *               This is not a good way to get the class, but CLDC 1.0 will
	 *               fail if we use HttpBinding.class, and if we use
	 *               Class.forName() the link will be weak for the obfuscator
	 *               and we'll had to deal with the exceptions.
	 */
	private static Class factory = new HttpBinding().getClass();

	/**
	 * Keep track of all opened bindings, so we can close them safely if the
	 * caller does not.
	 */
	private static Vector allBindings = new Vector(10);

	protected int responseCode;
	protected String responseMessage;
	protected String contentType;
	protected int contentLength;

	public static synchronized void setFactory(final Class factory) {
		Binding.factory = factory;
	}

	/**
	 * Create a new binding.
	 * 
	 * @return new binding, null if the factory is invalid.
	 */
	public static Binding create() {
		try {
			final Binding binding = (Binding) factory.newInstance();
			allBindings.addElement(binding);
			return binding;
		} catch (InstantiationException e) {
			return null;
		} catch (IllegalAccessException e) {
			return null;
		}
	}

	/**
	 * Close all opened bindings.
	 */
	public static synchronized void closeAll() {
		log.debug("closeAll: " + allBindings.size() + " pending");
		for (final Enumeration i = allBindings.elements(); i.hasMoreElements();) {
			final Binding binding = (Binding) i.nextElement();
			binding.close();
		}
		allBindings.removeAllElements();
	}

	/**
	 * Open underlying connection.
	 * 
	 * @param url
	 *            url to post to
	 * @throws NetException
	 *             on errors
	 */
	public abstract void open(final String url) throws NetException;

	/**
	 * Close underlying connection.<br/>
	 * Subclasses must keep always the call super.close(), as it frees some
	 * common resources that wont otherwise.
	 */
	public void close() {
		allBindings.removeElement(this);
	}

	/**
	 * Begin the post operation, open the output stream to send content to the
	 * server.
	 * 
	 * @return OutputStream to send content.
	 * @throws NetException
	 *             on errors
	 */
	public abstract DataOutputStream beginPost() throws NetException;

	/**
	 * End the post operation, and get server response.
	 * 
	 * @return server response.
	 * @throws NetException
	 *             on errors
	 */
	public abstract DataInputStream endPost() throws NetException;

	/**
	 * Server response code.
	 * 
	 * @return response code
	 */
	public int getResponseCode() {
		return responseCode;
	}

	/**
	 * Server response message.
	 * 
	 * @return response message
	 */
	public String getResponseMessage() {
		return responseMessage;
	}

	/**
	 * Server returned content type.
	 * 
	 * @return content type
	 */
	public String getContentType() {
		return contentType;
	}

	/**
	 * Server returned content length.
	 * 
	 * @return content length
	 */
	public int getContentLength() {
		return contentLength;
	}

	/**
	 * Simply make a call to an http url and get the response as a string.
	 * 
	 * @param url
	 *            url
	 * @return page text
	 * @throws bm.net.NetException
	 *             on errors
	 */
	public abstract String get(String url) throws NetException;
}
