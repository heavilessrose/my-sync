
@class LuHttpRequest;

@protocol LuHttpRequestDelegate <NSObject>

@optional

// These are the default delegate methods for request status
// You can use different ones by setting didStartSelector / didFinishSelector / didFailSelector
- (void)requestStarted:(LuHttpRequest *)request;
- (void)requestReceivedResponseHeaders:(LuHttpRequest *)request;
- (void)requestFinished:(LuHttpRequest *)request;
- (void)requestFailed:(LuHttpRequest *)request;

// When a delegate implements this method, it is expected to process all incoming data itself
// This means that responseData / responseString / downloadDestinationPath etc are ignored
// You can have the request call a different method by setting didReceiveDataSelector
- (void)request:(LuHttpRequest *)request didReceiveData:(NSData *)data;

// If a delegate implements one of these, it will be asked to supply credentials when none are available
// The delegate can then either restart the request ([request retryUsingSuppliedCredentials]) once credentials have been set
// or cancel it ([request cancelAuthentication])
- (void)authenticationNeededForRequest:(LuHttpRequest *)request;
- (void)proxyAuthenticationNeededForRequest:(LuHttpRequest *)request;

@end
