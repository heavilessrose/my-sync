package com.pica.ui.tools;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnFocusChangeListener;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ExpandableListView.OnChildClickListener;
import android.widget.ExpandableListView.OnGroupClickListener;
import android.widget.ExpandableListView.OnGroupCollapseListener;
import android.widget.ExpandableListView.OnGroupExpandListener;

import com.pica.msn.Contact;
import com.pica.msn.EventListener;
import com.pica.msn.MSNApplication;
import com.pica.msn.MainActivity;
import com.pica.msn.R;

public class RosterView extends LinearLayout implements OnItemSelectedListener{
	MSNApplication app;
	public RosterItem myItem;
	public LabelView lab;
	/**联系人列表 普通分组 可以显示离线状态的联系人列表*/
	public RosterList rlist;
	
	/**联系人列表 普通分组 仅显示在线联系人*/
	public RosterList normalOnlyOnlineRoster;
	
	public RosterView(Context context) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
		setOrientation(LinearLayout.VERTICAL);
		setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.FILL_PARENT));		
		init();
		// TODO Auto-generated constructor stub
	}
	
	public void resetMyItem()
	{
        this.removeAllViews();
        init();
        rlist.updateADImage();
	}
	
	private void init()
	{    
        LayoutParams Mparam = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        Contact me = new Contact();
        me.SUBITEM_STATE = app.getMyState();
        me.CONTACT_contactFlag = app.CONTACT_NORMAL_FLAG;
        me.CONTACT_nickname = app.myVCARD_myNickname;
        me.CONTACT_impresa = app.myVCARD_myImpresa;
        me.CONTACT_imageData=app.myVCARD_headImageData;
        myItem = new RosterItem(getContext(), me, -1, true, true);
		myItem.setLayoutParams(Mparam);
		myItem.setBackgroundResource(R.drawable.myitem_bg);
        addView(myItem);
        
        LayoutParams Rparams = new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT,1);
        rlist = new RosterList(getContext());
        rlist.setOnGroupCollapseListener(new OnGroupCollapseListener()
        {	
        	public void onGroupCollapse(int arg0) {
    		// TODO Auto-generated method stub
//    		app.groupIdx_item = -1;
//    		app.childIdx_item = -1;
    		
    		rlist.lastSelectGroupIndex = arg0;
    		rlist.selectRoster_imid = null;
    		rlist.selectRoster_groupIndex = -1;
			if(RosterItem.currentRosterItem != null)
			{
				RosterItem.hideVcard();				
			}
    		
    	}});
        rlist.setOnGroupExpandListener(new OnGroupExpandListener()
        {	
        	public void onGroupExpand(int arg0) {
    		// TODO Auto-generated method stub
//    		app.groupIdx_item = -1;
//    		app.childIdx_item = -1;
    		
    		rlist.lastSelectGroupIndex = arg0;
    		rlist.selectRoster_imid = null;
    		rlist.selectRoster_groupIndex = -1;
    	}});
        rlist.setLayoutParams(Rparams);

        rlist.setOnGroupClickListener(new OnGroupClickListener()
        {
			
			public boolean onGroupClick(ExpandableListView parent, View v,
					int groupPosition, long id)
			{
				/**9.27 TEST*/
				if(v instanceof GroupView)
				{
					if(groupPosition == rlist.lastSelectGroupIndex)
					{
						rlist.expandGroup(groupPosition);
						return true;
					}
					else
					{					
						rlist.lastSelectGroupIndex = groupPosition;
					}				
					app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);				
				}
				return true;
				/**9.27 TEST*/
			}
        	
        });
        
        rlist.setOnChildClickListener(new OnChildClickListener() { 
			
			public boolean onChildClick(ExpandableListView arg0, View arg1,
					int arg2, int arg3, long arg4)
			{
				/**9.27 TEST*/
				rlist.lastSelectGroupIndex = -1;
				if( rlist.selectRoster_groupIndex ==( (RosterItem)arg1 ).groupIndex
						&&(rlist.selectRoster_imid.equals(((RosterItem)arg1 ).imid)) )
				{
					Contact item = app.getCurrentGroupData().elementAt(arg2).contactVector.elementAt(arg3);
					((MainActivity)getContext()).PopOperaMenu(item);
					return true;
				}
				else
				{
					rlist.selectRoster_imid = ( (RosterItem)arg1 ).imid;
					rlist.selectRoster_groupIndex = ( (RosterItem)arg1 ).groupIndex;
					
				}				
				app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);							
				/**9.27 TEST*/

//					app.groupIdx_item = arg2;
//					app.childIdx_item = arg3;
//					if(arg1 instanceof RosterItem)
//					{
//						Contact item = app.getCurrentGroupData().elementAt(arg2).contactVector.elementAt(arg3);						
//						Contact thisContact=RosterItem.currentHeadContact;
//		      			if(item!=null&&thisContact!=null)
//		      			{
//		          			if(item==thisContact)//一致 则不变
//		          			{
//		          				
//		          			}
//		          			else//不一致 则先关闭原来的 打开新的
//		          			{
//		          				RosterItem.hideHead();
//								RosterItem rosterItem=(RosterItem)arg1;
//								rosterItem.getHead(item);
//		          			}	
//		      			}
//
//						((MainActivity)getContext()).PopOperaMenu(item);
//					}
//					else
//					{
//						rlist.selectItem_imid = ( (RosterMiniItem)arg1 ).imid;
//						app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//						return true;
//					}


				return true;
			} 
        }); 

        rlist.setOnItemSelectedListener(this);
        addView(rlist);
	}

//	
	public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3)
	{
		//Log.v("onItemSelected", arg2+"");
		Contact contact;
//		RosterItem.hideVcard();		
		if(arg1 == null)
		{
		    return;
		}
		/**9.27 TEST*/
		try {
		if(arg1 instanceof GroupView)
		{
			rlist.lastSelectGroupIndex = ( (GroupView) arg1 ).groupIndex;
			rlist.selectRoster_imid = null;
			rlist.selectRoster_groupIndex = -1;
			if(RosterItem.currentRosterItem != null)
			{
				RosterItem.hideVcard();				
			}
		}
		else if(arg1 instanceof ImageView)//广告图片
		{
			rlist.lastSelectGroupIndex = -1;
			rlist.selectRoster_imid = null;
			rlist.selectRoster_groupIndex = -1;
			if(RosterItem.currentRosterItem != null)
			{
				RosterItem.hideVcard();				
			}
		}
		else if(arg1 instanceof RosterItem)
		{
			rlist.lastSelectGroupIndex = -1;
			rlist.selectRoster_imid = ( (RosterItem)arg1 ).imid;
			rlist.selectRoster_groupIndex = ( (RosterItem)arg1 ).groupIndex;			
		}
		app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);		
		}catch(Exception e)
		{
		    e.printStackTrace();
		}
		/**9.27 TEST*/
		
		
//			if(RosterItem.currentRosterItem != null)
//			{
//				RosterItem.hideVcard();				
//			}
//			//rlist.selectItem_imid = null;
//		}
		
		/*if(rlist==null)
		{
			return;
		}
		else
		{
			app.eventNotify(EventListener.EVENT_UPDATE_CONTACT_UI, null);
//			Object ob1=arg0.getSelectedItem(); 
//			if(arg1 instanceof RosterItem)
//			{
//				contact=(Contact)ob1;
//				RosterItem item=(RosterItem)arg1;
//				item.showVcard(contact, 3000,item);
//			}
//			else
//			{
//				return;
//			}
		}*/

	}

	
	public void onNothingSelected(AdapterView<?> arg0) {
		// TODO Auto-generated method stub
		//Log.v("onNothingSelected","onNothingSelected");
		if(RosterItem.currentRosterItem != null)
		{
			RosterItem.hideVcard();
		}
		RosterItem.hideHead();
	}







}
