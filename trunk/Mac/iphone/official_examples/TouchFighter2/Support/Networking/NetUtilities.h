/*
File: NetUtilities.h
Abstract: Set of network related utility functions.

Version: 1.3
*/

#import <Foundation/Foundation.h>
#import <sys/socket.h>

//PROTOTYPES:

#ifdef __cplusplus
extern "C"
{
#endif
const struct sockaddr* IPAddressGetCurrent(); //Returns the address of the prefered interface or NULL if there's no connected interface
BOOL IPAddressIsLocal(const struct sockaddr* address); //Returns YES if "address" is on the same subnet as any connected interface - Returns NO if "address" is invalid

const struct sockaddr* IPAddressFromString(NSString* string); //Returns NULL if "string" is invalid
NSString* IPAddressToString(const struct sockaddr* address, BOOL numericHost, BOOL numericService); //Returns nil if "address" is invalid - Use the numeric arguments to force numeric over names e.g. "17.149.160.49" instead of "apple.com"

NSString* HostGetName();
NSString* HostGetUniqueID();
#ifdef __cplusplus
}
#endif
