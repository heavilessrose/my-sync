package com.winksi.j2me.cc.model.sms;

/**
 * SMS tools,send sms or receive sms
 * @author jerry
 */
import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.tools.StringTools;
import java.io.IOException;

import javax.wireless.messaging.Message;

import javax.microedition.io.Connector;
import javax.microedition.io.PushRegistry;
import javax.wireless.messaging.BinaryMessage;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.MessageListener;
import javax.wireless.messaging.TextMessage;

public class SMSTools implements MessageListener {

	/** Connections detected at start up. */
	private String[] connections;
	/** Flag to signal end of processing. */
	private boolean done;
	/** The port on which we listen for SMS messages */
	private String smsPort;
	/** SMS message connection for inbound text messages. */
	private MessageConnection smsconn;
	/** Current message read from the network. */
	private Message msg;
	/** Address of the message's sender */
	private String senderAddress;
	private String reveiceMessage;

	public SMSTools(String smsPort) {
		this.smsPort = smsPort;
	}

	/** 监控短信端口 */
	public void monitorSMSPort() {
		/** SMS connection to be read. */
		String smsConnection = "sms://:" + smsPort;

		/** Open the message connection. */
		if (smsconn == null) {
			try {
				smsconn = (MessageConnection) Connector.open(smsConnection);
				smsconn.setMessageListener(this);
			} catch (IOException ioe) {
				try {
					exitReceive();
					smsconn = (MessageConnection) Connector.open(smsConnection);
					smsconn.setMessageListener(this);
					//                System.out.println("sms error : "+ioe.toString());
					//                ioe.printStackTrace();
				} catch (IOException ex) {
					exitReceive();
					ex.printStackTrace();
				}
				//                System.out.println("sms error : "+ioe.toString());
				//                ioe.printStackTrace();
			}
		}

		/** Initialize the text if we were started manually. */
		connections = PushRegistry.listConnections(true);

		done = false;

		//        getReceive();
	}

	public void getReceive() {
		try {
			msg = smsconn.receive();

			if (msg != null) {
				senderAddress = msg.getAddress();
				String message = "";
				if (msg instanceof TextMessage) {
					message = ((TextMessage) msg).getPayloadText();
				} else {
					StringBuffer buf = new StringBuffer();
					byte[] data = ((BinaryMessage) msg).getPayloadData();

					for (int i = 0; i < data.length; i++) {
						char c = (char) data[i];
						buf.append(c);
						//                        int intData = (int) data[i] & 0xFF;
						//
						//                        if (intData < 0x10) {
						//                            buf.append("0");
						//                        }
						//
						//                        buf.append(Integer.toHexString(intData));
						//                        buf.append(' ');
					}

					message = buf.toString();
				}

				reveiceMessage = message;
				//                System.out.println(reveiceMessage);
			}
			msg = null;
			exitReceive();
		} catch (IOException e) {
			// e.printStackTrace();              
		}
	}

	public void notifyIncomingMessage(MessageConnection conn) {
		done = false;
	}

	public String[] getConnections() {
		return this.connections;
	}

	public String getSenderMsg() {
		return this.senderAddress;
	}

	public String getReceiveMsg() {
		return this.reveiceMessage;
	}

	public void exitReceive() {
		done = true;

		if (smsconn != null) {
			try {
				smsconn.setMessageListener(null);
				smsconn.close();
			} catch (IOException e) {
				System.out.println("SMSTool:exitReceive(): exception while close smsconn");
			}
		}
	}

}
