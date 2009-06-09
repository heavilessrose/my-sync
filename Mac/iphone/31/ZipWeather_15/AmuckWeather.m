//
//  AmuckWeather.m
//  ZipWeather
//
//  Created by apple on 10/15/08.
//  Copyright 2008 ACS Technologies. All rights reserved.
//

#import "AmuckWeather.h"


@implementation AmuckWeather

+(NSString*)getWeatherXmlForZipCode: (NSString*)zipCode {
	NSError *error;
	NSURLResponse *response;
	NSData *dataReply;
	NSString *stringReply;
	NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL: [NSURL URLWithString: [NSString stringWithFormat:@"http://weather.yahooapis.com/forecastrss?p=%@", zipCode]]];
	[request setHTTPMethod: @"GET"];
	dataReply = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
	stringReply = [[NSString alloc] initWithData:dataReply encoding:NSUTF8StringEncoding];
	NSLog(stringReply);
	return stringReply;
}

@end