/* 
 * Copyright (C) 2007 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.apis.app;

import com.example.android.apis.R;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RemoteViews;

/**
 * Demonstrates adding notifications to the status bar
 */
public class StatusBarNotifications extends Activity {

    private NotificationManager mNotificationManager;

    // Use our layout id for a unique identifier
    private static int MOOD_NOTIFICATIONS = R.layout.status_bar_notifications;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.status_bar_notifications);

        Button button;

        // Get the notification manager serivce.
        mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        button = (Button) findViewById(R.id.happy);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_happy, R.string.status_bar_notifications_happy_message,
                        false);
            }
        });

        button = (Button) findViewById(R.id.neutral);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_neutral, R.string.status_bar_notifications_ok_message,
                        false);
            }
        });

        button = (Button) findViewById(R.id.sad);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_sad, R.string.status_bar_notifications_sad_message, false);
            }
        });

        button = (Button) findViewById(R.id.happyMarquee);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_happy, R.string.status_bar_notifications_happy_message,
                        true);
            }
        });

        button = (Button) findViewById(R.id.neutralMarquee);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_neutral, R.string.status_bar_notifications_ok_message, true);
            }
        });

        button = (Button) findViewById(R.id.sadMarquee);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMood(R.drawable.stat_sad, R.string.status_bar_notifications_sad_message, true);
            }
        });

        button = (Button) findViewById(R.id.happyViews);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMoodView(R.drawable.stat_happy, R.string.status_bar_notifications_happy_message);
            }
        });

        button = (Button) findViewById(R.id.neutralViews);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMoodView(R.drawable.stat_neutral, R.string.status_bar_notifications_ok_message);
            }
        });

        button = (Button) findViewById(R.id.sadViews);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setMoodView(R.drawable.stat_sad, R.string.status_bar_notifications_sad_message);
            }
        });
        
        button = (Button) findViewById(R.id.defaultSound);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setDefault(Notification.DEFAULT_SOUND);
            }
        });
        
        button = (Button) findViewById(R.id.defaultVibrate);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setDefault(Notification.DEFAULT_VIBRATE);
            }
        });
        
        button = (Button) findViewById(R.id.defaultAll);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                setDefault(Notification.DEFAULT_ALL);
            }
        });
        
        button = (Button) findViewById(R.id.clear);
        button.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                mNotificationManager.cancel(R.layout.status_bar_notifications);
            }
        });
    }

    private void setMood(int moodId, int textId, boolean showTicker) {
        // In this sample, we'll use the same text for the ticker and the expanded notification
        CharSequence text = getText(textId);

        // choose the ticker text
        String tickerText = showTicker ? getString(textId) : null;

        // Set the icon, scrolling text and timestamp
        Notification notification = new Notification(moodId, tickerText,
                System.currentTimeMillis());

        // The PendingIntent to launch our activity if the user selects this notification
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, StatusBarNotifications.class), 0);

        // Set the info for the views that show in the notification panel.
        notification.setLatestEventInfo(this, getText(R.string.status_bar_notifications_mood_title),
                       text, contentIntent);

        // Send the notification.
        // We use a layout id because it is a unique number.  We use it later to cancel.
        mNotificationManager.notify(R.layout.status_bar_notifications, notification);
    }

    private void setMoodView(int moodId, int textId) {
        // Instead of the normal constructor, we're going to use the one with no args and fill
        // in all of the data ourselves.  The normal one uses the default layout for notifications.
        // You probably want that in most cases, but if you want to do something custom, you
        // can set the contentView field to your own RemoteViews object.
        Notification notif = new Notification();

        // This is who should be launched if the user selects our notification.
        notif.contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, StatusBarNotifications.class), 0);

        // In this sample, we'll use the same text for the ticker and the expanded notification
        CharSequence text = getText(textId);
        notif.tickerText = text;

        // the icon for the status bar
        notif.icon = moodId;

        // our custom view
        RemoteViews contentView = new RemoteViews(getPackageName(), R.layout.status_bar_balloon);
        contentView.setTextViewText(R.id.text, text);
        contentView.setImageViewResource(R.id.icon, moodId);
        notif.contentView = contentView;

        // we use a string id because is a unique number.  we use it later to cancel the
        // notification
        mNotificationManager.notify(R.layout.status_bar_notifications, notif);
    }
    
    private void setDefault(int defaults) {
        
        // This method sets the defaults on the notification before posting it.
        
        // This is who should be launched if the user selects our notification.
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, StatusBarNotifications.class), 0);

        // In this sample, we'll use the same text for the ticker and the expanded notification
        CharSequence text = getText(R.string.status_bar_notifications_happy_message);

        final Notification notification = new Notification(
                R.drawable.stat_happy,       // the icon for the status bar
                text,                        // the text to display in the ticker
                System.currentTimeMillis()); // the timestamp for the notification

        notification.setLatestEventInfo(
                this,                        // the context to use
                getText(R.string.status_bar_notifications_mood_title),
                                             // the title for the notification
                text,                        // the details to display in the notification
                contentIntent);              // the contentIntent (see above)

        notification.defaults = defaults;
        
        mNotificationManager.notify(
                   R.layout.status_bar_notifications, // we use a string id because it is a unique
                                                      // number.  we use it later to cancel the
                   notification);                     // notification
    }    
}
