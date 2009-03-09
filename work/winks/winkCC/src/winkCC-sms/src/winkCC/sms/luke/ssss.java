package winkCC.sms.luke;

import java.io.IOException;
import java.io.InterruptedIOException;

import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.Connector;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.Message;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;
import javax.wireless.messaging.MessagePart;
import javax.wireless.messaging.MultipartMessage;
import javax.wireless.messaging.SizeExceededException;
import javax.wireless.messaging.TextMessage;

public abstract class ssss {
	public static String SEGMENTATIONERROR = "segments can't be zero";

//	public abstract void composeBinaryMessage() throws TooMuchDataException;

	/**
	 * Create a new MessageConnection
	 * 
	 * @param connUrl
	 *            is the server or client URL for the connection
	 * @param messageListener
	 *            is the message listener for this connection
	 * @return a MessageConnection object
	 * @throws a
	 *             ConnectionNotFoundException if the Connection target is not
	 *             found, or the protocol not supported
	 * @throws an
	 *             IOException if the Port number or application identifier is
	 *             already reserved, or the connection has been closed, or if an
	 *             attempt is made to register a listener on a client connection
	 * @throws an
	 *             IllegalArgumentException if a Message type parameter is
	 *             invalid or attempting to create a *Stream
	 * @throws SecurityException
	 *             if permission to open the MessageConnection was not granted,
	 *             or, when setting the message listener, it was determined that
	 *             the application does not have permission to receive on the
	 *             given port number
	 */
	final public MessageConnection newMessageConnection(String connUrl,
			MessageListener messageListener)
			throws ConnectionNotFoundException, IOException,
			IllegalArgumentException, SecurityException {
		MessageConnection mc = null;
		mc = (MessageConnection) Connector.open(connUrl);
		mc.setMessageListener(messageListener);
		return mc;
	}

	/**
	 * Closes the specified message connection
	 * 
	 * @param connection
	 *            is the connection to close
	 */
	static final public void closeConnection(MessageConnection connection) {
		try {
			if (connection != null) {
				// deregister the msg listener
				connection.setMessageListener(null);
				connection.close();
				connection = null;
			}
		} catch (IOException ioe) {
			// Handle or ignore the exception...
		}
	}

	/**
	 * Sends a Message on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            the message to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendMessage(final MessageConnection mc,
			final Message msg, final String url)
			throws IllegalArgumentException, SecurityException {
		Thread th = new Thread() {
			public void run() {
				if (url != null)
					msg.setAddress(url);
				int segcount = mc.numberOfSegments(msg);
				if (segcount == 0) {
					//										alertUser(UiConstants.TXT_SEGCOUNT);
				} else {
					try {
						mc.send(msg);
					} catch (InterruptedIOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				// Handle the Exception:

				// IOException if the message could not be sent
				// because there was a network failure or
				// if the connection was closed

				// IllegalArgumentException if the message was
				// incomplete or contained invalid information
				// This exception is also thrown if the payload
				// of the message exceeds the maximum length
				// for the given messaging protocol

				// InterruptedIOException if a timeout occurs
				// while trying to send the message or this
				// Connection object was closed during this
				// send operation

				// NullPointerException if the parameter is null

				// SecurityException if the application does not
				// have permission to send the message

			}
		};
		th.start();
	}

	/**
	 * Sends a TextMessage on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            the TextMessage to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendTextMessage(MessageConnection mc, TextMessage msg,
			String url) {
		sendMessage(mc, msg, url);
	}

	/**
	 * Sends a TextMessage on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            the message to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendTextMessage(MessageConnection mc, String msg,
			String url) {
		TextMessage tmsg = null;
		tmsg = (TextMessage) mc.newMessage(MessageConnection.TEXT_MESSAGE);
		tmsg.setPayloadText(msg);
		sendTextMessage(mc, tmsg, url);
	}

	/**
	 * Sends a BinaryMessage on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            the Binary Message to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendBinaryMessage(MessageConnection mc,
			BinaryMessage msg, String url) {
		sendMessage(mc, msg, url);
	}

	/**
	 * Sends a BinaryMessage on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            the byte[] message to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendBinaryMessage(MessageConnection mc, byte[] msg,
			String url) {
		BinaryMessage bmsg;
		bmsg = (BinaryMessage) mc.newMessage(MessageConnection.BINARY_MESSAGE);
		bmsg.setPayloadData(msg);
		sendMessage(mc, bmsg, url);
	}

	/**
	 * Sends a multi-part message on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msg
	 *            is the multiplart message to send
	 * @param url
	 *            the destination address, typically used in server mode
	 */
	final public void sendMultipartMessage(MessageConnection mc,
			MultipartMessage msg, String subject, String url) {
		sendMessage(mc, msg, url);
	}

	/**
	 * Sends a multi-part message on the specified connection
	 * 
	 * @param mc
	 *            the MessageConnection
	 * @param msgParts
	 *            the array of message parts to send
	 * @param startContentID
	 *            is the ID of the start multimedia content part (SMIL)
	 * @param to
	 *            is the message's TO list
	 * @param cc
	 *            is the message's CC list
	 * @param bcc
	 *            is the message's BCC list
	 * @param subject
	 *            is the message's subject
	 * @param priority
	 *            is the message's priority
	 * @param url
	 *            is the destination URL, typically used in server mode
	 * @throws SizeExceededException
	 */
	final public void sendMultipartMessage(MessageConnection mc,
			MessagePart[] msgParts, String startContentID, String[] to,
			String[] cc, String[] bcc, String subject, String priority,
			String url) throws SizeExceededException {
		int i = 0;
		MultipartMessage multipartMessage;
		multipartMessage = (MultipartMessage) mc
				.newMessage(MessageConnection.MULTIPART_MESSAGE);
		if (to != null) {
			for (i = 0; i < to.length; i++) {
				multipartMessage.addAddress("to", to[i]);
			}
		}
		if (cc != null) {
			for (i = 0; i < cc.length; i++) {
				multipartMessage.addAddress("cc", cc[i]);
			}
		}
		if (bcc != null) {
			for (i = 0; i < bcc.length; i++) {
				multipartMessage.addAddress("bcc", bcc[i]);
			}
		}
		multipartMessage.setSubject(subject);
		if ((priority.equals("high")) || (priority.equals("normal"))
				|| (priority.equals("low"))) {
			multipartMessage.setHeader("X-Mms-Priority", priority);
		}
		for (i = 0; i < msgParts.length; i++) {
			multipartMessage.addMessagePart(msgParts[i]);
		}
		multipartMessage.setStartContentId(startContentID);
		sendMessage(mc, multipartMessage, url);
	}

}
