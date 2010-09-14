package com.pica.msn;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.ContextMenu;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.ContextMenu.ContextMenuInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class RegisterActivity extends Activity {
	
	/* 页面上方的图片 */
	private ImageView topImage;
	/* 输入注册名的提示语 */
	private TextView userNameTitle;
	/* 输入用户名的文本框 */
	private EditText userNameT;
	/* @msn.cn的邮箱地址后缀 */
	private TextView emailAddress;
	/* 输入注册密码的提示语 */
	private TextView passWordTitle;
	/* 输入注册密码的文本框 */
	private EditText passWordT;
	/* 注册按钮 */
	private Button registerButton;
	private MSNApplication app;
	/* 注册时服务器返回的信息 */
	private String descInfo;
	/* 提示信息的Id */
	private int noticeId;
	/* 注册用户名*/
	private String registerUsername = null;
	/* 注册密码 */
	private String  registerPassword = null;
	public ProgressDialog noticeDialog = null;
	private static final int MENU_1 = Menu.FIRST;
	private static final int MENU_2 = Menu.FIRST + 1;
	
	Context context = null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		context = this;
		app = MSNApplication.getApplication(this);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.setContentView(R.layout.register);
		app.screenManager.pushActivity(this);
		int size = (int)(18 * app.screenScale);
		userNameTitle = (TextView) this.findViewById(R.id.register_username_title);
		userNameTitle.setTextSize(size);
		emailAddress = (TextView) this.findViewById(R.id.register_email);
		emailAddress.setTextSize(size);
		passWordTitle = (TextView) this.findViewById(R.id.register_password_title);
		passWordTitle.setTextSize(size);
		
		//如何是横屏的话，就不显示页面顶部的图片
		topImage = (ImageView)findViewById(R.id.register_image);
	    if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE){
	    	topImage.setVisibility(View.GONE);
	    } else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT){
	    	topImage.setVisibility(View.VISIBLE);
	    } 
	    
	    passWordT = (EditText) this.findViewById(R.id.register_password);
	    userNameT = (EditText) this.findViewById(R.id.register_username);
	    TextWatcher textWatcher = new TextWatcher(){

			@Override
			public void afterTextChanged(Editable s) {
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				String userName = userNameT.getText().toString().trim();
				if(userName.length() == 1 && !Character.isLetter(userName.charAt(0))){
					showDialog(UiEventListener.UI_EVENT_REGISTER_USERNAME_HEAD_ERROR);
					return;
				}
			}};
			
		userNameT.addTextChangedListener(textWatcher);
	    registerButton = (Button) this.findViewById(R.id.register_button);
//	    registerButton.setTextSize(20 * app.screenScale);
	    registerButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				registerUsername = userNameT.getText().toString().trim();
				if((registerUsername.length() == 0)){
		    		showDialog(UiEventListener.UI_EVENT_USERNAME_NULL_ERROR);
		    		return;
		    	} else if(registerUsername.length() < 3){
		    		showDialog(UiEventListener.UI_EVENT_REGISTER_USERNAME_ERROR);
		    		return;
		    	}
				registerPassword = passWordT.getText().toString().trim();
		    	if(registerPassword.length() == 0){
		    		showDialog(UiEventListener.UI_EVENT_PASSWORD_NULL_ERROR);
		    		return;
		    	} else if(registerPassword.length() < 6){
		    		showDialog(UiEventListener.UI_EVENT_REGISTER_PASSWORD_ERROR);
		    		return;
		    	}
		    	app.registerUsername = registerUsername;
		    	app.registerPassword = registerPassword;
		    	showDialog(UiEventListener.UI_EVENT_FIRST_TIME);
			}
		});
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE){
			topImage.setVisibility(View.GONE);
		} else if(this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			topImage.setVisibility(View.VISIBLE);
		}
	}
	
//	@Override
//	public boolean dispatchKeyEvent(KeyEvent event) {
//		if(event.getKeyCode() == KeyEvent.KEYCODE_BACK || event.getKeyCode() == KeyEvent.KEYCODE_MENU
//				|| event.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP || event.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN
//				|| event.getKeyCode() == KeyEvent.KEYCODE_DEL){
//			if(event.getKeyCode() == KeyEvent.KEYCODE_BACK){
////				 Process.killProcess(Process.myPid());
//			}
//			return super.dispatchKeyEvent(event);
//		}
//		return true;
//	}
	
	@Override
	protected void onDestroy() {
		app.screenManager.popActivity(this);
		super.onDestroy();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		app.mCurActivityIdx = app.Register_Activity;
		app.SetCurActivity(this);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
//		menu.add(Menu.NONE, MENU_1, Menu.NONE, R.string.about).setIcon(R.drawable.about);
		menu.add(Menu.NONE, MENU_2, Menu.NONE, R.string.register_menu_back).setIcon(R.drawable.ic_menu_revert);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		super.onOptionsItemSelected(item);
		switch(item.getItemId()){
		case MENU_1:
			showDialog(UiEventListener.UI_EVENT_ABOUT);
			break;
		case MENU_2:
			finish();
			break;
		}
		return true;
	}
	
	
	@Override
	protected Dialog onCreateDialog(int id) {
		if(id == UiEventListener.UI_EVENT_USERNAME_NULL_ERROR){
			return new AlertDialog.Builder(RegisterActivity.this)
	        .setCancelable(false)
	        .setIcon(R.drawable.msn)
	        .setTitle(R.string.error_prompt)
	        .setMessage(R.string.text_input_username_cannot_null)
	        .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
	            public void onClick(DialogInterface dialog, int whichButton) {
	
	            }
	        })
	        .create();
		} else if(id == UiEventListener.UI_EVENT_PASSWORD_NULL_ERROR){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.text_input_password_cannot_null)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	removeDialog(UiEventListener.UI_EVENT_PASSWORD_NULL_ERROR);
                }
            })
            .create();
		} else if(id == UiEventListener.UI_EVENT_REGISTER_USERNAME_ERROR){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.register_username_input_error)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
//                	userNameT.setText("");
                	removeDialog(UiEventListener.UI_EVENT_REGISTER_USERNAME_ERROR);
                }
            })
            .create();
		} else if(id == UiEventListener.UI_EVENT_REGISTER_PASSWORD_ERROR){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.register_password_input_error)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	passWordT.setText("");
                }
            })
            .create();
		} else if(id == UiEventListener.UI_EVENT_REGISTER_USERNAME_HEAD_ERROR){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.register_username_first_input_error)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	userNameT.setText("");
                }
            })
            .create();
		} else if(id == UiEventListener.EVENT_REGISTER_FAILURE){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(R.string.register_event_failure)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	passWordT.setText("");
                }
            })
            .create();
		} else if(id == UiEventListener.EVENT_REGISTER_SUCCESS){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.text_show_info_title)
            .setMessage(descInfo)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	Intent intent = new Intent();
                	Bundle bundle = new Bundle();
                	bundle.putString("registerUsername", registerUsername);
                	bundle.putString("registerPassword", registerPassword);
                	intent.putExtras(bundle);
                	setResult(RESULT_OK, intent);
                	finish();
                }
            })
            .create();
		} else if(id == UiEventListener.EVENT_REGISTER_ERROR){
			if(descInfo == null || "".equals(descInfo)){
				descInfo = getString(R.string.register_event_failure).toString();
			}
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.error_prompt)
            .setMessage(descInfo)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	passWordT.setText("");
                	removeDialog(UiEventListener.EVENT_REGISTER_ERROR);
                }
            })
            .create();
		} else if(id == UiEventListener.UI_EVENT_FIRST_TIME){
			return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.permission)
            .setMessage(R.string.permission_message)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                	startRegister();
    		    	noticeDialog = ProgressDialog.show(
    		    	   RegisterActivity.this,
                       getString(R.string.text_show_info_title),
                       getString(R.string.register_diglog_body), 
                       true
                     );
                }
            })
            .create();
		} else if(id == UiEventListener.UI_EVENT_ABOUT){
			String aboutMsg="Windows Live Messenger.\nPowered by MMIM Interactive Co,Ltd.\n"+this.getText(R.string.about_version)+
    		MSNApplication.VERSION + "\n"+MSNApplication.EMAIL_ABOUT;
            return new AlertDialog.Builder(RegisterActivity.this)
            .setCancelable(false)
            .setIcon(R.drawable.msn)
            .setTitle(R.string.about)
            .setMessage(aboutMsg)
            .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int whichButton) {
                }
            })
            .create();
		}
		return super.onCreateDialog(id);
	}
	
	/** 注册 */
	private void registerMsn()
	{
    	app.reset();
		try
		{
			String resultCode = null;// 只有返回3000才认为成功
			String regSendContent = null;
			String identifier = app.getRegisterIdentifier();
			if (!"".equals(identifier)){
				regSendContent = "msnid=" + app.registerUsername + "&password=" + app.registerPassword
					+ "&version=" + MSNApplication.VERSION + "&identifier="+ identifier;
			} else {
				regSendContent = "msnid=" + app.registerUsername + "&password=" + app.registerPassword
					+ "&version=" + MSNApplication.VERSION;
			}

			String response = MsnUtil.httpRequestRegister("/regdisp", "version=" + MSNApplication.VERSION, 
					MSNApplication.REGISTER_SERVER, null);
			if (response == null || response.length() == 0)
			{
				new Exception();
			}
			// 考虑可能拦截两次的情况
			if (!response.startsWith("REGURL=")){
				response = MsnUtil.httpRequestRegister("/regdisp", "version=" + MSNApplication.VERSION, 
						MSNApplication.REGISTER_SERVER, null);
				if (response.startsWith("REGURL=")){
					response = MsnUtil.httpRequestRegister("/regdisp", "version=" + MSNApplication.VERSION, 
							MSNApplication.REGISTER_SERVER, null);
				}
			}
			// 注册的地址
			String regUrl = response.substring("REGURL=".length()+7);
			
			String responseReg = MsnUtil.httpRequestRegister("", regSendContent, regUrl, null);
			// 正常情况下，肯定返回desc字段
			if (responseReg == null || responseReg.length() == 0 || responseReg.indexOf(";DESC=") == -1){
				// 注册失败，请稍后再试
				noticeId = UiEventListener.EVENT_REGISTER_FAILURE;
				Message msg = new Message();
				msg.what = 1;
				handler.sendMessage(msg);
			} else {
				descInfo = responseReg.substring(responseReg.indexOf(";DESC=") + 6);
				resultCode = responseReg.substring("resultcode=".length(), responseReg.indexOf(";DESC="));
				// 注册失败
				if (!resultCode.equals("3000")){
					noticeId = UiEventListener.EVENT_REGISTER_ERROR;
					Message msg = new Message();
    				msg.what = 1;
    				handler.sendMessage(msg);
				} else {
					// 注册成功
					app.SetLiveId(app.registerUsername, app.registerPassword);
					app.setDoMain("@msn.cn");// "@msn.cn"
					if (identifier == null || identifier.trim().length() <= 0)
					{
						identifier = app.registerUsername + MSNApplication.MSN_CHINA_DOMAIN;
						app.setRegisterIdentifier(identifier);
					}
					noticeId = UiEventListener.EVENT_REGISTER_SUCCESS;
					Message msg = new Message();
    				msg.what = 1;
    				handler.sendMessage(msg);
				}
			}

		} catch (Exception e){
			e.printStackTrace();
			noticeId = UiEventListener.EVENT_REGISTER_ERROR;
			Message msg = new Message();
			msg.what = 1;
			handler.sendMessage(msg);
		} finally {
			if(noticeDialog !=null && noticeDialog.isShowing()){
				noticeDialog.dismiss();
			}
		}
	}
	
	public void startRegister(){
		new Thread(new Runnable(){
			@Override
			public void run() {
				registerMsn();
			}}).start();
	}
	
	private Handler handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			if(!Thread.currentThread().isInterrupted()){
				switch (msg.what) {
				case 1:
					showDialog(noticeId);
					break;
				}				
			}
			super.handleMessage(msg);
		}
	};
}
