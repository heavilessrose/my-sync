package com.pica.ui.tools;

import java.io.DataInputStream;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Message;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.HttpAuthHandler;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.ScrollView;

import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.R;
import com.pica.msn.ZipwapParser;

public class ZwpView extends LinearLayout{
	MSNApplication app;
	public LabelView lab;
	public SessionList slist;
	String tempHtml;
	public WebView wv;
	private StringBuffer buf;
	private StringBuffer oldBuf;
	ScrollView recItem;
	public ZipwapParser zParser;
	private ZwpToolBarView tool;
	
	/**是否需要保存页面的html文本*/
	public boolean isNeedSave = false;
	/**是否需要清除缓存*/
	boolean isNeedRemove = false;
	
	/**当前索引*/
	public int currentIndex=-1;
	private int size = 0;
	public ZwpView(Context context)
	{
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) 
    	{
    		return;
    	}
		setOrientation(LinearLayout.VERTICAL);
		setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT));
		init();
	}
	
	private void init()
	{         
		setGravity(Gravity.CENTER_HORIZONTAL);
		LayoutInflater inflate = (LayoutInflater)getContext().getSystemService(getContext().LAYOUT_INFLATER_SERVICE);	        
		LinearLayout recItem = (LinearLayout)inflate.inflate(R.layout.zwpview, null);
		addView(recItem);

		wv = (WebView)findViewById(R.id.wv);
		wv.getSettings().setDefaultTextEncodingName("utf-8");
		wv.getSettings().setSupportZoom(false);
		wv.getSettings().setSavePassword(false);
		wv.setMapTrackballToArrowKeys(false);   
		wv.setWebViewClient(mWebViewClient);
		
		wv.getSettings().setSupportZoom(true);
		wv.getSettings().setBuiltInZoomControls(true);
		
        if(app.screenScale == 1 || app.screenScale == 1.5)
        {
            wv.getSettings().setDefaultFontSize(20);
        }
        else if(app.screenScale == 0.75)
        {
        	wv.getSettings().setDefaultFontSize(14);
        }
		
		wv.requestFocus();
        zParser = new ZipwapParser();
        buf = new StringBuffer();
        oldBuf = new StringBuffer();
        
        tool = (ZwpToolBarView)findViewById(R.id.tool);
        tool.btn1.setOnClickListener(mHomeListener);
        tool.btn2.setOnClickListener(mSplashListener);
	}
	
    private OnClickListener mHomeListener = new OnClickListener() {
        public void onClick(View v) {
        	//如果当前页面不是主页面，则把当前页面url,页面的html文本加入缓存向量
        	if(app.currentUrl != app.zwp_homePageAddress)
        	{
	            isNeedSave = true;
        	}
        	else
        	{
        		isNeedSave = false;
        	}
        	isNeedRemove = false;
        	
        	zParser.tempBaseUrl = app.getBaseUrl(app.zwp_homePageAddress);
            app.tempUrl = app.zwp_homePageAddress;
            
            LoadZwp();
        	app.bZwpLoad = true;
        	app.bZwpDialog = true;
            app.jabber.sendZipwapRequest(app.zwp_homePageAddress, null, (byte)1, app.myVCARD_myNickname, app.agent); 
            
            
        }
    };
    
    private OnClickListener mSplashListener = new OnClickListener() {
        public void onClick(View v) {
            isNeedSave = false;
            zParser.tempBaseUrl = app.currentUrl;
            LoadZwp();
            app.bZwpDialog = true;
            app.bZwpLoad = true;
            app.jabber.sendZipwapRequest(app.currentUrl, null, (byte)1, app.myVCARD_myNickname, app.agent);
            
        }
    };
    
    private OnClickListener mBackListener = new OnClickListener() {
        public void onClick(View v) {
        	app.bZwpLoad = true;
        	app.bZwpDialog = false;
        	goBack();
        }
    };
	
	public void splash(DataInputStream input, int len)
	{
        try
        {
        	zParser.htmlString.delete(0, zParser.htmlString.length());
			zParser.parseElement(input, len);
		}
        catch (Exception e) 
        {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
		buf.delete(0, buf.length());
		buf.append("<html><header><meta name=viewport width=320; content=\"text/html; charset=gb2312\"/></header><body>");
		buf.append(zParser.htmlString.toString());
		buf.append("</body></html>");
		//点击"取消后"不加载页面
        if(!app.bZwpLoad)
   		{
   			return;
   		}
        if(zParser.htmlString.length() == 0)
        {
        	return;
        }
        wv.requestFocus();
		wv.loadDataWithBaseURL(null, buf.toString(), "text/html", "utf-8", null);
		
		//在当前页面加载成功后才把上一个页面保存到向量中,此时oldBuf是上一个页面的html文本
		if(isNeedSave)
		{
			isNeedSave = false;
			//缓存六个页面
			if(size >= 6)
			{
				if(app.tempSave)
				{
					app.htmlStrings.insertElementAt(oldBuf.toString(), 0);
				}
				else
				{
					app.htmlStrings.insertElementAt("NoCache", 0);
				}
				app.htmlStrings.setSize(6);
			}
			if(size < 6)
			{
				if(app.tempSave)
				{
					app.htmlStrings.insertElementAt(oldBuf.toString(), 0);
				}
				else
				{
					app.htmlStrings.insertElementAt("NoCache", 0);
				}
				size = size + 1;
			}
			//页面更新成功，保存baseUrl和currentUrl
            app.baseUrls.addElement(app.baseUrl);
            app.urls.addElement(app.currentUrl);            
		}
		oldBuf.delete(0, oldBuf.length());
		oldBuf.append(buf.toString());
		
		if(isNeedRemove)
		{
			isNeedRemove = false;
			if(app.baseUrls.size() != 0)
			{
				app.baseUrls.remove(app.baseUrls.size()-1);					
			}				
			if(app.urls.size() != 0)
			{
				app.urls.remove(app.urls.size()-1);					
			}			
			if(app.htmlStrings.size() != 0)
			{
				app.htmlStrings.remove(0);
				size = size -1;
				app.htmlStrings.setSize(size);
			}
		}
		
		//更新全局变量
		if(app.tempUrl != null)
		{
			app.currentUrl = app.tempUrl;
			app.baseUrl = app.getBaseUrl(app.tempUrl);		
		}
		
	}
	
	public void goBack()
	{
		if(app.bZwpDialog)
		{
			CancelZwp();
			app.bZwpDialog = false;
			app.bZwpLoad = false;
		}
		else
		{
			if(app.htmlStrings.size() != 0)
			{
				//取出向量的第一个对象（上一个页面的HTML文本）进行加载
				tempHtml = (String) app.htmlStrings.elementAt(0);
				//如果是页面属性是NoCache，则使用url请求页面
				if(tempHtml.equals("NoCache"))
				{					
					app.tempUrl = (String) app.urls.elementAt(app.urls.size()-1);
					zParser.tempBaseUrl = app.getBaseUrl(app.tempUrl);
					//后退操作不需要保存
					isNeedSave = false;
					//需要在splash()里清除缓存
					isNeedRemove = true;
					
					LoadZwp();
					app.bZwpDialog = true;
		            app.bZwpLoad = true;					
					app.jabber.sendZipwapRequest(app.tempUrl, null, (byte)1, app.myVCARD_myNickname, app.agent);
				}
				else
				{
					wv.loadDataWithBaseURL(null, tempHtml, "text/html", "utf-8", null);
					app.jabber.noCache = false;
					app.htmlStrings.remove(0);
					size = size -1;
					app.htmlStrings.setSize(size);
					//加载成功才更新全局变量
					if(app.baseUrls.size() != 0)
					{
						app.baseUrl = (String) app.baseUrls.remove(app.baseUrls.size()-1);					
					}				
					if(app.urls.size() != 0)
					{
						app.currentUrl = (String) app.urls.remove(app.urls.size()-1);					
					}
					//把取出的HTML文本保存到buf中，以便点击子链接后把当前页面存入缓存
					//因为没有进入splash函数，所以需要对buf进行处理
					oldBuf.delete(0, oldBuf.length());
					oldBuf.append(tempHtml);
				}
			}
			//如果无页面缓存，则使用URL进行刷新
			else
			{
				if( (app.baseUrls.size() != 0) && (app.urls.size() != 0) )
				{
					app.tempUrl = (String) app.urls.elementAt(app.urls.size()-1);
					zParser.tempBaseUrl = app.getBaseUrl(app.tempUrl);
					isNeedSave = false;
					isNeedRemove = true;
					LoadZwp();
					app.bZwpDialog = true;
		            app.bZwpLoad = true;
					app.jabber.sendZipwapRequest(app.tempUrl, null, (byte)1, app.myVCARD_myNickname, app.agent);
				}				
			}
		}	
	}
	
 	private final WebViewClient mWebViewClient = new WebViewClient() {
        @Override
        public void onLoadResource(WebView view, String url) {
        	//Log.e("onLoadResource", "1111111111111111111111111111");
        }
        
        public void onFormResubmission (WebView view, Message dontResend, Message resend) 
        {
        	//Log.e("onFormResubmission", "1111111111111111111111111111");
        }
        
        public void onPageFinished(WebView view, String url) 
        {
        	app.bZwpDialog = false;
        	app.bZwpLoad = true;
        	CancelZwp();
        	return;
        }
        
        public void onPageStarted (WebView view, String url, Bitmap favicon)
        {
        	//Log.e("onPageStarted", "1111111111111111111111111111");
        }
        
        public void doUpdateVisitedHistory (WebView view, String url, boolean isReload) 
        {
        	//Log.e("doUpdateVisitedHistory", "1111111111111111111111111111");
        }
        
        public void onReceivedHttpAuthRequest (WebView view, HttpAuthHandler handler, String host, String realm) 
        {
        	//Log.e("onReceivedHttpAuthRequest", "1111111111111111111111111111");
        }
        
        public void onScaleChanged (WebView view, float oldScale, float newScale) 
        {
        	//Log.e("onScaleChanged", "1111111111111111111111111111");
        }
        
        public void onTooManyRedirects (WebView view, Message cancelMsg, Message continueMsg) 
        {
        	//Log.e("onTooManyRedirects", "1111111111111111111111111111");
        }
        
        public void onUnhandledKeyEvent (WebView view, KeyEvent event)
        {
        	//Log.e("onUnhandledKeyEvent", "1111111111111111111111111111");
        }
        
        public boolean shouldOverrideKeyEvent (WebView view, KeyEvent event) 
        {
        	return false;
        }
        
        public boolean shouldOverrideUrlLoading(WebView view, String url) 
        {
        	String type = null;
        	String advist = null;
        	int firstIndex = url.indexOf("link***");
        	int secondIndex = -1;
        	int thirdIndex = -1;
        	
        	if(firstIndex != -1)
        	{
        		secondIndex = url.indexOf("***", firstIndex + 7);
        		type = url.substring(firstIndex + 7, secondIndex);
        		thirdIndex = url.indexOf("***", secondIndex + 3);
        		if(thirdIndex != -1)
        		{
        			advist = url.substring(secondIndex + 3, thirdIndex);
        		}
        	}
        	//截去在解析时刻意加的串，返回正确链接地址
        	if(url.lastIndexOf("***") != -1)
        	{
        		url = url.substring(url.lastIndexOf("***") + 3);
        	}
        	
        	url = app.getAbsoluteUrl(url, app.baseUrl);
        	
        	if(type != null && type.length()!= 0)
        	{
        		//0xFF  使用系统浏览器打开
        		if(type.equals("255") || type.equals("-1"))
        		{
        			Uri uri = Uri.parse(url);
                	Intent it = new Intent(Intent.ACTION_VIEW, uri);
                	getContext().startActivity(it);
                	if(advist != null && advist.length()!= 0)
                	{
                		String dbid = advist.substring(advist.indexOf("dbid='") + 6, advist.indexOf("'", advist.indexOf("dbid='") + 6));
                		String sale = advist.substring(advist.indexOf("sale='") + 6, advist.indexOf("'", advist.indexOf("sale='") + 6));
                		app.jabber.advistClick(dbid, sale);
                	}
                    return true;
        		}
        		//0xF1  立即短信
        		if(type.equals("241"))
        		{
        			////////////////////////////////////////
        			if(advist != null && advist.length()!= 0)
                	{
                		String dbid = advist.substring(advist.indexOf("dbid='"), advist.indexOf("'", advist.indexOf("dbid='") + 6));
                		String sale = advist.substring(advist.indexOf("sale='"), advist.indexOf("'", advist.indexOf("sale='") + 6));
                		app.jabber.advistClick(dbid, sale);
                	}
        			return true;

        		}
        		
        		//0xF2  编辑短信
        		if(type.equals("242"))
        		{
        			////////////////////////////////////
        			if(advist != null && advist.length()!= 0)
                	{
                		String dbid = advist.substring(advist.indexOf("dbid='"), advist.indexOf("'", advist.indexOf("dbid='") + 6));
                		String sale = advist.substring(advist.indexOf("sale='"), advist.indexOf("'", advist.indexOf("sale='") + 6));
                		app.jabber.advistClick(dbid, sale);
                	}
        			return true;
        		}
        	}
        	
            if(url.startsWith(app.SCHEME_TEL)) 
            {
            	Uri uri = Uri.parse(url);
            	Intent it = new Intent(Intent.ACTION_DIAL, uri);  
            	getContext().startActivity(it);  
                return true;
            }
            else if(url.startsWith(app.SCHEME_SMS)) 
            { 	
            	 Uri uri = Uri.parse(url);   
				 Intent it = new Intent(Intent.ACTION_SENDTO, uri);   			
				 //it.putExtra("sms_body", "The SMS text");   			
				 getContext().startActivity(it);
				 return true;
            }
            else if(url.startsWith(app.SCHEME_MAIL)) 
            {
            	Uri uri = Uri.parse(url);
            	Intent it = new Intent(Intent.ACTION_SENDTO, uri);
            	getContext().startActivity(it);
            	return true;
            }
            else
            {
	            LoadZwp();
	            app.bZwpDialog = true;
	            app.bZwpLoad = true;
	            
	            isNeedSave = true;
	            zParser.tempBaseUrl = app.getBaseUrl(url);
	            app.tempUrl = url;
	            
	            String method = null;
	            String[] keyArray = new String[app.urlAndMethod.size()];
	            app.urlAndMethod.keySet().toArray(keyArray);
	            for(int i=0; i<app.urlAndMethod.size(); i++)
	            {
	            	String key = keyArray[i];
	            	if(url.contains(key))
	            	{
	            		method = app.urlAndMethod.get(key);
	            		break;
	            	}
	            }
	            //如果是表单提交按钮，则用相应的method进行请求，否则都用get进行请求
	            if(method != null)
	            {
		            if(method.equals("get"))
		            {
		            	app.jabber.sendZipwapRequest(url, null, (byte)0, app.myVCARD_myNickname, app.agent);
		            }
		            else if(method.equals("post"))
		            {
		            	app.jabber.sendZipwapRequest(url, null, (byte)1, app.myVCARD_myNickname, app.agent);
		            }
		            else //put
		            {
		            	app.jabber.sendZipwapRequest(url, null, (byte)2, app.myVCARD_myNickname, app.agent);
		            }
	            }
	            else
	            {
	            	app.jabber.sendZipwapRequest(url, null, (byte)0, app.myVCARD_myNickname, app.agent);
	            }
            }
            return true;
        }
        
        public void onReceivedError (WebView view, int errorCode, String description, String failingUrl) 
        {
        	app.bZwpDialog = false;
        	app.bZwpLoad = true;
        	CancelZwp();
        	return;
        }
	};
	
	public void LoadZwp()
	{
   	 	Message m = new Message(); 
	    m.what = EventListener.EVENT_ZWP_LOAD; 
	    if(app.MainHandler != null)
	    app.MainHandler.sendMessage(m); 
	}
	
	private void CancelZwp()
	{
   	 	Message m = new Message(); 
	    m.what = EventListener.EVENT_ZWP_CANCEL; 
	    app.MainHandler.sendMessage(m);
	}
	
	public boolean dispatchKeyEvent(KeyEvent event) 
    {
		if ( (event.getKeyCode() == KeyEvent.KEYCODE_BACK) && (event.getAction() == KeyEvent.ACTION_DOWN) )
        {
        	goBack();
        	return true;
        }
		return super.dispatchKeyEvent(event);
    }
}