
#import "LuHttpRequest.h"
#if TARGET_OS_IPHONE
#import "Reachability.h"
#import <MobileCoreServices/MobileCoreServices.h>
#else
#import <SystemConfiguration/SystemConfiguration.h>
#endif
//#import "LuInputStream.h"


NSString* const LuHttpErrorDomain = @"LuHttpErrorDomain";

static NSString *LuHttpRequestRunLoopMode = @"LuHttpRequestRunLoopMode";

static const CFOptionFlags kNetworkEvents = kCFStreamEventOpenCompleted 
| kCFStreamEventHasBytesAvailable 
| kCFStreamEventEndEncountered 
| kCFStreamEventErrorOccurred;

// In memory caches of credentials, used on when useSessionPersistence is YES
static NSMutableArray *sessionCredentialsStore = nil;
static NSMutableArray *sessionProxyCredentialsStore = nil;

// This lock mediates access to session credentials
static NSRecursiveLock *sessionCredentialsLock = nil;

// We keep track of cookies we have received here so we can remove them from the sharedHTTPCookieStorage later
static NSMutableArray *sessionCookies = nil;

// The number of times we will allow requests to redirect before we fail with a redirection error
const int RedirectionLimit = 5;

// The default number of seconds to use for a timeout
static NSTimeInterval defaultTimeOutSeconds = 10;