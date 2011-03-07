//
//  xmlTestViewController.m
//  xmlTest
//
//  Created by luke on 11-3-2.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "xmlTestViewController.h"

#import <stdio.h>
#import <libxml/tree.h>
#import <libxml/parser.h>
#import <libxml/HTMLparser.h>
#import <libxml/xpath.h>
#import <libxml/xpathInternals.h>

@implementation xmlTestViewController

- (NSString *)docPath {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];
	return documentsDirectoryPath;
}

static void traverse_property(xmlAttrPtr a_attr, NSMutableDictionary *nodeDic) {
	
	xmlAttrPtr cur_attr = a_attr;
	NSString *attrKey = nil;
	NSString *attrValue = nil;
	NSMutableDictionary *attrDic = [NSMutableDictionary dictionary];
	while (cur_attr) {
		if (cur_attr->children->type == XML_TEXT_NODE) {
			printf("\tATTRIBUTE: %s\t%s\n", cur_attr->name, cur_attr->children->content);
			attrKey = [NSString stringWithCString:(const char *)cur_attr->name encoding:NSUTF8StringEncoding];
			attrValue = [NSString stringWithCString:(const char *)cur_attr->children->content encoding:NSUTF8StringEncoding];
			[attrDic setObject:attrValue forKey:attrKey];
		}
		cur_attr = cur_attr->next;
	}
	if ([attrDic count] > 0) {
		[nodeDic setObject:attrDic forKey:@"attr"];
	}
}

static void traverse_element(xmlNodePtr a_node, NSMutableDictionary *pNodeDic) {
	
	xmlNodePtr cur_node = NULL;
	xmlNodePtr child = NULL;
	xmlAttrPtr cur_attr = NULL;
	if (!a_node) {
		return;
	}
	
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		NSMutableDictionary *nodeDic = [NSMutableDictionary dictionary];
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("Element:   %s\n", cur_node->name);
			[pNodeDic setObject:nodeDic forKey:[NSString stringWithCString:(const char *)cur_node->name encoding:NSUTF8StringEncoding]];
			if (cur_attr = cur_node->properties) {
				traverse_property(cur_attr, nodeDic);
			}
        } else if (cur_node->type == XML_TEXT_NODE){
			printf("\tTEXT:	   %s\n", cur_node->content);
			[pNodeDic setObject:[NSString stringWithCString:(const char *)cur_node->content encoding:NSUTF8StringEncoding] forKey:[NSString stringWithCString:(const char *)cur_node->name encoding:NSUTF8StringEncoding]];
		} else {
			printf("errXXXXXXX type: %d, Element: %s", cur_node->type, cur_node->name);
		}
		child = cur_node->children;
		if (child) {
			printf("---> %s\n", child->name);
		}
        traverse_element(child, nodeDic);
    }
}

static xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *xpath) {
	
    xmlXPathObjectPtr result;
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
	
    if (context == NULL) {
		printf("context is NULL\n");
		return NULL;
    }
    result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (result == NULL) {
		printf("xmlXPathEvalExpression return NULL\n");
		return NULL;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		printf("nodeset is empty\n");
		return NULL;
    }
    return result;
}

WBXmlErr doxpath(xmlDocPtr doc, NSString *xpathStr, NSMutableArray *results) {
	
	xmlNodePtr cur = NULL;
	if (doc) {
		const xmlChar *xpath = (const xmlChar *)[xpathStr UTF8String];
		xmlXPathObjectPtr res = get_nodeset(doc, xpath);
		
		if (res) {
			xmlNodeSetPtr nodeset = res->nodesetval;
			for (int i = 0; i < nodeset->nodeNr; i++) {
				cur = nodeset->nodeTab[i];
				printf("######%s---%d\n", cur->name, i);
				NSMutableDictionary *nodeDic = [NSMutableDictionary dictionary];
				traverse_property(cur->properties, nodeDic);
				cur = cur->xmlChildrenNode;
				traverse_element(cur, nodeDic);
				[results addObject:nodeDic];
			}
			xmlXPathFreeObject(res);
			return WBXML_XPATH_OK;
		}
		return WBXML_EMPTY_SET;
	}
	return WBXML_INVALIAD_DOC;
}

WBXmlErr writeToXml(NSArray *nodeArr) {
	
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, node = NULL;
	
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);
	
	for (NSDictionary *nodeDic in nodeArr) {
		
	}
}

/*
 - (void)xmlDocCreat:(NSDictionary *)dict {
 
 // create doc  
 xmlDocPtr doc = NULL;
 xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
 
 doc = xmlNewDoc(BAD_CAST "1.0");
 root_node = xmlNewNode(NULL, BAD_CAST "root");
 xmlDocSetRootElement(doc, root_node);
 
 // add child node to root_node
 xmlNewChild(root_node, NULL, BAD_CAST "node1",BAD_CAST "content of node1");
 
 // add child node with attribs
 node=xmlNewChild(root_node, NULL, BAD_CAST "node3", BAD_CAST"node has attributes");
 xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
 
 // another way to create nodes.
 node = xmlNewNode(NULL, BAD_CAST "node4");
 node1 = xmlNewText(BAD_CAST"other way to create content");
 xmlAddChild(node, node1);
 xmlAddChild(root_node, node);
 
 //　Dumping document to stdio or file
 NSString *filePath = [self docPath];
 NSString *file = [filePath stringByAppendingPathComponent:@"test.xml"];
 NSLog(@"%@", file);
 xmlSaveFormatFileEnc([file UTF8String], doc, "UTF-8", 1);
 //xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
 
 // free the document
 xmlFreeDoc(doc);
 xmlCleanupParser();
 xmlMemoryDump();//debug memory for regression tests
 }
 
- (void)xmlParse:(const char *)xmlfile {
	
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	doc = xmlReadFile(xmlfile, "UTF-8", 256);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
        return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "catalog")) {
		fprintf(stderr,"document of the wrong type, root node != catalog");
        xmlFreeDoc(doc);
        return;
	}
	
	//traverse_element(cur);
	xmlFreeDoc(doc);
}
*/


- (void)xmlTests {
	
	NSString *filePath = [[NSBundle mainBundle] pathForResource:@"doc1" ofType:@"xml"];
	const char *xmlfile = [filePath UTF8String];
	xmlDocPtr doc = xmlReadFile(xmlfile, "UTF-8", 256);
	//	[self xmlDocCreat:nil];
	//	[self xmlParse:xmlfile];
	
	NSString *xpath = @"/catalog/journal/article";
	NSMutableArray *results = [NSMutableArray array];
	doxpath(doc, xpath, results);
	NSLog(@"------------------------------------\n%@", results);
}
#pragma mark -

/*
 // The designated initializer. Override to perform setup that is required before the view is loaded.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
 if (self) {
 // Custom initialization
 }
 return self;
 }
 */

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	[self xmlTests];
}


/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
