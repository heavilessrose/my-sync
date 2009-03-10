/**
 * 短信目的地址可包含:端口
 */
package winkCC.sms;

import java.io.IOException;
import java.io.InputStream;
import java.io.InterruptedIOException;
import java.util.Date;

import javax.microedition.io.Connector;
import javax.microedition.io.PushRegistry;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.ImageItem;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.StringItem;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.Message;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;
import javax.wireless.messaging.MessagePart;
import javax.wireless.messaging.MultipartMessage;
import javax.wireless.messaging.TextMessage;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * 短信模块
 * 
 * @author WangYinghua
 * 
 */
public class SmsMessage {
	private static ILog log = LogFactory.getLog("SmsMessage");
	private static SmsMessage instance = null;

	public static final String SEGCOUNT = "segment can't be zero";

	private SmsMessage() {

	}

	public static SmsMessage getInstance() {
		if (instance == null) {
			instance = new SmsMessage();
		}
		return instance;
	}

	/**
	 * 新建一个MessageConnection
	 * 
	 * @param connUrl
	 *            可以是client或server
	 * @param messageListener
	 *            是否收到短信的监听器
	 * @return
	 */
	final public MessageConnection newMessageConnection(String connUrl,
			MessageListener messageListener)
	/*
	 * throws ConnectionNotFoundException, IOException,
	 * IllegalArgumentException, SecurityException
	 */{
		MessageConnection mc = null;
		try {
			mc = (MessageConnection) Connector.open(connUrl);
		} catch (IOException e) {
			log.debug("open " + connUrl + " error");
			e.printStackTrace();
		}
		try {
			mc.setMessageListener(messageListener);
		} catch (IOException e) {
			log.debug("setMessageListener " + messageListener + " error");
			e.printStackTrace();
		}
		return mc;
	}

	/**
	 * 关闭连接
	 * 
	 * @param connection
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
		}
	}

	///////////////// 发送短信
	final private void sendMessage(final MessageConnection mc,
			final Message msg, final String url)
	/* throws IllegalArgumentException, SecurityException */{
		Thread th = new Thread() {
			public void run() {
				if (url != null)
					msg.setAddress(url);
				int segcount = mc.numberOfSegments(msg);
				if (segcount == 0) {
					// alertUser(UiConstants.TXT_SEGCOUNT);
					log.debug(SEGCOUNT);
				} else {
					try {
						mc.send(msg);
					} catch (InterruptedIOException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		};
		th.start();
	}

	final private void sendTextMessage(MessageConnection mc, TextMessage msg,
			String url) {
		sendMessage(mc, msg, url);
	}

	/**
	 * 发送字符短信
	 * 
	 * @param mc
	 *            messageConnection
	 * @param msg
	 *            短信内容
	 * @param url
	 *            对方号码
	 */
	final public void sendTextMessage(MessageConnection mc, String msg,
			String url) {
		TextMessage tmsg = null;
		tmsg = (TextMessage) mc.newMessage(MessageConnection.TEXT_MESSAGE);
		tmsg.setPayloadText(msg);
		sendTextMessage(mc, tmsg, url);
	}

	final private void sendBinaryMessage(MessageConnection mc,
			BinaryMessage msg, String url) {
		sendMessage(mc, msg, url);
	}

	/**
	 * 发送二进制短信
	 * 
	 * @param mc
	 * @param msg
	 * @param url
	 *            对方号码
	 */
	final public void sendBinaryMessage(MessageConnection mc, byte[] msg,
			String url) {
		BinaryMessage bmsg;
		bmsg = (BinaryMessage) mc.newMessage(MessageConnection.BINARY_MESSAGE);
		bmsg.setPayloadData(msg);
		sendMessage(mc, bmsg, url);
	}

	////////////////////////接收及处理短信
	/**
	 * 启动新线程来读取短信
	 */
	public final void readMessageThreaded(final MessageConnection mc) {
		Thread th = new Thread() {
			public void run() {
				readMessage(mc);
			}
		};
		th.start();
	}

	Item[] receives = null;
	Object mutex = new Object();

	Item[] getReceives() {
		synchronized (mutex) {
			return receives;
		}
	}

	/**
	 * 接收,读取,处理短信
	 * 
	 * @param mc
	 */
	public void readMessage(MessageConnection mc) {
		try {
			Message message = null;
			message = mc.receive();
			if (message != null) {
				synchronized (mutex) {
					receives = processMessage(message);
				}
			}
		} catch (IOException ioe) {
			log.debug("readMessage exception: " + ioe);
		}
	}

	/**
	 * 接收到短信后的处理
	 * 
	 * @param msg
	 * @return
	 */
	private Item[] processMessage(Message msg) {
		Item[] items = null;

		if (msg instanceof TextMessage) {
			TextMessage tmsg = (TextMessage) msg;
			items = processTextMessage(tmsg);
		} else if (msg instanceof BinaryMessage) {
			BinaryMessage bmsg = (BinaryMessage) msg;
			items = processBinaryMessage(bmsg);
		} else if (msg instanceof MultipartMessage) {
			MultipartMessage mpmsg = (MultipartMessage) msg;
			items = processMultipartMessage(mpmsg);
		} else {
			// 忽略
			log.debug("Unrecognized message type...");
			return null;
		}
		return items;
	}

	private Item[] processTextMessage(TextMessage tmsg) {
		Item[] items;
		String msg = tmsg.getPayloadText();
		items = new Item[1];
		Item item = new StringItem(null, msg);
		items[0] = item;
		//		for (int i = 0; i < receives.length; i++) {
		//			form.append(receives[i]);
		//		}
		return items;
	}

	private Item[] processBinaryMessage(BinaryMessage bmsg) {
		Item[] items;
		byte[] data = bmsg.getPayloadData();
		String hex = toHex(data);
		items = new Item[1];
		Item item = new StringItem(null, hex);
		items[0] = item;
		return items;
	}

	final private String toHex(byte[] data) {
		StringBuffer buf = new StringBuffer();
		for (int i = 0; i < data.length; i++) {
			int intData = (int) data[i] & 0xFF;
			if (intData < 0x10)
				buf.append("0");
			buf.append(Integer.toHexString(intData));
			buf.append(' ');
		}
		return (buf.toString());
	}

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

	///////////////push
	/**
	 * 动态注册push启动
	 */
	public void dynamicRegisterSmsMonitor(String url, String midlet,
			String filter) {
		try {
			PushRegistry.registerConnection(url, midlet, filter);
		} catch (ClassNotFoundException e) {
			log.debug("midlet to register is not exist", e);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 注销push
	 * 
	 * @param url
	 */
	public void unregisterSmsMonitor(String url) {
		boolean status = false;
		try {
			// 成功返回true, 否则返回false
			status = PushRegistry.unregisterConnection(url);
		} catch (SecurityException e) {
			log.debug("unregister not success", e);
		}
	}

	/**
	 * Discover whether there are pending push inbound connections and, if so,
	 * process
	 */
	public void processPushConnectionsThreaded() {
		Thread th = new Thread() {
			public void run() {
				String[] connections = PushRegistry.listConnections(true);
				if (connections != null && connections.length > 0) {
					for (int i = 0; i < connections.length; i++) {
						String connUrl = connections[i];
						if ((connUrl.startsWith("sms"))
								|| (connUrl.startsWith("cbs"))
								|| (connUrl.startsWith("mms"))) {
							try {
								MessageConnection mc;
								mc = (MessageConnection) Connector
										.open(connUrl);
								readMessage(mc);
							} catch (IOException ioe) {
								// Ignore
							}
						}
					}
				}
			}
		};
		th.start();
	}

}
