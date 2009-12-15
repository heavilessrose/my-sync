/*
 *  WeCommons.h
 *  weHelp_UI
 *
 *  Created by luke on 09-12-14.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#if TARGET_IPHONE_SIMULATOR
#define CONFIG_FILE @"/Users/luke/Documents/my-sync/trunk/Mac/iphone/weHelp/weHelp_UI/weConf.plist"
#define SHOW_LOG
#else
#define CONFIG_FILE @"/var/mobile/weConf.plist"
#endif
