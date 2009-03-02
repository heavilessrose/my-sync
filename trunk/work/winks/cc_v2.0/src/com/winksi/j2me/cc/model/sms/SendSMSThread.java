package com.winksi.j2me.cc.model.sms;

import javax.microedition.io.Connector;
import javax.wireless.messaging.MessageConnection;
import javax.wireless.messaging.TextMessage;

import com.winksi.j2me.cc.control.*;
import java.io.IOException;

/** 发送短信线程类 */
public class SendSMSThread extends Thread {//implements Runnable

	private GUIController m_controller = null;
	private String smsPort;
	/** 是否发送的标志变量 */
	private boolean m_bRunIt;
	private String m_sDestNumber = "";
	private String m_sMessageText = "";

	/** 短信发送的目的地号码 */
	public synchronized void setDestNumber(String value) {
		m_sDestNumber = value;
	}

	public synchronized void setSmsPort(String port) {
		this.smsPort = port;
	}

	public synchronized void setMessageText(String value) {
		m_sMessageText = value;
	}

	public SendSMSThread(GUIController controll) {
		m_controller = controll;
		m_bRunIt = false;
	}

	public synchronized void notifySend(boolean value) {
		m_bRunIt = value;
	}

	public void run() {
		/* Use networking if necessary */
		long lngStart;
		long lngTimeTaken;

		//        while (true) {
		try {
			if (m_bRunIt) {
				//                    System.out.println("/* Start Send SMS!");
				MessageConnection conn = null;
				try {
					String addr = "sms://" + m_sDestNumber + ":" + smsPort;
					//                    System.out.println("open:"+addr);
					conn = (MessageConnection) Connector.open(addr);
					//                         System.out.println("open:"+addr);
					TextMessage msg = (TextMessage) conn.newMessage(MessageConnection.TEXT_MESSAGE);
					msg.setPayloadText(m_sMessageText);
					conn.send(msg);
					conn.close();
					//                         System.out.println("/*End Send SMS!");
				} catch (SecurityException exc) {
					m_controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
				} catch (IOException e) {
				} finally {
					conn = null;
					m_sMessageText = "";
					m_sDestNumber = "";
				}
				m_bRunIt = false;
			}

			lngStart = System.currentTimeMillis();
			lngTimeTaken = System.currentTimeMillis() - lngStart;
			if (lngTimeTaken < 100) {
				Thread.sleep(75 - lngTimeTaken);
			}

		} catch (InterruptedException exc) {
			m_bRunIt = false;
			exc.printStackTrace();
		}
		//        }
		System.out.println("SendSMSThread:run(): send sms success~ ");
	}
}
