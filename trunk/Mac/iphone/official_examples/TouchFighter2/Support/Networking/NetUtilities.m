/*
File: NetUtilities.m
Abstract: Set of network related utility functions.

Version: 1.3
*/

#import <arpa/inet.h>
#import <ifaddrs.h>
#import <netdb.h>
#import <netinet/in.h>
#ifndef __CODEX__
#if !TARGET_OS_IPHONE
#import <netinet6/in6.h>
#endif
#endif
#if !TARGET_OS_IPHONE
#import <IOKit/IOKitLib.h>
#import <IOKit/network/IOEthernetInterface.h>
#import <IOKit/network/IONetworkInterface.h>
#import <IOKit/network/IOEthernetController.h>
#import <CommonCrypto/CommonDigest.h>
#endif

#include "NetUtilities.h"

//FUNCTIONS:

#if !TARGET_OS_IPHONE

static NSString* _GetPrimaryMACAddress()
{
	NSString*					string = nil;
	CFMutableDictionaryRef		matchingDictionary;
	CFMutableDictionaryRef		propertyMatchingDictionary;
	io_iterator_t				intfIterator;
	io_object_t					intfService;
    io_object_t					controllerService;
    CFDataRef					addressData;
	unsigned char*				address;
	
	if((matchingDictionary = IOServiceMatching(kIOEthernetInterfaceClass))) {
		if((propertyMatchingDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks))) {
			CFDictionarySetValue(propertyMatchingDictionary, CFSTR(kIOPrimaryInterface), kCFBooleanTrue); 
			CFDictionarySetValue(matchingDictionary, CFSTR(kIOPropertyMatchKey), propertyMatchingDictionary);
            if(IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDictionary, &intfIterator) == KERN_SUCCESS) { //NOTE: This consumes a reference of "matchingDictionary"
				if((intfService = IOIteratorNext(intfIterator))) {
					if(IORegistryEntryGetParentEntry(intfService, kIOServicePlane, &controllerService) == KERN_SUCCESS) {
						if((addressData = IORegistryEntryCreateCFProperty(controllerService, CFSTR(kIOMACAddress), kCFAllocatorDefault, 0))) {
							address = (unsigned char*)CFDataGetBytePtr(addressData);
							string = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X", address[0], address[1], address[2], address[3], address[4], address[5]];
							CFRelease(addressData);
						}
						IOObjectRelease(controllerService);
					}
					IOObjectRelease(intfService);
				}
				IOObjectRelease(intfIterator);
			}
			CFRelease(propertyMatchingDictionary);
		}
	}
	
	return string;
}

#endif

NSString* HostGetUniqueID()
{
	static NSString*			uniqueID = nil;
#if !TARGET_OS_IPHONE
	unsigned char				md5[CC_MD5_DIGEST_LENGTH];
	const char*					buffer;
#endif
	
	if(uniqueID == nil) {
#if TARGET_OS_IPHONE
		uniqueID = [[UIDevice currentDevice] uniqueIdentifier];
		uniqueID = [[[uniqueID uppercaseString] stringByReplacingOccurrencesOfString:@":" withString:@"-"] copy];
#else
		uniqueID = _GetPrimaryMACAddress(); //NOTE: We hash the MAC address to preserve privacy
		if((buffer = [uniqueID UTF8String])) {
			bzero(md5, CC_MD5_DIGEST_LENGTH);
			CC_MD5(buffer, strlen(buffer), md5);
			uniqueID = [NSString stringWithFormat:@"%08X-%08X-%08X-%08X", *((unsigned int*)&md5[0]), *((unsigned int*)&md5[4]), *((unsigned int*)&md5[8]), *((unsigned int*)&md5[12])];
		}
		else
		uniqueID = nil;
#endif
		
		if(uniqueID == nil)
		[NSException raise:NSInternalInconsistencyException format:@"Unable to retrieve unique ID"];
	}
	
	return uniqueID;
}

NSString* HostGetName()
{
	static NSString*				name = nil;
	
	if(name == nil) {
#if TARGET_OS_IPHONE
		name = [[UIDevice currentDevice] name];
#else
		name = [[NSHost currentHost] name];
#endif
		
		name = [name copy];
	}

	return name;
}

const struct sockaddr* IPAddressGetCurrent()
{
	NSData*							data = nil;
	struct ifaddrs*					list;
	struct ifaddrs*					ifap;
	
	if(getifaddrs(&list) < 0)
	return NULL;
	
	for(ifap = list; ifap; ifap = ifap->ifa_next) {
		if((ifap->ifa_name[0] == 'l') && (ifap->ifa_name[1] == 'o')) //Ignore loopback
		continue;
		
#ifndef __CODEX__
#if TARGET_OS_IPHONE
		if(ifap->ifa_addr->sa_family == AF_INET)
#else
		if((ifap->ifa_addr->sa_family == AF_INET) || (ifap->ifa_addr->sa_family == AF_INET6))
#endif
#else
		if(ifap->ifa_addr->sa_family == AF_INET)
#endif
		{
			data = [NSData dataWithBytes:ifap->ifa_addr length:ifap->ifa_addr->sa_len];
			break;
		}
	}
	
	freeifaddrs(list);
	
	return [data bytes];
}

#ifndef __CODEX__
//FIXME: Handle IPv6 addresses
#endif
BOOL IPAddressIsLocal(const struct sockaddr* address)
{
	BOOL							local = NO;
	struct ifaddrs*					list;
	struct ifaddrs*					ifap;
	
	if((address == NULL) || (address->sa_family != AF_INET))
	return NO;
	
	if(getifaddrs(&list) < 0)
	return NO;
	
	for(ifap = list; ifap; ifap = ifap->ifa_next) {
		if((ifap->ifa_name[0] == 'l') && (ifap->ifa_name[1] == 'o')) //Ignore loopback
		continue;
		
		if((ifap->ifa_addr->sa_family == AF_INET) && (ifap->ifa_netmask->sa_family == AF_INET)) {
			if((((struct sockaddr_in*)(ifap->ifa_addr))->sin_addr.s_addr & ((struct sockaddr_in*)(ifap->ifa_netmask))->sin_addr.s_addr) == (((struct sockaddr_in*)address)->sin_addr.s_addr & ((struct sockaddr_in*)(ifap->ifa_netmask))->sin_addr.s_addr)) {
				local = YES;
				break;
			}
		}
	}
	
	freeifaddrs(list);
	
	return local;
}

NSString* IPAddressToString(const struct sockaddr* address, BOOL numericHost, BOOL numericService)
{
	char							hostBuffer[NI_MAXHOST] = {0};
	char							serviceBuffer[NI_MAXSERV] = {0};
	
	if(address) {
		if(getnameinfo(address, address->sa_len, hostBuffer, NI_MAXHOST, serviceBuffer, NI_MAXSERV, (numericHost ? NI_NUMERICHOST : 0) | (numericService ? NI_NUMERICSERV : 0) | NI_NOFQDN) < 0)
		return nil;
		if(!numericService && !serviceBuffer[0]) { //NOTE: Check if there is no service name for this port
			if(getnameinfo(address, address->sa_len, NULL, 0, serviceBuffer, NI_MAXSERV, NI_NUMERICSERV) < 0)
			return nil;
		}
		
		return (serviceBuffer[0] != '0' ? [NSString stringWithFormat:@"%s:%s", hostBuffer, serviceBuffer] : [NSString stringWithFormat:@"%s", hostBuffer]);
	}
	
	return nil;
}

const struct sockaddr* IPAddressFromString(NSString* string)
{
	NSRange							range;
	NSData*							data;
	struct addrinfo*				info;
	in_port_t						port;
	
	if(![string length])
	return NULL;
	
	//NOTE: getaddrinfo() cannot handle service strings that are plain numerical e.g. "22" instead of "ssh"
	range = [string rangeOfString:@":" options:0 range:NSMakeRange(0, [string length])];
	if(range.location != NSNotFound) {
		if(getaddrinfo([[string substringToIndex:range.location] UTF8String], [[string substringFromIndex:(range.location + 1)] UTF8String], NULL, &info) != 0) {
			port = htons([[string substringFromIndex:(range.location + 1)] intValue]);
			if(port == 0)
			return NULL;
			
			if(getaddrinfo([[string substringToIndex:range.location] UTF8String], NULL, NULL, &info) != 0)
			return NULL;
			
			switch(info->ai_addr->sa_family) {
				
				case AF_INET:
				((struct sockaddr_in*)info->ai_addr)->sin_port = port;
				break;
				
#ifndef __CODEX__
#if !TARGET_OS_IPHONE
				case AF_INET6:
				((struct sockaddr_in6*)info->ai_addr)->sin6_port = port;
				break;
#endif
#endif
				
				default:
				freeaddrinfo(info);
				return NULL;
				break;
				
			}
		}
	}
	else {
		if(getaddrinfo([string UTF8String], NULL, NULL, &info) != 0)
		return NULL;
	}
	
	data = [NSData dataWithBytes:info->ai_addr length:info->ai_addr->sa_len];
	
	freeaddrinfo(info);
	
	return (struct sockaddr*)[data bytes];
}
