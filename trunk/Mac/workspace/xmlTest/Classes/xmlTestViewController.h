//
//  xmlTestViewController.h
//  xmlTest
//
//  Created by luke on 11-3-2.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
	WBXML_INVALIAD_DOC = -0x50,
	WBXML_EMPTY_SET,
	WBXML_WRITE_FAIL,
	WBXML_WRITE_OK = 0,
	WBXML_XPATH_OK = 0,
} WBXmlErr;

@interface xmlTestViewController : UIViewController {

}

@end

