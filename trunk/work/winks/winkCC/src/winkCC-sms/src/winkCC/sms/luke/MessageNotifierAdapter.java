/**
 * 
 */
package winkCC.sms.luke;

import java.io.IOException;
import java.io.InputStream;
import java.util.Date;

import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.ImageItem;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.StringItem;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.Message;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessagePart;
import javax.wireless.messaging.MultipartMessage;
import javax.wireless.messaging.TextMessage;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * @author WangYinghua
 * 
 */
public abstract class MessageNotifierAdapter implements MessageNotifier {
	ILog log = LogFactory.getLog("MessageNotifierAdapter");

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * winkCC.sms.MessageNotifier#notifyIncomingMessage(javax.wireless.messaging
	 * .MessageConnection)
	 */
	public void notifyIncomingMessage(MessageConnection messageConnection) {
		readMessageThreaded(messageConnection);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * winkCC.sms.MessageNotifier#processBinaryMessage(javax.wireless.messaging
	 * .BinaryMessage)
	 */
	public Item[] processBinaryMessage(BinaryMessage bmsg) {
		Item[] items;
		byte[] data = bmsg.getPayloadData();
		String hex = toHex(data);
		items = new Item[1];
		Item item = new StringItem(null, hex);
		items[0] = item;
		return items;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * winkCC.sms.MessageNotifier#processMessage(javax.wireless.messaging.Message
	 * )
	 */
	public Item[] processMessage(Message message) {
		Item[] items = null;

		// Process the received message appropriately to its type
		if (message instanceof TextMessage) {
			TextMessage tmsg = (TextMessage) message;
			items = processTextMessage(tmsg);
		} else if (message instanceof BinaryMessage) {
			BinaryMessage bmsg = (BinaryMessage) message;
			items = processBinaryMessage(bmsg);
		} else if (message instanceof MultipartMessage) {
			MultipartMessage mpmsg = (MultipartMessage) message;
			items = processMultipartMessage(mpmsg);
		} else {
			// Ignore
			System.out.println("Unrecognized message type...");
			return null;
		}
		return items;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * winkCC.sms.MessageNotifier#processTextMessage(javax.wireless.messaging
	 * .TextMessage)
	 */
	public Item[] processTextMessage(TextMessage tmsg) {
		Item[] items;
		String msg = tmsg.getPayloadText();
		items = new Item[1];
		Item item = new StringItem(null, msg);
		log.info(((StringItem) item).getText());
		items[0] = item;
		return items;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @seewinkCC.sms.MessageNotifier#readMessage(javax.wireless.messaging.
	 * MessageConnection)
	 */
	public void readMessage(MessageConnection messageConnection) {
		try {
			Message message = null;
			message = messageConnection.receive();
			if (message != null) {
				processMessage(message);
			}
		} catch (IOException ioe) {
			System.out.println("readMessage exception: " + ioe);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * winkCC.sms.MessageNotifier#readMessageThreaded(javax.wireless.messaging
	 * .MessageConnection)
	 */
	public void readMessageThreaded(final MessageConnection messageConnection) {
		Thread th = new Thread() {
			public void run() {
				readMessage(messageConnection);
			}
		};
		th.start();
	}

	/**
	 * toHex converts a byte array to human-readable hexadecimal This method was
	 * borrowed from examples in the Sun Wireless Toolkit :-)
	 * 
	 * @param data
	 *            is the array of bytes to convert
	 * @return a String containing the byte[] in hexadecimal format
	 */
	final private String toHex(byte[] data) {
		StringBuffer buf = new StringBuffer();
		for (int i = 0; i < data.length; i++) {
			int intData = (int) data[i] & 0xFF;
			if (intData < 0x10)
				buf.append("0");
			// display 2 digits per byte i.e. 09
			buf.append(Integer.toHexString(intData));
			buf.append(' ');
		}
		return (buf.toString());
	}

	//////////// 
	// MIME Types
	public final static String MIMETYPE_TEXT_PLAIN = "text/plain";
	public final static String MIMETYPE_TEXT_XML = "text/xml";
	public final static String MIMETYPE_TEXT_HTML = "text/html";
	public final static String MIMETYPE_IMAGE_PNG = "image/png";
	public final static String MIMETYPE_IMAGE_JPEG = "image/jpg";
	public final static String MIMETYPE_IMAGE_GIF = "image/gif";
	public final static String MIMETYPE_APPLICATION_OCTET_STREAM = "application/octet-stream";

	/**
	 * Process MultipartMessage
	 * 
	 * @param mpmsg
	 *            is the MultipartMessageto process
	 * @return array of javax.microedition.lcdui.Item
	 */
	public Item[] processMultipartMessage(MultipartMessage mpmsg) {

		// Get the Multipart message headers from the access methods:
		//  X-Mms-From, X-Mms-To, X-Mms-CC, X-Mms-BCC, X-Mms-Subject
		String from = mpmsg.getAddress();
		String tos[] = mpmsg.getAddresses("to");
		String ccs[] = mpmsg.getAddresses("cc");
		String bccs[] = mpmsg.getAddresses("bcc");
		String froms[] = mpmsg.getAddresses("from");
		String subject = mpmsg.getSubject();

		// Get the rest of the headers by calling the getHeader() method
		String mmsPriorityHeader = mpmsg.getHeader("X-Mms-Priority");
		String mmsDeliveryTimeHeader = mpmsg.getHeader("X-Mms-Delivery-Time");

		// Get the message timestamp
		Date timestamp = mpmsg.getTimestamp();

		// Get the start-content ID, which will be set for multimedia
		// content, and will contain the presentation information
		// (not used in this method)
		String startContentID = mpmsg.getStartContentId();

		// Get to the message parts array
		MessagePart[] messageParts = mpmsg.getMessageParts();
		int itemCount = messageParts.length + 2;
		// +2 is to account for from & subject
		Item[] messageItems = new Item[messageParts.length];

		// Create the UI elements for the message's Date and Subject
		messageItems[0] = new StringItem("From", from);
		messageItems[1] = new StringItem("Subject", subject);

		// process each message part
		for (int i = 0; i < messageParts.length; i++) {
			MessagePart messagePart = messageParts[i];
			if (messagePart != null) {
				String mimeType = messagePart.getMIMEType();
				int length = messagePart.getLength();
				byte[] content = messagePart.getContent();
				String encoding = messagePart.getEncoding();

				// Message content can be retrieved as an InputStream
				InputStream contentAsInputStream = messagePart
						.getContentAsStream();

				// Used for multimedia messages, such as SMIL messages
				String contentID = messagePart.getContentID();
				String contentLocation = messagePart.getContentLocation();

				try {
					// text/plain
					if (mimeType.equals(MIMETYPE_TEXT_PLAIN)) {
						String text = new String(content);
						messageItems[i] = new StringItem(null, text);
					}
					// text/xml
					else if (mimeType.equals(MIMETYPE_TEXT_XML)) {
						messageItems[i] = new StringItem(null,
								"text/xml is not supported");
					}
					// text/html
					else if (mimeType.equals(MIMETYPE_TEXT_HTML)) {
						messageItems[i] = new StringItem(null,
								"text/html is not supported");
					}
					// application/octet-stream
					else if (mimeType.equals(MIMETYPE_APPLICATION_OCTET_STREAM)) {
						String hex = toHex(content);
						messageItems[i] = new StringItem(null, hex);
					}
					// image/png
					else if (mimeType.equals(MIMETYPE_IMAGE_PNG)) {
						Image img = Image.createImage(content, 0, length);
						messageItems[i] = new ImageItem(null, img, 0, "Image");
					}
					// image/jpg
					else if (mimeType.equals(MIMETYPE_IMAGE_JPEG)) {
						messageItems[i] = new StringItem(null,
								"image/jpeg is not supported");
					}
					// image/gif
					else if (mimeType.equals(MIMETYPE_IMAGE_GIF)) {
						messageItems[i] = new StringItem(null,
								"image/gif is not supported");
					}
				} catch (Exception e) {
					System.out.println("processMultipartMessage, Exception: "
							+ e);
				}
			}
		}
		return messageItems;
	}

}
