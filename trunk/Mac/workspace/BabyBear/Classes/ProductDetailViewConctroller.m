//
//  ProductDetailViewConctroller.m
//  BabyBear
//
//  Created by luke on 10-8-20.
//  Copyright 2010 Luke. All rights reserved.
//

#import "ProductDetailViewConctroller.h"
#import "TapImage.h"
#import "Utils.h"


@interface ProductDetailViewConctroller ()

@property (nonatomic, retain) UIScrollView			*scrollView;
@property (nonatomic, assign) BaseProduct			*product;
@property (nonatomic, retain) NSMutableDictionary	*imageDownloadsInProgress;
@property (nonatomic, retain) NSMutableDictionary	*downloadedPreImgs;
@property (nonatomic, retain) ProductInfoView		*infoView;
@property (nonatomic, retain) ScrollShowView		*scrollshow;
@property (nonatomic, retain) UITableView			*tableView;
@end


@implementation ProductDetailViewConctroller

#pragma mark -
#pragma mark View lifecycle
@synthesize product, scrollshow, infoView, scrollView;

- (id)initWithProduct:(BaseProduct *)aProduct
{
	if (self = [super init]) {
		
		self.title = NSLocalizedString(@"ProductDetail", nil);
		self.product = aProduct;
		self.imageDownloadsInProgress = [NSMutableDictionary dictionary];
		self.downloadedPreImgs = [NSMutableDictionary dictionary];
	}
	return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
	self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, 320, 460)];
	scrollView.backgroundColor = [UIColor grayColor];
	//[scrollView setContentSize:CGSizeMake(320, 1000)];
	
	CGRect ProductInfoRect = CGRectMake(0, 0, 0, 0);
	NSString *details = @"asdfasdfasdfasdfadsfds asdf46asd4f98 as6df4ads8f7 asd6f54asdf sd65f4a";
	ProductInfoRect = [ProductInfoView calcFrame:details frame:ProductInfoRect];
	self.infoView = [[ProductInfoView alloc] initWithDetailInfo:details andFrame:ProductInfoRect];
	[scrollView addSubview:infoView];
	[infoView release];
	
    CGRect scrollRect = CGRectMake(0, ProductInfoRect.size.height, 320, 100);
    CGSize pageContentSize = CGSizeMake(80, 80);
	self.scrollshow = [[ScrollShowView alloc] initWithFrame:scrollRect pageContentSize:pageContentSize pageDelegate:self];
	scrollshow.backgroundColor = [UIColor darkGrayColor];
    scrollshow.pageStyle = PAGESTYLE_PADDING;
    scrollshow.x_padding = 10.0f;
    scrollshow.y_padding = 10.0f;
	//[self.view addSubview:scrollshow];
	[scrollView addSubview:scrollshow];
	
	CGRect tableRect = CGRectMake(0, ProductInfoRect.size.height + 100, 320, 300);
	self.tableView = [[UITableView alloc] initWithFrame:tableRect style:UITableViewStyleGrouped];
	[scrollView addSubview:tableView];
	
	[scrollView setContentSize:CGSizeMake(320, ProductInfoRect.size.height + 100 + 300)];
	[self.view addSubview:scrollView];
	[scrollView release];
}

/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/
/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}
*/
/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


#pragma mark -
#pragma mark Table view data source

@synthesize tableView;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return 7;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *ScrollShowCellID = @"ScrollShowCell";
	UITableViewCell *sCell = nil;
	sCell = [self.tableView dequeueReusableCellWithIdentifier:ScrollShowCellID];
	if (sCell == nil) {
		sCell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ScrollShowCellID] autorelease];
	}

    return sCell;
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // Navigation logic may go here. Create and push another view controller.
	/*
	 <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
	 [self.navigationController pushViewController:detailViewController animated:YES];
	 [detailViewController release];
	 */
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


- (void)dealloc {
	[imageDownloadsInProgress release];
	[downloadedPreImgs release];
	[infoView release];
	[scrollshow release];
	[scrollView release];
	
	[product.productImgs removeAllObjects];
	
    [super dealloc];
}

#pragma mark -
#pragma mark ImageDownloader support

@synthesize imageDownloadsInProgress, downloadedPreImgs;

- (void)startImgDownload:(NSNumber *)index
{
    ImageDownloader *imgDownloader = [imageDownloadsInProgress objectForKey:index];
    if (imgDownloader == nil) {
        imgDownloader = [[ImageDownloader alloc] init];
        imgDownloader.product = self.product;
        imgDownloader.index = [index intValue];
        imgDownloader.delegate = self;
        [imageDownloadsInProgress setObject:imgDownloader forKey:index];
        [imgDownloader startDownload:DT_PRODUCT_IMG imgIndex:[index intValue]];
        [imgDownloader release];
    }
}

- (void)imageDidLoadWithIndex:(int)aIndex
{
	/*
	NSNumber *aIndexKey = [NSNumber numberWithInt:aIndex];
    ImageDownloader *imgDownloader = [imageDownloadsInProgress objectForKey:aIndexKey];
    if (imgDownloader != nil) {
		TapImage *ImgViewAtPage = [self.product.productImgs objectAtIndex:aIndex];
		NSLog(@"%@", ImgViewAtPage);
		//ImgViewAtPage.image = [imgDownloader.product.productImgs objectAtIndex:aIndex];
		
		UIImage *ii = [UIImage imageNamed:@"icon.png"];
		
		
		[Utils writeData:UIImagePNGRepresentation(ii) toFile:@"ii" type:FILE_IMG replaceExists:YES];
		
//		TapImage *mv = [[TapImage alloc] initWithImage:ii];
//		[self.view addSubview:mv];
//		[mv release];
	}
	 */
}

#pragma mark -
#pragma mark ScrollShowView delegate methods

- (UIView *)viewForPageAtIndex:(ScrollShowView *)scrollView pageIndex:(int)index
{
    CGRect ImgViewAtPageRect = CGRectMake(0, 0, 80, 80.0f);
    TapImage *ImgViewAtPage = [[[TapImage alloc] initWithFrame:ImgViewAtPageRect] autorelease];
	ImgViewAtPage.userInteractionEnabled = YES;
	//ImgViewAtPage.image = [downloadedPreImgs objectForKey:[NSNumber numberWithInt:index]];
	ImgViewAtPage.image = nil;
	ImgViewAtPage.index = index;
	[product.productImgs addObject:ImgViewAtPage];
	[self startImgDownload:[NSNumber numberWithInt:index]];
    return ImgViewAtPage;
}

- (int)itemCount:(ScrollShowView *)scrollView
{
	return [self.product.pgallary count];
}

@end

