#import "AppController.h"

// Developed by Aaron Hillegass of Big Nerd Ranch, Inc.
// Use this code anyway you want, but neither Aaron nor BNR is
// responsible for anything you do with it or anything
// bad that results from its use.

#define AWS_ID @"1CKE6MZ6S27EFQ458402"

@implementation AppController

- (void)awakeFromNib
{
    [tableView setDoubleAction:@selector(openItem:)];
    [tableView setTarget:self];
    [webView setFrameLoadDelegate:self];
}


#pragma mark Action methods

- (void)fetchBooks:(id)sender
{
    // Show the user that something is going on
    [progress startAnimation:nil];
    
    // Put together the request
    // See http://www.amazon.com/gp/aws/landing.html
    
    // Get the string and percent-escape for insertion into URL
	NSString *searchString = [[searchField stringValue] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	NSLog(@"searchString = %@", searchString);
    // Create the URL
    NSString *urlString = [NSString stringWithFormat:
        @"http://webservices.amazon.com/onca/xml?Service=AWSECommerceService&Operation=ItemSearch&SubscriptionId=%@&SearchIndex=Books&Keywords=%@",
        AWS_ID, searchString];

    NSURL *url = [NSURL URLWithString:urlString];
    NSURLRequest *urlRequest = [NSURLRequest requestWithURL:url 
                                                cachePolicy:NSURLRequestReturnCacheDataElseLoad
                                            timeoutInterval:30];  
    
    // Fetch the XML response
    NSData *urlData;
    NSURLResponse *response;
    NSError *error;
    urlData = [NSURLConnection sendSynchronousRequest:urlRequest
                                    returningResponse:&response
                                                error:&error];
    
    [progress stopAnimation:nil];

    if (!urlData) {
        NSRunAlertPanel(@"Error loading", @"%@", nil, nil, nil, [error localizedDescription]);
        return;
    }
    
    // Parse the XML response
	[doc release];
	doc = [[NSXMLDocument alloc] initWithData:urlData
												   options:0 
													 error:&error];
	
	if (!doc) {
		NSAlert *alert = [NSAlert alertWithError:error];
		[alert runModal];
		return;
	}
	
	[itemNodes release];
	itemNodes = [doc nodesForXPath:@"ItemSearchResponse/Items/Item" error:&error];
	if (!itemNodes) {
		NSAlert *alert = [NSAlert alertWithError:error];
		[alert runModal];
		return;
	}
	NSLog(@"itemNodes = %@", itemNodes);
	[itemNodes retain];

    // Update the interface
    [tableView reloadData];
}

- (NSString *)stringForPath:(NSString *)xp ofNode:(NSXMLNode *)n
{
	NSError *error;
	NSArray *nodes = [n nodesForXPath:xp error:&error];
	if (!nodes) {
		NSAlert *alert = [NSAlert alertWithError:error];
		[alert runModal];
		return nil;
	}
	if ([nodes count] == 0) {
		return nil;
	}
	else return [[nodes objectAtIndex:0] stringValue];
}

- (void)openItem:(id)sender
{
    int row = [tableView clickedRow];
    if (row == -1) {
        return;
    }
	
    NSXMLNode *clickedItem = [itemNodes objectAtIndex:row];
	NSString *urlString = [self stringForPath:@"DetailPageURL"
								 ofNode:clickedItem];
    //NSURL *url = [NSURL URLWithString:urlString];
    //[[NSWorkspace sharedWorkspace] openURL:url];
 
    [NSApp beginSheet:browserWindow
       modalForWindow:[tableView window]
        modalDelegate:nil
       didEndSelector:NULL
          contextInfo:NULL];
    [webView setMainFrameURL:urlString];
}

- (void)dismissSheet:(id)sender
{
    [NSApp endSheet:browserWindow];
    [browserWindow orderOut:nil];
}

- (void)webView:(WebView *)sender 
didStartProvisionalLoadForFrame:(WebFrame *)frame
{
    [webProgressIndicator startAnimation:nil];
}

- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    [webProgressIndicator stopAnimation:nil];

}

- (void)webView:(WebView *)sender 
didFailProvisionalLoadWithError:(NSError *)error 
       forFrame:(WebFrame *)frame
{
    [webProgressIndicator stopAnimation:nil];
    NSAlert *alert = [NSAlert alertWithError:error];
    [alert runModal];
    [self dismissSheet:nil];
}

#pragma mark TableView data source methods


- (int)numberOfRowsInTableView:(NSTableView *)tv
{
    return [itemNodes count];
}

- (id)tableView:(NSTableView *)tv 
objectValueForTableColumn:(NSTableColumn *)tableColumn 
            row:(int)row
{
	NSXMLNode *itemNode = [itemNodes objectAtIndex:row];
	NSString *xPath = [tableColumn identifier];
	
	return [self stringForPath:xPath ofNode:itemNode];
}

@end
