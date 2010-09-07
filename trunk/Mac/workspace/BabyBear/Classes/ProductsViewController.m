//
//  ProductsViewController.m
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductsViewController.h"
#import "ProductDetailViewConctroller.h"

#define kCustomRowCount	7

#if !TARGET_IPHONE_SIMULATOR
#define kProductsFeedUrl @"http://localhost/babybear/0_products/product-01.xml"
#else
#define kProductsFeedUrl @"http://localhost/~luke/babybear/0_products/product-01.xml"
#endif

@interface ProductsViewController ()
//@property (nonatomic, retain) ProductCell	*tmpProductCell;
@property (nonatomic, assign) BOOL			isProductsFetched;
@property (nonatomic, assign) NSIndexPath	*indexSelected;

- (void)startImgDownload:(Product *)aProduct forIndexPath:(NSIndexPath *)indexPath;
@end


@implementation ProductsViewController


#pragma mark -
#pragma mark Initialization

/*
- (id)initWithStyle:(UITableViewStyle)style {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if ((self = [super initWithStyle:style])) {
    }
    return self;
}
*/


#pragma mark -
#pragma mark View lifecycle

@synthesize tableView, dataSource;
@synthesize isProductsFetched;

- (id)initWithDataSource:(Class)aDataSourceClass
{
	if ([self init]) {
		tableView = nil;
		
		id<ProductsDatasource, UITableViewDataSource> aDataSource = [[aDataSourceClass alloc] init];
		aDataSource.controller = self;
		self.dataSource = aDataSource;
		[aDataSource release];
		
		//self.productTypeArr = [NSMutableArray array];
		self.title = NSLocalizedString(@"Products", nil);
		//self.tableView.delegate = self;
		//self.dataSource = dataSource;
		
		//UIImage* anImage = [UIImage imageNamed:@"MyViewControllerImage.png"];
		UITabBarItem* barItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"Products", nil) image:nil tag:4561];
		self.tabBarItem = barItem;
		[barItem release];
	}
	return self;
}

- (void)loadView
{
	UITableView *table = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] 
													  style:[dataSource tableViewStyle]];
	
	// set the tableview as the controller view
    self.tableView = table;
	// set the tableview delegate to this object and the datasource to the datasource which has already been set
	self.tableView.delegate = self;
	self.tableView.dataSource = dataSource;
	// set the autoresizing mask so that the table will always fill the view
	self.tableView.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight);
	
	// set the cell separator to a single straight line.
	self.tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
	self.tableView.separatorColor = [UIColor redColor];
	//self.tableView.sectionFooterHeight = 20;
	//self.tableView.sectionHeaderHeight = 20;
	
	// 造成index view不显示
	//self.tableView.sectionIndexMinimumDisplayRowCount = 10;
	
	//[self.tableView reloadData];
	self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	self.view.backgroundColor = [UIColor redColor];
	CGRect newframe = self.tableView.frame;
	newframe.origin.y = 0;
	[self.tableView setFrame:newframe];
	[self.view addSubview:self.tableView];
	[self.view release];
	[table release];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    //self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
	self.imageDownloadsInProgress = [NSMutableDictionary dictionary];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	if (self.indexSelected) {
		[self.tableView deselectRowAtIndexPath:self.indexSelected animated:YES];
	}
	
	// force to reload
	//[tableView reloadSectionIndexTitles];
	[tableView reloadData];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
	
	[self.tableView flashScrollIndicators];
	if (!isProductsFetched) {
		//self.products = [NSMutableArray array];
		
		NSURLRequest *urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:kProductsFeedUrl]];
		self.productsFeedConnection = [[[NSURLConnection alloc] initWithRequest:urlRequest delegate:self] autorelease];
		
		// malformed URL
		NSAssert(self.productsFeedConnection != nil, @"Failure to create URL connection.");
		
		[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	}
}

/*
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
*/

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
	
	isProductsFetched = YES;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


#pragma mark -
#pragma mark Table view data source
/*
@synthesize productTypeArr;
@synthesize tmpProductCell;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [products count];
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	int count = 0;
	switch (section) {
		case 0:
			
			break;
		default:
			break;
	}
 
//	int count = [products count];
//	
//	// return enough rows to fill the screen
//    if (count == 0) {
//        return kCustomRowCount;
//    }
//    return count;
 
    return count;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)aTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *ProductCellIdentifier = @"ProductCell";
	static NSString *PlaceholderCellIdentifier = @"PlaceholderCell";
	
    // add a placeholder cell while waiting on table data
    int nodeCount = [self.products count];
	if (nodeCount == 0 && indexPath.row == 0) {
        UITableViewCell *cell = [aTableView dequeueReusableCellWithIdentifier:PlaceholderCellIdentifier];
        if (cell == nil) {
            cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle
										   reuseIdentifier:PlaceholderCellIdentifier] autorelease];   
            cell.detailTextLabel.textAlignment = UITextAlignmentCenter;
			cell.selectionStyle = UITableViewCellSelectionStyleNone;
        }
		
		cell.detailTextLabel.text = @"Loading…";
		
		return cell;
    }
	
    ProductCell *cell = (ProductCell *)[aTableView dequeueReusableCellWithIdentifier:ProductCellIdentifier];
	if (cell == nil) {
		NSLog(@"create ProductCell");
		[[NSBundle mainBundle] loadNibNamed:@"ProductCell" owner:self options:nil];
        cell = tmpProductCell;
		self.tmpProductCell = nil;
		cell.selectionStyle = UITableViewCellSelectionStyleGray;
    }
	
    // Leave cells empty if there's no data yet
    if (nodeCount > 0) {
		
		// Set up the cell...
		NSArray *aTypeProducts = [self.products objectForKey:[NSNumber numberWithInt:indexPath.section]];
		Product *aProduct = [aTypeProducts objectAtIndex:indexPath.row];
		cell.textLabel.text = aProduct.pname;
        cell.detailTextLabel.text = aProduct.pdesc;
		
        // Only load cached images; delay new downloads until scrolling ends
        if (!aProduct.productIcon) {
            if (self.tableView.dragging == NO && self.tableView.decelerating == NO) {
                [self startImgDownload:aProduct forIndexPath:indexPath];
            }
            // if a download is deferred or in progress, return a placeholder image
            cell.imageView.image = [UIImage imageNamed:@"Placeholder.png"];                
        }
        else {
			cell.imageView.image = aProduct.productIcon;
        }
		// UITableViewCellAccessoryDetailDisclosureButton
		// UITableViewCellAccessoryCheckmark
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
	
    return cell;
}


#pragma mark index

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView
{
	return productTypeArr;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return [productTypeArr objectAtIndex:section];
}
*/
#pragma mark Table view delegate

@synthesize indexSelected;

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
	BaseProduct *aProduct = [dataSource productForIndexPath:indexPath];
	ProductDetailViewConctroller *detailViewController = [[ProductDetailViewConctroller alloc] 
														  initWithProduct:aProduct];
	if (detailViewController) {
		[self.navigationController pushViewController:detailViewController animated:YES];
		[detailViewController release];
	}
	self.indexSelected = indexPath;
}


#pragma mark -
#pragma mark Downloading Feed


@synthesize queue, productsFeedConnection, productsData;

- (void)handleError:(NSError *)aErr
{
    NSString *errMsg = [aErr localizedDescription];
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Cannot Show Products"
														message:errMsg
													   delegate:nil
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
    [alertView show];
    [alertView release];
}

- (void)handleLoadedProducts:(NSDictionary *)loadedProducts
{
    //[self.products addObjectsFromArray:loadedProducts];
    
    // tell our table view to reload its data, now that parsing has completed
    [self.tableView  reloadData];
}

#pragma mark NSURLConnection delegate methods

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    self.productsData = [NSMutableData data];    // start off with new data
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [productsData appendData:data];  // append incoming data
	NSLog(@"received productsData: %d", [productsData length]);
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    
    if ([error code] == kCFURLErrorNotConnectedToInternet)
	{
        // if we can identify the error, we can present a more precise message to the user.
        NSDictionary *userInfo = [NSDictionary dictionaryWithObject:@"No Connection Error"
															 forKey:NSLocalizedDescriptionKey];
        NSError *noConnectionError = [NSError errorWithDomain:NSCocoaErrorDomain
														 code:kCFURLErrorNotConnectedToInternet
													 userInfo:userInfo];
        [self handleError:noConnectionError];
    }
	else
	{
        // otherwise handle the error generically
        [self handleError:error];
    }
    
    self.productsFeedConnection = nil;   // release our connection
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    self.productsFeedConnection = nil;   // release our connection
    
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;   
    
    // create the queue to run our ParseOperation
    self.queue = [[NSOperationQueue alloc] init];
    
    // create an ParseOperation (NSOperation subclass) to parse the RSS feed data so that the UI is not blocked
    // "ownership of productsData has been transferred to the parse operation and should no longer be
    // referenced in this thread.
    //
    XmlParseOperation *parser = [[XmlParseOperation alloc] initWithData:productsData delegate:self];
    
    [queue addOperation:parser]; // this will start the "ParseOperation"
    
    [parser release];
	[queue release];
    
    // ownership of productsData has been transferred to the parse operation
    // and should no longer be referenced in this thread
    self.productsData = nil;
}



#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc
{
	[dataSource release];
	
	[queue release];
	
	[productsFeedConnection release];
	[productsData release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark Icon photos support

@synthesize imageDownloadsInProgress;

- (void)startImgDownload:(Product *)aProduct forIndexPath:(NSIndexPath *)indexPath
{
    ImageDownloader *imgDownloader = [imageDownloadsInProgress objectForKey:indexPath];
    if (imgDownloader == nil) {
        imgDownloader = [[ImageDownloader alloc] init];
        imgDownloader.product = aProduct;
        imgDownloader.indexPathInTableView = indexPath;
        imgDownloader.delegate = self;
        [imageDownloadsInProgress setObject:imgDownloader forKey:indexPath];
        [imgDownloader startDownload:DT_PRODUCT_ICON imgIndex:-1];
        [imgDownloader release];
    }
}

// this method is used in case the user scrolled into a set of cells that don't have their app icons yet
- (void)loadImagesForOnscreenRows
{
	int count = [dataSource allDatasCount];
    if (count > 0) {
        NSArray *visiblePaths = [self.tableView indexPathsForVisibleRows];
        for (NSIndexPath *indexPath in visiblePaths) {
            Product *aProduct = (Product *)[dataSource productForIndexPath:indexPath];
            
            if (!aProduct.productIcon) {
				// avoid the app icon download if the app already has an icon
                [self startImgDownload:aProduct forIndexPath:indexPath];
            }
        }
    }
}

// called by our ImageDownloader when an icon is ready to be displayed
- (void)imageDidLoadWithIndexPath:(NSIndexPath *)indexPath
{
    ImageDownloader *imgDownloader = [imageDownloadsInProgress objectForKey:indexPath];
    if (imgDownloader != nil) {
        UITableViewCell *cell = [dataSource tableView:tableView cellForRowAtIndexPath:imgDownloader.indexPathInTableView];
        
        // Display the newly loaded image
        cell.imageView.image = imgDownloader.product.productIcon;
		[tableView reloadData];
    }
}

#pragma mark -
#pragma mark Delayed image loading (UIScrollViewDelegate)

// Load images for all onscreen rows when scrolling is finished
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    if (!decelerate) {
        [self loadImagesForOnscreenRows];
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    [self loadImagesForOnscreenRows];
}

#pragma mark -
#pragma mark xml parse


- (void)xmlWillParse
{
	
}

- (void)xmlParsing:(int)aState withErr:(NSError *)aErr
{
	
}

- (void)xmlDidFinishParsing:(NSDictionary *)aProductList
{
    [self performSelectorOnMainThread:@selector(handleLoadedProducts:) withObject:aProductList waitUntilDone:NO];
    
    self.queue = nil;   // we are finished with the queue and our ParseOperation
}

- (void)xmlParseErrOccurred:(NSError *)aErr
{
    [self performSelectorOnMainThread:@selector(handleError:) withObject:aErr waitUntilDone:NO];
}

@end

