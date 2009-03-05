/**
 * 
 */
package gr.fire.browser.util;


import gr.fire.browser.Browser;

import java.util.Hashtable;

/**
 * 
 * A PageListener registered to a Browser instance will be notified upon completion of requests to Browser.loadPageAsync.
 * @see Browser#loadPageAsync(String, String, Hashtable, byte[])
 * @author padeler
 *
 */
public interface PageListener
{
	/**
	 * The rendering of a page was completed. The result is in the Page instance.
	 * @param url The usr that was used when the request was made. This may not be the same with the url in the Page parameter since there may have been redirects.
	 * @param method The method that was used for the Http request
	 * @param requestParams The parameters that where send with the request. 
	 * @param page The result of the request.
	 */
	public void pageLoadCompleted(String url,String method,Hashtable requestParams, Page page);
	
	/**
	 * The loadPageAsync failed with an exception. A callback to the PageListener is made with the error.
	 * @param url
	 * @param method
	 * @param requestParams
	 * @param error
	 */
	public void pageLoadFailed(String url,String method,Hashtable requestParams, Throwable error);	
	
}
