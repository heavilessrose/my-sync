package com.pica.msn;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class SearchFileActivity extends Activity implements AdapterView.OnItemClickListener{
    private String currentDir = "/";
    private String choosenFileName = null;    
    private EfficientAdapter adapter;
    
    public static final long FileLengthMax = 1024*500;   
    private ListView listview;
    private TextView tv;
    private ImageView returnButton;    
    private String[] str;
    
    /**ҳ���־���� ѡ��ͼƬ ѡ���ļ�*/
    private byte currentType;
    
    /**�Ƿ�ֻ��ʾ¼�Ƶ�amr�ļ�*/
	private boolean isOnlyShowAMR;
	private MSNApplication app;
	
	//////////////////////������̱�ɱ����bug
	boolean isKilled = false;
    
    /**��¼ 
     * @param isReconnect �Ƿ������µ�¼
     * */
	private void reLogin(boolean isReconnect, Bundle Map)
	{
		app.reset();
		String domain = Map.getString("domain");
		String liveId = Map.getString("liveId");
		String password = Map.getString("password");
		boolean isAutoLogin = Map.getBoolean("isAutoLogin");
		boolean isSavePassword = Map.getBoolean("isSavePassword");
		String status = Map.getString("status");
	    		
		app.SetLiveId(liveId, password);
	            
		reLoginSet(app.GetLiveID());
	        		
		app.isSavePassword = isSavePassword;
		app.isAutoLogin = isAutoLogin;
		app.SetLoginStatus(status);
            
		if(!isReconnect)
		{
			Intent intent= new Intent();
			intent.setClass(this, LoginMonitorActivity.class);
			intent.putExtra("type",MSNApplication.MONITOR_LOGIN_FLAG);
			startActivity(intent);
		                
			app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
			app.DB.initGroupData();
			app.DB = new DatabaseHelper(this, app.DB_NAME, null, app.DB_VERSION);
			app.DB.initRosterData();
			/////////////////////
		
			//ȡ���ݿ��������
			app.initVcardFromRMS(app.GetLiveID());
			app.getContactGroupVerFromRMS(app.GetLiveID());
			app.initGroupsContactsFromFile(app.GetLiveID());
		 	//app.initGroupsFromRMS(app.GetLiveID());
			//app.initContactsFromRMS(app.GetLiveID());
			app.initContactHeadFromRMS(app.GetLiveID());
			app.getDeviceIdFromRMS(app.GetLiveID());
		}
		else
		{
			if(app.LMHandler!=null)
			{
				Message m = new Message(); 
				m.what = EventListener.EVENT_LOGIN_MONITOR_SETTEXT; 
				//m.obj= getText(R.string.findserver);
				m.obj=getText(R.string.text_signing_in);
				app.LMHandler.sendMessage(m);    
			}
		}
	/////////��ʱд��
//        	app.setGroupOrder(app.ROSTER_NORMAL_NO_OFFLINE);
//        	app.isShowOnline=true;
//        	app.isReceiveHeadChange=true;
        	////////
		app.startJabber();
	}
    	
	private void reLoginSet(String liveId)
	{
		app.DB = new DatabaseHelper(this, MSNApplication.DB_NAME, null, MSNApplication.DB_VERSION);
		app.DB.initUserData();   
		Cursor cur = app.DB.getLoginUserData(liveId);
	            
		if(cur != null)
		{
			if(cur.getCount() != 0)
			{
				try
				{
					app.SetPicaId(MsnUtil.getUnvalidFormRs(cur.getString(2)));
					app.SetEditAccount(MsnUtil.getUnvalidFormRs(cur.getString(3)), cur.getString(4));
					app.isSavePassword=cur.getString(5).equals("true");
					app.isAutoLogin=cur.getString(6).equals("true");
					app.isOpenOnlineSound =cur.getString(7).equals("true");
					app.isOpenNewMessageSound=cur.getString(8).equals("true");
					app.SetLoginStatus(cur.getString(15));
					app.isShowOnline =cur.getString(16).equals("true");
					app.isReceiveHeadChange=cur.getString(17).equals("true");
					/////////////ȡ���Լ���ͷ��
					if(app.isReceiveHeadChange)
					{
						app.getVcardHead(liveId);
					}
				    		        		
				    		        	
					////////////
					app.isReceiveVcardChange=cur.getString(18).equals("true");
					app.isNudge=cur.getString(19).equals("true");
					//////////////////���Ӱ�״̬������
					if(app.isShowOnline)
			        {
			        	app.setGroupOrder((byte)(app.getGroupOrder() + 1));
			        }				
					else
					{
						app.setGroupOrder((byte)(app.getGroupOrder() - 1));
					}
					////////////////////
				}
				catch(Exception e)
				{
		     
				}
			}
			else// end if(cur.getCount() != 0)
			{

			}
		}
		else// end if(cur != null)
		{

		}
	}
    	
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		/**������̱�ɱ����bug*/
		outState.putBoolean("isKilled", true);
		outState.putString("domain", app.GetDomain());
		outState.putString("liveId", app.GetLiveID());
		outState.putString("password", app.GetPassWord());
		outState.putBoolean("isAutoLogin", app.isAutoLogin);
		outState.putBoolean("isSavePassword", app.isSavePassword);
		outState.putString("status", app.sStatus);
	}
    	
	public void onResume() 
	{
		super.onResume();
		app.isPause = false;
		/**������̱�ɱ����bug*/
		if(isKilled && app == null)
		{
			finish();
			return;
		}
 	}	
	
    public void onPause()
    {
        super.onPause();
        app.isPause = true;
    }
    //////////////////////////////
	
    public void onCreate(Bundle savedInstanceState)
    {
    	requestWindowFeature(Window.FEATURE_NO_TITLE); 
        super.onCreate(savedInstanceState);
        setContentView(R.layout.searchfile);
        
        ///////////////////////������̱�ɱ����bug
        if(savedInstanceState != null && savedInstanceState.getBoolean("isKilled") && app == null)
        {
        	isKilled = true;
        	finish();
        	app = MSNApplication.getApplication(this);
        	app.screenManager.pushActivity(this);
        	if(!app.killedToRelogin)//û���ڱ��Activity�����µ�¼
        	{
        		reLogin(false, savedInstanceState);
        	}
        	//��������Activity�оͲ�Ҫ���ظ�����
        	app.killedToRelogin = true;
        	return;
        }
        ////////////////////////////////
        
        app = MSNApplication.getApplication(this);
        app.screenManager.pushActivity(this);
        Bundle extras=getIntent().getExtras();
        currentType=extras.getByte("type");
        isOnlyShowAMR = extras.getBoolean("isOnlyShowAMR");
        Resources r = this.getResources();
        
        tv = (TextView) findViewById(R.id.currentDirName);               
        tv.setText(getText(R.string.text_trans_nofold));
        if(isOnlyShowAMR)
        {        	
        	String[] temp = {"VioceClipDir","sdcard"};
        	str = temp;
        }
        else
        {
        	str = sortFileByType(currentDir, isOnlyShowAMR);
        }
             
        listview = (ListView) findViewById(R.id.allFile);
		adapter = new EfficientAdapter(SearchFileActivity.this);
		listview.setAdapter(adapter);        
        listview.setOnItemClickListener(this);
        
        //���ذ�ť        
        returnButton = (ImageButton)findViewById(R.id.returnButton);
        returnButton.setEnabled(false);
        returnButton.setBackgroundDrawable(r.getDrawable(R.drawable.returnbutton));
       
        returnButton.setOnClickListener(new OnClickListener(){
			
			public void onClick(View arg0)
			{
				// TODO Auto-generated method stub
				choosenFileName = null;
				if(currentDir.equals("/"))
				{
					tv.setText(getText(R.string.text_trans_nofold));
				}
				//�Ӳ˵�"���������ļ�"�����activity�����صĲ��Ǹ�Ŀ¼��
				//����/data/data/com.pica.msn/files/Ŀ¼��/sdcardĿ¼
				else if(currentDir.equals("/data/data/com.pica.msn/files/") && isOnlyShowAMR)
				{
					currentDir = "/";
					tv.setText(getText(R.string.text_trans_nofold));
					String[] temp = {"VioceClipDir","sdcard"};
					str = temp;
				}
				//�Ӳ˵�"���������ļ�"�����activity�����صĲ��Ǹ�Ŀ¼������/data/data/com.pica.msn/files/Ŀ¼��/sdcardĿ¼
				else if(currentDir.equals("/sdcard/") && isOnlyShowAMR)
				{
					currentDir = "/";
					tv.setText(getText(R.string.text_trans_nofold));
					String[] temp = {"VioceClipDir","sdcard"};
					str = temp;
				}
				else
				{					
					String temp =null;
					temp= currentDir.substring(0, currentDir.length()-1);
					currentDir = currentDir.substring(0, temp.lastIndexOf('/')+1);
					if(currentDir.equals("/"))
					{
						returnButton.setEnabled(false);
						tv.setText(getText(R.string.text_trans_nofold));
					}
					else
					{
						tv.setText(currentDir);
					}					
				}
				
				//������ǴӲ˵�"���������ļ�"�����activity
				if( !(isOnlyShowAMR && currentDir.equals("/")) )
				{
					str = sortFileByType(currentDir, isOnlyShowAMR);
				}				
				listview.setAdapter(adapter);
				adapter.notifyDataSetChanged();				
			}
        });        
    }   

    public void onDestroy()
    {
        app.screenManager.popActivity(this);
        super.onDestroy();
    }

	
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub
		String temp1 = str[arg2].toString();
		//������"VioceClipDir"������"data/data/com.pica.msn/files"Ŀ¼
		if(temp1.equals("VioceClipDir"))
		{
			temp1 = "data/data/com.pica.msn/files";
		}			
		currentDir = currentDir + temp1 +"/";
		returnButton.setEnabled(true);
		
		File tempfile = new File(currentDir);				
		if(tempfile.isFile())
		{
			choosenFileName = currentDir;			
			if(isCanSend(choosenFileName))
			{
				//�ж��ļ���С�Ƿ񳬳������͵����ֵ
				if(tempfile.length() < FileLengthMax)
				{
					Intent intent=null;
					if(currentType==MSNApplication.SEARCHFILE_MAINROSTER_FLAG)
					{
						intent = new Intent(SearchFileActivity.this, MainActivity.class);	
					}
					else if(currentType==MSNApplication.SEARCHFILE_CHATROOM_FLAG)
					{
						intent = new Intent(SearchFileActivity.this, ChatRoomActivity.class);	
					}
			
					intent.putExtra("filename", choosenFileName);
					setResult(RESULT_OK, intent);
					finish();
				}
				else
				{
					new AlertDialog.Builder(SearchFileActivity.this)
					.setCancelable(false)
	                .setIcon(R.drawable.alert_dialog_icon)
	                .setTitle(getText(R.string.text_trans_outmaxsize))
	                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
	                    public void onClick(DialogInterface dialog, int whichButton) {

	                        /* User clicked OK so do some stuff */
	                    }
	                })
	                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
	                    public void onClick(DialogInterface dialog, int whichButton) {

	                        /* User clicked Cancel so do some stuff */
	                    }
	                })
	                .create().show();
					String temp2 =null;
					temp2= currentDir.substring(0, currentDir.length()-1);
					currentDir = currentDir.substring(0, temp2.lastIndexOf('/')+1);	
				}
			}
			else
			{
				new AlertDialog.Builder(SearchFileActivity.this)
				.setCancelable(false)
                .setIcon(R.drawable.alert_dialog_icon)
                .setTitle(getText(R.string.text_trans_notsafe))
                .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {

                        /* User clicked OK so do some stuff */
                    }
                })
                .setNegativeButton(R.string.alert_dialog_cancel, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {

                        /* User clicked Cancel so do some stuff */
                    }
                })
                .create().show();
				String temp2 =null;
				temp2= currentDir.substring(0, currentDir.length()-1);
				currentDir = currentDir.substring(0, temp2.lastIndexOf('/')+1);				
			}		
		}
		else
		{
			//��ЩĿ¼����list()Ҳ���ܷ���null
			if(tempfile.list() == null)
			{			
				ArrayAdapter<String> adapter;
				adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
				listview.setAdapter(adapter);								
			}
			else
			{
				str = sortFileByType(currentDir, isOnlyShowAMR);								
				adapter.notifyDataSetChanged();
			}			
		}
		if(currentDir.equals("/"))
		{
			tv.setText(getText(R.string.text_trans_nofold)); 
		}
		else
		{
			if(currentDir.contains("data/data/com.pica.msn/files"))
			{
				tv.setText("voiceClipDir");
			}
			else
			{
				tv.setText(currentDir); 
			}
			
		}		   	
	}	
	
	/**
	 * �����ļ����ĺ�׺�ж��ļ��Ƿ��ܷ���
	 * @param filename
	 * @return
	 */
	private Boolean isCanSend(String filename)
	{
		//�û���ѡ�ļ��ĺ�׺��
		String suffix;
		if(filename.contains("."))
		{
			suffix = filename.substring(filename.lastIndexOf("."),filename.length()-1);
		}
		else
		{
			suffix = "";
		}
		//�������͵��ļ���׺�� 
		String[] fileSuffix ={".ade", ".adp", ".app", ".asp", ".bas", ".bat", ".cer", ".chm", ".cmd",
						".com", ".cpl", ".crt", ".csh", ".exe", ".fxp", ".f30hlp", ".hta", ".inf", ".ins",
						".isp", ".its", ".js", ".jse", ".ksh", ".lnk", ".mad", ".maf", ".mag", ".mam",
						".maq", ".mar", ".mas", ".mat", ".mau", ".mav", ".maw", ".mda", ".mdb", ".mde",
						".mdt", ".mdw", ".mdz", ".msc", ".msi", ".msp", ".mst", ".ops", ".pcd", ".pif",
						".prf", ".prg", ".pst", ".reg", ".scf", ".scr", ".sct", ".shb", ".shs", ".tmp",
						".url",	".vb", ".vbe", ".vbs", ".vsd", ".vsmacros", ".vss", ".vst", ".vsw", ".ws",
						".wsc", ".wsf", ".wsh", ".sis", ".sisx", ".jad", ".jar", ".cab", ".rc", ".sh", ".apk"};
		int i = fileSuffix.length;
		//��׺�ж��ܷ���
		boolean isCanSend = true;
		for(int j=0; j<i; j++)
		{
			if(suffix.equalsIgnoreCase(fileSuffix[j]))
			{
				isCanSend = false;
				break;
			}					
		}
		//�ж��Ƿ��ܴ��ļ����������򲻷���
		try
	    {
			FileInputStream fileinput = new FileInputStream(filename);
		}
	    catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			isCanSend = false;
		}
	    return isCanSend;
	}
	
	/**
	 * �������ͣ�Ŀ¼���ļ����г�Ŀ¼�µ��ļ���
	 * @param file		Ŀ¼
	 * @param currentDir  Ŀ¼��
	 * @return  װ��Ŀ¼�������ļ���������
	 */
	public String[] sortFileByType(String currentDir, boolean isOnlyShowAMR)
	{
		File file = new File(currentDir);
		if(isOnlyShowAMR)
		{
			String[] tempname = file.list();
			int i = tempname.length;
			int n = 0;//δ�����ļ�������
			File tempFile;
			for(int j=0; j<i; j++)
			{
				tempFile = null;
				tempFile = new File(currentDir +tempname[j]);
				// ���������ļ��������ļ��л���amr�ļ�
				if(!tempFile.isHidden()&&( (tempFile.isDirectory()) || (tempname[j].endsWith(".amr")) ) )
				{				
					n++;
				}				
			}			
			String[] childname = new String[n];
			int k=0;
			int m=n-1;
			for(int j=0; j<i; j++)
			{
				tempFile = null;
				tempFile = new File(currentDir +tempname[j]);
				//�ļ����������ļ����ļ����ŷ���
				if(!tempFile.isHidden())
				{
					//�������ͷֱ����������˲���
					if(tempFile.isDirectory())
					{
						childname[k] = tempname[j];
						k++;
					}
					else
					{
						if(tempname[j].endsWith(".amr"))
						{
							childname[m] = tempname[j];
							m--;
						}
						
					}
				}				
			}			
			return childname;
		}
		else
		{
			String[] tempname = file.list();
			int i = tempname.length;
			int n = 0;//δ�����ļ�������
			File tempFile;
			for(int j=0; j<i; j++)
			{
				tempFile = null;
				tempFile = new File(currentDir +tempname[j]);
				if(!tempFile.isHidden())
				{				
					n++;
				}				
			}			
			String[] childname = new String[n];
			int k=0;
			int m=n-1;
			for(int j=0; j<i; j++)
			{
				tempFile = null;
				tempFile = new File(currentDir +tempname[j]);
				//�ļ����������ļ����ļ����ŷ���
				if(!tempFile.isHidden())
				{
					//�������ͷֱ����������˲���
					if(tempFile.isDirectory())
					{
						childname[k] = tempname[j];
						k++;
					}
					else
					{
						childname[m] = tempname[j];
						m--;
					}
				}				
			}			
			return childname;
		}		
	}
	
	
	 private class EfficientAdapter extends BaseAdapter 
	 {
	        private LayoutInflater mInflater;
	        private Bitmap txtIcon;
	        private Bitmap musicIcon;
	        private Bitmap pictureIcon;
	        private Bitmap rarIcon;
	        private Bitmap unknownIcon;
	        private Bitmap folderIcon;

	        public EfficientAdapter(Context context)
	        {	            
	            mInflater = LayoutInflater.from(context);	            
	            txtIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.txtfile);
	            musicIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.musicfile);
	            pictureIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.picturefile);
	            rarIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.rarfile);
	            unknownIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.unknownfile);
	            folderIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.folder);
	        }

	        public int getCount() {
	            return str.length;
	        }
	        
	        public Object getItem(int position) {
	            return position;
	        }
	        
	        public long getItemId(int position) {
	            return position;
	        }

	        public View getView(int position, View convertView, ViewGroup parent)
	        {	            
	            ViewHolder holder;
	            
	            if (convertView == null)
	            {
	                convertView = mInflater.inflate(R.layout.list_item_icon_text, null);
	                holder = new ViewHolder();
	                holder.text = (TextView) convertView.findViewById(R.id.text);
	                holder.icon = (ImageView) convertView.findViewById(R.id.icon);
	                convertView.setTag(holder);
	            }
	            else 
	            {	                
	                holder = (ViewHolder) convertView.getTag();
	            }	            
	            holder.text.setText(str[position]);
	            File tempfile = new File(currentDir + str[position]);	            
	            String suffix;
	            if(str[position].contains("."))
	            {
	            	suffix =str[position].substring(str[position].lastIndexOf(".")+1,str[position].length());
	            }
	            else
	            {
	            	suffix = "";
	            }
	            if(tempfile.isFile())
	            {
	            	holder.icon.setImageBitmap(unknownIcon);
	            	if(suffix.equals("txt"))
	            	{
	            		holder.icon.setImageBitmap(txtIcon);
	            	}
	            	if(suffix.equals("3gp")||suffix.equals("wav")||suffix.equals("amr")||suffix.equals("mid")
	            			||suffix.equals("ogg")||suffix.equals("mp3")||suffix.equals("mp4"))
	            	{
	            		holder.icon.setImageBitmap(musicIcon);
	            	}
	            	else if(suffix.equals("jpg")||suffix.equals("png")||suffix.equals("gif")||suffix.equals("bmp"))
	            	{
	            		holder.icon.setImageBitmap(pictureIcon);
	            	}
	            	else if(suffix.equals("zip")||suffix.equals("rar"))
	            	{
	            		holder.icon.setImageBitmap(rarIcon);
	            	}            		            	
	            }
	            else
	            {
	            	holder.icon.setImageBitmap(folderIcon);
	            }
	            return convertView;
	        }

	        class ViewHolder 
	        {
	            TextView text;
	            ImageView icon;
	        }
	 }	 			
}
