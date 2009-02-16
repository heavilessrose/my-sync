package bm.net.rpc;

import bm.core.ResourceManager;
import bm.core.ControlledTask;
import bm.core.event.ProgressEvent;
import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.tools.Tools;
import bm.net.ChecksumedOutputStream;
import bm.net.Net;
import bm.net.NetException;
import bm.net.bind.Binding;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/*
 * File Information
 *
 * Created on       : 25-nov-2006 17:07:18
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-11-29 10:04:31 +0000 (jue, 29 nov 2007) $
 * Revision         : $Revision: 17 $
 */

/**
 * Remote Procedure Call implementation.<br/>
 * A call is made upon a remote object name, to a given method, and supports a
 * number of parameters with some limitations.<br/>
 * Parameters can be of one of these types: Serializable, String, Long, Byte,
 * Boolean, Integer, Short.<br/>
 * They can also be collections of objects of the above types, to any nesting
 * level. Collections can be simple arrays, Vector or Hashtable.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 17 $
 */
public class RemoteCall implements Serializable {
	private static Log log = LogFactory.getLog("RemoteCall");

	private static final byte VERSION = 2;

	private static String defaultURL;

	private String object;
	private String method;
	private Object[] parameters;
	private boolean encrypted;

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.net.rpc.RemoteCall";
	}

	/**
	 * Get the default url for remote calls.
	 * 
	 * @return default url
	 */
	public static String getDefaultURL() {
		return defaultURL;
	}

	/**
	 * Set the default url for remote calls.
	 * 
	 * @param defaultURL
	 *            default URL
	 */
	public static synchronized void setDefaultURL(final String defaultURL) {
		RemoteCall.defaultURL = defaultURL;
	}

	// ToDo: support encryption and clear the comment below

	/**
	 * Create a new remote call object.<br/>
	 * Encryption is not yet supported.
	 * 
	 * @param object
	 *            target object
	 * @param method
	 *            method to invoke
	 * @param paramCount
	 *            parameter count
	 * @param encrypted
	 *            perform encryption
	 */
	public RemoteCall(final String object, final String method,
			final int paramCount, final boolean encrypted) {
		this.object = object;
		this.method = method;
		if (paramCount > 0) {
			parameters = new Object[paramCount];
		} else {
			parameters = null;
		}
		this.encrypted = encrypted;
	}

	/**
	 * Create a new remote call object.<br/>
	 * By default no encryption is performed.
	 * 
	 * @param object
	 *            target object
	 * @param method
	 *            method to invoke
	 * @param paramCount
	 *            parameter count
	 */
	public RemoteCall(final String object, final String method,
			final int paramCount) {
		this.object = object;
		this.method = method;
		if (paramCount > 0) {
			parameters = new Object[paramCount];
		} else {
			parameters = null;
		}
		encrypted = false;
	}

	/**
	 * Get object name.
	 * 
	 * @return object name
	 */
	public String getObject() {
		return object;
	}

	/**
	 * Get method name.
	 * 
	 * @return method name
	 */
	public String getMethod() {
		return method;
	}

	/**
	 * If the call is encrypted.
	 * 
	 * @return true if so
	 */
	public boolean isEncrypted() {
		return encrypted;
	}

	/**
	 * Get the number of parameters.
	 * 
	 * @return parameter count
	 */
	public int getParameterCount() {
		return parameters != null ? parameters.length : 0;
	}

	/**
	 * Set parameter value.
	 * 
	 * @param position
	 *            parameter position.
	 * @param value
	 *            parameter value.
	 */
	public void setParameter(final int position, final Object value) {
		if (parameters != null && position >= 0 && position < parameters.length) {
			parameters[position] = value;
		}
	}

	/**
	 * Get parameter value.
	 * 
	 * @param position
	 *            parameter position.
	 * @return parameter value.
	 */
	public Object getParameter(final int position) {
		if (parameters != null && position >= 0 && position < parameters.length) {
			return parameters[position];
		} else {
			return null;
		}
	}

	/**
	 * Write object status to stream.
	 * 
	 * @param out
	 *            output stream
	 * @throws SerializationException
	 *             on errors
	 */
	public void serialize(final SerializerOutputStream out)
			throws SerializationException {
		out.writeString(object);
		out.writeString(method);
		out.writeNullableObject(parameters);
	}

	/**
	 * Read object status from stream.
	 * 
	 * @param in
	 *            input stream
	 * @throws SerializationException
	 *             on errors
	 */
	public void deserialize(final SerializerInputStream in)
			throws SerializationException {
		object = in.readString();
		method = in.readString();
		parameters = (Object[]) in.readNullableObject();
	}

	/**
	 * Invoke the call on the default url.
	 * 
	 * @return return value of method, null if none
	 * @throws NetException
	 *             network error
	 * @throws RemoteCallException
	 *             error invoking method
	 */
	public Object invoke() throws RemoteCallException, NetException {
		// #ifdef debug_mode
		if (defaultURL == null) {
			throw new RemoteCallException(Net.ERR_RC_NOT_CONFIGURED,
					"defaultURL not configured");
		}
		// #endif
		return invoke(defaultURL);
	}

	/**
	 * Invoke the call on the given remote address.
	 * 
	 * @param url
	 *            RPC server url.
	 * @return return value of method, null if none
	 * @throws NetException
	 *             network error
	 * @throws RemoteCallException
	 *             error invoking method
	 */
	public Object invoke(final String url) throws RemoteCallException,
			NetException {
		log.debug("invoke: " + object + "." + method + "("
				+ Tools.arrayAsVector(parameters) + ")");
		Object retval = null;
		int retries = 0;
		Exception lastException = null;
		boolean done = false;
		while (!done && retries < Net.MAX_RETRIES
				&& !ControlledTask.isCancelled()) {
			try {
				lastException = null;
				retval = doInvoke(url);
				log.debug("return value: " + retval);
				done = true;
			} catch (NetException e) {
				retries++;
				lastException = e;
				log.error(e);
				try {
					Thread.sleep(Net.WAIT_TIME);
				} catch (InterruptedException e1) {
				}
			} catch (SerializationException e) {
				retries++;
				lastException = e;
				log.error(e);
				try {
					Thread.sleep(Net.WAIT_TIME);
				} catch (InterruptedException e1) {
				}
			} catch (IOException e) {
				retries++;
				lastException = e;
				log.error(e);
				try {
					Thread.sleep(Net.WAIT_TIME);
				} catch (InterruptedException e1) {
				}
			}
		}
		if (lastException != null) {
			throw new NetException(Net.ERR_RC_INVOKATION, lastException);
		} else {
			return retval;
		}
	}

	/*
	 * Remote Procedure Call protocol:
	 * 
	 * Device request:
	 * 
	 * (data: {byte version} // RPC version, currently 2 {byte encryption} //
	 * Encryption: 0 - none, 1 - Blowfish {String deviceGuid} // Device Globally
	 * Unique Identifier {long deviceId} // Device remote id {String object} //
	 * Name of object to call {String method} // Object method to call {byte[]
	 * parameters} // Parameters serialized using core Tools.writeNullableObject
	 * ) {String "END_OF_DATA"} // Constant text to identify data boundary {long
	 * CRC} // Adler32 CRC code of the data
	 * 
	 * Server response:
	 * 
	 * {int responseCode} // Result of the method call {String responseMessage}
	 * // Result message {byte[] responseData} // An object serialized using
	 * Tools.writeNullableObject // In version 1 the object was serialized with
	 * Tools.writeObject // and null was not a valid return value
	 * 
	 * responseCode can be one of:
	 * 
	 * 0 - RC_SUCCESS Everything went fine 1 - RC_INVALID_OBJECT The requested
	 * object is invalid 2 - RC_INVALID_METHOD The object has no method with the
	 * requested name 3 - RC_NOT_ALLOWED The caller is not allowed to execute
	 * the requested method 4 - RC_INVALID_ARGUMENTS The arguments passed are
	 * invalid 5 - RC_INVOKATION_EXCEPTION The method produced an exception when
	 * invoked 6 - RC_INVALID_CRC The CRC of the data sent is invalid
	 */
	private Object doInvoke(final String url) throws NetException,
			SerializationException, IOException, RemoteCallException {
		final Binding binding = Binding.create();
		final ProgressEvent pe = new ProgressEvent();
		pe.setAnimate(true);
		pe.setSource(this);
		try {
			pe
					.setMessage(ResourceManager
							.getResource("bm.net.rpc.SendingCall"));
			pe.dispatch();
			binding.open(url);
			final DataOutputStream dos = binding.beginPost();
			final ChecksumedOutputStream cos = new ChecksumedOutputStream(dos);
			final DataOutputStream dos2 = new DataOutputStream(cos);
			final SerializerOutputStream out = new SerializerOutputStream(dos2);
			out.writeByte(VERSION);
			out.writeByte((byte) (encrypted ? 1 : 0));
			out.writeString(Net.getDeviceGuid() != null ? Net.getDeviceGuid()
					: "");
			out.writeLong(Net.getDeviceId() != null ? Net.getDeviceId()
					.longValue() : 0);
			serialize(out);
			final long crc = cos.getCRC();
			out.writeString("END_OF_DATA");
			out.writeLong(crc);
			pe.setMessage(ResourceManager
					.getResource("bm.net.rpc.WaitingResponse"));
			pe.dispatch();
			log.debug("Call serialized");
			final DataInputStream is = binding.endPost();
			final SerializerInputStream in = new SerializerInputStream(is);
			pe.setMessage(ResourceManager
					.getResource("bm.net.rpc.ReadingResponse"));
			pe.dispatch();
			final int responseCode = in.readInt();
			final String responseMessage = in.readString();
			if (responseCode == Net.RC_SUCCESS) {
				return in.readNullableObject();
			} else if (responseCode != Net.RC_INVALID_CRC) {
				log.error("Unrecoverable error: " + responseCode + " "
						+ responseMessage);
				throw new RemoteCallException(Net.ERR_RC_UNRECOVERABLE_ERROR,
						responseMessage);
			} else {
				log.error("Invalid CRC");
				throw new IOException("Invalid CRC"); // Perform retry
			}
		} finally {
			binding.close();
		}
	}
}
