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
package com.astrientlabs.jobs;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;

import com.astrientlabs.ad.AdCache;
import com.astrientlabs.cache.Cache;
import com.astrientlabs.colors.Colors;
import com.astrientlabs.i18n.TextMap;
import com.astrientlabs.log.Logger;
import com.astrientlabs.midlet.Yipi;
import com.astrientlabs.search.SearchQuery;
import com.astrientlabs.ui.ResultDisplayWindow;
import com.astrientlabs.ui.SearchWindow;
import com.astrientlabs.ui.SplashCanvas;
import com.astrientlabs.util.Images;
import com.astrientlabs.util.Metric;
import com.astrientlabs.util.Stats;

/** 系统初始化 */
public class StartupJob implements Runnable, CommandListener {
	private SplashCanvas splashScreen;

	public StartupJob(SplashCanvas splashCanvas) {
		this.splashScreen = splashCanvas;
	}

	public void run() {
		try {
			Metric metric = new Metric("startup");
			Yipi.instance.systemPrefs.initialize();
			splashScreen.setPercentDone(10);
			Yipi.instance.systemPrefs.read();
			splashScreen.setPercentDone(20);

			Colors.init();

			Cache.instance = new Cache(Yipi.instance.systemPrefs.getInt(
					"cache", 12));

			Stats.downloads = Yipi.instance.systemPrefs.getInt("downloads", 0);

			Images.init();
			AdCache.instance = new AdCache();
			AdCache.instance.open();

			SearchQuery.loadCannedResults();
			splashScreen.setPercentDone(40);

			String locale = Yipi.instance.systemPrefs.get("locale");
			TextMap.instance.init(locale);
			splashScreen.setPercentDone(50);

			SearchWindow.instance = new SearchWindow();
			splashScreen.setPercentDone(90);
			ResultDisplayWindow.instance = new ResultDisplayWindow();

			splashScreen.setPercentDone(100);

			Yipi.instance.display.setCurrent(SearchWindow.instance);

			Logger.instance.log("startup", System.getProperty("phone.imei"));
			metric.elapsed();
		} catch (Throwable t) {
			// TODO: display error, exit app
			t.printStackTrace();
		}
	}

	public void commandAction(Command arg0, Displayable arg1) {
		Yipi.instance.exit();
	}
}
