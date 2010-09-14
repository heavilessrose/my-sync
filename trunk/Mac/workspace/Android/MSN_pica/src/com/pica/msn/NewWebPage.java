package com.pica.msn;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class NewWebPage extends Activity 
{
	private MSNApplication msn;
    /** Called when the activity is first created. */
	public static final int EXIT_ID = Menu.FIRST;
	WebView webView = null;
	
    @Override
    public void onCreate(Bundle icicle) 
    {
        super.onCreate(icicle);
		msn=MSNApplication.getApplication();
        requestWindowFeature(Window.FEATURE_NO_TITLE);
	    final Window win = getWindow(); 
	    win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); 
        webView = new WebView(this);
        setContentView(webView);
        webView.getSettings().setTextSize(WebSettings.TextSize.SMALLEST);
        webView.loadUrl(msn.jabber.updateUrl);
 
       }
    @Override
	 public boolean onCreateOptionsMenu(Menu menu) 
    {
	        super.onCreateOptionsMenu(menu);
            menu.add(0, EXIT_ID, 0, R.string.exit);
	        return true;
	 }
	 @Override
	 public boolean onOptionsItemSelected(MenuItem item) 
	 {
	        switch (item.getItemId()) 
	        {
	           case EXIT_ID:
	            Intent mIntent = new Intent();
	            setResult(RESULT_CANCELED, mIntent);
	            finish();
	            break;
	        }
	        return super.onOptionsItemSelected(item);
	 }
 }
