//
//  FeaturedViewController.m
//  BabyBear
//
//  Created by luke on 10-8-11.
//  Copyright 2010 Luke. All rights reserved.
//

#import "FeaturedViewController.h"

#define kCustomRowCount		7

@interface FeaturedViewController ()

@property (nonatomic, retain) RecommendCell		*tmpRecomCell;
@property (nonatomic, retain) FeatureCell		*tmpFeatureCell;
@end


@implementation FeaturedViewController

@synthesize entries, tmpRecomCell, tmpFeatureCell;

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

/*
- (void)viewDidLoad {
    [super viewDidLoad];

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}
*/

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

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	int count = [entries count];
	
	// ff there's no data yet, return enough rows to fill the screen
    if (count == 0)
	{
        return kCustomRowCount;
    }
    return count;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *RecomCellIdentifier = @"RecommendCell";
    static NSString *FeatureCellIdentifier = @"FeatureCell";
	static NSString *PlaceholderCellIdentifier = @"PlaceholderCell";
	
    // add a placeholder cell while waiting on table data
    int nodeCount = [self.entries count];
	
	if (nodeCount == 0 && indexPath.row == 0)
	{
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:PlaceholderCellIdentifier];
        if (cell == nil)
		{
            cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle
										   reuseIdentifier:PlaceholderCellIdentifier] autorelease];   
            cell.detailTextLabel.textAlignment = UITextAlignmentCenter;
			cell.selectionStyle = UITableViewCellSelectionStyleNone;
        }
		
		cell.detailTextLabel.text = NSLocalizedString(@"Loading…", nil);
		
		return cell;
    }
	
    FeatureCell *cell = (FeatureCell *)[tableView dequeueReusableCellWithIdentifier:FeatureCellIdentifier];
    if (cell == nil) {
		// UITableViewCellStyleDefault
		[[NSBundle mainBundle] loadNibNamed:@"FeatureCell" owner:self options:nil];
        cell = tmpFeatureCell;
		self.tmpFeatureCell = nil;
    }
    
    // Leave cells empty if there's no data yet
    if (nodeCount > 0) {
		
	}
    return cell;
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
    [entries release];
	[tmpRecomCell release];
	[tmpFeatureCell release];
	
    [super dealloc];
}

@end


#pragma mark -
#pragma mark xml
/*
using namespace rapidxml;
//xml_document<> doc;    // character type defaults to char
//doc.parse<0>(text);    // 0 means default parse flags

void parse()
{
	xml_document<> doc;
	
	// xml declaration
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);
	
	// root node
	xml_node<>* root = doc.allocate_node(node_element, "rootnode");
	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	root->append_attribute(doc.allocate_attribute("type", "example"));
	doc.append_node(root);
	
	// child node
	xml_node<>* child = doc.allocate_node(node_element, "childnode");
	root->append_node(child);
	
	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), doc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)
	
	std::string xml_no_indent;
	// print_no_indenting is the only flag that print() knows about
	print(std::back_inserter(xml_as_string), doc, print_no_indenting);
	// xml_no_indent now contains non-indented XML	
}


// <?xml version="1.0" encoding="utf-8"?>
// <rootnode version="1.0" type="example">
// <childnode entry="1">
// <evendeepernode attr1="cat" attr2="dog"/>
// <evendeepernode attr1="lion" attr2="wolf"/>
// </childnode>
// <childnode entry="2">
// </childnode>
// </rootnode>
 
void traverse_xml(std::string input_xml)
{
    // (input_xml contains the above XML)
	
    // make a safe-to-modify copy of input_xml
    // (you should never modify the contents of an std::string directly)
    vector<char> xml_copy(input_xml.begin(), input_xml.end());
    xml_copy.push_back('\0');
	
    // only use xml_copy from here on!
    xml_document<> doc;
    // we are choosing to parse the XML declaration
    // parse_no_data_nodes prevents RapidXML from using the somewhat surprising
    // behavior of having both values and data nodes, and having data nodes take
    // precedence over values when printing
    // >>> note that this will skip parsing of CDATA nodes <<<
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);
	
    // alternatively, use one of the two commented lines below to parse CDATA nodes, 
    // but please note the above caveat about surprising interactions between 
    // values and data nodes (also read http://www.ffuts.org/blog/a-rapidxml-gotcha/)
    // if you use one of these two declarations try to use data nodes exclusively and
    // avoid using value()
    //doc.parse<parse_declaration_node>(&xml_copy[0]); // just get the XML declaration
    //doc.parse<parse_full>(&xml_copy[0]); // parses everything (slowest)
	
    // since we have parsed the XML declaration, it is the first node
    // (otherwise the first node would be our root node)
    string encoding = doc.first_node()->first_attribute("encoding")->value();
    // encoding == "utf-8"
	
    // we didn't keep track of our previous traversal, so let's start again
    // we can match nodes by name, skipping the xml declaration entirely
    xml_node<>* cur_node = doc.first_node("rootnode");
    string rootnode_type = cur_node->first_attribute("type")->value();
    // rootnode_type == "example"
	
    // go straight to the first evendeepernode
    cur_node = cur_node->first_node("childnode")->first_node("evendeepernode");
    string attr2 = cur_node->first_attribute("attr2")->value();
    // attr2 == "dog"
	
    // and then to the second evendeepernode
    cur_node = cur_node->next_sibling("evendeepernode");
    attr2 = cur_node->first_attribute("attr2")->value();
    // now attr2 == "wolf"
}
*/

