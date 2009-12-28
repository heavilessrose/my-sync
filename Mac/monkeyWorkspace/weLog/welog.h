//
//  welog.h
//  WeLogLib
//
//  Created by luke on 09-12-28.
//  Copyright 2009 WeTeam. All rights reserved.
//
/*
#import <Foundation/Foundation.h>


@interface WeLog : NSObject {

}

@end
*/


#ifndef _WE_LOG_H_
#define _WE_LOG_H_ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#ifdef WELOG_ENABLE
	
	/*****************************************************/
	//#if TARGET_IPHONE_SIMULATOR
	//#define WELOG_ERR_ENABLE
	//#define WELOG_DEBUG_ENABLE
	//#define WELOG_INFO_ENABLE
	//#define WE_HEX_OUT_ENABLE
	//#endif
	
#ifndef WELOG_INFO_ENABLE
#define INFOLOG(args...) 
#else
#define WELOG_DEBUG_ENABLE
#define WELOG_ERR_ENABLE
#define INFOLOG(args...) _WeLog(__FILE__,__LINE__,__PRETTY_FUNCTION__,args);
#endif
	
#ifndef WELOG_DEBUG_ENABLE
#define DEBUGLOG(args...) 
#else
#define DEBUGLOG(args...) _WeLog(__FILE__,__LINE__,__PRETTY_FUNCTION__,args);
#define WELOG_ERR_ENABLE
#endif
	
#ifndef WELOG_ERR_ENABLE
#define ERRLOG(args...) 
#else
#define ERRLOG(args...) _WeLog(__FILE__,__LINE__,__PRETTY_FUNCTION__,args);
#endif
	
#ifndef WE_HEX_OUT_ENABLE
#define HEXLOG(args...) 
#else
#define HEXLOG(args...) HEX_DataOut(args...);
#endif
	/*****************************************************/
	
	void delLogFile();
	int Winks_fdelete(char *file_name);
	int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size);
	void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
#define welog(args...) _WeLog(__FILE__,__LINE__,__PRETTY_FUNCTION__,args);
//#define hexlog(args...) HEX_DataOut(args);
	

#else

#define welog(x...)

#endif /* WELOG_ENABLE */

void _WeLog(const char *file, int lineNumber, const char *funcName, NSString *format,...);

#endif /* _WE_LOG_H_ */
