package com.pica.msn;

import java.util.Vector;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.util.Log;

public class DatabaseHelper extends SQLiteOpenHelper{

	public DatabaseHelper(Context context, String name, CursorFactory factory,
			int version) {
		super(context, name, null, version);
		// TODO Auto-generated constructor stub
	}

	@Override public void onCreate(SQLiteDatabase db)
	{}
	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {}
	
	public void CloseDB()
	{
		close();
	}
	public void initUserData()
	{
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("create table if not exists UserTable(");
		sb.append("strLiveID text,");
		sb.append("strPassWord text,");
		sb.append("strPicaID text,");
		sb.append("strEditName text,");
		sb.append("strDomain text,");
		sb.append("bSavePassword text,");
		sb.append("bAutoLogin text,");
		sb.append("bOpenOnlineSound text,");
		sb.append("bOpenNewMessageSound text,");
		
		sb.append("bLastLogin text,");
		sb.append("myVCARD_myNickname text,");
		sb.append("myVCARD_myImpresa text,");
		sb.append("myVCARD_resourceId text,");
		sb.append("myContactGroupVersion text,");
		sb.append("devicedId text,");
		sb.append("loginStatus text,");
		sb.append("bShowOnline text,");
		sb.append("bGetRosterIcon text,");
		sb.append("isReceiveContactImpresaChange text,");
		sb.append("bNudge text);");
		
//		String sql = "create table if not exists UserTable("+"strLiveID text,"
//											   +"strPassWord text,"
//											   +"strPicaID text,"
//											   +"strEditName text,"
//											   +"strDomain text,"
//											   +"bSavePassword text,"	   
//											   +"bAutoLogin text,"
//											   +"bOpenOnlineSound text,"
//											   +"bOpenNewMessageSound text,"
//											   
//											   +"bLastLogin text,"	   
//											   +"myVCARD_myNickname text,"
//											   +"myVCARD_myImpresa text,"
//											   +"myVCARD_resourceId text,"
//											 //  +"myVcardVersion text,"	   
//											   +"myContactGroupVersion text,"
//											   +"devicedId text,"
//											   +"loginStatus text,"
//											   +"bShowOnline text,"
//											   +"bGetRosterIcon text,"
//											   +"isReceiveContactImpresaChange text"
//											   +");";
		try { db.execSQL(sb.toString()); }
		catch (SQLException e) {
		//Log.e("ERROR", e.toString());
		}
		close();
	}

	
	
	public void insertUserData(String LiveID, String PassWord, String PicaID,
								String EditName, String Domain, String bSavePassword,
								String bAutoLogin, String bOpenOnlineSound, String bOpenNewMessageSound,
								String bLastLogin, String myVCARD_myNickname, String myVCARD_myImpresa,
								String myVCARD_resourceId,String myContactGroupVersion,
								String devicedId, String loginstatus, String bShowOnline, String bGetRosterIcon,
								String isReceiveContactImpresaChange, String bNudge)
	{
		LiveID=MsnUtil.toValidRs(LiveID);
		PassWord=MsnUtil.toValidRs(PassWord);
		PicaID=MsnUtil.toValidRs(PicaID);
		EditName=MsnUtil.toValidRs(EditName);
		Domain=MsnUtil.toValidRs(Domain);
		bSavePassword=MsnUtil.toValidRs(bSavePassword);
		bAutoLogin=MsnUtil.toValidRs(bAutoLogin);
		bOpenOnlineSound=MsnUtil.toValidRs(bOpenOnlineSound);
		bOpenNewMessageSound=MsnUtil.toValidRs(bOpenNewMessageSound);
		bLastLogin=MsnUtil.toValidRs(bLastLogin);
		myVCARD_myNickname=MsnUtil.toValidRs(myVCARD_myNickname);
		myVCARD_myImpresa=MsnUtil.toValidRs(myVCARD_myImpresa);
		myVCARD_resourceId=MsnUtil.toValidRs(myVCARD_resourceId);
		myContactGroupVersion=MsnUtil.toValidRs(myContactGroupVersion);
		devicedId=MsnUtil.toValidRs(devicedId);
		loginstatus=MsnUtil.toValidRs(loginstatus);
		bShowOnline=MsnUtil.toValidRs(bShowOnline);
		bGetRosterIcon=MsnUtil.toValidRs(bGetRosterIcon);
		isReceiveContactImpresaChange=MsnUtil.toValidRs(isReceiveContactImpresaChange);
		bNudge=MsnUtil.toValidRs(bNudge);
		SQLiteDatabase db = getWritableDatabase();
		
		//initUserLastLogin();
		
		String check = "select * from UserTable where strLiveID = "+"'"+LiveID+"'";
		try 
		{			
			Cursor tmp = db.rawQuery(check, null);

			if(tmp.getCount() == 0)
			{
				StringBuffer sb = new StringBuffer();
				sb.append("insert into UserTable (strLiveID, strPassWord, strPicaID, strEditName, strDomain,");
				sb.append(" bSavePassword, bAutoLogin, bOpenOnlineSound, bOpenNewMessageSound, bLastLogin, myVCARD_myNickname,");
				sb.append(" myVCARD_myImpresa, myVCARD_resourceId, myContactGroupVersion, devicedId, loginStatus, bShowOnline, bGetRosterIcon,isReceiveContactImpresaChange, bNudge)");
				sb.append("values('");
				sb.append(LiveID);
				sb.append("','");
				sb.append(PassWord);
				sb.append("','");
				sb.append(PicaID);
				sb.append("','");
				sb.append(EditName);
				sb.append("','");
				sb.append(Domain);
				sb.append("','");
				sb.append(bSavePassword);
				sb.append("','");
				sb.append(bAutoLogin);
				sb.append("','");
				sb.append(bOpenOnlineSound);
				sb.append("','");
				sb.append(bOpenNewMessageSound);
				sb.append("','");
				sb.append(bLastLogin);
				sb.append("','");
				sb.append(myVCARD_myNickname);
				sb.append("','");
				sb.append(myVCARD_myImpresa);
				sb.append("','");
				sb.append(myVCARD_resourceId);
				sb.append("','");
				sb.append(myContactGroupVersion);
				sb.append("','");
				sb.append(devicedId);
				sb.append("','");
				sb.append(loginstatus);
				sb.append("','");
				sb.append(bShowOnline);
				sb.append("','");
				sb.append(bGetRosterIcon);
				sb.append("','");
				sb.append(isReceiveContactImpresaChange);
				sb.append("','");
				sb.append(bNudge);
				sb.append("');");
				
//				String sql = "insert into UserTable (strLiveID, strPassWord, strPicaID, strEditName, strDomain," +
//						" bSavePassword, bAutoLogin, bOpenOnlineSound, bOpenNewMessageSound, bLastLogin, myVCARD_myNickname," +
//						" myVCARD_myImpresa, myVCARD_resourceId, myContactGroupVersion, devicedId, loginStatus, bShowOnline, bGetRosterIcon,isReceiveContactImpresaChange)"
//					+"values("+"'"+LiveID+"'"+","+"'"+PassWord+"'"+","+"'"+PicaID+"'"+","+"'"+EditName+"'"+","+"'"+Domain+"'"
//					+","+"'"+bSavePassword+"'"+","+"'"+bAutoLogin+"'"+","+"'"+bOpenOnlineSound+"'"+","+"'"+bOpenNewMessageSound+"'"
//					+","+"'"+bLastLogin+"'"+","+"'"+myVCARD_myNickname+"'"+","+"'"+myVCARD_myImpresa+"'"+","+"'"+myVCARD_resourceId+"'"
//					+","+"'"+myContactGroupVersion+"'"+","+"'"+devicedId+"'"+","+"'"+loginstatus+"'"+","+"'"+bShowOnline+"'"+","+"'"+bGetRosterIcon+"'"+","+"'"+isReceiveContactImpresaChange+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ 
				    //Log.e("ERROR", e.toString()); 
				    }
				close();
			}
			else
			{
				ContentValues cv = new ContentValues();
				cv.put("strPassWord", PassWord);
				cv.put("strPicaID", PicaID);
				cv.put("strEditName", EditName);
				cv.put("strDomain", Domain);
				cv.put("bSavePassword", bSavePassword);
				cv.put("bAutoLogin", bAutoLogin);
				cv.put("bOpenOnlineSound", bOpenOnlineSound);
				cv.put("bOpenNewMessageSound", bOpenNewMessageSound);
				cv.put("bLastLogin", bLastLogin);
				cv.put("myVCARD_myNickname", myVCARD_myNickname);
				cv.put("myVCARD_myImpresa", myVCARD_myImpresa);
				cv.put("myVCARD_resourceId", myVCARD_resourceId);
				//cv.put("myVcardVersion", myVcardVersion);
				cv.put("myContactGroupVersion", myContactGroupVersion);
				cv.put("devicedId", devicedId);
				cv.put("loginStatus", loginstatus);
				cv.put("bShowOnline", bShowOnline);
				cv.put("bGetRosterIcon", bGetRosterIcon);
				cv.put("isReceiveContactImpresaChange", isReceiveContactImpresaChange);
				cv.put("bNudge", bNudge);
				db.update("UserTable", cv, "strLiveID = "+"'"+LiveID+"'", null);
				close();
			}		
		}
		catch (SQLException ce) 
		{
			//Log.e("ERROR", ce.toString());
			
			StringBuffer sb = new StringBuffer();
			sb.append("insert into UserTable (strLiveID, strPassWord, strPicaID, strEditName, strDomain,");
			sb.append(" bSavePassword, bAutoLogin, bOpenOnlineSound, bOpenNewMessageSound, bLastLogin, myVCARD_myNickname,");
			sb.append(" myVCARD_myImpresa, myVCARD_resourceId, myContactGroupVersion, devicedId, loginStatus, bShowOnline, bGetRosterIcon,isReceiveContactImpresaChange, bNudge)");
			sb.append("values('");
			sb.append(LiveID);
			sb.append("','");
			sb.append(PassWord);
			sb.append("','");
			sb.append(PicaID);
			sb.append("','");
			sb.append(EditName);
			sb.append("','");
			sb.append(Domain);
			sb.append("','");
			sb.append(bSavePassword);
			sb.append("','");
			sb.append(bAutoLogin);
			sb.append("','");
			sb.append(bOpenOnlineSound);
			sb.append("','");
			sb.append(bOpenNewMessageSound);
			sb.append("','");
			sb.append(bLastLogin);
			sb.append("','");
			sb.append(myVCARD_myNickname);
			sb.append("','");
			sb.append(myVCARD_myImpresa);
			sb.append("','");
			sb.append(myVCARD_resourceId);
			sb.append("','");
			sb.append(myContactGroupVersion);
			sb.append("','");
			sb.append(devicedId);
			sb.append("','");
			sb.append(loginstatus);
			sb.append("','");
			sb.append(bShowOnline);
			sb.append("','");
			sb.append(bGetRosterIcon);
			sb.append("','");
			sb.append(isReceiveContactImpresaChange);
			sb.append("','");
			sb.append(bNudge);
			sb.append("');");		
			
//				String sql = "insert into UserTable (strLiveID, strPassWord, strPicaID, strEditName, strDomain," +
//				" bSavePassword, bAutoLogin, bOpenOnlineSound, bOpenNewMessageSound, bLastLogin, myVCARD_myNickname," +
//				" myVCARD_myImpresa, myVCARD_resourceId,  myContactGroupVersion, devicedId, loginStatus, bShowOnline, bGetRosterIcon,isReceiveContactImpresaChange)"
//			+"values("+"'"+LiveID+"'"+","+"'"+PassWord+"'"+","+"'"+PicaID+"'"+","+"'"+EditName+"'"+","+"'"+Domain+"'"
//			+","+"'"+bSavePassword+"'"+","+"'"+bAutoLogin+"'"+","+"'"+bOpenOnlineSound+"'"+","+"'"+bOpenNewMessageSound+"'"
//			+","+"'"+bLastLogin+"'"+","+"'"+myVCARD_myNickname+"'"+","+"'"+myVCARD_myImpresa+"'"+","+"'"+myVCARD_resourceId+"'"
//			+","+"'"+myContactGroupVersion+"'"+","+"'"+devicedId+"'"+","+"'"+loginstatus+"'"+","+"'"+bShowOnline+"'"+","+"'"+bGetRosterIcon+"'"+","+"'"+isReceiveContactImpresaChange+"'"+");";
			try { db.execSQL(sb.toString());}
			catch (SQLException cce)
			{ Log.e("ERROR", cce.toString()); }
			close();
		}
	}
	
	public Cursor checkFirstTime(String LiveId)
	{
		LiveId=MsnUtil.toValidRs(LiveId);
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from UserTable where strLiveID = "+"'"+LiveId+"'";
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}
	}
	
	public void initUserLastLogin()
	{
		SQLiteDatabase db = getWritableDatabase();
		ContentValues cv = new ContentValues();
		cv.put("bLastLogin", "false");
		db.update("UserTable", cv, null, null);
		close();	
	}
	
	public Cursor getAllUserData()
	{
		SQLiteDatabase db = getReadableDatabase();
		Cursor cur;
		String col[] = {"strLiveID", "strPassWord", "strPicaID", "strEditName", "strDomain", "bSavePassword",
				"bAutoLogin", "bOpenOnlineSound", "bOpenNewMessageSound", "loginStatus", "bShowOnline", "bGetRosterIcon","isReceiveContactImpresaChange, bNudge"};
		try
		{
			cur = db.query("UserTable", col, null, null, null, null, null);
		}
		catch(SQLException e){
			//Log.e("ERROR", e.toString());
			close();
			return null;
		}
		cur.moveToFirst();
		close();
		return cur;
	}
	
	public Cursor getLastUserData()
	{		
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from UserTable where bLastLogin = 'true'";
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}
	}
	
	public Cursor getLoginUserData(String strLiveID)
	{
		strLiveID=MsnUtil.toValidRs(strLiveID);
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from UserTable where strLiveID = "+"'"+strLiveID+"'";
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}
	}

	public void setSystemUserData(String strLiveID, String bOpenOnlineSound, String bOpenNewMessageSound, String bShowOnline, String bNudge)//, String bGetRosterIcon)
	{

		strLiveID=MsnUtil.toValidRs(strLiveID);
		bOpenOnlineSound=MsnUtil.toValidRs(bOpenOnlineSound);
		bOpenNewMessageSound=MsnUtil.toValidRs(bOpenNewMessageSound);
		bNudge=MsnUtil.toValidRs(bNudge);

		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("bOpenOnlineSound", bOpenOnlineSound);
		cv.put("bOpenNewMessageSound", bOpenNewMessageSound);
		cv.put("bShowOnline", bShowOnline);
		cv.put("bNudge", bNudge);
		//cv.put("bGetRosterIcon", bGetRosterIcon);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}	
	
	/**流量设置 是否接收联系人头像变化 是否接收联系人个人信息变化*/
	public void setFlowUserData(String strLiveID, String isReceiveContactIcon, String isReceiveContactImpresa)
	{
		strLiveID=MsnUtil.toValidRs(strLiveID);
		isReceiveContactIcon=MsnUtil.toValidRs(isReceiveContactIcon);
		isReceiveContactImpresa=MsnUtil.toValidRs(isReceiveContactImpresa);
		
		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("bGetRosterIcon", isReceiveContactIcon);
		cv.put("isReceiveContactImpresaChange", isReceiveContactImpresa);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}
	
	public void setVCARD_Nickname_Impresa_Data(String strLiveID, String myVCARD_myNickname, String myVCARD_myImpresa)
	{

		strLiveID=MsnUtil.toValidRs(strLiveID);
		myVCARD_myNickname=MsnUtil.toValidRs(myVCARD_myNickname);
		myVCARD_myImpresa=MsnUtil.toValidRs(myVCARD_myImpresa);

		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("myVCARD_myNickname", myVCARD_myNickname);
		cv.put("myVCARD_myImpresa", myVCARD_myImpresa);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}
	
	public void setVCARD_ResourceId_Data(String strLiveID, String myVCARD_resourceId)
	{

		strLiveID=MsnUtil.toValidRs(strLiveID);
		myVCARD_resourceId=MsnUtil.toValidRs(myVCARD_resourceId);

		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("myVCARD_resourceId", myVCARD_resourceId);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}
	
//	public void SetVCARD_Version_Data(String strLiveID, String myVcardVersion)
//	{
//
//		strLiveID=toValidRs(strLiveID);
//		myVcardVersion=toValidRs(myVcardVersion);
//
//		SQLiteDatabase db = getWritableDatabase();
//
//		ContentValues cv = new ContentValues();
//		cv.put("myVcardVersion", myVcardVersion);
//		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
//		close();
//	}
	
	public void setContact_GroupVersion_Data(String strLiveID, String myContactGroupVersion)
	{
		strLiveID=MsnUtil.toValidRs(strLiveID);
		myContactGroupVersion=MsnUtil.toValidRs(myContactGroupVersion);

		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("myContactGroupVersion", myContactGroupVersion);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}
	
	public void setDeviceId_Data(String strLiveID, String devicedId)
	{

		strLiveID=MsnUtil.toValidRs(strLiveID);
		devicedId=MsnUtil.toValidRs(devicedId);

		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("devicedId", devicedId);
		db.update("UserTable", cv, "strLiveID = "+"'"+strLiveID+"'", null);
		close();
	}	
	
	public void initGroupData()
	{
		SQLiteDatabase db = getWritableDatabase();
		StringBuffer sb = new StringBuffer();
		sb.append("create table if not exists GroupTable(");
		sb.append("ACCOUNT_id text,");
		sb.append("GROUP_id text,");
		sb.append("GROUP_gbcode text,");
		sb.append("GROUP_name text,");
		sb.append("GROUP_flag text);");
		
//		String sql = "create table if not exists GroupTable("+"ACCOUNT_id text,"
//											   +"GROUP_id text,"
//											   +"GROUP_gbcode text,"
//											   +"GROUP_name text,"
//											   +"GROUP_flag text"
//											   +");";
		try { db.execSQL(sb.toString()); }
		catch (SQLException e) {
		//Log.e("ERROR", e.toString());
		}
		close();
	}
	
	public void insertGroupData(String ACCOUNT_id, String GROUP_id, String GROUP_gbcode,
			String GROUP_name, String GROUP_flag)
	{		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		GROUP_id=MsnUtil.toValidRs(GROUP_id);
		GROUP_gbcode=MsnUtil.toValidRs(GROUP_gbcode);
		GROUP_name=MsnUtil.toValidRs(GROUP_name);
		GROUP_flag=MsnUtil.toValidRs(GROUP_flag);
		
		SQLiteDatabase db = getWritableDatabase();
		StringBuffer check = new StringBuffer();
		check.append("select * from GroupTable where ACCOUNT_id = '");
		check.append(ACCOUNT_id);
		check.append("' and GROUP_id = '");
		check.append(GROUP_id);
		check.append("'");
		
		//String check = "select * from GroupTable where ACCOUNT_id = '" + ACCOUNT_id + "' and GROUP_id = '" + GROUP_id + "'";
		try 
		{
			Cursor tmp = db.rawQuery(check.toString(), null);
			//int i = tmp.getCount();
			if(tmp.getCount() == 0)
			{
				StringBuffer sb = new StringBuffer();
				sb.append("insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)");
				sb.append("values('");
				sb.append(ACCOUNT_id);
				sb.append("','");
				sb.append(GROUP_id);
				sb.append("','");
				sb.append(GROUP_gbcode);
				sb.append("','");
				sb.append(GROUP_name);
				sb.append("','");
				sb.append(GROUP_flag);
				sb.append("');");
				
				
//				String sql = "insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+GROUP_id+"'"+","+"'"+GROUP_gbcode+"'"+","+"'"+GROUP_name+"'"+","+"'"+GROUP_flag+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
			}
			else
			{
				ContentValues cv = new ContentValues();
				cv.put("GROUP_gbcode", GROUP_gbcode);
				cv.put("GROUP_name", GROUP_name);
				cv.put("GROUP_flag", GROUP_flag);
				db.update("GroupTable", cv, "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "GROUP_id = "+"'"+GROUP_id+"'", null);
				close();
			}		
		}
		catch (SQLException ce) 
		{
			//Log.e("ERROR", ce.toString());
			StringBuffer sb = new StringBuffer();
			sb.append("insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)");
			sb.append("values('");
			sb.append(ACCOUNT_id);
			sb.append("','");
			sb.append(GROUP_id);
			sb.append("','");
			sb.append(GROUP_gbcode);
			sb.append("','");
			sb.append(GROUP_name);
			sb.append("','");
			sb.append(GROUP_flag);
			sb.append("');");
			
//			String sql = "insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+GROUP_id+"'"+","+"'"+GROUP_gbcode+"'"+","+"'"+GROUP_name+"'"+","+"'"+GROUP_flag+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
		}
	}
	
	public void insertAllGroupData(String ACCOUNT_id, String GROUP_id, String GROUP_gbcode,
			String GROUP_name, String GROUP_flag)
	{
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		GROUP_id=MsnUtil.toValidRs(GROUP_id);
		GROUP_gbcode=MsnUtil.toValidRs(GROUP_gbcode);
		GROUP_name=MsnUtil.toValidRs(GROUP_name);
		GROUP_flag=MsnUtil.toValidRs(GROUP_flag);
	
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer check = new StringBuffer();
		check.append("select * from GroupTable where ACCOUNT_id = '");
		check.append(ACCOUNT_id);
		check.append("' and GROUP_id = '");
		check.append(GROUP_id);
		check.append("'");
		
		//String check = "select * from GroupTable where ACCOUNT_id = "+"'"+ACCOUNT_id+"'" + " and " + "GROUP_id = "+"'"+GROUP_id+"'";
		try 
		{
			Cursor tmp = db.rawQuery(check.toString(), null);
			//int i = tmp.getCount();
			if(tmp.getCount() == 0)
			{
				StringBuffer sb = new StringBuffer();
				sb.append("insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)");
				sb.append("values('");
				sb.append(ACCOUNT_id);
				sb.append("','");
				sb.append(GROUP_id);
				
				sb.append("','");
				sb.append(GROUP_gbcode);				
				sb.append("','");
				sb.append(GROUP_name);				
				sb.append("','");
				sb.append(GROUP_flag);
				sb.append("');");
							
//				String sql = "insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+GROUP_id+"'"+","+"'"+GROUP_gbcode+"'"+","+"'"+GROUP_name+"'"+","+"'"+GROUP_flag+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
			}
			else
			{
				ContentValues cv = new ContentValues();
				cv.put("GROUP_gbcode", GROUP_gbcode);
				cv.put("GROUP_name", GROUP_name);
				cv.put("GROUP_flag", GROUP_flag);
				db.update("GroupTable", cv, "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "GROUP_id = "+"'"+GROUP_id+"'", null);
			}		
		}
		catch (SQLException ce) 
		{
			//Log.e("ERROR", ce.toString());
			String sql = "insert into GroupTable (ACCOUNT_id, GROUP_id, GROUP_gbcode, GROUP_name, GROUP_flag)"
				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+GROUP_id+"'"+","+"'"+GROUP_gbcode+"'"+","+"'"+GROUP_name+"'"+","+"'"+GROUP_flag+"'"+");";
				try { db.execSQL(sql);}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
		}
	}
	
	public Cursor getGroupData(String ACCOUNT_id)
	{
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from GroupTable where ACCOUNT_id = '" + ACCOUNT_id + "'";
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}			
	}
	
	public void setGroupData(String ACCOUNT_id, String GROUP_id, String GROUP_gbcode, String GROUP_name, String GROUP_flag)
	{

		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		GROUP_id=MsnUtil.toValidRs(GROUP_id);
		GROUP_gbcode=MsnUtil.toValidRs(GROUP_gbcode);
		GROUP_name=MsnUtil.toValidRs(GROUP_name);
		GROUP_flag=MsnUtil.toValidRs(GROUP_flag);
		

		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("ACCOUNT_id = '");
		sb.append(ACCOUNT_id);
		sb.append("' and GROUP_id = '");
		sb.append(GROUP_id);
		sb.append("'");

		ContentValues cv = new ContentValues();
		cv.put("GROUP_gbcode", GROUP_gbcode);
		cv.put("GROUP_name", GROUP_name);
		cv.put("GROUP_flag", GROUP_flag);
		db.update("GroupTable", cv, sb.toString(), null);
		close();
	}
	
	public void deleteGroupData(String ACCOUNT_id)
	{
		try
		{
			
			ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);

			SQLiteDatabase db = getWritableDatabase();
			
			db.delete("GroupTable", "ACCOUNT_id = '" + ACCOUNT_id + "'", null);
			close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}

	}
	
	public void deleteOneGroupData(String ACCOUNT_id, String GROUP_id)
	{
		try
		{
		
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);

		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("ACCOUNT_id = '");
		sb.append(ACCOUNT_id);
		sb.append("' and GROUP_id = '");
		sb.append(GROUP_id);
		sb.append("'");

		db.delete("GroupTable", sb.toString(), null);
		close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void initRosterData()
	{
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("create table if not exists RosterTable(ACCOUNT_id text,SUBITEM_JID text,CONTACT_imid text,CONTACT_nickname text,");
		sb.append("CONTACT_gbcode text,CONTACT_groupId text,CONTACT_portraithash text,CONTACT_impresa text,CONTACT_gleam text,CONTACT_url text);");
//		String sql = "create table if not exists RosterTable("+"ACCOUNT_id text,"
//											   +"SUBITEM_JID text,"
//											   +"CONTACT_imid text,"
//											   +"CONTACT_nickname text,"
//											   +"CONTACT_gbcode text,"
//											   +"CONTACT_groupId text,"	   
//											   +"CONTACT_portraithash text,"
//											   +"CONTACT_impresa text,"
//											   +"CONTACT_gleam text,"
//											   +"CONTACT_url text"
//											   +");";
		try { db.execSQL(sb.toString()); }
		catch (SQLException e) {
		//Log.e("ERROR", e.toString());
		}
		close();
	}
	
	public void insertRosterData(String ACCOUNT_id, String SUBITEM_JID, String CONTACT_imid,
			String CONTACT_nickname, String CONTACT_gbcode, String CONTACT_groupId,
			String CONTACT_portraithash, String CONTACT_impresa, String CONTACT_gleam, String CONTACT_url)
	{
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		SUBITEM_JID=MsnUtil.toValidRs(SUBITEM_JID);
		CONTACT_imid=MsnUtil.toValidRs(CONTACT_imid);
		CONTACT_nickname=MsnUtil.toValidRs(CONTACT_nickname);
		CONTACT_gbcode=MsnUtil.toValidRs(CONTACT_gbcode);
		CONTACT_groupId=MsnUtil.toValidRs(CONTACT_groupId);
		CONTACT_portraithash=MsnUtil.toValidRs(CONTACT_portraithash);
		CONTACT_impresa=MsnUtil.toValidRs(CONTACT_impresa);
		CONTACT_gleam=MsnUtil.toValidRs(CONTACT_gleam);
		CONTACT_url=MsnUtil.toValidRs(CONTACT_url);
		
		
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer check = new StringBuffer();
		check.append("select * from RosterTable where ACCOUNT_id = '");
		check.append(ACCOUNT_id);
		check.append("' and CONTACT_imid = '");
		check.append(CONTACT_imid);
		check.append("'");
		
		//String check = "select * from RosterTable where ACCOUNT_id = "+"'"+ACCOUNT_id+"'" + " and " + "CONTACT_imid = "+"'"+CONTACT_imid+"'";
		try 
		{
			Cursor tmp = db.rawQuery(check.toString(), null);	
			if(tmp.getCount() == 0)
			{
				StringBuffer sql = new StringBuffer();
				sql.append("insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)");
				sql.append("values('");
				sql.append(ACCOUNT_id);
				sql.append("','");
				sql.append(SUBITEM_JID);
				sql.append("','");
				sql.append(CONTACT_imid);
				sql.append("','");
				sql.append(CONTACT_nickname);
				sql.append("','");
				sql.append(CONTACT_gbcode);
				sql.append("','");
				sql.append(CONTACT_groupId);
				
				sql.append("','");
				sql.append(CONTACT_portraithash);
				sql.append("','");
				sql.append(CONTACT_impresa);
				sql.append("','");
				sql.append(CONTACT_gleam);
				sql.append("','");
				sql.append(CONTACT_url);
				sql.append("');");			
				
//				String sql = "insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+SUBITEM_JID+"'"+","+"'"+CONTACT_imid+"'"+","+"'"+CONTACT_nickname+"'"+","+"'"+CONTACT_gbcode+"'"+","+"'"+CONTACT_groupId+"'"+","+"'"+CONTACT_portraithash+"'"+","+"'"+CONTACT_impresa+"'"+","+"'"+CONTACT_gleam+"'"+","+"'"+CONTACT_url+"'"+");";
				try { db.execSQL(sql.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
			}
			else
			{
				ContentValues cv = new ContentValues();
				cv.put("SUBITEM_JID", SUBITEM_JID);
				cv.put("CONTACT_nickname", CONTACT_nickname);
				cv.put("CONTACT_gbcode", CONTACT_gbcode);
				cv.put("CONTACT_groupId", CONTACT_groupId);
				cv.put("CONTACT_portraithash", CONTACT_portraithash);
				cv.put("CONTACT_impresa", CONTACT_impresa);
				cv.put("CONTACT_gleam", CONTACT_gleam);
				cv.put("CONTACT_url", CONTACT_url);
				db.update("RosterTable", cv, "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "CONTACT_imid = "+"'"+CONTACT_imid+"'", null);
				close();
			}		
		}
		catch (SQLException ce) 
		{
			StringBuffer sql = new StringBuffer();
			sql.append("insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)");
			sql.append("values('");
			sql.append(ACCOUNT_id);
			sql.append("','");
			sql.append(SUBITEM_JID);
			sql.append("','");
			sql.append(CONTACT_imid);
			sql.append("','");
			sql.append(CONTACT_nickname);
			sql.append("','");
			sql.append(CONTACT_gbcode);
			sql.append("','");
			sql.append(CONTACT_groupId);
			
			sql.append("','");
			sql.append(CONTACT_portraithash);
			sql.append("','");
			sql.append(CONTACT_impresa);
			sql.append("','");
			sql.append(CONTACT_gleam);
			sql.append("','");
			sql.append(CONTACT_url);
			sql.append("');");			
			
//			String sql = "insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+SUBITEM_JID+"'"+","+"'"+CONTACT_imid+"'"+","+"'"+CONTACT_nickname+"'"+","+"'"+CONTACT_gbcode+"'"+","+"'"+CONTACT_groupId+"'"+","+"'"+CONTACT_portraithash+"'"+","+"'"+CONTACT_impresa+"'"+","+"'"+CONTACT_gleam+"'"+","+"'"+CONTACT_url+"'"+");";
				try { db.execSQL(sql.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
		}
	}	
	
	public void insertAllRosterData(String ACCOUNT_id, String SUBITEM_JID, String CONTACT_imid,
			String CONTACT_nickname, String CONTACT_gbcode, String CONTACT_groupId,
			String CONTACT_portraithash, String CONTACT_impresa, String CONTACT_gleam, String CONTACT_url)
	{
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		SUBITEM_JID=MsnUtil.toValidRs(SUBITEM_JID);
		CONTACT_imid=MsnUtil.toValidRs(CONTACT_imid);
		CONTACT_nickname=MsnUtil.toValidRs(CONTACT_nickname);
		CONTACT_gbcode=MsnUtil.toValidRs(CONTACT_gbcode);
		CONTACT_groupId=MsnUtil.toValidRs(CONTACT_groupId);
		CONTACT_portraithash=MsnUtil.toValidRs(CONTACT_portraithash);
		CONTACT_impresa=MsnUtil.toValidRs(CONTACT_impresa);
		CONTACT_gleam=MsnUtil.toValidRs(CONTACT_gleam);
		CONTACT_url=MsnUtil.toValidRs(CONTACT_url);
		
		
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer check = new StringBuffer();
		check.append("select * from RosterTable where ACCOUNT_id = '");
		check.append(ACCOUNT_id);
		check.append("' and CONTACT_imid = '");
		check.append(CONTACT_imid);
		check.append("'");
		//String check = "select * from RosterTable where ACCOUNT_id = "+"'"+ACCOUNT_id+"'" + " and " + "CONTACT_imid = "+"'"+CONTACT_imid+"'";
		try 
		{
			Cursor tmp = db.rawQuery(check.toString(), null);	
			if(tmp.getCount() == 0)
			{
				StringBuffer sql = new StringBuffer();				
				sql.append("insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)");
				sql.append("values('");
				sql.append(ACCOUNT_id);
				sql.append("','");
				sql.append(SUBITEM_JID);
				sql.append("','");
				sql.append(CONTACT_imid);
				sql.append("','");
				sql.append(CONTACT_nickname);
				sql.append("','");
				sql.append(CONTACT_gbcode);
				sql.append("','");
				sql.append(CONTACT_groupId);
				sql.append("','");
				sql.append(CONTACT_portraithash);
				sql.append("','");
				sql.append(CONTACT_impresa);
				sql.append("','");
				sql.append(CONTACT_gleam);
				sql.append("','");
				sql.append(CONTACT_url);
				sql.append("');");
				
//				String sql = "insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+SUBITEM_JID+"'"+","+"'"+CONTACT_imid+"'"+","+"'"+CONTACT_nickname+"'"+","+"'"+CONTACT_gbcode+"'"+","+"'"+CONTACT_groupId+"'"+","+"'"+CONTACT_portraithash+"'"+","+"'"+CONTACT_impresa+"'"+","+"'"+CONTACT_gleam+"'"+","+"'"+CONTACT_url+"'"+");";
				try { db.execSQL(sql.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
			}
			else
			{
				ContentValues cv = new ContentValues();
				cv.put("SUBITEM_JID", SUBITEM_JID);
				cv.put("CONTACT_nickname", CONTACT_nickname);
				cv.put("CONTACT_gbcode", CONTACT_gbcode);
				cv.put("CONTACT_groupId", CONTACT_groupId);
				cv.put("CONTACT_portraithash", CONTACT_portraithash);
				cv.put("CONTACT_impresa", CONTACT_impresa);
				cv.put("CONTACT_gleam", CONTACT_gleam);
				cv.put("CONTACT_url", CONTACT_url);
				db.update("RosterTable", cv, "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "CONTACT_imid = "+"'"+CONTACT_imid+"'", null);
			}		
		}
		catch (SQLException ce) 
		{
			StringBuffer sql = new StringBuffer();				
			sql.append("insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)");
			sql.append("values('");
			sql.append(ACCOUNT_id);
			sql.append("','");
			sql.append(SUBITEM_JID);
			sql.append("','");
			sql.append(CONTACT_imid);
			sql.append("','");
			sql.append(CONTACT_nickname);
			sql.append("','");
			sql.append(CONTACT_gbcode);
			sql.append("','");
			sql.append(CONTACT_groupId);
			sql.append("','");
			sql.append(CONTACT_portraithash);
			sql.append("','");
			sql.append(CONTACT_impresa);
			sql.append("','");
			sql.append(CONTACT_gleam);
			sql.append("','");
			sql.append(CONTACT_url);
			sql.append("');");
//			String sql = "insert into RosterTable (ACCOUNT_id, SUBITEM_JID, CONTACT_imid, CONTACT_nickname, CONTACT_gbcode, CONTACT_groupId, CONTACT_portraithash, CONTACT_impresa, CONTACT_gleam, CONTACT_url)"
//				+"values("+"'"+ACCOUNT_id+"'"+","+"'"+SUBITEM_JID+"'"+","+"'"+CONTACT_imid+"'"+","+"'"+CONTACT_nickname+"'"+","+"'"+CONTACT_gbcode+"'"+","+"'"+CONTACT_groupId+"'"+","+"'"+CONTACT_portraithash+"'"+","+"'"+CONTACT_impresa+"'"+","+"'"+CONTACT_gleam+"'"+","+"'"+CONTACT_url+"'"+");";
				try { db.execSQL(sql.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
		}
	}
	
	public Cursor getRosterData(String ACCOUNT_id)
	{
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from RosterTable where ACCOUNT_id = '" + ACCOUNT_id + "'";
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}			
	}
	
	public void setRosterData(String ACCOUNT_id, String CONTACT_imid, String SUBITEM_JID, 
			String CONTACT_nickname, String CONTACT_gbcode, String CONTACT_groupId, String CONTACT_portraithash,
			String CONTACT_impresa, String CONTACT_gleam, String CONTACT_url)
	{
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);
		CONTACT_imid=MsnUtil.toValidRs(CONTACT_imid);
		SUBITEM_JID=MsnUtil.toValidRs(SUBITEM_JID);
		CONTACT_nickname=MsnUtil.toValidRs(CONTACT_nickname);
		CONTACT_gbcode=MsnUtil.toValidRs(CONTACT_gbcode);
		CONTACT_groupId=MsnUtil.toValidRs(CONTACT_groupId);
		CONTACT_portraithash=MsnUtil.toValidRs(CONTACT_portraithash);
		CONTACT_impresa=MsnUtil.toValidRs(CONTACT_impresa);
		CONTACT_gleam=MsnUtil.toValidRs(CONTACT_gleam);
		CONTACT_url=MsnUtil.toValidRs(CONTACT_url);


		SQLiteDatabase db = getWritableDatabase();

		ContentValues cv = new ContentValues();
		cv.put("SUBITEM_JID", SUBITEM_JID);
		cv.put("CONTACT_nickname", CONTACT_nickname);
		cv.put("CONTACT_gbcode", CONTACT_gbcode);
		cv.put("CONTACT_groupId", CONTACT_groupId);
		cv.put("CONTACT_portraithash", CONTACT_portraithash);
		cv.put("CONTACT_impresa", CONTACT_impresa);
		cv.put("CONTACT_gleam", CONTACT_gleam);
		cv.put("CONTACT_url", CONTACT_url);
		db.update("RosterTable", cv, "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "CONTACT_imid = "+"'"+CONTACT_imid+"'", null);
		close();
	}
	
	public void deleteRosterData(String ACCOUNT_id)
	{
		try
		{
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);		

		SQLiteDatabase db = getWritableDatabase();

		db.delete("RosterTable", "ACCOUNT_id = '" + ACCOUNT_id + "'", null);
		close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void deleteOneRosterData(String ACCOUNT_id, String SUBITEM_JID)
	{

		try
		{
		
		ACCOUNT_id=MsnUtil.toValidRs(ACCOUNT_id);		

		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("ACCOUNT_id = '");
		sb.append(ACCOUNT_id);
		sb.append("' and SUBITEM_JID = '");
		sb.append(SUBITEM_JID);
		sb.append("'");		
		
		db.delete("RosterTable", sb.toString(), null);
		//db.delete("RosterTable", "ACCOUNT_id = "+"'"+ACCOUNT_id+"'"+" and " + "SUBITEM_JID = "+"'"+SUBITEM_JID+"'", null);
		close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void initHistoryData(String tabName)
	{
		SQLiteDatabase db = getWritableDatabase();
		StringBuffer sb = new StringBuffer();
		sb.append("create table if not exists a");
		sb.append(tabName);
		sb.append("(strHistoryTitle text,strHistoryText text);");
		//String sql = "create table if not exists a" + tabName + "(strHistoryTitle text,strHistoryText text);";			
											   
		try { db.execSQL(sb.toString()); }
		catch (SQLException e) {
		//Log.e("ERROR", e.toString());
		}
		close();
	}
	
	public void insertHistoryData(String HistoryTab, String HistoryTitle, String HistoryText)
	{	
		HistoryTitle=MsnUtil.toValidRs(HistoryTitle);
		HistoryText=MsnUtil.toValidRs(HistoryText);
		
		SQLiteDatabase db = getWritableDatabase();
		
//		StringBuffer check = new StringBuffer();
//		check.append("select * from a");
//		check.append(HistoryTab);
//		check.append(" where strHistoryTitle = '");
//		check.append(HistoryTitle);
//		check.append("' and strHistoryText = '");
//		check.append(HistoryText);
//		check.append("'");
		
		//String check = "select * from " + "a" +HistoryTab + " where strHistoryTitle = "+"'"+HistoryTitle+"'" + " and " + "strHistoryText = "+"'"+HistoryText+"'";
		try 
		{
//			Cursor tmp = db.rawQuery(check.toString(), null);
			//int i = tmp.getCount();
//			if(tmp.getCount() == 0)
			{
				StringBuffer sb = new StringBuffer();
				sb.append("insert into a");
				sb.append(HistoryTab);
				sb.append(" (strHistoryTitle, strHistoryText)");
				sb.append("values('");
				sb.append(HistoryTitle);
				sb.append("','");
				sb.append(HistoryText);
				sb.append("');");
//				String sql = "insert into "+ "a" +HistoryTab+" (strHistoryTitle, strHistoryText)"
//				+"values("+"'"+HistoryTitle+"'"+","+"'"+HistoryText+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
			}
//			else
//			{
//				close();
//			}		
		}
		catch (SQLException ce) 
		{
			Log.e("ERROR", ce.toString());
			
			StringBuffer sb = new StringBuffer();
			sb.append("insert into a");
			sb.append(HistoryTab);
			sb.append(" (strHistoryTitle, strHistoryText)");
			sb.append("values('");
			sb.append(HistoryTitle);
			sb.append("','");
			sb.append(HistoryText);
			sb.append("');");
//				String sql = "insert into "+ "a" +HistoryTab+" (strHistoryTitle, strHistoryText)"
//				+"values("+"'"+HistoryTitle+"'"+","+"'"+HistoryText+"'"+");";
				try { db.execSQL(sb.toString());}
				catch (SQLException e)
				{ Log.e("ERROR", e.toString()); }
				close();
		}
	}

	public boolean checkAlreadyTitle(String HistoryTab, String HistoryTitle)
	{	
		HistoryTitle=MsnUtil.toValidRs(HistoryTitle);
		SQLiteDatabase db = getWritableDatabase();
		StringBuffer sb = new StringBuffer();
		sb.append("select * from a");
		sb.append(HistoryTab);
		sb.append(" where strHistoryTitle = '");
		sb.append(HistoryTitle);
		sb.append("'");
		
		//String check = "select * from a" + HistoryTab + " where strHistoryTitle = '" + HistoryTitle + "'";
		try 
		{
			Cursor tmp = db.rawQuery(sb.toString(), null);
			if(tmp.getCount() == 0)
			{
				close();
				return true;
			}
			else
			{
				close();
				return false;
			}		
		}
		catch (SQLException ce) 
		{
			Log.e("ERROR", ce.toString());
			close();
			return true;
		}
	}	
	
	public boolean checkNotAlready(String HistoryTab, String HistoryTitle, String HistoryText)
	{	
		HistoryTitle=MsnUtil.toValidRs(HistoryTitle);
		HistoryText=MsnUtil.toValidRs(HistoryText);
		SQLiteDatabase db = getWritableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("select * from a");
		sb.append(HistoryTab);
		sb.append(" where strHistoryTitle = '");
		sb.append(HistoryTitle);
		sb.append("' and strHistoryText = '");
		sb.append(HistoryText);
		sb.append("'");
		
//		String check = "select * from a" +HistoryTab + " where strHistoryTitle = '" 
//					+ HistoryTitle + "' and strHistoryText = '" + HistoryText + "'";
		try 
		{
			Cursor tmp = db.rawQuery(sb.toString(), null);
			//int i = tmp.getCount();
			if(tmp.getCount() == 0)
			{
				close();
				return true;
			}
			else
			{
				close();
				return false;
			}		
		}
		catch (SQLException ce) 
		{
			Log.e("ERROR", ce.toString());
			close();
			return true;
		}
	}
	
	public Vector checkHistory(String HistoryTab)
	{
		SQLiteDatabase db = getReadableDatabase();
		
		String sql = "select * from a" + HistoryTab;
		try
		{
			Cursor cur = db.rawQuery(sql, null);
			cur.moveToFirst();
			close();
    		if(cur != null)
    		{
	    		if(cur.getCount() == 0)
	    		{
	    			return null;
	    		}

	    		Vector vc = new Vector();
	    		for(int m = 0; m < cur.getCount(); m++)
	    		{
	    			if(vc.size() == 0)
	    			{
	    				vc.addElement(cur.getString(0));
	    			}
	    			else
	    			{
		    			for(int n = 0; n < vc.size(); n++)
		    			{
		    				if(vc.get(n).equals(cur.getString(0)))
		    				{
		    					break;
		    				}
		    				else
		    				{
		    					if(n == vc.size() - 1)
		    					{
		    						vc.addElement(cur.getString(0));
		    					}
		    					continue;
		    				}
		    			}
	    			}
					cur.moveToNext();
	    		}
	    		return vc;
    		}
			return null;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}
	}
	
	public Cursor loadHistoryData(String HistoryTab, String HistoryTitle)
	{
		HistoryTitle=MsnUtil.toValidRs(HistoryTitle);
		SQLiteDatabase db = getReadableDatabase();
		
		StringBuffer sb = new StringBuffer();
		sb.append("select * from a");
		sb.append(HistoryTab);
		sb.append(" where strHistoryTitle = '");
		sb.append(HistoryTitle);
		sb.append("'");
		
		//String sql = "select * from " + "a" +HistoryTab + " where strHistoryTitle = "+"'"+HistoryTitle+"'";
		try
		{
			Cursor cur = db.rawQuery(sb.toString(), null);
			cur.moveToFirst();
			close();
			return cur;
		}
		catch(SQLException e)
		{
			close();
			return null;
		}
	}
	
	public void deleteHistoryData(String HistoryTab, String HistoryTitle)
	{
		try
		{
		
		HistoryTitle=MsnUtil.toValidRs(HistoryTitle);

		SQLiteDatabase db = getWritableDatabase();

		db.delete("a" + HistoryTab, "strHistoryTitle = '" + HistoryTitle + "'", null);
		close();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
