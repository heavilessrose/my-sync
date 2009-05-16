/*
File: Message.h
Abstract: Network messages.

Version: 2.0
*/

#import <Foundation/Foundation.h>

typedef enum {
	kMessageType_StartGame = 0,
	kMessageType_SelectedShip,
	kMessageType_GameInfo
} MessageType;

@interface Message : NSObject
{
@public
	MessageType			type;
	id					data;
}

+ (id) messageWithType:(MessageType)type data:(id)data;
@end
