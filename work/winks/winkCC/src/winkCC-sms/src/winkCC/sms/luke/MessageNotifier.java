package winkCC.sms.luke;

import javax.microedition.lcdui.Item;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.Message;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;
import javax.wireless.messaging.MultipartMessage;
import javax.wireless.messaging.TextMessage;

/**
 * @author WangYinghua
 * 
 */
public interface MessageNotifier extends MessageListener {

	public void notifyIncomingMessage(final MessageConnection messageConnection);

	public void readMessageThreaded(final MessageConnection messageConnection);

	public void readMessage(final MessageConnection messageConnection);

	public Item[] processMessage(final Message message);

	public Item[] processTextMessage(final TextMessage tmsg);

	public Item[] processBinaryMessage(BinaryMessage bmsg);

	public Item[] processMultipartMessage(MultipartMessage mpmsg);

}
