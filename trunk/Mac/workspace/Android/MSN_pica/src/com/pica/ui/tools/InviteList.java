package com.pica.ui.tools;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import com.pica.msn.Contact;
import com.pica.msn.Item;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;
import com.pica.msn.Group;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;

public class InviteList extends ListView{
	public List<Map<String,Contact>> InviteData;
	public InviteListAdapter mAdapter;
	MSNApplication app;
	public InviteList(Context context) {
		super(context);
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	init();
    	this.setBackgroundColor(Color.WHITE);
        mAdapter = new InviteListAdapter();
        setAdapter((ListAdapter) mAdapter);	
	}

	private void init()
	{
		InviteData= new ArrayList<Map<String, Contact>>();
		Vector tempVc=new Vector();//临时存储 用来判断是否添加
		//for(int i = 0; i < app.childData.size(); i++)
		Group group;
		Contact contact;
		boolean exist;
		Vector chaters;
		Contact chater;
		Map<String, Contact>  currentMap;
		for(int i=0;i<app.groupData.size();i++)
		{
			group = null;
			group=app.groupData.elementAt(i);
			for(int n = 0; n < group.contactVector.size(); n++)
			{
				//Contact contact = app.childData.get(i).get(n).get("CONTACT");
				contact = null;
				contact = group.contactVector.elementAt(n);
				if (contact.CONTACT_contactFlag == MSNApplication.CONTACT_NORMAL_FLAG
						&& contact.SUBITEM_STATE != MSNApplication.STATE_OFFLINE)
				{
					exist = false;
					if (contact.CONTACT_imid.equals(app.GetLiveID()))
						continue;// 不能添加自己
					chaters = (Vector) app.currentSession.SESSION_chaters;
					for (int s = 0; s < chaters.size(); s++)
					{
						chater = null;
						chater = (Contact) chaters.elementAt(s);
						if (chater.CONTACT_contactFlag == MSNApplication.CONTACT_STRANGER_FLAG)
							continue;// 临时会话里面可能会有陌生人
						if (contact.CONTACT_imid.equals(chater.CONTACT_imid))
						{
							exist = true;// 存在此用户 不再显示给用户添加
							break;
						}
					}
					if (!exist)
					{
						if(tempVc.contains(contact.CONTACT_imid))
							continue;//已经添加过一次 不用再添加
						tempVc.addElement(contact.CONTACT_imid);
						
						currentMap = null;
		    			currentMap = new HashMap<String, Contact>();      
		    			InviteData.add(0,currentMap);
		                currentMap.put("INVITE", contact);
					}
					
				}
				
			}
		}
		
	}
	
	
	public class InviteListAdapter extends BaseAdapter{

		
		public int getCount() {
			// TODO Auto-generated method stub
			//return 5;
			return InviteData.size();
		}

		
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			//return position;
			
			return InviteData.get(position);
		}

		
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return position;
		}

		
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub
		        AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
		                ViewGroup.LayoutParams.FILL_PARENT, 40);       
		        
		        Map<String, Contact> map=InviteData.get(position);
		        Contact temp=map.get("INVITE");
		       				        	      
		        InviteItem gItem = new InviteItem(getContext(), temp);;
				gItem.setLayoutParams(lp);
				return gItem;
		}
        // Sample data set.  children[i] contains the children (String[]) for groups[i].


    }
}
