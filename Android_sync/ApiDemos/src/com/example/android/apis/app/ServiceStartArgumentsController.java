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

// Need the following import to get access to the app resources, since this
// class is in a sub-package.
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.example.android.apis.R;

import java.util.HashMap;

/**
 * Example of explicitly starting the {@link ServiceStartArguments}.
 */
public class ServiceStartArgumentsController extends Activity {
    @Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.service_start_arguments_controller);

        // Watch for button clicks.
        Button button = (Button)findViewById(R.id.start1);
        button.setOnClickListener(mStart1Listener);
        button = (Button)findViewById(R.id.start2);
        button.setOnClickListener(mStart2Listener);
        button = (Button)findViewById(R.id.start3);
        button.setOnClickListener(mStart3Listener);
    }

    private OnClickListener mStart1Listener = new OnClickListener() {
        public void onClick(View v) {
            startService(new Intent(ServiceStartArgumentsController.this,
                    ServiceStartArguments.class).putExtra("name", "One"));
        }
    };

    private OnClickListener mStart2Listener = new OnClickListener() {
        public void onClick(View v) {
            startService(new Intent(ServiceStartArgumentsController.this,
                    ServiceStartArguments.class).putExtra("name", "Two"));
        }
    };

    private OnClickListener mStart3Listener = new OnClickListener() {
        public void onClick(View v) {
            startService(new Intent(ServiceStartArgumentsController.this,
                    ServiceStartArguments.class).putExtra("name", "Three"));
        }
    };
}
