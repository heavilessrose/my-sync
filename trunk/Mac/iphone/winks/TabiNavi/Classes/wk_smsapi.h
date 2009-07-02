//
//  wk_smsapi.h
//  TabiNavi
//
//  Created by wang luke on 7/2/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "device.h"

@interface wk_smsapi : NSObject {

}

/*!
 @method     发送短信
 @abstract   sendMessage:msg to:对方电话号码
 @discussion 成功返回0，失败返回－1
 */
-(int)sendMessage:(NSString*) msg to:(NSString*)receiver;
@end
