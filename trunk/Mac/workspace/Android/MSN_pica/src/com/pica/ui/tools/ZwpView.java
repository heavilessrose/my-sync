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
	
	/**�Ƿ���Ҫ����ҳ���html�ı�*/
	public boolean isNeedSave = false;
	/**�Ƿ���Ҫ�������*/
	boolean isNeedRemove = false;
	
	/**��ǰ����*/
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
        	//�����ǰҳ�治����ҳ�棬��ѵ�ǰҳ��url,ҳ���html�ı����뻺������
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
		//���"ȡ����"������ҳ��
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
		
		//�ڵ�ǰҳ����سɹ���Ű���һ��ҳ�汣�浽������,��ʱoldBuf����һ��ҳ���html�ı�
		if(isNeedSave)
		{
			isNeedSave = false;
			//��������ҳ��
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
			//ҳ����³ɹ�������baseUrl��currentUrl
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
		
		//����ȫ�ֱ���
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
				//ȡ�������ĵ�һ��������һ��ҳ���HTML�ı������м���
				tempHtml = (String) app.htmlStrings.elementAt(0);
				//�����ҳ��������NoCache����ʹ��url����ҳ��
				if(tempHtml.equals("NoCache"))
				{					
					app.tempUrl = (String) app.urls.elementAt(app.urls.size()-1);
					zParser.tempBaseUrl = app.getBaseUrl(app.tempUrl);
					//���˲�������Ҫ����
					isNeedSave = false;
					//��Ҫ��splash()���������
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
					//���سɹ��Ÿ���ȫ�ֱ���
					if(app.baseUrls.size() != 0)
					{
						app.baseUrl = (String) app.baseUrls.remove(app.baseUrls.size()-1);					
					}				
					if(app.urls.size() != 0)
					{
						app.currentUrl = (String) app.urls.remove(app.urls.size()-1);					
					}
					//��ȡ����HTML�ı����浽buf�У��Ա��������Ӻ�ѵ�ǰҳ����뻺��
					//��Ϊû�н���splash������������Ҫ��buf���д���
					oldBuf.delete(0, oldBuf.length());
					oldBuf.append(tempHtml);
				}
			}
			//�����ҳ�滺�棬��ʹ��URL����ˢ��
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
        	//��ȥ�ڽ���ʱ����ӵĴ���������ȷ���ӵ�ַ
        	if(url.lastIndexOf("***") != -1)
        	{
        		url = url.substring(url.lastIndexOf("***") + 3);
        	}
        	
        	url = app.getAbsoluteUrl(url, app.baseUrl);
        	
        	if(type != null && type.length()!= 0)
        	{
        		//0xFF  ʹ��ϵͳ�������
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
        		//0xF1  ��������
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
        		
        		//0xF2  �༭����
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
	            //����Ǳ��ύ��ť��������Ӧ��method�������󣬷�����get��������
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