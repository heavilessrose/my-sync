package org.anddev.android.smsexample;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.gsm.SmsMessage;
import android.util.Log;
import android.widget.Toast;

public class SMSReceiver extends BroadcastReceiver {
	/** TAG used for Debug-Logging */
	protected static final String LOG_TAG = "SMSReceiver";

	/**
	 * The Action fired by the Android-System when a SMS was received. We are
	 * using the Default Package-Visibility
	 */
	private static final String ACTION = "android.provider.Telephony.SMS_RECEIVED";

	// @Override
	public void onReceive(Context context, Intent intent) {

		Log.i(LOG_TAG, "[inside onReceive] ");

		if (intent.getAction().equals(ACTION)) {

			StringBuilder sb = new StringBuilder();
			Bundle bundle = intent.getExtras();

			if (bundle != null) {
				Object[] pdusObj = (Object[]) bundle.get("pdus");
				SmsMessage[] messages = new SmsMessage[pdusObj.length];
				for (int i = 0; i < pdusObj.length; i++) {
					messages[i] = SmsMessage.createFromPdu((byte[]) pdusObj[i]);
				}

				Log.i(LOG_TAG, "[SMSApp Bundle] " + bundle.toString());

				// Feed the StringBuilder with all Messages found.
				for (SmsMessage currentMessage : messages) {
					sb.append("Received compressed SMS\nFrom: ");
					// Sender-Number
					sb.append(currentMessage.getDisplayOriginatingAddress());
					sb.append("\n----Message----\n");
					// Actual Message-Content
					sb.append(currentMessage.getDisplayMessageBody());
				}
			}
			// Logger Debug-Output
			Log.i(LOG_TAG, "[SMSApp] onReceiveIntent: " + sb);

			// Show the Notification containing the Message.
			Toast.makeText(context, sb.toString(), Toast.LENGTH_LONG).show();

			// Consume this intent, that no other application will notice it.
			// ²»×¢ÊÍ»á±¨´í
			// this.abortBroadcast();

			// Start the Main-Activity
			Intent i = new Intent(context, SMSActivity.class);
			// i.setLaunchFlags(Intent.NEW_TASK_LAUNCH);
			i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(i);

		}
	}
}