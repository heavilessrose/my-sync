/**
 * ����Ŀ�ĵ�ַ�ɰ���:�˿�
 */
package winkCC.sms;

import java.io.IOException;
import java.io.InterruptedIOException;

import javax.microedition.io.Connector;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.StringItem;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.Message;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;
import javax.wireless.messaging.TextMessage;

import winkCC.log.ILog;
import winkCC.log.LogFactory;

/**
 * ����ģ��
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
	 * �½�һ��MessageConnection
	 * 
	 * @param connUrl
	 *            ������client��server
	 * @param messageListener
	 *            �Ƿ��յ����ŵļ�����
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
	 * �ر�����
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

	///////////////// ���Ͷ���
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
	 * �����ַ�����
	 * 
	 * @param mc
	 *            messageConnection
	 * @param msg
	 *            ��������
	 * @param url
	 *            �Է�����
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
	 * ���Ͷ����ƶ���
	 * 
	 * @param mc
	 * @param msg
	 * @param url
	 *            �Է�����
	 */
	final public void sendBinaryMessage(MessageConnection mc, byte[] msg,
			String url) {
		BinaryMessage bmsg;
		bmsg = (BinaryMessage) mc.newMessage(MessageConnection.BINARY_MESSAGE);
		bmsg.setPayloadData(msg);
		sendMessage(mc, bmsg, url);
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

	Item[] getReceives() {
		synchronized (mutex) {
			return receives;
		}
	}

	/**
	 * ��ȡ����
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
	 * ���յ����ź�Ĵ���
	 * 
	 * @param msg
	 * @return
	 */
	public Item[] processMessage(Message msg) {
		Item[] items = null;

		if (msg instanceof TextMessage) {
			TextMessage tmsg = (TextMessage) msg;
			items = processTextMessage(tmsg);
		} else if (msg instanceof BinaryMessage) {
			BinaryMessage bmsg = (BinaryMessage) msg;
			items = processBinaryMessage(bmsg);
		}
		//		else if (message instanceof MultipartMessage) {
		//			MultipartMessage mpmsg = (MultipartMessage) message;
		//			items = processMultipartMessage(mpmsg);
		//		} 
		else {
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
}
