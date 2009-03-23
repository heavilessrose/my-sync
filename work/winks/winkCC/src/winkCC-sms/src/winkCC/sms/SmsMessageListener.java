package winkCC.sms;

import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;

/**
 * 
 * @author WangYinghua
 * 
 */
public interface SmsMessageListener extends MessageListener {
	void notifyIncomingMessage(MessageConnection conn);
}
