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
import javax.wireless.messaging.MessagePart;
import javax.wireless.messaging.MultipartMessage;
import javax.wireless.messaging.TextMessage;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * 短信模块.<br>
 * 为发送与监听分别提供一个MessageConnection. 从而可以实现发送和监听不必使用相同的端口.
 * 
 * @author WangYinghua
 * 
 */
public class SmsMessage {
	private static ILog log = LogFactory.getLog("SmsMessage");
	private static SmsMessage instance = null;

	/** client mode MessageConnection, 只用来发送短信 */
	private static MessageConnection clientMessageConnection = null;
	/** server mode MessageConnection, 只用来监听收到的短信 */
	private static MessageConnection serverMessageConnection = null;
	/** 短信发送的目的地号码 */
	private String _destNumber = null;
	/** 短信发送的目的端口 */
	private String _destPort = null;
	/** 监听的端口 */
	private String _monitorPort = null;

	public static final String SEGCOUNT = "segment can't be zero";

	private SmsMessage() {

	}

	/**
	 * 取得目的地号码
	 * 
	 * @return _destNumber
	 */
	public String getDestNum() {
		return _destNumber;
	}

	/**
	 * 取得发送端口
	 * 
	 * @return _destPort
	 */
	public String getDestPort() {
		return _destPort;
	}

	/**
	 * 取得监听端口
	 * 
	 * @return _monitorPort
	 */
	public String getMonitorPort() {
		return _monitorPort;
	}

	/**
	 * 此对象为单例.
	 * 
	 * @return SmsMessage单例对象.
	 */
	public static SmsMessage getInstance() {
		if (instance == null) {
			instance = new SmsMessage();
		}
		return instance;
	}

	/**
	 * 创建client mode短信连接的scheme.<br>
	 * 此连接只允许发送短信.
	 * 
	 * @param destNumber
	 *            目标号码.
	 * @param destPort
	 *            目标端口. 可为空, 则发送普通短信.
	 * @return client mode scheme
	 */
	private String createClientScheme(String destNumber, String destPort) {
		String scheme = null;
		if (destNumber != null && !destNumber.equals(""))
			scheme = "sms://" + destNumber;
		else
			return null;
		if (destPort != null && !destPort.equals(""))
			scheme += ":" + destPort;
		System.out.println("client: " + scheme);
		return scheme;
	}

	/**
	 * 创建server mode的连接scheme.
	 * 
	 * @param monitorPort
	 *            短信监听端口.
	 * @return server scheme.
	 */
	private String createServerScheme(String monitorPort) {
		String scheme = null;
		if ((monitorPort != null && !monitorPort.equals(""))) {
			scheme = "sms://:" + monitorPort;
			System.out.println("server: " + scheme);
			return scheme;
		} else {
			return null;
		}
	}

	/**
	 * 发送短信初始化.
	 * 
	 * @param destNumber
	 *            目标手机号.
	 * @param destPort
	 *            监听端口.
	 */
	private void sendInit(String destNumber, String destPort) {
		_destNumber = destNumber;
		_destPort = destPort;
		String url = createClientScheme(destNumber, destPort);
		clientMessageConnection = newMessageConnection(url);
	}

	/**
	 * 设定监听器.
	 * 
	 * @param listener
	 */
	private void setListener(SmsMessageListener listener) {
		try {
			serverMessageConnection.setMessageListener(listener);
		} catch (IOException e) {
			log.debug("setMessageListener " + listener + " error");
			e.printStackTrace();
		}
	}

	/**
	 * 关闭连接.<br>
	 * 短信操作完成后必须关闭短信连接.
	 */
	static final public void close() {
		try {
			if (serverMessageConnection != null) {
				// 注销监听器
				serverMessageConnection.setMessageListener(null);
				serverMessageConnection.close();
				serverMessageConnection = null;
			}
			if (clientMessageConnection != null) {
				// 注销监听器(client mode 不会有监听)
				clientMessageConnection.setMessageListener(null);
				clientMessageConnection.close();
				clientMessageConnection = null;
			}
		} catch (IOException ioe) {
		}
	}

	/**
	 * 新建一个MessageConnection
	 * 
	 * @param connUrl
	 *            <p>
	 *            可以是client mode或server mode. <br>
	 *            client模式时, 只能发送消息,并且需要提供目标地址即对方手机号.<br>
	 *            client: sms://+15121234567:5000. <br>
	 *            </p>
	 *            <p>
	 *            server模式时, 可以发送和接收短信. 需提供监听端口.<br>
	 *            如果端口已被占用则 Connector.open() 抛出 IOException.
	 *            Java应用可以使用任何非保留端口(0-49151 为保留端口), 出于安全性考虑平台可能并不允许使用所有的非保留端口.<br>
	 *            server: sms://:5000.
	 *            </p>
	 * @return MessageConnection
	 */
	final private MessageConnection newMessageConnection(String connUrl) {
		MessageConnection mc = null;
		try {
			mc = (MessageConnection) Connector.open(connUrl);
		} catch (IOException e) {
			log.debug("open " + connUrl + " error");
			e.printStackTrace();
		}
		return mc;
	}

	/**
	 * 发送短信
	 * 
	 * @param msg
	 * @param destNum
	 * @param destPort
	 */
	final private void sendMessage(final Message msg, final String destNum,
			final String destPort)
	/* throws IllegalArgumentException, SecurityException */{
		Thread th = new Thread() {
			public void run() {
				if (destNum != null) {
					String url = "sms://" + destNum;
					if (destPort != null) {
						msg.setAddress(url + ":" + destPort);
						System.out.println("sendMessage: " + url + ":"
								+ destPort);
					} else {
						msg.setAddress(url);
						System.out.println(url);
					}
				}
				int segcount = clientMessageConnection.numberOfSegments(msg);
				if (segcount == 0) {
					log.debug(SEGCOUNT);
				} else {
					try {
						clientMessageConnection.send(msg);
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

	/**
	 * 发送字符短信
	 * 
	 * @param msg
	 * @param desNum
	 * @param destPort
	 */
	final public void sendTextMessage(String msg, String desNum, String destPort) {
		if (clientMessageConnection == null)
			sendInit(desNum, destPort);
		TextMessage tmsg = null;
		tmsg = (TextMessage) clientMessageConnection
				.newMessage(MessageConnection.TEXT_MESSAGE);
		tmsg.setPayloadText(msg);
		sendMessage(tmsg, _destNumber, _destPort);
	}

	/**
	 * 设置短信监听器及监听端口.
	 * 
	 * @param listener
	 * @param monitorPort
	 */
	final public void monitor(SmsMessageListener listener, String monitorPort) {
		_monitorPort = monitorPort;
		if (serverMessageConnection == null) {
			String url = createServerScheme(monitorPort);
			serverMessageConnection = newMessageConnection(url);
		}
		setListener(listener);
	}

	/**
	 * 发送二进制短信
	 * 
	 * @param msg
	 * @param desNum
	 * @param destPort
	 */
	final public void sendBinaryMessage(byte[] msg, String desNum,
			String destPort) {
		if (clientMessageConnection == null)
			sendInit(desNum, destPort);
		BinaryMessage bmsg;
		bmsg = (BinaryMessage) clientMessageConnection
				.newMessage(MessageConnection.BINARY_MESSAGE);
		bmsg.setPayloadData(msg);
		sendMessage(bmsg, _destNumber, _destPort);
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

	/**
	 * 返回收到的短信内容.
	 * 
	 * @return 包含内容的Item[]
	 */
	public Item[] getReceives() {
		synchronized (mutex) {
			return receives;
		}
	}

	/**
	 * 异步的通过MessageConnection读取短信,并开始处理短信.
	 * 
	 * @param mc
	 *            通过短信监听器得到的MessageConnection.
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
	 * @return 短信内容
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

	//////////// 彩信
	// MIME Types
	public final static String MIMETYPE_TEXT_PLAIN = "text/plain";
	public final static String MIMETYPE_TEXT_XML = "text/xml";
	public final static String MIMETYPE_TEXT_HTML = "text/html";
	public final static String MIMETYPE_IMAGE_PNG = "image/png";
	public final static String MIMETYPE_IMAGE_JPEG = "image/jpg";
	public final static String MIMETYPE_IMAGE_GIF = "image/gif";
	public final static String MIMETYPE_APPLICATION_OCTET_STREAM = "application/octet-stream";

	/**
	 * 处理收到的彩信.
	 * 
	 * @param mpmsg
	 *            处理的彩信
	 * @return Item[]
	 */
	private Item[] processMultipartMessage(MultipartMessage mpmsg) {

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

	///// push
	/**
	 * 动态注册push启动.
	 * 
	 * @param url
	 * @param midlet
	 * @param filter
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
	 * 注销push监听.
	 * 
	 * @param url
	 */
	public void unregisterSmsMonitor(String url) {
		try {
			PushRegistry.unregisterConnection(url);
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
