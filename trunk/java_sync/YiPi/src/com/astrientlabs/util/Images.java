/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.util;

import javax.microedition.lcdui.Image;

public class Images {
	public static Image FAKE_AD;
	public static Image PICTURE;
	public static Image TITLE;
	public static Image CANNED_RESULT;
	public static Image MORE_RESULTS;

	public static void init() {
		try {
			MORE_RESULTS = Image.createImage("/more.png");
			TITLE = Image.createImage("/title.png");
			FAKE_AD = Image.createImage("/splash.png");
			PICTURE = Image.createImage("/pic.png");
			CANNED_RESULT = Image.createImage("/cresult.png");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
