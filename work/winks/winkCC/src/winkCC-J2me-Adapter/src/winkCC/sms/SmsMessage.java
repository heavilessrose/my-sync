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
 * ����ģ��.<br>
 * Ϊ����������ֱ��ṩһ��MessageConnection. �Ӷ�����ʵ�ַ��ͺͼ�������ʹ����ͬ�Ķ˿�.
 * 
 * @author WangYinghua
 * 
 */
public class SmsMessage {
	private static ILog log = LogFactory.getLog("SmsMessage");
	private static SmsMessage instance = null;

	/** client mode MessageConnection, ֻ�������Ͷ��� */
	private static MessageConnection clientMessageConnection = null;
	/** server mode MessageConnection, ֻ���������յ��Ķ��� */
	private static MessageConnection serverMessageConnection = null;
	/** ���ŷ��͵�Ŀ�ĵغ��� */
	private String _destNumber = null;
	/** ���ŷ��͵�Ŀ�Ķ˿� */
	private String _destPort = null;
	/** �����Ķ˿� */
	private String _monitorPort = null;

	public static final String SEGCOUNT = "segment can't be zero";

	private SmsMessage() {

	}

	/**
	 * ȡ��Ŀ�ĵغ���
	 * 
	 * @return _destNumber
	 */
	public String getDestNum() {
		return _destNumber;
	}

	/**
	 * ȡ�÷��Ͷ˿�
	 * 
	 * @return _destPort
	 */
	public String getDestPort() {
		return _destPort;
	}

	/**
	 * ȡ�ü����˿�
	 * 
	 * @return _monitorPort
	 */
	public String getMonitorPort() {
		return _monitorPort;
	}

	/**
	 * �˶���Ϊ����.
	 * 
	 * @return SmsMessage��������.
	 */
	public static SmsMessage getInstance() {
		if (instance == null) {
			instance = new SmsMessage();
		}
		return instance;
	}

	/**
	 * ����client mode�������ӵ�scheme.<br>
	 * ������ֻ�����Ͷ���.
	 * 
	 * @param destNumber
	 *            Ŀ�����.
	 * @param destPort
	 *            Ŀ��˿�. ��Ϊ��, ������ͨ����.
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
	 * ����server mode������scheme.
	 * 
	 * @param monitorPort
	 *            ���ż����˿�.
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
	 * ���Ͷ��ų�ʼ��.
	 * 
	 * @param destNumber
	 *            Ŀ���ֻ���.
	 * @param destPort
	 *            �����˿�.
	 */
	private void sendInit(String destNumber, String destPort) {
		_destNumber = destNumber;
		_destPort = destPort;
		String url = createClientScheme(destNumber, destPort);
		clientMessageConnection = newMessageConnection(url);
	}

	/**
	 * �趨������.
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
	 * �ر�����.<br>
	 * ���Ų�����ɺ����رն�������.
	 */
	static final public void close() {
		try {
			if (serverMessageConnection != null) {
				// ע��������
				serverMessageConnection.setMessageListener(null);
				serverMessageConnection.close();
				serverMessageConnection = null;
			}
			if (clientMessageConnection != null) {
				// ע��������(client mode �����м���)
				clientMessageConnection.setMessageListener(null);
				clientMessageConnection.close();
				clientMessageConnection = null;
			}
		} catch (IOException ioe) {
		}
	}

	/**
	 * �½�һ��MessageConnection
	 * 
	 * @param connUrl
	 *            <p>
	 *            ������client mode��server mode. <br>
	 *            clientģʽʱ, ֻ�ܷ�����Ϣ,������Ҫ�ṩĿ���ַ���Է��ֻ���.<br>
	 *            client: sms://+15121234567:5000. <br>
	 *            </p>
	 *            <p>
	 *            serverģʽʱ, ���Է��ͺͽ��ն���. ���ṩ�����˿�.<br>
	 *            ����˿��ѱ�ռ���� Connector.open() �׳� IOException.
	 *            JavaӦ�ÿ���ʹ���κηǱ����˿�(0-49151 Ϊ�����˿�), ���ڰ�ȫ�Կ���ƽ̨���ܲ�������ʹ�����еķǱ����˿�.<br>
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
	 * ���Ͷ���
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
	 * �����ַ�����
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
	 * ���ö��ż������������˿�.
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
	 * ���Ͷ����ƶ���
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

	////////////////////////���ռ��������
	/**
	 * �������߳�����ȡ����
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
	 * �����յ��Ķ�������.
	 * 
	 * @return �������ݵ�Item[]
	 */
	public Item[] getReceives() {
		synchronized (mutex) {
			return receives;
		}
	}

	/**
	 * �첽��ͨ��MessageConnection��ȡ����,����ʼ�������.
	 * 
	 * @param mc
	 *            ͨ�����ż������õ���MessageConnection.
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
	 * ���յ����ź�Ĵ���
	 * 
	 * @param msg
	 * @return ��������
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
			// ����
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

	//////////// ����
	// MIME Types
	public final static String MIMETYPE_TEXT_PLAIN = "text/plain";
	public final static String MIMETYPE_TEXT_XML = "text/xml";
	public final static String MIMETYPE_TEXT_HTML = "text/html";
	public final static String MIMETYPE_IMAGE_PNG = "image/png";
	public final static String MIMETYPE_IMAGE_JPEG = "image/jpg";
	public final static String MIMETYPE_IMAGE_GIF = "image/gif";
	public final static String MIMETYPE_APPLICATION_OCTET_STREAM = "application/octet-stream";

	/**
	 * �����յ��Ĳ���.
	 * 
	 * @param mpmsg
	 *            ����Ĳ���
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
	 * ��̬ע��push����.
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
	 * ע��push����.
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
